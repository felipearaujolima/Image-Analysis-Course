#ifndef _MORPH_H_
#define _MORPH_H_

#include "common.h"
#include "image.h"
#include "adjacency.h"

Image* applyDilate(Image* img, AdjRel* A);
Image* applyErode(Image* img, AdjRel* A);


#endif