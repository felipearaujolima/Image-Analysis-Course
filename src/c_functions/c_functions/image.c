#include "image.h"

/* Miscellaneous */
long applyNormalizationValue(long maxval) {
    long norm_val = 1;

    if (maxval < 0)
        printf("Error in normalization");
    else if (maxval <= 1)
        norm_val = 1;
    else if (maxval <= 255)
        norm_val = 255;
    else if (maxval <= 4095)
        norm_val = 4095;
    else if (maxval <= 65535)
        norm_val = 65535;
    else if (maxval <= 4294967295)
        norm_val = 4294967295;
    else
        printf("Error in normalization");

    return norm_val;
}
void applyMinMaxValues(Image* img, int* min, int* max) {
    *min = *max = img->val[0];

    for (int p = 1; p < img->n; p++) {
        if (img->val[p] < *min)
            *min = img->val[p];
        else if (img->val[p] > *max)
            *max = img->val[p];
    }
}

uchar applyImageDepth(Image* img) {
    int img_min, img_max;
    applyMinMaxValues(img, &img_min, &img_max);

    long max_range;

    if (img_min >= 0)
        max_range = applyNormalizationValue(img_max) + 1;
    else
        max_range = applyNormalizationValue(img_max - img_min) + 1;
    printf("%d \n", max_range);
    
    return (uchar)applyLog(max_range, 2);
}
bool applyIsColorImage(Image* img) {
    return ((img->Cb != NULL) && (img->Cr != NULL));
}
void applySetCbCr(Image* img, ushort value)
{
    int p;

    if (!applyIsColorImage(img)) {
        img->Cb = applyAllocUShortArray(img->n);
        img->Cr = applyAllocUShortArray(img->n);
    }
    for (p = 0; p < img->n; p++) {
        img->Cb[p] = value;
        img->Cr[p] = value;
    }
}
void applySetAlpha(Image* img, ushort value)
{
    int p;
    if (img->alpha == NULL) {
        img->alpha = applyAllocUShortArray(img->n);
    }

    for (p = 0; p < img->n; p++) {
        img->alpha[p] = value;
    }
}

