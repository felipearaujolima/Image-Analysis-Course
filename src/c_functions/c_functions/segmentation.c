#include "segmentation.h"

Image* ImageBasins(Image* img, AdjRel* Ain) {
    
    AdjRel* A = NULL;
    A = CopyAdjacency(Ain);

    int dx, dy, dz;
    applyMaxAdjShifts(A, &dx, &dy, &dz);

    Image* basins = CreateImage(img->xsize, img->ysize, img->zsize);
    float K = sqrtf(dx * dx + dy * dy + dz * dz);
    float* w = applyAllocFloatArray(A->n); // weight vectors
    float wt = 0.0; // total weight


    for (int i = 1; i < A->n; i++) {
        w[i] = K / sqrtf(PowerOfTwo(A->dx[i]) + PowerOfTwo(A->dy[i]) + PowerOfTwo(A->dz[i]));
        wt += w[i];
    }

    for (int i = 1; i < A->n; i++) {
        w[i] = (w[i] / wt);
    }

    if (applyIsColorImage(img)) {
        for (int p = 0; p < img->n; p++) {
            Voxel u = applyGetVoxelCoord(img, p);
            double acc_dist = 0.0f;

            for (int i = 1; i < A->n; i++) {
                Voxel v = applyGetAdjacentVoxel(A, u, i);

                if (isValidVoxel(img, v)) {
                    int q = applyGetVoxelIndex(img, v);
                    acc_dist += w[i] * (abs(img->val[q] - img->val[p]) +
                        abs(img->Cb[q] - img->Cb[p]) +
                        abs(img->Cr[q] - img->Cr[p]));
                }
            }
            basins->val[p] = Round(acc_dist);
        }
    }
    else {
        for (int p = 0; p < img->n; p++) {
            Voxel u = applyGetVoxelCoord(img, p);
            float acc_dist = 0.0f;

            for (int i = 1; i < A->n; i++) {
                Voxel v = applyGetAdjacentVoxel(A, u, i);

                if (isValidVoxel(img, v)) {
                    int q = applyGetVoxelIndex(img, v);
                    acc_dist += w[i] * abs(img->val[q] - img->val[p]);
                }
            }
            basins->val[p] = Round(acc_dist);
        }
    }

    DestroyAdjRel(&A);
    applyFree(w);

    return basins;
}

void UpdateTreeAttributes(Image* M, int* N, Image* I, Image* root, int p) {
    int r = root->val[p];
    M->val[r] += I->val[p];
    N[r] += 1;

}

float TreeAttributesDist(Image* I, int q, int r, Image* M, int* N) {
    float dist = 0.0;

    dist += powf(I->val[q] - M->val[r] / N[r], 2.0);
    return(sqrtf(dist));
}

