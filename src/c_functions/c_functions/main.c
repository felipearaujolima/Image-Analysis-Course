// External Functions
#include <stdio.h> 
#include <stdlib.h> 
#include <png.h> 
#include <math.h>

// Internal Functions
#include "adjacency.h"
#include "convolution.h"
#include "morphological_op.h"
#include "image.h"

/* Main for Convolution Operations */
int main()
{

	char* name_img = "square.png";
	
	Image* img = ReadImage(name_img, "standard");
	//int filter[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
	int filter[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

	Kernel* kernel = CreateKernel(3, filter);
	Image* out = applySimpleKernel(img, kernel, name_img);
	
	//applyAbs(out);
	//applyLinearStreching(out);

	WriteImage(out, "out.png");
	DestroyImage(out);
	DestroyKernel(&kernel);
	printf("Done. \n");
}

/* Main for Morphological Operations */

//int main() {
	/* For Dilation or Erosion */

	//char* name_img = "dog.png";
	//Image* img = ReadImage(name_img, "standard");
	//
	//// Adjacency* adj = Rectangular(5, 5);
	//Adjacency* adj = Circular(0, 3);
	//
	////applyDilation(img, adj, name_img);
	//applyErosion(img, adj, name_img);
	//
	//WriteImage(img, "out.png");
	//DestroyImage(img);
	//DestroyAdjacency(&adj);
	//printf("Done. \n");
	//
	//return 0;

	/* For making a threshold in the image */

	//char* name_img = "iris.png";
	//int thr = 20;
	//int min = 0;
	//int max = 255;
	//Image* img = ReadImage(name_img, "standard");
	//
	//applyThreshold(img, thr, min, max);
	//
	//WriteImage(img, "out.png");
	//DestroyImage(img);
	//printf("Done. \n");
	//
	//return 0;

	/* For appling MorphologicalGradients in the image */

	//char* name_img = "dog.png";
	//// Adjacency* adj = Rectangular(5, 5);
	//Adjacency* adj = Circular(0, 3);
	//
	//Image* img = applyMorphologicalGradients(name_img, adj, "dil_img");
	//
	//WriteImage(img, "out.png");
	//DestroyImage(img);
	//DestroyAdjacency(&adj);
	//printf("Done. \n");
	//
	//return 0;

	/* Applying Closing and Opening Operation */

	//char* name_img = "iris.png";
	//int thr = 20;
	//int min = 0;
	//int max = 255;
	//Image* img = ReadImage(name_img, "standard");
	//
	//applyThreshold(img, thr, min, max);
	//Adjacency* adj_closing = Circular(0, 10);
	//img = applyClosingOperation(img, adj_closing, name_img);
	//DestroyAdjacency(&adj_closing);
	//
	//
	//Adjacency* adj_opening = Circular(0, 12);
	//img = applyOpeningOperation(img, adj_opening, name_img);
	//DestroyAdjacency(&adj_opening);
	//
	//WriteImage(img, "out.png");
	//DestroyImage(img);
	//printf("Done. \n");
	//
	//return 0;

//}




