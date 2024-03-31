#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <png.h> 

typedef struct _image {

	int nx, ny;
	png_byte color_type;
	png_byte bit_depth;
	png_bytep* row_pointers;
	
} Image;


void* DestroyImage(Image** img);
Image* ReadImage(char* name_img);
void WriteImage(Image* img, char* name_img);


#endif