Image* ImageForest(Image* I, AdjRel* A, Image* S) {
    float arcw, arcv;
    int p, i, q, aux, r;
    Voxel u, v, w;
    Image* M = CreateImage(I->xsize, I->ysize, I->zsize);
    int* N = applyAllocIntArray(I->n);
    Image* root = CreateImage(I->xsize, I->ysize, I->zsize);
    Image* cost = CreateImage(I->xsize, I->ysize, I->zsize); // CopyImage(I); //
    Image* label = CreateImage(I->xsize, I->ysize, I->zsize);
    GQueue* Q = CreateGQueue(applyMaximumValue(I) + 1, I->n, cost->val);
    png_byte color_type;
    
    for (p = 0; p < cost->n; p++) {
        if (S->val[p] == 0) {
            cost->val[p] = IFT_INFINITY_INT;
        }
        else {
            cost->val[p] = 0;
            root->val[p] = p;
            InsertGQueue(&Q, p);
        }
    }


    while(!EmptyGQueue(Q)){
        p = RemoveGQueue(Q);
        u = applyGetVoxelCoord(I, p);
        r = root->val[p];
        UpdateTreeAttributes(M, N, I, root, p);
        for (i = 1; i < A->n; i++) {
            v = applyGetAdjacentVoxel(A, u, i);
            if (isValidVoxel(I, v)) {
                q = applyGetVoxelIndex(I, v);
                if(cost -> val[q] > cost -> val[p]){
                    

                    // Custo sendo brilho
                    size_t numberOfChannels = 1;
                    if (applyIsColorImage(I)) {
                        if (I->alpha == NULL) {
                            numberOfChannels = 3;//RGB
                            color_type = PNG_COLOR_TYPE_RGB;
                        }
                        else {
                            numberOfChannels = 4;//RGB_ALPHA
                            color_type = PNG_COLOR_TYPE_RGB_ALPHA;
                        }
                    }
                    else {
                        if (I->alpha == NULL) {
                            numberOfChannels = 1;//GRAY
                            color_type = PNG_COLOR_TYPE_GRAY;
                        }
                        else {
                            numberOfChannels = 2;//GRAY_ALPHA
                            color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
                        }
                    }
                    
                    //if (color_type == PNG_COLOR_TYPE_GRAY) {
                    //    //arcw = powf(I->val[p] - I->val[root->val[p]], 2.0);
                    //    arcw = powf(I->val[p] - I->val[q], 2.0);
                    //
                    //}           
                    //else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
                    //    arcw = powf(I->val[p] - I->val[q], 2.0) + powf(I->alpha[p] - I->alpha[q], 2.0);
                    //    //arcw = powf(I->val[p] - I->val[root->val[p]], 2.0) + powf(I->alpha[p] - I->alpha[root->val[p]], 2.0);
                    //}
                    //else if (color_type == PNG_COLOR_TYPE_RGB) {
                    //    arcw = powf(I->val[p] - I->val[q], 2.0) + powf(I->Cb[p] - I->Cb[q], 2.0) + powf(I->Cr[p] - I->Cr[q], 2.0);
                    //    //arcw = powf(I->val[p] - I->val[root->val[p]], 2.0) + powf(I->Cb[p] - I->Cb[root->val[p]], 2.0) + powf(I->Cr[p] - I->Cr[root->val[p]], 2.0);
                    //}
                    //else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
                    //    arcw = powf(I->val[p] - I->val[q], 2.0) + powf(I->Cb[p] - I->Cb[q], 2.0) + powf(I->Cr[p] - I->Cr[q], 2.0) + powf(I->alpha[p] - I->alpha[q], 2.0);
                    //}

                    arcw = TreeAttributesDist(I, q, r, M, N);
                    
                    //w = applyGetVoxelCoord(I, root->val[p]);
                    //arcv = applyLog(sqrtf(powf(u.x - w.x, 2.0) + powf(u.y - w.y, 2.0) + powf(u.z - w.z, 2.0) + powf(u.t - w.t, 2.0)), 2);
                    

                    //aux = Max(cost->val[p], (int)(applyLog(arcw, 2)));
                    aux = Max(cost->val[p], (int)(arcw));
                    if (aux < cost->val[q]) {
                        if (cost->val[q] != IFT_INFINITY_INT) {
                            RemoveGQueueElem(Q, q);
                        }
                        cost->val[q] = aux; 
                        label->val[q] = label->val[p];
                        root->val[q] = root->val[p];
                        InsertGQueue(&Q, q);
                    }
                }
            }
        }
    
    }

    DestroyImage(&M);
    applyFree(N);
    DestroyImage(&root);
    DestroyGQueue(&Q);

    return(cost);
}


double DiceSimilarity(Image* bin_source, Image* bin_target) {

    ulong vol_intersec = 0; // |intersection(volume(bin_source), volume(bin_target))|
    ulong vol_sum = 0; // |volume(bin_source)| + |volume(bin_target)|

    for (size_t i = 0; i < bin_target->n; i++) {
        vol_intersec += ((bin_source->val[i] != 0) && (bin_target->val[i] != 0));
        vol_sum += ((bin_source->val[i] != 0) + (bin_target->val[i] != 0));
    }

    double dice = (2.0 * vol_intersec) / (vol_sum);

    return dice;
}


