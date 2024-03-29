#include <stdio.h> 
#include <stdlib.h> 
#include <png.h> 
#include <math.h>
#include "imagem_basics.h"
#include "adjacencia_basics.h"

void applyDilation(Imagem* img, Adjacencia* adj)
{
	for (int b = 0; b < img->nbandas; b++) {
		for (int y = 0; y < img->ny; y++) {
			for (int x = 0; x < img->nx; x++) {
				int pixel = img->banda[b][y][x];
				int out;

				out = pixel;
				for (int i = 0; i < adj->nadj; i++) { /* aceessando os deslocamentos */
					int x_ = x + adj->dx[i];  /* o deslocamento da vez em x */
					int y_ = y + adj->dy[i]; /* o deslocamento da vez em y */


					if (x_ >= 0 && x_ < img->nx && y_ >= 0 && y_ < img->ny) { /* validando se está dentro dos limites da imagem */

						int neighboor = img->banda[b][y_][x_]; /* pegando o vizinho */

						if (neighboor > out) {
							out = neighboor;
						}

					}
				}

				img->banda[b][y][x] = out;


			}

		}

	}

}

int main() {
	Imagem* img = LeImagem("image_test.ppm");
	printf("imagem com %d bandas. \n", img->nbandas);
	Adjacencia* adj = Retangular(1, 2); /* aqui nós só temos os deslocamentos necessários em x e y */
	
	applyDilation(&img, &adj); /* passando o endereço, assim podemos mudar diretamente a variável, para isso preciso que meu parametro seja do tipo ponteiro */

	// note que endereço é a localização fisica na memória e ponteiro é uma variavel que armazena esse endereço


	GravaImagem(img, "out.ppm");
	DestroiImagem(&img);
	DestroiAdjacencia(&adj);
	printf("All good :) \n");
	return 0;
}




