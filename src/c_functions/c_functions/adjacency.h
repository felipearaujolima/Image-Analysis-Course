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

/* For stuff */
Voxel applyGetAdjacentVoxel(AdjRel* A, Voxel u, int adj);
AdjRel* CopyAdjacency(AdjRel* A);

/* For creating and destroing adj */
AdjRel* CreateAdjRel(int n);
void DestroyAdjRel(AdjRel** A);

/* Creating rectangular and circular adj's */
AdjRel* adjRectangular(int xsize, int ysize);
AdjRel* adjCircular(float r);

#endif