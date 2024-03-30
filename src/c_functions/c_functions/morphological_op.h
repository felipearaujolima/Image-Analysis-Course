#ifndef _MORPHOLOGICAL_OP_H_
#define _MORPHOLOGICAL_OP_H_

#include "image.h"
#include "adjacency.h"

void applyDilation(Image* img, Adjacency* adj, char* name_img);
void applyErosion(Image* img, Adjacency* adj, char* name_img);

#endif