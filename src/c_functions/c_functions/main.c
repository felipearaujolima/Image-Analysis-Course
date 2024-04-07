#include <png.h>
#include <stdio.h>
#include <stdlib.h>


#include "image.h"
#include "adjacency.h"
#include "morph.h"

int main()
{
	char* input_file = "iris.png";
	char* output_file = "out.png";


	Image* img = ReadImage(input_file);
	//AdjRel* adj= adjCircular(3.0);
	AdjRel* adj= adjRectangular(3, 3);
	//Image* dil = applyDilate(img, adj);
	Image* ero = applyErode(img, adj);

	WriteImagePNG(ero, output_file);
	DestroyImage(&img);
	DestroyImage(&ero);


	printf("Done. \n");
	return 0;
}