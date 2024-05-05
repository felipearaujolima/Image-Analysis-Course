#include <png.h>
#include <stdio.h>
#include <stdlib.h>


#include "image.h"
#include "adjacency.h"
#include "morph.h"
#include "kernel.h"
#include "seed.h"
#include "segmentation.h"

// project 1 optimize 
//int main()
//{
//    FILE* fp;
//    errno_t err = fopen_s(&fp, "similarity.csv", "w+");
//    if (err == 0) {
//        fprintf(fp, "valor_adj_,valor_adj_ift,valor_thr,valor_similarity\n");
//
//        int images[] = { 2, 11, 23, 31, 45, 52, 67, 73, 84, 90, 100 };
//        //double adj_values[] = { 10.0, 25.0, 30.0, 40.0 };
//        double adj_values[] = { 35.0};
//        //double ift_values[] = { 2.0, 3.0, 5.0, 7.0 };
//        double ift_values[] = {5.0, 7.0};
//        int thr_values[] = { 1, 2, 4, 6, 8, 10 };
//
//        for (int a = 0; a < 1; a++) {
//            for (int b = 0; b < 2; b++) {
//                for (int c = 0; c < 6; c++) {
//                    for (int d = 0; d < 10; d++) {
//                        int i = images[d];
//                        double x = adj_values[a];
//                        double y = ift_values[b];
//                        int z = thr_values[c];
//
//                        char comp_file[20];
//                        char seeds_file[21];
//                        char input_file[15];
//                        char output_file_iris_cost[25];
//                        char output_file_pupil[20];
//                        char output_file_iris[20];
//                        char out_file[23];
//                        char ground_truth_file[33];
//
//                        sprintf_s(out_file, sizeof(out_file), "%06d_out_raw.png", i);
//                        sprintf_s(comp_file, sizeof(comp_file), "%06d_comp.png", i);
//                        sprintf_s(seeds_file, sizeof(seeds_file), "%06d_seeds.png", i);
//                        sprintf_s(input_file, sizeof(input_file), "%06d.png", i);
//                        sprintf_s(output_file_iris_cost, sizeof(output_file_iris_cost), "%06d_iris_cost.png", i);
//                        sprintf_s(output_file_pupil, sizeof(output_file_pupil), "%06d_pupil.png", i);
//                        sprintf_s(output_file_iris, sizeof(output_file_iris), "%06d_iris.png", i);
//                        sprintf_s(ground_truth_file, sizeof(ground_truth_file), "%06d_ground_truth_iris.png", i);
//
//                        Image* img = ReadImage(input_file);
//                        Image* thr = applyThreshold(img, 0, 1, 255);
//                        AdjRel* adj = adjCircular(25.0);
//                        Image* pupil = applyClose(thr, adj);
//                        Image* comp = ComponentArea(pupil, NULL);
//                        Image* pupil_ = applyThreshold(comp, applyMaximumValue(comp), applyMaximumValue(comp) + 1, 255);
//
//                        AdjRel* adj_ = adjCircular(x);
//                        Image* seeds = applySub(applyDilate(pupil_, adj_), pupil_);
//
//                        AdjRel* adj_ift = adjCircular(y);
//                        Image* out = ImageForest(img, adj_ift, seeds);
//                        Image* out_cost = applySub(pupil_, applyThreshold(out, 0, z, 255));
//
//                        AdjRel* adj_iris = adjCircular(4.0);
//                        Image* iris = applyOpen(out_cost, adj_iris);
//
//                        Image* ground_truth = ReadImage(ground_truth_file);
//                        double similarity = DiceSimilarity(iris, ground_truth);
//
//                        fprintf(fp, "%f,%f,%d,%f\n", x, y, z, similarity);
//
//                        WriteImagePNG(out, out_file);
//                        WriteImagePNG(out_cost, output_file_iris_cost);
//                        WriteImagePNG(pupil, output_file_pupil);
//                        WriteImagePNG(iris, output_file_iris);
//                        WriteImagePNG(seeds, seeds_file);
//                        WriteImagePNG(pupil_, comp_file);
//
//                        DestroyAdjRel(&adj);
//                        DestroyAdjRel(&adj_);
//                        DestroyAdjRel(&adj_iris);
//                        DestroyAdjRel(&adj_ift);
//
//                        DestroyImage(&pupil_);
//                        DestroyImage(&comp);
//                        DestroyImage(&iris);
//                        DestroyImage(&pupil);
//                        DestroyImage(&out_cost);
//                        DestroyImage(&out);
//                        DestroyImage(&thr);
//                        DestroyImage(&img);
//                        DestroyImage(&seeds);
//
//                        printf("Done with image %06d. \n", i);
//                    }
//                }
//            }
//        }
//
//        fclose(fp);
//    }
//    else {
//        printf("Unable to open file similarity.csv\n");
//    }
//    return 0;
//}

