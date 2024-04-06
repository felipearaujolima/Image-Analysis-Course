#ifndef _ADJACENCY_H_
#define _ADJACENCY_H_

#include "common.h"

typedef struct adjrel {
    int* dx, * dy, * dz, * dt;
    /* displacements to achieve the n adjacent voxels. */
    int n; /* number of adjacent voxels. */
} AdjRel;

typedef struct fastadjrel {
    int n;
    /* number of adjacent voxels */
    int* dq;
    /* displacements to reach adjacent voxels for a given image */
    int bx, by, bz; /* sizes of the image's border to be disconsidered */
} FastAdjRel;

AdjRel* CreateAdjRel(int n);
void DestroyAdjRel(AdjRel** A);
AdjRel* iftRectangular(int xsize, int ysize);
AdjRel* iftCircular(float r);

#endif