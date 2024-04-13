#include "kernel.h"


Kernel* AllocKernel(AdjRel* A, int* list)
{      

	int new_order[] = { 4, 0, 3, 6, 1, 7, 2, 5, 8 };

	int* filter_list= (int*)malloc(A ->n * sizeof(int));

	for (int i = 0; i < A -> n; i++) {
		filter_list[i] = list[new_order[i]];
	}

	Kernel* K = CreateKernel(A);
	int i;
	
	for (i = 0; i < A->n; i++) {
		K->weight[i] = filter_list[i];
	}

	DestroyAdjRel(&A);
	free(filter_list);
	return(K);
}

Kernel* CreateKernel(AdjRel* A)
{
	Kernel* K = (Kernel*)applyAlloc(1, sizeof(Kernel));

	K->A = CopyAdjacency(A);
	K->weight = applyAllocFloatArray(K->A->n);

	return(K);
}

void DestroyKernel(Kernel** K)
{
	Kernel* aux = *K;

	if (aux != NULL) {
		DestroyAdjRel(&aux->A);
		applyFree(aux->weight);
		applyFree(aux);
		*K = NULL;
	}
}

MKernel* CreateMKernel(AdjRel* A, int nbands) {
	MKernel* kernel = (MKernel*)applyAlloc(1, sizeof(MKernel));

	kernel->A = CopyAdjacency(A);
	kernel->nbands = nbands;

	kernel->weight = (Band*)applyAlloc(nbands, sizeof(Band));

	int i;
	for (i = 0; i < nbands; i++) {
		kernel->weight[i].val = applyAllocFloatArray(A->n);
	}

	return kernel;
}

void DestroyMKernel(MKernel** K) {
	MKernel* kernel = *K;

	int i;
	for (i = 0; i < kernel->nbands; i++)
		applyFree(kernel->weight[i].val);
	applyFree(kernel->weight);

	DestroyAdjRel(&kernel->A);

	applyFree(kernel);
	*K = 0;
}

MKernel* CopyMKernel(MKernel* K) {
	MKernel* cpy = NULL;

	cpy = CreateMKernel(K->A, K->nbands);

	for (int i = 0; i < K->nbands; i++) {
		for (int j = 0; j < K->A->n; j++) {
			cpy->weight[i].val[j] = K->weight[i].val[j];
		}
	}

	return cpy;
}

Image* LinearFilter(Image* img, Kernel* K)
{
	Image* fimg = CopyImage(img);
	AdjRel* A = K->A;

	if (applyIsColorImage(img)) {
		for (int p = 0; p < img->n; p++) {
			Voxel u = applyGetVoxelCoord(img, p);
			if (u.x >= 1 & u.x < img->xsize - 1 & u.y >= 1 & u.y < img->ysize - 1) {
				float val1 = 0.0, val2 = 0.0, val3 = 0.0;
				for (int i = 0; i < A->n; i++) {
					Voxel v = applyGetAdjacentVoxel(A, u, i);
					if (isValidVoxel(img, v)) {
						int q = applyGetVoxelIndex(img, v);
						val1 += ((float)img->val[q] * K->weight[i]);
						val2 += ((float)img->Cb[q] * K->weight[i]);
						val3 += ((float)img->Cr[q] * K->weight[i]);
					}
				}
				fimg->val[p] = Round(val1);
				fimg->Cb[p] = Round(val2);
				fimg->Cr[p] = Round(val3);
			}
		}
	}
	else {
		for (int p = 0; p < img->n; p++) {
			Voxel u = applyGetVoxelCoord(img, p);
			if (u.x >= 1 & u.x < img->xsize - 1 & u.y >= 1 & u.y < img->ysize - 1) {
				float val = 0.0;
				for (int i = 0; i < A->n; i++) {
					Voxel v = applyGetAdjacentVoxel(A, u, i);
					if (isValidVoxel(img, v)) {
						int q = applyGetVoxelIndex(img, v);
						val += ((float)img->val[q] * K->weight[i]);
					}
				}
				fimg->val[p] = Round(val);
			}
		}
	}

	return(fimg);
}