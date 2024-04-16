#include <png.h>
#include <stdio.h>
#include <stdlib.h>


#include "image.h"
#include "adjacency.h"
#include "morph.h"
#include "kernel.h"
#include "seed.h"
#include "segmentation.h"

int main()
{
    char comp_file[20];
    char seeds_file[21];
    char input_file[15];
    char output_file_iris_cost[25];
    char output_file_pupil[20];
    char output_file_iris[20];

    for (int i = 1; i <= 100; i++) {
        sprintf_s(comp_file, sizeof(comp_file), "%06d_comp.png", i);
        sprintf_s(seeds_file, sizeof(seeds_file), "%06d_seeds.png", i);
        sprintf_s(input_file, sizeof(input_file), "%06d.png", i);
        sprintf_s(output_file_iris_cost, sizeof(output_file_iris_cost), "%06d_iris_cost.png", i);
        sprintf_s(output_file_pupil, sizeof(output_file_pupil), "%06d_pupil.png", i);
        sprintf_s(output_file_iris, sizeof(output_file_iris), "%06d_iris.png", i);

        Image* img = ReadImage(input_file);
        Image* thr = applyThreshold(img, 0, 1, 255);

        AdjRel* adj = adjCircular(25.0);
        Image* pupil = applyClose(thr, adj);


        Image* comp = ComponentArea(pupil, NULL);
        Image* pupil_ = applyThreshold(comp, applyMaximumValue(comp), applyMaximumValue(comp) + 1, 255);
        
        
        AdjRel* adj_ = adjCircular(15.0);
        Image* seeds = applySub(applyDilate(pupil_, adj_), pupil_);
        //Image* seeds = applyDilate(pupil_, adj_);
        
        AdjRel* adj_ift = adjCircular(6.0);
        Image* out = ImageForest(img, adj_ift, seeds);
        Image* out_cost = applySub(pupil_, applyThreshold(out, 0, 1, 255));
        
        AdjRel* adj_iris = adjCircular(2.0);
        Image* iris = applyClose(applyOpen(out_cost, adj_iris), adj_iris);
        
        
        

        WriteImagePNG(out_cost, output_file_iris_cost);
        WriteImagePNG(pupil, output_file_pupil);
        WriteImagePNG(iris, output_file_iris);
        WriteImagePNG(seeds, seeds_file);
        WriteImagePNG(pupil_, comp_file);
        
        DestroyAdjRel(&adj);
        DestroyAdjRel(&adj_);
        DestroyAdjRel(&adj_iris);
        DestroyAdjRel(&adj_ift);

        DestroyImage(&pupil_);
        DestroyImage(&comp);
        DestroyImage(&iris);
        DestroyImage(&pupil);
        DestroyImage(&out_cost);
        DestroyImage(&out);
        DestroyImage(&thr);
        DestroyImage(&img);
        DestroyImage(&seeds);

        printf("Done with image %06d. \n", i);
    }
    return 0;
}

//int main() 
//{
//	char* input_file = "000007.png"; 
//
//	Image* img = ReadImage(input_file);
//	Image* thr = applyThreshold(img, 0, 2, 255);
//	
//	AdjRel* adj = adjCircular(25.0);
//	Image* pupil = applyClose(thr,adj);
//	DestroyImage(&thr);
//	DestroyAdjRel(&adj);
//
//	AdjRel* adj_ = adjCircular(25.0);
//	Image* seeds = applySub(applyDilate(pupil, adj_), pupil);
//	DestroyAdjRel(&adj_);
//
//	AdjRel* adj_ift = adjCircular(6.0);
//	Image* out =  ImageForest(img, adj_ift, seeds);
//	DestroyImage(&img);
//	DestroyImage(&seeds);
//	DestroyAdjRel(&adj_ift);
//
//	Image* out_cost = applySub(pupil, applyThreshold(out, 0, 1, 255));
//	DestroyImage(&out);
//
//	AdjRel* adj_iris = adjCircular(2.0);
//	Image* iris = applyClose(applyOpen(out_cost, adj_iris), adj_iris);
//	DestroyAdjRel(&adj_iris);
//	
//	WriteImagePNG(out_cost, "000007_iris_cost.png");
//	DestroyImage(&out_cost);
//	WriteImagePNG(pupil, "000007_pupil.png");
//	DestroyImage(&pupil);
//	WriteImagePNG(iris, "000007_iris.png");
//	DestroyImage(&iris);
//	
//	printf("Done. \n");
//	return 0;
//}

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