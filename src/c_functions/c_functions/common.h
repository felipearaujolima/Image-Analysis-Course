#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef unsigned short ushort;
typedef unsigned char  uchar;

typedef struct color {
	int val[3];
	float alpha;
} Color;

/* Miscellaneous */
double applyLog(double val, double base);

/* Memory functions */
void applyFree(void* data);
void* applyAlloc(size_t n, size_t size);
int* applyAllocIntArray(long n);
ushort* applyAllocUShortArray(long n);

/* Color functions */
Color applyRGBtoYCbCrBT2020(Color cin, int rgbBitDepth, int yCbCrBitDepth);
Color applyRGBtoYCbCr(Color cin, int normalization_value);
Color applyYCbCrBT2020toRGB(Color cin, int yCbCrBitDepth, int rgbBitDepth);

