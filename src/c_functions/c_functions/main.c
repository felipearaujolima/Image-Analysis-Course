#include <png.h>
#include <stdio.h>
#include <stdlib.h>


#include "image.h"

int main()
{
	char* input_file = "dog.png";
	char* output_file = "out.png";


	Image* img = ReadImage(input_file);


	WriteImagePNG(img, output_file);
	DestroyImage(&img);

	printf("Done. \n");
	return 0;
}