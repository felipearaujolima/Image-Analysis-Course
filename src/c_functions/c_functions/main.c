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
	// Calling and creating the image
	char* name_img = "square.png";
	Image* img = ReadImage(name_img);

	// Creating the rectangular adjancency 
	Adjacency* adj = Rectangular(10, 10);

	//applyDilation(img, adj, name_img);
	applyErosion(img, adj, name_img);

	// Writing the output image and destroing the remainings 
	WriteImage(img, "square_erosion.png");
	DestroyImage(img);
	DestroyAdjacency(&adj);
	printf("Done. \n");
	return 0;
}




