#include "common.h"


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

/* Create/Read image functions */
Image* CreateImage(int xsize, int ysize, int zsize);
Image* applyCreateImageFromBuffer(int xsize, int ysize, int zsize, int* val);
png_bytep* ReadPngImageAux(const char* file_name, png_structp* png_ptr, png_infop* info_ptr);
Image* ReadImage(char* file_name);

/* Destroy/Write image functions */
void DestroyImage(Image** img);
void WritePngImageAux(const char* file_name, png_bytep* row_pointers, int width, int height, int bit_depth, int color_type);
void WriteImagePNG(Image* img, const char* file_name);