// Ground truth
//int main()
//{
//
//    char input_file[28];
//	char output_file[33];
//
//    for (int i = 1; i <= 100; i++) {
//        sprintf_s(input_file, sizeof(input_file), "%06d_ground_truth.png", i);
//		sprintf_s(output_file, sizeof(output_file), "%06d_ground_truth_iris.png", i);
//
//
//        Image* img = ReadImage(input_file);
//		int min, max;
//		applyMinMaxValues(img, &min, &max);
//		printf("%d min \n", min);
//		printf("%d max \n", max);
//
//        Image* thr = applyThreshold(img, 0, min, 255);
//        AdjRel* adj = adjCircular(25.0);
//        Image* pupil = applyClose(thr, adj);
//        Image* comp = ComponentArea(pupil, NULL);
//        Image* pupil_ = applyThreshold(comp, applyMaximumValue(comp), applyMaximumValue(comp) + 1, 255);
//
//
//		
//		Image* iris_aux = ComponentArea(applySub(pupil_, applyThreshold(img, min + 1, max - 1, 255)), NULL);
//
//		AdjRel* close = adjCircular(3.0);
//		Image* iris = applySub(pupil_, applyClose(applyThreshold(iris_aux, applyMaximumValue(iris_aux), applyMaximumValue(iris_aux) + 1, 255), close));
//
//
//        WriteImagePNG(iris, output_file);
//
//        DestroyAdjRel(&adj);
//		DestroyAdjRel(&close);
//
//
//        DestroyImage(&pupil_);
//        DestroyImage(&comp);
//        DestroyImage(&pupil);
//        DestroyImage(&thr);
//        DestroyImage(&img);
//		DestroyImage(&iris_aux);
//		DestroyImage(&iris);
//
//        printf("Done with image %06d. \n", i);
//    }
//    return 0;
//}

// IFT
int main()
{
    char comp_file[20];
    char seeds_file[21];
    char input_file[15];
    char output_file_iris_cost[25];
    char output_file_pupil[20];
    char output_file_iris[20];
    char out_file[23];
    char ground_truth_file[33];

    FILE* fp;
    errno_t err = fopen_s(&fp, "final_similarity.csv", "w+");
    fprintf(fp, "png,valor_similarity\n");

    for (int i = 1; i <= 100; i++) {
        sprintf_s(out_file, sizeof(out_file), "%06d_out_raw.png", i);
        sprintf_s(comp_file, sizeof(comp_file), "%06d_comp.png", i);
        sprintf_s(seeds_file, sizeof(seeds_file), "%06d_seeds.png", i);
        sprintf_s(input_file, sizeof(input_file), "%06d.png", i);
        sprintf_s(output_file_iris_cost, sizeof(output_file_iris_cost), "%06d_iris_cost.png", i);
        sprintf_s(output_file_pupil, sizeof(output_file_pupil), "%06d_pupil.png", i);
        sprintf_s(output_file_iris, sizeof(output_file_iris), "%06d_iris.png", i);
        sprintf_s(ground_truth_file, sizeof(ground_truth_file), "%06d_ground_truth_iris.png", i);

        Image* img = ReadImage(input_file);
        Image* thr = applyThreshold(img, 0, 1, 255);
        AdjRel* adj = adjCircular(25.0);
        Image* pupil = applyClose(thr, adj);
        Image* comp = ComponentArea(pupil, NULL);
        Image* pupil_ = applyThreshold(comp, applyMaximumValue(comp), applyMaximumValue(comp) + 1, 255);
        


        AdjRel* adj_ = adjCircular(35.0);
        Image* seeds = applySub(applyDilate(pupil_, adj_), pupil_);
        
        AdjRel* adj_ift = adjCircular(5.0);
        Image* out = ImageForest(img, adj_ift, seeds);
        Image* out_cost = applySub(pupil_, applyThreshold(out, 0, 1, 255));
        


        AdjRel* adj_iris = adjCircular(4.0);
        Image* iris = applyOpen(out_cost, adj_iris);
        
        Image* ground_truth = ReadImage(ground_truth_file);
        double similarity = DiceSimilarity(iris, ground_truth);


        fprintf(fp, "%d,%f\n", i, similarity);

        
        
        WriteImagePNG(out, out_file);
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


// Component area
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

// Kernel
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


// Basics
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