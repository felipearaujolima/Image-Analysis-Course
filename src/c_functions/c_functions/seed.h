#ifndef _SEED_H_
#define _SEED_H_

#include "common.h"
#include "image.h"
#include "adjacency.h"

int RootVoxel(Image* pred, int p);
Image* LabelComp(Image* bin, AdjRel* A);
Image* ColorizeComp(Image* label);
Image* ComponentArea(Image* bin, AdjRel* Ain);



#endif
