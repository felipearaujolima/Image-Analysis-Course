#include "image.h"
#include "adjacency.h"
#include <stdio.h> 

//void applyDilation(Image* img, Adjacencia* adj)
//{
//
//    if (img->color_type == PNG_COLOR_TYPE_GRAY) {
//
//        for (int y = 0; y < img->ny; y++) {
//            for (int x = 0; x < img->nx; x++) {
//                png_bytep px = &(img->row_pointers[y][x]);
//                int out = *px;
//
//                for (int i = 0; i < adj->nadj; i++) {
//                    int x_ = x + adj->dx[i];
//                    int y_ = y + adj->dy[i];
//
//                    if (x_ >= 0 && x_ < img->nx && y_ >= 0 && y_ < img->ny) {
//                        png_bytep nb = &(img->row_pointers[y_][x_]);
//                        if (*nb > out) {
//                            out = *nb;
//                        }
//                    }
//                }
//
//                *px = out;
//            }
//        }
//    }
//    else if (img->color_type == PNG_COLOR_TYPE_RGB) {
//        for (int y = 0; y < img->ny; y++) {
//            for (int x = 0; x < img->nx; x++) {
//                png_bytep px = &(img->row_pointers[y][x * 3]);
//                int out[3];
//                out[0] = px[0];
//                out[1] = px[1];
//                out[2] = px[2];
//
//                for (int i = 0; i < adj->nadj; i++) {
//                    int x_ = x + adj->dx[i];
//                    int y_ = y + adj->dy[i];
//
//                    if (x_ >= 0 && x_ < img->nx && y_ >= 0 && y_ < img->ny) {
//                        png_bytep nb = &(img->row_pointers[y_][x_ * 3]);
//                        if (nb[0] > out[0]) {
//                            out[0] = nb[0];
//                        }
//                        if (nb[1] > out[1]) {
//                            out[1] = nb[1];
//                        }
//                        if (nb[2] > out[2]) {
//                            out[2] = nb[2];
//                        }
//
//                    }
//                }
//
//                px[0] = out[0];
//                px[1] = out[1];
//                px[2] = out[2];
//            }
//        }
//    }
//    else if (img->color_type == PNG_COLOR_TYPE_RGBA) {
//        for (int y = 0; y < img->ny; y++) {
//            for (int x = 0; x < img->nx; x++) {
//                png_bytep px = &(img->row_pointers[y][x * 4]);
//                int out[4];
//                out[0] = px[0];
//                out[1] = px[1];
//                out[2] = px[2];
//                out[3] = px[3];
//
//                for (int i = 0; i < adj->nadj; i++) {
//                    int x_ = x + adj->dx[i];
//                    int y_ = y + adj->dy[i];
//
//                    if (x_ >= 0 && x_ < img->nx && y_ >= 0 && y_ < img->ny) {
//                        png_bytep nb = &(img->row_pointers[y_][x_ * 4]);
//                        if (nb[0] > out[0]) {
//                            out[0] = nb[0];
//                        }
//                        if (nb[1] > out[1]) {
//                            out[1] = nb[1];
//                        }
//                        if (nb[2] > out[2]) {
//                            out[2] = nb[2];
//                        }
//                        if (nb[3] > out[3]) {
//                            out[3] = nb[3];
//                        }
//
//                    }
//                }
//
//                px[0] = out[0];
//                px[1] = out[1];
//                px[2] = out[2];
//                px[3] = out[3];
//            }
//        }
//    }
//}

void applyDilation(Image* img, Adjacency* adj, char* name_img)
{
    // Cria uma cópia da imagem original
    Image* aux = ReadImage(name_img);

    if (img->color_type == PNG_COLOR_TYPE_GRAY) {

        for (int y = 0; y < img->ny; y++) {
            for (int x = 0; x < img->nx; x++) {
                png_bytep px = &(aux->row_pointers[y][x]);
                int out = *px;

                for (int i = 0; i < adj->nadj; i++) {
                    int x_ = x + adj->dx[i];
                    int y_ = y + adj->dy[i];

                    if (x_ >= 0 && x_ < img->nx && y_ >= 0 && y_ < img->ny) {
                        png_bytep nb = &(img->row_pointers[y_][x_]);
                        if (*nb > out) {
                            out = *nb;
                        }
                    }
                }

                *px = out;
            }
        }
    }
    else if (img->color_type == PNG_COLOR_TYPE_RGB) {
        for (int y = 0; y < img->ny; y++) {
            for (int x = 0; x < img->nx; x++) {
                png_bytep px = &(aux->row_pointers[y][x * 3]);
                int out[3];
                out[0] = px[0];
                out[1] = px[1];
                out[2] = px[2];

                for (int i = 0; i < adj->nadj; i++) {
                    int x_ = x + adj->dx[i];
                    int y_ = y + adj->dy[i];

                    if (x_ >= 0 && x_ < img->nx && y_ >= 0 && y_ < img->ny) {
                        png_bytep nb = &(img->row_pointers[y_][x_ * 3]);
                        if (nb[0] > out[0]) {
                            out[0] = nb[0];
                        }
                        if (nb[1] > out[1]) {
                            out[1] = nb[1];
                        }
                        if (nb[2] > out[2]) {
                            out[2] = nb[2];
                        }

                    }
                }

                px[0] = out[0];
                px[1] = out[1];
                px[2] = out[2];
            }
        }
    }
    else if (img->color_type == PNG_COLOR_TYPE_RGBA) {
        for (int y = 0; y < img->ny; y++) {
            for (int x = 0; x < img->nx; x++) {
                png_bytep px = &(aux->row_pointers[y][x * 4]);
                int out[4];
                out[0] = px[0];
                out[1] = px[1];
                out[2] = px[2];
                out[3] = px[3];

                for (int i = 0; i < adj->nadj; i++) {
                    int x_ = x + adj->dx[i];
                    int y_ = y + adj->dy[i];

                    if (x_ >= 0 && x_ < img->nx && y_ >= 0 && y_ < img->ny) {
                        png_bytep nb = &(img->row_pointers[y_][x_ * 4]);
                        if (nb[0] > out[0]) {
                            out[0] = nb[0];
                        }
                        if (nb[1] > out[1]) {
                            out[1] = nb[1];
                        }
                        if (nb[2] > out[2]) {
                            out[2] = nb[2];
                        }
                        if (nb[3] > out[3]) {
                            out[3] = nb[3];
                        }

                    }
                }

                px[0] = out[0];
                px[1] = out[1];
                px[2] = out[2];
                px[3] = out[3];
            }
        }
    }

    // Copia a imagem temporária de volta para a imagem original
    for (int y = 0; y < img->ny; y++) {
        for (int x = 0; x < img->nx; x++) {
            img->row_pointers[y][x] = aux->row_pointers[y][x];
        }
    }
    for (int y = 0; y < aux->ny; y++)
        free(aux->row_pointers[y]);
    free(aux->row_pointers);

    // Libera a memória da imagem temporária
    DestroyImage(aux);
}

