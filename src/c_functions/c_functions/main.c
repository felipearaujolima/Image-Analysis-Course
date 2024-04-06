#include <png.h>
#include <stdio.h>
#include <stdlib.h>


#include "image.h"
#include "adjacency.h"

int main()
{
	char* input_file = "dog.png";
	char* output_file = "out.png";


	Image* img = ReadImage(input_file);
	AdjRel* adj_circular = iftCircular(3.0);

	WriteImagePNG(img, output_file);
	DestroyImage(&img);

	printf("Done. \n");
	return 0;
}