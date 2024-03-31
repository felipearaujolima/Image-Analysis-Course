#include "image.h"
#include "adjacency.h"
#include <stdio.h> 


void applyDilation(Image* img, Adjacency* adj, char* name_img)
{
    // Cria uma cópia da imagem original
    Image* aux = ReadImage(name_img);

    for (int y = 0; y < img->ny; y++) {
        for (int x = 0; x < img->nx; x++) {
            aux->row_pointers[y][x] = img->row_pointers[y][x];
        }
    }

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

        // Copia a imagem temporária de volta para a imagem original
        for (int y = 0; y < img->ny; y++) {
            for (int x = 0; x < img->nx; x++) {
                img->row_pointers[y][x] = aux->row_pointers[y][x];
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

        // Copia a imagem temporária de volta para a imagem original
        for (int y = 0; y < img->ny; y++) {
            for (int x = 0; x < img->nx; x++) {
                img->row_pointers[y][x * 3] = aux->row_pointers[y][x * 3];
                img->row_pointers[y][x * 3 + 1] = aux->row_pointers[y][x * 3 + 1];
                img->row_pointers[y][x * 3 + 2] = aux->row_pointers[y][x * 3 + 2];
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

        // Copia a imagem temporária de volta para a imagem original
        for (int y = 0; y < img->ny; y++) {
            for (int x = 0; x < img->nx; x++) {
                img->row_pointers[y][x * 4] = aux->row_pointers[y][x * 4];
                img->row_pointers[y][x * 4 + 1] = aux->row_pointers[y][x * 4 + 1];
                img->row_pointers[y][x * 4 + 2] = aux->row_pointers[y][x * 4 + 2];
                img->row_pointers[y][x * 4 + 3] = aux->row_pointers[y][x * 4 + 3];
            }
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

    for (int y = 0; y < img->ny; y++) {
        for (int x = 0; x < img->nx; x++) {
            aux->row_pointers[y][x] = img->row_pointers[y][x];
        }
    }

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

        // Copia a imagem temporária de volta para a imagem original
        for (int y = 0; y < img->ny; y++) {
            for (int x = 0; x < img->nx; x++) {
                img->row_pointers[y][x] = aux->row_pointers[y][x];
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

        // Copia a imagem temporária de volta para a imagem original
        for (int y = 0; y < img->ny; y++) {
            for (int x = 0; x < img->nx; x++) {
                img->row_pointers[y][x * 3] = aux->row_pointers[y][x * 3];
                img->row_pointers[y][x * 3 + 1] = aux->row_pointers[y][x * 3 + 1];
                img->row_pointers[y][x * 3 + 2] = aux->row_pointers[y][x * 3 + 2];
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

        // Copia a imagem temporária de volta para a imagem original
        for (int y = 0; y < img->ny; y++) {
            for (int x = 0; x < img->nx; x++) {
                img->row_pointers[y][x * 4] = aux->row_pointers[y][x * 4];
                img->row_pointers[y][x * 4 + 1] = aux->row_pointers[y][x * 4 + 1];
                img->row_pointers[y][x * 4 + 2] = aux->row_pointers[y][x * 4 + 2];
                img->row_pointers[y][x * 4 + 3] = aux->row_pointers[y][x * 4 + 3];
            }
        }
    }

    for (int y = 0; y < aux->ny; y++)
        free(aux->row_pointers[y]);
    free(aux->row_pointers);

    // Libera a memória da imagem temporária
    DestroyImage(aux);
}

void applyThreshold(Image* img, int thr, int min, int max)
{

    if (img->color_type == PNG_COLOR_TYPE_GRAY) {

        for (int y = 0; y < img->ny; y++) {
            for (int x = 0; x < img->nx; x++) {
                png_bytep px = &(img->row_pointers[y][x]);
                int out = *px;

                if (out < thr) {
                    out = max;
                }
                else {
                    out = min;
                }
                *px = out;
            }
        }
    }
    else if (img->color_type == PNG_COLOR_TYPE_RGB) {
        for (int y = 0; y < img->ny; y++) {
            for (int x = 0; x < img->nx; x++) {
                png_bytep px = &(img->row_pointers[y][x * 3]);
                int out[3];
                out[0] = px[0];
                out[1] = px[1];
                out[2] = px[2];

                if (out[0] < thr) {
                    out[0] = max;
                }
                else {
                    out[0] = min;
                }


                if (out[1] < thr) {
                    out[1] = max;
                }
                else {
                    out[1] = min;
                }


                if (out[2] < thr) {
                    out[2] = max;
                }
                else {
                    out[2] = min;
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
                png_bytep px = &(img->row_pointers[y][x * 4]);
                int out[4];
                out[0] = px[0];
                out[1] = px[1];
                out[2] = px[2];
                out[3] = px[3];

                if (out[0] < thr) {
                    out[0] = max;
                }
                else {
                    out[0] = min;
                }


                if (out[1] < thr) {
                    out[1] = max;
                }
                else {
                    out[1] = min;
                }


                if (out[2] < thr) {
                    out[2] = max;
                }
                else {
                    out[2] = min;
                }


                if (out[3] < thr) {
                    out[3] = max;
                }
                else {
                    out[3] = min;
                }

                px[0] = out[0];
                px[1] = out[1];
                px[2] = out[2];
                px[3] = out[3];
            }
        }
    }

}

void applySubtraction(Image* img_1, Image* img_2, Image* out) 
{
    // Image* aux = ReadImage(name_img);
    if (out->color_type == PNG_COLOR_TYPE_GRAY) {

        for (int y = 0; y < out->ny; y++) {
            for (int x = 0; x < out->nx; x++) {
                png_bytep pixel_result = &(out->row_pointers[y][x]);
                png_bytep pixel_1 = &(img_1->row_pointers[y][x]);
                png_bytep pixel_2 = &(img_2->row_pointers[y][x]);
                int result = *pixel_result;
                int first = *pixel_1;
                int second = *pixel_2;

                result = first - second;
                if (result < 0) {
                    result = 0;
                }
                *pixel_result = result;
            }
        }
    }
    else if (out->color_type == PNG_COLOR_TYPE_RGB) {
        for (int y = 0; y < out->ny; y++) {
            for (int x = 0; x < out->nx; x++) {


                // png_bytep pixel_result = &(out->row_pointers[y][x * 3]);
                int result[3];
                // result[0] = pixel_result[0];
                // result[1] = pixel_result[1];
                // result[2] = pixel_result[2];
            
                png_bytep pixel_1 = &(img_1->row_pointers[y][x * 3]);
                int first[3];
                first[0] = pixel_1[0];
                first[1] = pixel_1[1];
                first[2] = pixel_1[2];
            
                png_bytep pixel_2 = &(img_2->row_pointers[y][x * 3]);
                int second[3];
                second[0] = pixel_2[0];
                second[1] = pixel_2[1];
                second[2] = pixel_2[2];
            
                result[0] = first[0] - second[0];
            
                result[1] = first[1] - second[1];
            
                result[2] = first[2] - second[2];
            
            
                pixel_1[0] = result[0];
                pixel_1[1] = result[1];
                pixel_1[2] = result[2];
            }
            

        }
        for (int y = 0; y < out->ny; y++) {
            for (int x = 0; x < out->nx; x++) {
                out->row_pointers[y][x * 3] = img_1->row_pointers[y][x * 3];
                out->row_pointers[y][x * 3 + 1] = img_1->row_pointers[y][x * 3 + 1];
                out->row_pointers[y][x * 3 + 2] = img_1->row_pointers[y][x * 3 + 2];
            }
        }


    }
    else if (out->color_type == PNG_COLOR_TYPE_RGBA) {
        for (int y = 0; y < out->ny; y++) {
            for (int x = 0; x < out->nx; x++) {
             
                png_bytep pixel_result = &(out->row_pointers[y][x * 4]);
                int result[4];
                result[0] = pixel_result[0];
                result[1] = pixel_result[1];
                result[2] = pixel_result[2];
                result[3] = pixel_result[3];

                png_bytep pixel_1 = &(img_1->row_pointers[y][x * 4]);
                int first[4];
                first[0] = pixel_1[0];
                first[1] = pixel_1[1];
                first[2] = pixel_1[2];
                first[3] = pixel_1[3];

                png_bytep pixel_2 = &(img_2->row_pointers[y][x * 4]);
                int second[4];
                second[0] = pixel_2[0];
                second[1] = pixel_2[1];
                second[2] = pixel_2[2];
                second[3] = pixel_2[3];

                result[0] = first[0] - second[0];
                result[1] = first[1] - second[1];
                result[2] = first[2] - second[2];
                result[3] = first[3] - second[3];

                pixel_result[0] = result[0];
                pixel_result[1] = result[1];
                pixel_result[2] = result[2];
                pixel_result[3] = result[3];
            }
        }
    }
}

Image* applyMorphologicalGradients(char* name_img, Adjacency* adj, char* type)
{
    Image* out = ReadImage(name_img);
    
    if (type = "dil_ero") {
        // erosion - dilation 
        Image* img_1 = ReadImage(name_img);
        Image* img_2 = ReadImage(name_img);

        applyErosion(img_1, adj, name_img);
        applyDilation(img_2, adj, name_img);
        applySubtraction(img_2, img_1, out);
        
        

        for (int y = 0; y < out->ny; y++) {
            free(img_1->row_pointers[y]);
            free(img_2->row_pointers[y]);
        }
        free(img_1->row_pointers);
        free(img_2->row_pointers);

        
        DestroyImage(img_1);
        DestroyImage(img_2);

        return out;
    }
    else if (type = "img_ero") {
        // erosion - image
        Image* img_1 = ReadImage(name_img);
        Image* img_2 = ReadImage(name_img);
        applyErosion(img_1, adj, name_img);
        applySubtraction(img_1, img_2, out);

        for (int y = 0; y < out->ny; y++) {
            free(img_1->row_pointers[y]);
            free(img_2->row_pointers[y]);
        }
        free(img_1->row_pointers);
        free(img_2->row_pointers);


        DestroyImage(img_1);
        DestroyImage(img_2);
        return out;
    }
    else if (type = "dil_img"){
        // image - dilation
        Image* img_1 = ReadImage(name_img);
        Image* img_2 = ReadImage(name_img);
        applyDilation(img_2, adj, name_img);
        applySubtraction(img_1, img_2, out);

        for (int y = 0; y < out->ny; y++) {
            free(img_1->row_pointers[y]);
            free(img_2->row_pointers[y]);
        }
        free(img_1->row_pointers);
        free(img_2->row_pointers);


        DestroyImage(img_1);
        DestroyImage(img_2);

        return out;
    }
}

Image* applyClosingOperation(Image* img, Adjacency* adj, char* name_img) 
{
    applyDilation(img, adj, name_img);
    applyErosion(img, adj, name_img);

    return img;

}

Image* applyOpeningOperation(Image* img, Adjacency* adj, char* name_img)
{
    applyErosion(img, adj, name_img);
    applyDilation(img, adj, name_img);

    return img;

}