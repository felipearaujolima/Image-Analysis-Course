#include "image.h"
#include "adjacency.h"
#include <stdio.h> 
#include "convolution.h"

Kernel* CreateKernel(int k, int* filter)
{
	Kernel* kernel= (Kernel*)calloc(1, sizeof(Kernel));

	if (k % 2 == 0) k += 1; 

	kernel->k = k;
	int nadj = k * k;
	kernel->nadj = nadj;

	kernel->dx = (int*)calloc(nadj, sizeof(int)); 
	kernel->dy = (int*)calloc(nadj, sizeof(int)); 

	kernel-> w = filter;

	int i = 0; 
	for (int dy = -k / 2; dy <= k / 2; dy++) { 
		for (int dx = -k / 2; dx <= k / 2; dx++) { 
			kernel->dx[i] = dx; 
			kernel->dy[i] = dy; 
			i++;
		}
	}

	return kernel;
}

void DestroyKernel(Kernel** kernel)
{
	if ((*kernel) != NULL) {
		free((*kernel)->dx);
		free((*kernel)->dy);

		free(*kernel);
		(*kernel) = NULL;
	}

}

Image* applySimpleKernel(Image* img, Kernel* kernel, char* name_img)
{

	Image* aux = ReadImage(name_img, "gray");

	if (img->color_type == PNG_COLOR_TYPE_GRAY) {
		for (int y = 0; y < img->ny; y++) {
			for (int x = 0; x < img->nx; x++) {
				aux->row_pointers[y][x] = img->row_pointers[y][x];
			}
		}
	}

	if (img->color_type == PNG_COLOR_TYPE_GRAY) {
		for (int y = kernel->k; y < img->ny - kernel->k; y++) {
			for (int x = kernel->k; x < img->nx - kernel->k; x++) {
				png_bytep px = &(aux->row_pointers[y][x]);
				int pixel = 0;

				for (int i = 0; i < kernel->nadj; i++) {
					int x_ = x + kernel->dx[i];
					int y_ = y + kernel->dy[i];

					int w_q = kernel->w[i];


					if (x_ >= 0 && x_ < img->nx && y_ >= 0 && y_ < img->ny) {
						png_bytep pixel_q = &(img->row_pointers[y_][x_]);
						pixel += *pixel_q * w_q;

					}
				}
				*px = pixel;
			}
		}
	}
	else if (img->color_type == PNG_COLOR_TYPE_RGB) {
		for (int y = kernel->k; y < img->ny - kernel->k; y++) {
			for (int x = kernel->k; x < img->nx - kernel->k; x++) {
				png_bytep px = &(aux->row_pointers[y][x]);
				int pixel = 0;

				for (int i = 0; i < kernel->nadj; i++) {
					int x_ = x + kernel->dx[i];
					int y_ = y + kernel->dy[i];

					int w_q = kernel->w[i];

					if (x_ >= 0 && x_ < img->nx && y_ >= 0 && y_ < img->ny) {
						png_bytep pixel_q = &(img->row_pointers[y_][x_ * 3]);
						pixel += (pixel_q[0] * w_q) + (pixel_q[1] * w_q) + (pixel_q[2] * w_q);

					}
				}
				*px = pixel;
			}
		}

	}


	for (int y = 0; y < img->ny; y++)
		free(img->row_pointers[y]);
	free(img->row_pointers);

	DestroyImage(img);
	return aux;
}