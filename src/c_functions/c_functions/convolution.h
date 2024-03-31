#ifndef _CONVOLUTION_H_
#define _CONVOLUTION_H_

#include <malloc.h>
#include "image.h"
#include "adjacency.h"

typedef struct _convolution { 
	int k;
	int nadj;
	int* dx, * dy;
	int* w;

} Kernel;
Kernel* CreateKernel(int k, int* filter);
Image* applySimpleKernel(Image* img, Kernel* kernel, char* name_img);
void DestroyKernel(Kernel** kernel);

#endif
