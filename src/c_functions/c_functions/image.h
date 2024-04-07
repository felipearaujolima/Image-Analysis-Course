#ifndef _IMAGE_H_
#define _IMAGE_H_


#include "common.h"
#define applyGetVoxelIndex(s, v) ((v.x)+(s)->tby[(v.y)]+(s)->tbz[(v.z)])
#define isValidVoxel(img, v)((v.x >= 0) && (v.x <= ((img)->xsize - 1)) && (v.y >= 0) && (v.y <= ((img)->ysize - 1)) && (v.z >= 0) && (v.z <= ((img)->zsize - 1)))
#define CopyVoxelSize(src, dst) (dst)->dx = (src)->dx; (dst)->dy = (src)->dy; (dst)->dz = (src)->dz;

typedef struct image {
    /** Brightness pixels array. */
    int* val;
    /** Blue component pixels array */
    ushort* Cb;
    /** Red component pixels array */
    ushort* Cr;
    /** alpha component pixels array */
    ushort* alpha;

    /** X axis size. */
    int xsize;
    /** Y axis size. */
    int ysize;
    /** Z axis size. */
    int zsize;

    /** X axis voxel size. */
    float dx;
    /** Y axis voxel size. */
    float dy;
    /** Z axis voxel size. */
    float dz;

    /** speed-up voxel access tables */
    int* tby, * tbz;
    /** Number of pixels. */
    int n; // number of voxels
} Image;

/* Miscellaneous */
long applyNormalizationValue(long maxval);
void applyMinMaxValues(Image* img, int* min, int* max);

uchar applyImageDepth(Image* img);
bool applyIsColorImage(Image* img);
void  applySetCbCr(Image* img, ushort value);
void  applySetAlpha(Image* img, ushort value);

/* For image working */
long MaxImageRange(uchar img_depth);
Voxel applyGetVoxelCoord(Image* img, int p);
void CopyCbCr(Image* src, Image* dst);
Image* CreateColorImage(int xsize, int ysize, int zsize, int depth);
Image* CreateImageFromImage(Image* src);

/* Create/Read image functions */
Image* CreateImage(int xsize, int ysize, int zsize);
Image* applyCreateImageFromBuffer(int xsize, int ysize, int zsize, int* val);
png_bytep* ReadPngImageAux(const char* file_name, png_structp* png_ptr, png_infop* info_ptr);
Image* ReadImage(char* file_name);

/* Destroy/Write image functions */
void DestroyImage(Image** img);
void WritePngImageAux(const char* file_name, png_bytep* row_pointers, int width, int height, int bit_depth, int color_type);
void WriteImagePNG(Image* img, const char* file_name);

#endif