/* Create/Read image functions */
Image* CreateImage(int xsize, int ysize, int zsize) {
    int* val = applyAllocIntArray(xsize * ysize * zsize);

    return applyCreateImageFromBuffer(xsize, ysize, zsize, val);
}
Image* applyCreateImageFromBuffer(int xsize, int ysize, int zsize, int* val) {
    Image* img = NULL;
    int      y, z, xysize;

    img = (Image*)applyAlloc(1, sizeof(Image));

    img->val = val;
    img->Cb = img->Cr = NULL;
    img->alpha = NULL;
    img->xsize = xsize;
    img->ysize = ysize;
    img->zsize = zsize;
    img->dx = 1.0;
    img->dy = 1.0;
    img->dz = 1.0;
    img->tby = applyAllocIntArray(ysize);
    img->tbz = applyAllocIntArray(zsize);
    img->n = xsize * ysize * zsize;


    img->tby[0] = 0;
    for (y = 1; y < ysize; y++)
        img->tby[y] = img->tby[y - 1] + xsize;

    img->tbz[0] = 0;
    xysize = xsize * ysize;
    for (z = 1; z < zsize; z++)
        img->tbz[z] = img->tbz[z - 1] + xysize;

    return (img);
}
png_bytep* ReadPngImageAux(const char* file_name, png_structp* png_ptr, png_infop* info_ptr)
{
    FILE* fp = fopen(file_name, "rb");

    int height;
    png_bytep* row_pointers;

    
    png_structp ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    *png_ptr = ptr;
    *info_ptr = png_create_info_struct(*png_ptr);
    
    png_init_io(*png_ptr, fp);
    
    // png_set_sig_bytes(*png_ptr, 8);
   
    png_read_info(*png_ptr, *info_ptr);


    int depth = png_get_bit_depth((*png_ptr), (*info_ptr));
  
    if (depth < 8) {
        png_set_expand_gray_1_2_4_to_8(ptr);
    }

    
    height = png_get_image_height(*png_ptr, *info_ptr);
    png_read_update_info(*png_ptr, *info_ptr);


    row_pointers = (png_bytep*)applyAlloc(height, sizeof(png_bytep));
    for (int y = 0; y < height; y++)
        row_pointers[y] = (png_byte*)applyAlloc(png_get_rowbytes(*png_ptr, *info_ptr), 1);

    png_read_image(*png_ptr, row_pointers);

    fclose(fp);

    return row_pointers;
}
Image* ReadImage(char* file_name) {

    
    png_infop info_ptr;
    png_structp png_ptr;
    png_bytep* row_pointers;

    row_pointers = ReadPngImageAux(file_name, &png_ptr, &info_ptr);
    
    int width, height, color_type, depth;

    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    depth = png_get_bit_depth(png_ptr, info_ptr);
    Image* img = CreateImage(width, height, 1);
    unsigned int numberChannels = png_get_channels(png_ptr, info_ptr);

    int byteshift = depth / 8;

    int x, y;

    int p = 0;

    if (color_type == PNG_COLOR_TYPE_GRAY)//gray image
    {
        for (y = 0; y < height; y++) {
            png_byte* row = row_pointers[y];
            for (x = 0; x < width; x++) {
                png_byte* ptr = &(row[x * numberChannels * byteshift]);
                img->val[p] = ptr[0];
                if (depth == 16) {
                    img->val[p] = (img->val[p] << 8) + ptr[1];
                }
                p++;
            }
        }
    }
    else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        if (img->alpha == NULL) {
            applySetAlpha(img, 0);
        }
        for (y = 0; y < height; y++) {
            png_byte* row = row_pointers[y];
            for (x = 0; x < width; x++) {
                png_byte* ptr = &(row[x * numberChannels * byteshift]);
                if (depth == 8) {
                    img->val[p] = ptr[0];
                    img->alpha[p] = ptr[1];
                }
                else if (depth == 16) {
                    img->val[p] = ptr[0];
                    img->val[p] = (img->val[p] << 8) + ptr[1];
                    img->alpha[p] = ptr[2];
                    img->alpha[p] = (img->alpha[p] << 8) + ptr[3];
                }
                p++;
            }
        }
    }
    else if (color_type == PNG_COLOR_TYPE_RGB) {//color image

        applySetCbCr(img, 128);
        Color rgb, ycbcr;

        for (y = 0; y < height; y++) {
            png_byte* row = row_pointers[y];

            for (x = 0; x < width; x++) {
                png_byte* ptr = &(row[x * numberChannels * byteshift]);
                rgb.val[0] = ptr[0 * byteshift];
                rgb.val[1] = ptr[1 * byteshift];
                rgb.val[2] = ptr[2 * byteshift];

                if (depth == 16) { //read second byte in case of 16bit images
                    rgb.val[0] = (rgb.val[0] << 8) + ptr[1];
                    rgb.val[1] = (rgb.val[1] << 8) + ptr[3];
                    rgb.val[2] = (rgb.val[2] << 8) + ptr[5];
                }

                ycbcr = applyRGBtoYCbCrBT2020(rgb, depth, depth);

                img->val[p] = ycbcr.val[0];
                img->Cb[p] = ycbcr.val[1];
                img->Cr[p] = ycbcr.val[2];

                p++;
            }
        }
    }
    else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
        applySetCbCr(img, 128);
        Color rgb, ycbcr;
        if (img->alpha == NULL) {
            applySetAlpha(img, 0);
        }

        for (y = 0; y < height; y++) {
            png_byte* row = row_pointers[y];
            for (x = 0; x < width; x++) {
                png_byte* ptr = &(row[x * numberChannels * byteshift]);
                rgb.val[0] = ptr[0 * byteshift];
                rgb.val[1] = ptr[1 * byteshift];
                rgb.val[2] = ptr[2 * byteshift];
                ushort alpha = ptr[3 * byteshift];

                if (depth == 16) { //read second byte in case of 16bit images
                    rgb.val[0] = (rgb.val[0] << 8) + ptr[1];
                    rgb.val[1] = (rgb.val[1] << 8) + ptr[3];
                    rgb.val[2] = (rgb.val[2] << 8) + ptr[5];
                    alpha = (alpha << 8) + ptr[7];
                }

                ycbcr = applyRGBtoYCbCr(rgb, depth == 8 ? 255 : 65535);

                img->val[p] = ycbcr.val[0];
                img->Cb[p] = ycbcr.val[1];
                img->Cr[p] = ycbcr.val[2];
                img->alpha[p] = alpha;

                p++;
            }
        }
    }

    for (y = 0; y < height; ++y) {
        applyFree(row_pointers[y]);
    }

    applyFree(row_pointers);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    img->dz = 0.0;

    return img;
}

