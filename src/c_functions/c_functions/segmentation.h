#ifndef _SEGMENTATION_H_
#define _SEGMENTATION_H_

#include "common.h"
#include "image.h"
#include "adjacency.h"
#include "queue.h"

Image* ImageBasins(Image* img, AdjRel* Ain);
void UpdateTreeAttributes(Image* M, int* N, Image* I, Image* root, int p);
float TreeAttributesDist(Image* I, int q, int r, Image* M, int* N);
Image* ImageForest(Image* I, AdjRel* A, Image* S);

#endif