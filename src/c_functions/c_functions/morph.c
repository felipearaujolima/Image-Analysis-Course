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


Image* applySub(Image* img1, Image* img2) {

	Image* sub_img = CreateImageFromImage(img1);

	if (applyIsColorImage(img1)) {
		for (int p = 0; p < img1->n; p++) {
			sub_img->val[p] = img1->val[p] - img2->val[p];
			sub_img->Cb[p] = img1->Cb[p];
			sub_img->Cr[p] = img1->Cr[p];
		}
	}
	else {
		for (int p = 0; p < img1->n; p++) {
			sub_img->val[p] = img1->val[p] - img2->val[p];
		}
	}

	return sub_img;
}

Image* applyThreshold(Image* img, int lowest, int highest, int value) {
	Image* bin = CreateImageFromImage(img);

	for (int p = 0; p < img->n; p++)
		if ((img->val[p] >= lowest) && (img->val[p] <= highest))
			bin->val[p] = value;
		else bin->val[p] = 0;

	return bin;
}

Image* applyClose(Image* img, AdjRel* A)
{
	Image* dil, * ero;

	dil = applyDilate(img, A);
	ero = applyErode(dil, A);
	DestroyImage(&dil);
	return(ero);

}

Image* applyOpen(Image* img, AdjRel* A)
{
	Image* dil, * ero;

	ero = applyErode(img, A);
	dil = applyDilate(ero, A);
	DestroyImage(&ero);
	return(dil);

}