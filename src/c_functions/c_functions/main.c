// External Functions
#include <stdio.h> 
#include <stdlib.h> 
#include <png.h> 
#include <math.h>

// Internal Functions
#include "adjacency.h"
#include "morphological_op.h"
#include "image.h"

int main() {
	/* For Dilation or Erosion */

	//char* name_img = "dog.png";
	//Image* img = ReadImage(name_img);
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
	//Image* img = ReadImage(name_img);
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
	//Image* img = applyMorphologicalGradients(name_img, adj, "dil_ero");
	//
	//WriteImage(img, "out.png");
	//DestroyImage(img);
	//DestroyAdjacency(&adj);
	//printf("Done. \n");
	//
	//return 0;

	/* Applying Closing and Opening Operation */

	char* name_img = "iris.png";
	int thr = 20;
	int min = 0;
	int max = 255;
	Image* img = ReadImage(name_img);
	
	applyThreshold(img, thr, min, max);
	Adjacency* adj_closing = Circular(0, 15);
	
	img = applyClosingOperation(img, adj_closing, name_img);
	DestroyAdjacency(&adj_closing);


	Adjacency* adj_opening = Circular(0, 17);
	img = applyOpeningOperation(img, adj_opening, name_img);
	
	WriteImage(img, "out.png");
	DestroyImage(img);
	DestroyAdjacency(&adj_opening);
	printf("Done. \n");
	
	return 0;

}




