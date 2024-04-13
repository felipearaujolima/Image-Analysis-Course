#include "seed.h"

int RootVoxel(Image* pred, int p)
{
    if (pred->val[p] == p)
        return(p);
    else
        return(pred->val[p] = RootVoxel(pred, pred->val[p])); 
}

Image* LabelComp(Image* bin, AdjRel* A)
{
    Image* label = NULL, * pred = NULL;
    int l = 1;

    pred = CreateImage(bin->xsize, bin->ysize, bin->zsize);

    for (int p = 0; p < pred->n; p++)
        pred->val[p] = p;


    for (int p = 0; p < bin->n; p++) {
        if (bin->val[p] != 0) {
            int rp = RootVoxel(pred, p);
            Voxel u = applyGetVoxelCoord(bin, p);
            for (int i = 1; i < A->n; i++) {
                Voxel v = applyGetAdjacentVoxel(A, u, i);
                if (isValidVoxel(bin, v)) {
                    int q = applyGetVoxelIndex(bin, v);
                    if (bin->val[q] != 0) {
                        int rq = RootVoxel(pred, q);
                        if (rq != rp) {
                            if (rp < rq)
                                pred->val[rq] = rp;
                            else
                                pred->val[rp] = rq;
                        }
                    }
                }
            }
        }
    }

    label = CreateImage(bin->xsize, bin->ysize, bin->zsize);

    for (int p = 0; p < pred->n; p++) {
        if (bin->val[p] != 0) {
            if (pred->val[p] == p) {
                label->val[p] = l; l++;
            }
            else {
                if (pred->val[pred->val[p]] != pred->val[p])
                    pred->val[p] = RootVoxel(pred, p);
            }
        }
    }


    for (int p = 0; p < pred->n; p++)
        if ((bin->val[p] != 0) && (pred->val[p] != p)) {
            label->val[p] = label->val[pred->val[p]];
        }

    CopyVoxelSize(bin, label);
    DestroyImage(&pred);



    return(label);
}

Image* ColorizeComp(Image* label)
{

    ColorTable* ctb;
    Image* img;
    int p;

    ctb = CreateRandomColorTable(applyMaximumValue(label) + 1);
    img = CreateImage(label->xsize, label->ysize, label->zsize);
    SetImage(img, 255);
    applySetCbCr(img, 128);

    for (p = 0; p < label->n; p++)
        if (label->val[p] > 0) {
            img->val[p] = ctb->color[label->val[p]].val[0];
            img->Cb[p] = ctb->color[label->val[p]].val[1];
            img->Cr[p] = ctb->color[label->val[p]].val[2];
        }
    DestroyColorTable(&ctb);
    return(img);
}

Image* ComponentArea(Image* bin, AdjRel* Ain)
{
    Image* label;
    int* size, ncomps, p;
    AdjRel* A;

    if (Ain == NULL) {
        A = adjCircular(1.42);
    }
    else {
        A = Ain;
    }

    label = LabelComp(bin, A);
    ncomps = applyMaximumValue(label);
    size = applyAllocIntArray(ncomps + 1);
    for (p = 0; p < label->n; p++)
        if (label->val[p] != 0)
            size[label->val[p]]++;

    for (p = 0; p < label->n; p++)
        label->val[p] = size[label->val[p]];

    applyFree(size);
    CopyVoxelSize(bin, label);

    if (Ain == NULL)
        DestroyAdjRel(&A);

    return(label);
}