/* Destroy/Write image functions */
void DestroyImage(Image** img) {
    if (img != NULL) {
        Image* aux = *img;

        if (aux != NULL) {
            if (aux->val != NULL) applyFree(aux->val);
            if (aux->Cb != NULL) applyFree(aux->Cb);
            if (aux->Cr != NULL) applyFree(aux->Cr);
            if (aux->alpha != NULL) applyFree(aux->alpha);
            if (aux->tby != NULL) applyFree(aux->tby);
            if (aux->tbz != NULL) applyFree(aux->tbz);
            applyFree(aux);
            *img = NULL;
        }
    }
}
void WritePngImageAux(const char* file_name, png_bytep* row_pointers, int width, int height, int bit_depth, int color_type) {

    /* create file */
    FILE* fp = fopen(file_name, "wb");


    /* initialize stuff */
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);



    png_infop info_ptr = png_create_info_struct(png_ptr);

    png_init_io(png_ptr, fp);


    png_set_IHDR(png_ptr, info_ptr, width, height,
        bit_depth, color_type, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);


    png_write_image(png_ptr, row_pointers);


    png_write_end(png_ptr, NULL);

    png_destroy_write_struct(&png_ptr, &info_ptr);

    /* cleanup heap allocation */
    for (int y = 0; y < height; y++)
        applyFree(row_pointers[y]);
    applyFree(row_pointers);

    fclose(fp);
}
void WriteImagePNG(Image* img, const char* file_name) {

    png_bytep* row_pointers;
    int width, height, depth, byteshift;

    width = img->xsize;
    height = img->ysize;
    png_byte color_type;
    depth = applyImageDepth(img);

    if (depth <= 8) {
        depth = 8;
    }
    else {
        depth = 16;
    }

    byteshift = depth / 8;
    //int offset = depth==16?1:0;//to read the second byte first in cases of 16bit images

    size_t numberOfChannels = 1;
    if (applyIsColorImage(img)) {
        if (img->alpha == NULL) {
            numberOfChannels = 3;//RGB
            color_type = PNG_COLOR_TYPE_RGB;
        }
        else {
            numberOfChannels = 4;//RGB_ALPHA
            color_type = PNG_COLOR_TYPE_RGB_ALPHA;
        }
    }
    else {
        if (img->alpha == NULL) {
            numberOfChannels = 1;//GRAY
            color_type = PNG_COLOR_TYPE_GRAY;
        }
        else {
            numberOfChannels = 2;//GRAY_ALPHA
            color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
        }
    }

    //size_t pixel_size = (iftIsColorImage(img)?3:1 ) * byteshift;
    row_pointers = (png_bytep*)applyAlloc(height, sizeof(png_bytep));
    for (int y = 0; y < height; y++)
        row_pointers[y] = (png_byte*)applyAlloc(width, numberOfChannels * byteshift);

    if (color_type == PNG_COLOR_TYPE_GRAY) {
        int p = 0;
        for (int y = 0; y < height; ++y) {
            png_byte* row = row_pointers[y];
            for (int x = 0; x < width; x++) {
                png_byte* ptr = &(row[x * numberOfChannels * byteshift]);

                ptr[0] = img->val[p] & 0xFF;//get first byte

                if (depth == 16) {//in 16bit image, we should store as big endian
                    ptr[1] = ptr[0];
                    ptr[0] = (img->val[p] >> 8) & 0xFF;//get second byte
                }

                p++;
            }
        }
    }
    else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        int p = 0;
        for (int y = 0; y < height; ++y) {
            png_byte* row = row_pointers[y];
            for (int x = 0; x < width; x++) {
                png_byte* ptr = &(row[x * numberOfChannels * byteshift]);

                if (depth == 8) {
                    ptr[0] = img->val[p] & 0xFF;//get first byte
                    ptr[1] = img->alpha[p] & 0xFF;//get second byte
                }


                if (depth == 16) {//in 16bit image, we should store as big endian
                    ptr[0] = img->val[p] >> 8;//get first byte
                    ptr[1] = img->val[p] & 0xFF;//get second byte


                    ptr[2] = img->alpha[p] >> 8;//get first byte;
                    ptr[3] = img->alpha[p] & 0xFF;;//get second byte
                }
                p++;
            }
        }
    }
    else if (color_type == PNG_COLOR_TYPE_RGB) {
        Color rgb, ycbcr;
        int p = 0;
        for (int y = 0; y < height; ++y) {
            png_byte* row = row_pointers[y];
            for (int x = 0; x < width; x++) {
                png_byte* ptr = &(row[x * numberOfChannels * byteshift]);

                ycbcr.val[0] = img->val[p];
                ycbcr.val[1] = img->Cb[p];
                ycbcr.val[2] = img->Cr[p];

                rgb = applyYCbCrBT2020toRGB(ycbcr, depth, depth);

                ptr[0 * byteshift] = rgb.val[0] & 0xFF;//get first byte
                ptr[1 * byteshift] = rgb.val[1] & 0xFF;
                ptr[2 * byteshift] = rgb.val[2] & 0xFF;

                if (depth == 16) {//in 16bit image, we should store as big endian
                    ptr[(0 * byteshift) + 1] = ptr[0 * byteshift];
                    ptr[(1 * byteshift) + 1] = ptr[1 * byteshift];
                    ptr[(2 * byteshift) + 1] = ptr[2 * byteshift];

                    ptr[0 * byteshift] = ((rgb.val[0] >> 8) & 0xFF);//get second byte
                    ptr[1 * byteshift] = ((rgb.val[1] >> 8) & 0xFF);
                    ptr[2 * byteshift] = ((rgb.val[2] >> 8) & 0xFF);
                }

                p++;
            }
        }

    }
    else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
        Color rgb, ycbcr;
        int p = 0;
        for (int y = 0; y < height; ++y) {
            png_byte* row = row_pointers[y];
            for (int x = 0; x < width; x++) {
                png_byte* ptr = &(row[x * numberOfChannels * byteshift]);

                ycbcr.val[0] = img->val[p];
                ycbcr.val[1] = img->Cb[p];
                ycbcr.val[2] = img->Cr[p];
                ushort alpha = img->alpha[p];

                rgb = applyYCbCrBT2020toRGB(ycbcr, depth, depth);

                ptr[0 * byteshift] = rgb.val[0] & 0xFF;//get first byte
                ptr[1 * byteshift] = rgb.val[1] & 0xFF;
                ptr[2 * byteshift] = rgb.val[2] & 0xFF;
                ptr[3 * byteshift] = alpha & 0xFF;

                if (depth == 16) {//in 16bit image, we should store as big endian
                    ptr[(0 * byteshift) + 1] = ptr[0 * byteshift];
                    ptr[(1 * byteshift) + 1] = ptr[1 * byteshift];
                    ptr[(2 * byteshift) + 1] = ptr[2 * byteshift];
                    ptr[(3 * byteshift) + 1] = ptr[(3 * byteshift)];

                    ptr[0 * byteshift] = ((rgb.val[0] >> 8) & 0xFF);//get second byte
                    ptr[1 * byteshift] = ((rgb.val[1] >> 8) & 0xFF);
                    ptr[2 * byteshift] = ((rgb.val[2] >> 8) & 0xFF);
                    ptr[(3 * byteshift)] = ((alpha >> 8) & 0xFF);
                }
                p++;
            }
        }

    }
    else {
        printf("Unknwon color scape");
    };


    WritePngImageAux(file_name, row_pointers, width, height, depth, color_type);
}