#include "common.h"


/* Miscellaneous */
void RandomSeed(unsigned int seed)
{

    srand(seed);

}
int RandomInteger(int low, int high) {
    int k;
    double d;

    d = (double)rand() / ((double)RAND_MAX + 0.5);
    k = Min((int)(d * (high - low + 1.0)) + low, high);

    return k;
}
double applyLog(double val, double base) {
    return (log(val) / log(base));
}
IntArray* CreateIntArray(long n) {
    IntArray* iarr = (IntArray*)applyAlloc(1, sizeof(IntArray));

    iarr->n = n;
    iarr->val = applyAllocIntArray(n);

    return iarr;
}
IntArray* IntRange(int begin, int end, int inc) {
    int n = ((end - begin) / inc) + 1;

    IntArray* space = CreateIntArray(n);

    for (int i = 0; i < n; ++i) {
        space->val[i] = begin + (inc * i);
    }

    return space;
}
void DestroyIntArray(IntArray **iarr) {
    if (iarr != NULL && *iarr != NULL) {
        IntArray *iarr_aux = *iarr;
        
        if (iarr_aux->val != NULL)
            applyFree(iarr_aux->val);
        applyFree(iarr_aux);
        *iarr = NULL;
    }
}
ColorTable* CreateRandomColorTable(int n_colors) {

    ColorTable* ctb = (ColorTable*)applyAlloc(1, sizeof(ColorTable));
    ctb->ncolors = n_colors;
    ctb->color = (Color*)applyAlloc(n_colors, sizeof(Color));

    RandomSeed(time(NULL));

    for (int c = 0; c < n_colors; c++) {
        ctb->color[c].val[0] = RandomInteger(0, 255);
        ctb->color[c].val[1] = RandomInteger(0, 255);
        ctb->color[c].val[2] = RandomInteger(0, 255);
        ctb->color[c] = applyRGBtoYCbCr(ctb->color[c], 255);
    }

    return ctb;
}
void DestroyColorTable(ColorTable** ctb)
{
    ColorTable* aux = *ctb;

    if (aux != NULL) {
        applyFree(aux->color);
        applyFree(aux);
        *ctb = NULL;
    }

}
/* Memory functions */
void applyFree(void* data) {
    //so you can just call free for NULL pointer, and be happy :)
    if (data != NULL)
        free(data);
}
void* applyAlloc(size_t n, size_t size) {
    return calloc(n, size);
}
int* applyAllocIntArray(long n) {
    int* v = NULL;

    v = (int*)applyAlloc(n, sizeof(int));
    return(v);
}
float* applyAllocFloatArray(long n) {
    float* v = NULL;
    v = (float*)applyAlloc(n, sizeof(float));
    return(v);
}
ushort* applyAllocUShortArray(long n) {
    ushort* v = NULL;

    v = (ushort*)applyAlloc(n, sizeof(ushort));
    return(v);
}

