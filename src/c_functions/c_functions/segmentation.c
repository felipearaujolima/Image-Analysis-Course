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

//Image* ImageForest(Image* I, AdjRel* A, Image* V_in) {
//
//    Image* V = CopyImage(V_in);
//    //Image* Pr = CopyImage(I);
//    int      q, p, w, i, cost_expand, aux_euclidian;
//    Voxel    p_coord, w_coord;
//    png_byte color_type;
//    FIFO* F = NULL;
//    
//    F = CreateFIFO(I->n);
//
//
//    for (q = 0; q < I->n; q++)
//    {
//        printf("%d \n", q);
//        //Pr->val[q] = IFT_NIL;
//
//        if (V_in->val[q] > 0) {
//            V->val[q] = 0;
//        }
//        else {
//            V->val[q] = IFT_INFINITY_INT;
//        }
//
//        InsertFIFO(F, q, V->val[q]);
//    }
//
//    while (!EmptyFIFO(F)) {
//
//        p = RemoveFIFO(F);
//        printf("%d \n", p);
//        p_coord = applyGetVoxelCoord(I, p);
//        for (i = 1; i < A->n; i++) {
//            w_coord = applyGetAdjacentVoxel(A, p_coord, i);
//            if (isValidVoxel(I, w_coord)) {
//                w = applyGetVoxelIndex(I, w_coord);
//
//                size_t numberOfChannels = 1;
//                if (applyIsColorImage(I)) {
//                    if (I->alpha == NULL) {
//                        numberOfChannels = 3;//RGB
//                        color_type = PNG_COLOR_TYPE_RGB;
//                    }
//                    else {
//                        numberOfChannels = 4;//RGB_ALPHA
//                        color_type = PNG_COLOR_TYPE_RGB_ALPHA;
//                    }
//                }
//                else {
//                    if (I->alpha == NULL) {
//                        numberOfChannels = 1;//GRAY
//                        color_type = PNG_COLOR_TYPE_GRAY;
//                    }
//                    else {
//                        numberOfChannels = 2;//GRAY_ALPHA
//                        color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
//                    }
//                }
//
//                if (color_type == PNG_COLOR_TYPE_GRAY) {
//                    aux_euclidian = PowerOfTwo(I->val[p] - I->val[w]);
//
//                }           
//                else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
//                    aux_euclidian = PowerOfTwo(I->val[p] - I->val[w]) + PowerOfTwo(I->alpha[p] - I->alpha[w]);
//                }
//                else if (color_type == PNG_COLOR_TYPE_RGB) {
//                    aux_euclidian = PowerOfTwo(I->val[p] - I->val[q]) + PowerOfTwo(I->Cb[p] - I->Cb[w]) + PowerOfTwo(I->Cr[p] - I->Cr[w]);
//                }
//                else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
//                    aux_euclidian = PowerOfTwo(I->val[p] - I->val[w]) + PowerOfTwo(I->Cb[p] - I->Cb[w]) + PowerOfTwo(I->Cr[p] - I->Cr[w]) + PowerOfTwo(I->alpha[p] - I->alpha[w]);
//                }
//
//                cost_expand = Max(I -> val[p], aux_euclidian);
//                if(V -> val[w] > cost_expand) {
//                    V->val[w] = cost_expand;
//                    if (IsInFIFO(F, w)) {
//                        UpdateWeightFIFO(F, w, V->val[w]);
//                    }
//                        
//                }
//            }
//
//        }
//    }
//
//    return (V);
//}



