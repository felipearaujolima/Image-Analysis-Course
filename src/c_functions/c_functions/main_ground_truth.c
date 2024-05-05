#include <png.h>
#include <stdio.h>
#include <stdlib.h>


#include "image.h"
#include "adjacency.h"
#include "morph.h"
#include "seed.h"
#include "segmentation.h"

// Ground truth
int main()
{

    char input_file[28];
	char output_file[33];

    for (int i = 1; i <= 100; i++) {
        sprintf_s(input_file, sizeof(input_file), "%06d_ground_truth.png", i);
		sprintf_s(output_file, sizeof(output_file), "%06d_ground_truth_iris.png", i);


        Image* img = ReadImage(input_file);
		int min, max;
		applyMinMaxValues(img, &min, &max);
		printf("%d min \n", min);
		printf("%d max \n", max);

        Image* thr = applyThreshold(img, 0, min, 255);
        AdjRel* adj = adjCircular(25.0);
        Image* pupil = applyClose(thr, adj);
        Image* comp = ComponentArea(pupil, NULL);
        Image* pupil_ = applyThreshold(comp, applyMaximumValue(comp), applyMaximumValue(comp) + 1, 255);


		
		Image* iris_aux = ComponentArea(applySub(pupil_, applyThreshold(img, min + 1, max - 1, 255)), NULL);

		AdjRel* close = adjCircular(3.0);
		Image* iris = applySub(pupil_, applyClose(applyThreshold(iris_aux, applyMaximumValue(iris_aux), applyMaximumValue(iris_aux) + 1, 255), close));


        WriteImagePNG(iris, output_file);

        DestroyAdjRel(&adj);
		DestroyAdjRel(&close);


        DestroyImage(&pupil_);
        DestroyImage(&comp);
        DestroyImage(&pupil);
        DestroyImage(&thr);
        DestroyImage(&img);
		DestroyImage(&iris_aux);
		DestroyImage(&iris);

        printf("Done with image %06d. \n", i);
    }
    return 0;
}