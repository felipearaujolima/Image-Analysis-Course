#include "image.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <png.h> 
#include <math.h>
#include <png.h>




void* DestroyImage(Image** img)
{
    free(img);
    (*img) = NULL;
    //Image* aux = *img; 
    //
    //if (aux != NULL) {
    //    for (int c = 0; c < aux->nchannels; c++) {
    //        for (int y = 0; y < aux->ny; y++)
    //            free(aux->channel[c][y]);
    //        free(aux->channel[c]);
    //    }
    //    free(aux);
    //    (*img) = NULL; 
    //}
}

Image* ReadImage(char* name_img)
{

    // Image* img = NULL;
    Image* img = (Image*)calloc(1, sizeof(Image));
    FILE* fp = fopen(name_img, "rb");

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL); /* the main struct in the libpng, the main pointer for the image */
    png_infop info = png_create_info_struct(png); /* this png_infop struct gives more info about the image we are loading
	such as: shape of the image (height and width), if it's grayscale or RGB, any filter on it, and etc */


    png_init_io(png, fp); /* starting to read */
    png_read_info(png, info); /* reading the image */


    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);

    if (bit_depth == 16)
        png_set_strip_16(png);
    
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);
    
    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);
    
    //if (png_get_valid(png, info, PNG_INFO_tRNS))
    //    png_set_tRNS_to_alpha(png);
    //
    //// These color_type don't have an alpha channel then fill it with 0xff.
    //if (color_type == PNG_COLOR_TYPE_RGB ||
    //    color_type == PNG_COLOR_TYPE_GRAY ||
    //    color_type == PNG_COLOR_TYPE_PALETTE)
    //    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    
    //if (color_type == PNG_COLOR_TYPE_GRAY ||
    //    color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    //    png_set_gray_to_rgb(png);
    
    png_read_update_info(png, info);



    png_bytep*  row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
    }

    png_read_image(png, row_pointers);

    img->nx = png_get_image_width(png, info);
    img->ny = png_get_image_height(png, info);
    img->color_type = png_get_color_type(png, info);
    img->bit_depth = png_get_bit_depth(png, info);
    img->row_pointers = row_pointers;

    fclose(fp);
    return img;

}

void WriteImage(Image* img, char* name_img) {


    FILE* fp = fopen(name_img, "wb");
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);
    png_init_io(png, fp);

    png_set_IHDR(
        png,
        info,
        img -> nx, img -> ny,
        img ->bit_depth, // 8,
        img->color_type,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );

    png_write_info(png, info);

    png_write_image(png, img -> row_pointers);
    png_write_end(png, NULL);

    for (int y = 0; y < img->ny; y++)
        free(img -> row_pointers[y]);
    free(img -> row_pointers);

    fclose(fp);
}