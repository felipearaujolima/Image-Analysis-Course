#ifndef _MORPH_H_
#define _MORPH_H_

#include "common.h"
#include "image.h"
#include "adjacency.h"

Image* applyDilate(Image* img, AdjRel* A);
Image* applyErode(Image* img, AdjRel* A);
Image* applySub(Image* img1, Image* img2);
Image* applyThreshold(Image* img, int lowest, int highest, int value);
Image* applyClose(Image* img, AdjRel* A);
Image* applyOpen(Image* img, AdjRel* A);

#endif