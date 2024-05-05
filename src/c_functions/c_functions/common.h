#ifndef _COMMON_H_
#define _COMMON_H_

#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define IFT_INFINITY_INT       INT_MAX
#define IFT_INFINITY_INT_NEG   INT_MIN
#define IFT_RANDOM_SEED (unsigned int) 42
#define IFT_MAXWEIGHT     4095.0
#define IFT_PI      3.14159265358979323846
#define IFT_WHITE       0
#define IFT_GRAY        1
#define IFT_BLACK       2
#define IFT_NIL        -1
#define IFT_INCREASING  1
#define IFT_DECREASING  0
#define IFT_EPSILON     1E-07

#define Round(x) ((x < 0)?(int)(x-0.5):(int)(x+0.5))
#define Min(x,y) (((x) < (y))?(x):(y))
#define Max(x,y) (((x) > (y))?(x):(y))
#define PowerOfTwo(x) ((x) * (x))

typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef unsigned long  ulong;

typedef struct voxel {
    int x, y, z, t;
} Voxel;

typedef struct int_array { 
    long n;
    int* val;
} IntArray;

typedef struct color {
	int val[3];
	float alpha;
} Color;

typedef struct band {
    float* val;
} Band;

typedef struct colortable {
    Color* color;
    int ncolors;
} ColorTable;

/* Miscellaneous */
void RandomSeed(unsigned int seed);
int RandomInteger(int low, int high);
double applyLog(double val, double base);
IntArray* CreateIntArray(long n);
IntArray* IntRange(int begin, int end, int inc);
void DestroyIntArray(IntArray** iarr);
ColorTable* CreateRandomColorTable(int n_colors);
void DestroyColorTable(ColorTable** ctb);
int SafeMod(int a, int n);
/* Memory functions */
void applyFree(void* data);
void* applyAlloc(size_t n, size_t size);
int* applyAllocIntArray(long n);
float* applyAllocFloatArray(long n);
ushort* applyAllocUShortArray(long n);
char* applyAllocCharArray(long n);
/* Color functions */
Color applyRGBtoYCbCrBT2020(Color cin, int rgbBitDepth, int yCbCrBitDepth);
Color applyRGBtoYCbCr(Color cin, int normalization_value);
Color applyYCbCrBT2020toRGB(Color cin, int yCbCrBitDepth, int rgbBitDepth);

#endif