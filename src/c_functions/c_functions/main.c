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
	
	
	//AdjRel* adj= adjRectangular(3, 3);

	//Image* dil = applyDilate(img, adj);
	//Image* ero = applyErode(img, adj);
	Image* thr = applyThreshold(img, 0, 20, 255);
	DestroyImage(&img);


	AdjRel* adj = adjCircular(15.0);
	Image* close = applyClose(thr,adj);
	DestroyImage(&thr);
	DestroyAdjRel(&adj);

	AdjRel* adj_ = adjCircular(30.0);
	Image* open = applyOpen(close, adj_);
	DestroyAdjRel(&adj_);
	DestroyImage(&close);


	WriteImagePNG(open, output_file);
	DestroyImage(&open);
	
	//DestroyImage(&dil);
	//DestroyImage(&sub);


	printf("Done. \n");
	return 0;
}