//Image* ImageForest(Image* img, AdjRel* Ain, Image* seeds) {
//    
//    Image* pathval = CopyImage(seeds);
//    Image* label = NULL;
//    GQueue* Q = NULL;
//
//
//    int      i, p, q, aux, aux_euclidian;
//    Voxel    u, v;
//    png_byte color_type;
//
//
//
//    AdjRel* A = NULL;
//    A = Ain;
//
//    // Initialization
//    label = CreateImage(img->xsize, img->ysize, img->zsize);
//    Q = CreateGQueue(applyMaximumValue(img) + 1, img->n, pathval->val);
//
//    for (p = 0; p < img->n; p++)
//    {
//        if (pathval->val[p] == 255) {
//            pathval->val[p] = 0;
//        }
//        else {
//            pathval->val[p] = IFT_INFINITY_INT;
//        }
//    }
//
//    // Image Foresting Transform
//    
//    while (!EmptyGQueue(Q))
//    {
//        p = RemoveGQueue(Q);
//        u = applyGetVoxelCoord(img, p);
//
//        for (i = 1; i < A->n; i++)
//        {
//            v = applyGetAdjacentVoxel(A, u, i);
//
//            if (isValidVoxel(img, v))
//            {
//                q = applyGetVoxelIndex(img, v);
//
//                size_t numberOfChannels = 1;
//                if (applyIsColorImage(img)) {
//                    if (img->alpha == NULL) {
//                        numberOfChannels = 3;//RGB
//                        color_type = PNG_COLOR_TYPE_RGB;
//                    }
//                    else {
//                        numberOfChannels = 4;//RGB_ALPHA
//                        color_type = PNG_COLOR_TYPE_RGB_ALPHA;
//                    }
//                }
//                else {
//                    if (img->alpha == NULL) {
//                        numberOfChannels = 1;//GRAY
//                        color_type = PNG_COLOR_TYPE_GRAY;
//                    }
//                    else {
//                        numberOfChannels = 2;//GRAY_ALPHA
//                        color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
//                    }
//                }
//
//                if (color_type == PNG_COLOR_TYPE_GRAY) {
//                    aux_euclidian = PowerOfTwo(img->val[p] - img->val[q]);
//
//                }
//                else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
//                    aux_euclidian = PowerOfTwo(img->val[p] - img->val[q]) + PowerOfTwo(img->alpha[p] - img->alpha[q]);
//                }
//                else if (color_type == PNG_COLOR_TYPE_RGB) {
//                    aux_euclidian = PowerOfTwo(img->val[p] - img->val[q]) + PowerOfTwo(img->Cb[p] - img->Cb[q]) + PowerOfTwo(img->Cr[p] - img->Cr[q]);
//                }
//                else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
//                    aux_euclidian = PowerOfTwo(img->val[p] - img->val[q]) + PowerOfTwo(img->Cb[p] - img->Cb[q]) + PowerOfTwo(img->Cr[p] - img->Cr[q]) + PowerOfTwo(img->alpha[p] - img->alpha[q]);
//                }
//
//                aux = Max(pathval->val[p], aux_euclidian);
//
//                if (pathval->val[q] > aux)
//                {
//                    label->val[q] = label->val[p];
//                    pathval->val[q] = aux;
//                    //InsertGQueue(&Q, q);
//
//                }
//            }
//        }
//    }
//
//    if (Ain == NULL) {
//        DestroyAdjRel(&A);
//    }
//    DestroyGQueue(&Q);
//    DestroyImage(&label);
//
//    //CopyVoxelSize(img, label);
//    //CopyVoxelSize(img, pathval);
//
//    return (pathval);
//}


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
    int p, i, q, aux;
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
                    
                    if (color_type == PNG_COLOR_TYPE_GRAY) {
                        arcw = powf(I->val[p] - I->val[root->val[p]], 2.0);
                        //arcw = powf(I->val[p] - I->val[q], 2.0);
                    
                    }           
                    else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
                        //arcw = powf(I->val[p] - I->val[q], 2.0) + powf(I->alpha[p] - I->alpha[q], 2.0);
                        arcw = powf(I->val[p] - I->val[root->val[p]], 2.0) + powf(I->alpha[p] - I->alpha[root->val[p]], 2.0);
                    }
                    else if (color_type == PNG_COLOR_TYPE_RGB) {
                        //arcw = powf(I->val[p] - I->val[q], 2.0) + powf(I->Cb[p] - I->Cb[q], 2.0) + powf(I->Cr[p] - I->Cr[q], 2.0);
                        arcw = powf(I->val[p] - I->val[root->val[p]], 2.0) + powf(I->Cb[p] - I->Cb[root->val[p]], 2.0) + powf(I->Cr[p] - I->Cr[root->val[p]], 2.0);
                    }
                    else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
                        arcw = powf(I->val[p] - I->val[q], 2.0) + powf(I->Cb[p] - I->Cb[q], 2.0) + powf(I->Cr[p] - I->Cr[q], 2.0) + powf(I->alpha[p] - I->alpha[q], 2.0);
                    }

                    //arcw = TreeAttributesDist(I, q, p, M, N);
                    
                    //w = applyGetVoxelCoord(I, root->val[p]);
                    //arcv = applyLog(sqrtf(powf(u.x - w.x, 2.0) + powf(u.y - w.y, 2.0) + powf(u.z - w.z, 2.0) + powf(u.t - w.t, 2.0)), 2);
                    

                    aux = Max(cost->val[p], (int)(applyLog(arcw, 2)));
                    //aux = Max(cost->val[p], (int)(arcw));
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