/* Color functions */
Color applyRGBtoYCbCr(Color cin, int normalization_value)
{
    Color cout;
    float a = (16.0 / 255.0) * (float)normalization_value;
    float b = (128.0 / 255.0) * (float)normalization_value;

    cout.val[0] = (int)(0.256789062 * (float)cin.val[0] +
        0.504128906 * (float)cin.val[1] +
        0.09790625 * (float)cin.val[2] + a);
    cout.val[1] = (int)(-0.148222656 * (float)cin.val[0] +
        -0.290992187 * (float)cin.val[1] +
        0.439214844 * (float)cin.val[2] + b);
    cout.val[2] = (int)(0.439214844 * (float)cin.val[0] +
        -0.367789063 * (float)cin.val[1] +
        -0.071425781 * (float)cin.val[2] + b);

    for (int i = 0; i < 3; i++) {
        if (cout.val[i] < 0) cout.val[i] = 0;
        if (cout.val[i] > normalization_value) cout.val[i] = normalization_value;
    }

    return(cout);
}
Color applyRGBtoYCbCrBT2020(Color cin, int rgbBitDepth, int yCbCrBitDepth)
{
    int minLum, minChr, quantLum, quantChr;
    Color cout;

    switch (yCbCrBitDepth) {
    case 8:
        minLum = 16; // 16 * 2^(bitDepth-8)
        minChr = 128; // 128 * 2^(bitDepth-8)
        quantLum = 219.0; // 219 * 2^(bitDepth-8)
        quantChr = 224.0;  // 224 * 2^(bitDepth-8)
        break;
    case 10:
        minLum = 64; // 16 * 2^(bitDepth-8)
        minChr = 512; // 128 * 2^(bitDepth-8)
        quantLum = 876; // 219 * 2^(bitDepth-8)
        quantChr = 896;  // 224 * 2^(bitDepth-8)
        break;
    case 12:
        minLum = 256; // 16 * 2^(bitDepth-8)
        minChr = 2048; // 128 * 2^(bitDepth-8)
        quantLum = 3504; // 219 * 2^(bitDepth-8)
        quantChr = 3584;  // 224 * 2^(bitDepth-8)
        break;
    case 16:
        minLum = 4096; // 16 * 2^(bitDepth-8)
        minChr = 32768; // 128 * 2^(bitDepth-8)
        quantLum = 56064.0; // 219 * 2^(bitDepth-8)
        quantChr = 57344.0;  // 224 * 2^(bitDepth-8)
        break;
    default:
        cout.val[0] = cout.val[1] = cout.val[2] = 0;
        return cout;
    }

    double maxRgbValue = (double)((1 << rgbBitDepth) - 1);
    double r = cin.val[0] / maxRgbValue;
    double g = cin.val[1] / maxRgbValue;
    double b = cin.val[2] / maxRgbValue;

    double y = 0.2627 * r + 0.6780 * g + 0.0593 * b;
    double cb = (b - y) / 1.8814;
    double cr = (r - y) / 1.4746;

    // clip luminance to [0..1] and chrominance to [-0.5..0.5]
    if (y < 0.0) y = 0.0;
    else if (y > 1.0) y = 1.0;
    if (cb < -0.5) cb = -0.5;
    else if (cb > 0.5) cb = 0.5;
    if (cr < -0.5) cr = -0.5;
    else if (cr > 0.5) cr = 0.5;

    // perform quantization
    cout.val[0] = (int)(y * quantLum) + minLum;
    cout.val[1] = (int)(cb * quantChr) + minChr;
    cout.val[2] = (int)(cr * quantChr) + minChr;

    return cout;
}
Color applyYCbCrBT2020toRGB(Color cin, int yCbCrBitDepth, int rgbBitDepth)
{
    int minLum, minChr;
    double quantLum, quantChr;
    Color cout;

    switch (yCbCrBitDepth) {
    case 8:
        minLum = 16; // 16 * 2^(bitDepth-8)
        minChr = 128; // 128 * 2^(bitDepth-8)
        quantLum = 219.0; // 219 * 2^(bitDepth-8)
        quantChr = 224.0;  // 224 * 2^(bitDepth-8)
        break;
    case 10:
        minLum = 64; // 16 * 2^(bitDepth-8)
        minChr = 512; // 128 * 2^(bitDepth-8)
        quantLum = 876.0; // 219 * 2^(bitDepth-8)
        quantChr = 896.0;  // 224 * 2^(bitDepth-8)
        break;
    case 12:
        minLum = 256; // 16 * 2^(bitDepth-8)
        minChr = 2048; // 128 * 2^(bitDepth-8)
        quantLum = 3504.0; // 219 * 2^(bitDepth-8)
        quantChr = 3584.0;  // 224 * 2^(bitDepth-8)
        break;
    case 16:
        minLum = 4096; // 16 * 2^(bitDepth-8)
        minChr = 32768; // 128 * 2^(bitDepth-8)
        quantLum = 56064.0; // 219 * 2^(bitDepth-8)
        quantChr = 57344.0;  // 224 * 2^(bitDepth-8)
        break;
    default:
        printf("Error in applying YCbCr to RGB");
        cout.val[0] = cout.val[1] = cout.val[2] = 0;
        return cout;
    }

    double y = (cin.val[0] - minLum) / quantLum;
    double cb = (cin.val[1] - minChr) / quantChr;
    double cr = (cin.val[2] - minChr) / quantChr;

    double r = cr * 1.4746 + y;
    double b = cb * 1.8814 + y;
    double g = (y - 0.2627 * r - 0.0593 * b) / 0.6780;

    // clip rgb values to [0..1]
    if (r < 0.0) r = 0.0;
    else if (r > 1.0) r = 1.0;
    if (g < 0.0) g = 0.0;
    else if (g > 1.0) g = 1.0;
    if (b < 0.0) b = 0.0;
    else if (b > 1.0) b = 1.0;

    // perform quantization
    double maxRgbValue = (double)((1 << rgbBitDepth) - 1);
    cout.val[0] = (int)(r * maxRgbValue);
    cout.val[1] = (int)(g * maxRgbValue);
    cout.val[2] = (int)(b * maxRgbValue);

    return cout;
}