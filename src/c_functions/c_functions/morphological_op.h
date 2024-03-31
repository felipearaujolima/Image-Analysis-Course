#ifndef _MORPHOLOGICAL_OP_H_
#define _MORPHOLOGICAL_OP_H_

#include "image.h"
#include "adjacency.h"

void applyDilation(Image* img, Adjacency* adj, char* name_img);
void applyErosion(Image* img, Adjacency* adj, char* name_img);
void applyThreshold(Image* img, int thr, int min, int max);
void applySubtraction(Image* img_1, Image* img_2, Image* out);
Image* applyMorphologicalGradients(char* name_img, Adjacency* adj, char* type);
Image* applyClosingOperation(Image* img, Adjacency* adj, char* name_img);
Image* applyOpeningOperation(Image* img, Adjacency* adj, char* name_img);
int applyFindMin(Image* img);
int applyFindMax(Image* img);
void applyAbs(Image* img);
void applyCalculateHistogram(Image* img, int* histogram);
void applyCalculateCDF(int* histogram, int* cdf, int N);
void applyLinearStreching(Image* img);

#endif