void applyErosion(Image* img, Adjacency* adj, char* name_img)
{
    // Cria uma cópia da imagem original
    Image* aux = ReadImage(name_img);

    if (img->color_type == PNG_COLOR_TYPE_GRAY) {

        for (int y = 0; y < img->ny; y++) {
            for (int x = 0; x < img->nx; x++) {
                png_bytep px = &(aux->row_pointers[y][x]);
                int out = *px;

                for (int i = 0; i < adj->nadj; i++) {
                    int x_ = x + adj->dx[i];
                    int y_ = y + adj->dy[i];

                    if (x_ >= 0 && x_ < img->nx && y_ >= 0 && y_ < img->ny) {
                        png_bytep nb = &(img->row_pointers[y_][x_]);
                        if (*nb < out) {
                            out = *nb;
                        }
                    }
                }

                *px = out;
            }
        }
    }
    else if (img->color_type == PNG_COLOR_TYPE_RGB) {
        for (int y = 0; y < img->ny; y++) {
            for (int x = 0; x < img->nx; x++) {
                png_bytep px = &(aux->row_pointers[y][x * 3]);
                int out[3];
                out[0] = px[0];
                out[1] = px[1];
                out[2] = px[2];

                for (int i = 0; i < adj->nadj; i++) {
                    int x_ = x + adj->dx[i];
                    int y_ = y + adj->dy[i];

                    if (x_ >= 0 && x_ < img->nx && y_ >= 0 && y_ < img->ny) {
                        png_bytep nb = &(img->row_pointers[y_][x_ * 3]);
                        if (nb[0] < out[0]) {
                            out[0] = nb[0];
                        }
                        if (nb[1] < out[1]) {
                            out[1] = nb[1];
                        }
                        if (nb[2] < out[2]) {
                            out[2] = nb[2];
                        }

                    }
                }

                px[0] = out[0];
                px[1] = out[1];
                px[2] = out[2];
            }
        }
    }
    else if (img->color_type == PNG_COLOR_TYPE_RGBA) {
        for (int y = 0; y < img->ny; y++) {
            for (int x = 0; x < img->nx; x++) {
                png_bytep px = &(aux->row_pointers[y][x * 4]);
                int out[4];
                out[0] = px[0];
                out[1] = px[1];
                out[2] = px[2];
                out[3] = px[3];

                for (int i = 0; i < adj->nadj; i++) {
                    int x_ = x + adj->dx[i];
                    int y_ = y + adj->dy[i];

                    if (x_ >= 0 && x_ < img->nx && y_ >= 0 && y_ < img->ny) {
                        png_bytep nb = &(img->row_pointers[y_][x_ * 4]);
                        if (nb[0] < out[0]) {
                            out[0] = nb[0];
                        }
                        if (nb[1] < out[1]) {
                            out[1] = nb[1];
                        }
                        if (nb[2] < out[2]) {
                            out[2] = nb[2];
                        }
                        if (nb[3] < out[3]) {
                            out[3] = nb[3];
                        }

                    }
                }

                px[0] = out[0];
                px[1] = out[1];
                px[2] = out[2];
                px[3] = out[3];
            }
        }
    }

    // Copia a imagem temporária de volta para a imagem original
    for (int y = 0; y < img->ny; y++) {
        for (int x = 0; x < img->nx; x++) {
            img->row_pointers[y][x] = aux->row_pointers[y][x];
        }
    }
    for (int y = 0; y < aux->ny; y++)
        free(aux->row_pointers[y]);
    free(aux->row_pointers);

    // Libera a memória da imagem temporária
    DestroyImage(aux);
}
