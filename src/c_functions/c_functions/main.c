#include <png.h>
#include <stdio.h>
#include <stdlib.h>


#include "image.h"
#include "adjacency.h"
#include "morph.h"
#include "kernel.h"
#include "seed.h"


int main() 
{
	printf("Done. \n");
	return 0;
}

//int main()
//{
	//char* input_file = "tools.png";
	//char* output_file = "out.png";
	//
	//Image* img = ReadImage(input_file);
	////AdjRel* adj = adjCircular(sqrt(2));
	////AdjRel* adj = adjCircular(10.0);
	////AdjRel* adj = adjRectangular(30, 5);
	//
	//Image* area = ComponentArea(img, NULL);
	//DestroyImage(&img);
	//Image* thr = applyThreshold(area, 5000, applyMaximumValue(area), 255);
	//DestroyImage(&area);
	//
	//AdjRel* adj = adjCircular(sqrt(2));
	//Image* img_out = LabelComp(thr, adj);
	//DestroyImage(&thr);
	//
	//Image* out_color = ColorizeComp(img_out);
	//DestroyImage(&img_out);
	//
	//
	//WriteImagePNG(out_color, output_file);
	//
	//DestroyImage(&out_color);
	//printf("Done. \n");
	//return 0;
//}

//int main()
//{
//	char* input_file = "plate.png";
//	char* output_file = "out.png";
//	int filter[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
//    //int filter[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
//	
//	Image* img = ReadImage(input_file);
//
//	AdjRel* adj = adjRectangular(3, 3);
//	Kernel* K = AllocKernel(adj, filter);
//
//	Image* img_filter = LinearFilter(img, K);
//	DestroyImage(&img);
//	
//	WriteImagePNG(img_filter, output_file);
//	DestroyImage(&img_filter);
//	DestroyKernel(&K);
//
//	printf("Done. \n");
//	return 0;
//}

//int main()
//{
//	char* input_file = "iris.png";
//	char* output_file = "out.png";
//
//	Image* img = ReadImage(input_file);
//	
//	
//	//AdjRel* adj= adjRectangular(3, 3);
//
//	//Image* dil = applyDilate(img, adj);
//	//Image* ero = applyErode(img, adj);
//	Image* thr = applyThreshold(img, 0, 20, 255);
//	DestroyImage(&img);
//
//
//	AdjRel* adj = adjCircular(15.0);
//	Image* close = applyClose(thr,adj);
//	DestroyImage(&thr);
//	DestroyAdjRel(&adj);
//
//	AdjRel* adj_ = adjCircular(30.0);
//	Image* open = applyOpen(close, adj_);
//	DestroyAdjRel(&adj_);
//	DestroyImage(&close);
//
//
//	WriteImagePNG(open, output_file);
//	DestroyImage(&open);
//	
//	//DestroyImage(&dil);
//	//DestroyImage(&sub);
//
//
//	printf("Done. \n");
//	return 0;
//}