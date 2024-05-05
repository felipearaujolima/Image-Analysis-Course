#include <png.h>
#include <stdio.h>
#include <stdlib.h>


#include "image.h"
#include "adjacency.h"
#include "morph.h"

#include "seed.h"
#include "segmentation.h"


int main()
{
    FILE* fp;
    errno_t err = fopen_s(&fp, "similarity.csv", "w+");
    if (err == 0) {
        fprintf(fp, "valor_adj_,valor_adj_ift,valor_thr,valor_similarity\n");

        int images[] = { 2, 11, 23, 31, 45, 52, 67, 73, 84, 90, 100 };
        //double adj_values[] = { 10.0, 25.0, 35.0 };
        double adj_values[] = { 35.0};
        //double ift_values[] = { 2.0, 3.0, 5.0, 7.0 };
        double ift_values[] = {5.0, 7.0};
        int thr_values[] = { 1, 2, 4, 6, 8, 10 };

        for (int a = 0; a < 1; a++) {
            for (int b = 0; b < 2; b++) {
                for (int c = 0; c < 6; c++) {
                    for (int d = 0; d < 10; d++) {
                        int i = images[d];
                        double x = adj_values[a];
                        double y = ift_values[b];
                        int z = thr_values[c];

                        char comp_file[20];
                        char seeds_file[21];
                        char input_file[15];
                        char output_file_iris_cost[25];
                        char output_file_pupil[20];
                        char output_file_iris[20];
                        char out_file[23];
                        char ground_truth_file[33];

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

                        AdjRel* adj_ = adjCircular(x);
                        Image* seeds = applySub(applyDilate(pupil_, adj_), pupil_);

                        AdjRel* adj_ift = adjCircular(y);
                        Image* out = ImageForest(img, adj_ift, seeds);
                        Image* out_cost = applySub(pupil_, applyThreshold(out, 0, z, 255));

                        AdjRel* adj_iris = adjCircular(4.0);
                        Image* iris = applyOpen(out_cost, adj_iris);

                        Image* ground_truth = ReadImage(ground_truth_file);
                        double similarity = DiceSimilarity(iris, ground_truth);

                        fprintf(fp, "%f,%f,%d,%f\n", x, y, z, similarity);

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
                }
            }
        }

        fclose(fp);
    }
    else {
        printf("Unable to open file similarity.csv\n");
    }
    return 0;
}