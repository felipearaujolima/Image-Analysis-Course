#include "morph.h"

Image* applyDilate(Image* img, AdjRel* A)
{
	Image* dil = CreateImage(img->xsize, img->ysize, img->zsize);

	for (int p = 0; p < img->n; p++) {
		Voxel u = applyGetVoxelCoord(img, p);
		dil->val[p] = img->val[p];
		for (int i = 1; i < A->n; i++) {
			Voxel v = applyGetAdjacentVoxel(A, u, i);
			if (isValidVoxel(img, v)) {
				int q = applyGetVoxelIndex(img, v);
				if (img->val[q] > dil->val[p])
					dil->val[p] = img->val[q];
			}
		}
	}
	
	if (applyIsColorImage(img))
		CopyCbCr(img, dil);
	CopyVoxelSize(img, dil);

	return(dil);
}


Image* applyErode(Image* img, AdjRel* A)
{
	Image* ero = CreateImage(img->xsize, img->ysize, img->zsize);

	for (int p = 0; p < img->n; p++) {
		Voxel u = applyGetVoxelCoord(img, p);
		ero->val[p] = img->val[p];
		for (int i = 1; i < A->n; i++) {
			Voxel v = applyGetAdjacentVoxel(A, u, i);
			if (isValidVoxel(img, v)) {
				int q = applyGetVoxelIndex(img, v);
				if (img->val[q] < ero->val[p])
					ero->val[p] = img->val[q];
			}
		}
	}
	

	if (applyIsColorImage(img))
		CopyCbCr(img, ero);
	CopyVoxelSize(img, ero);

	return(ero);
}