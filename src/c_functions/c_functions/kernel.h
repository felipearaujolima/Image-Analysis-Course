#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "common.h"
#include "adjacency.h"
#include "image.h"


typedef struct kernel {
	AdjRel* A;
	float* weight;
} Kernel;

typedef struct mkernel {
	AdjRel* A;
	Band* weight;
	int        nbands;
} MKernel;

Kernel* SobelXKernel2D();
Kernel* AllocKernel(AdjRel* A, int* list);
Kernel* CreateKernel(AdjRel* A);
void DestroyKernel(Kernel** K);
MKernel* CreateMKernel(AdjRel* A, int nbands);
void DestroyMKernel(MKernel** K);
MKernel* CopyMKernel(MKernel* K);
Image* LinearFilter(Image* img, Kernel* K);
#endif
