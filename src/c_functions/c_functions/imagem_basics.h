#ifndef _IMAGEM_BASICS_H_
#define _IMAGEM_BASICS_H_

typedef struct _imagem {
	int*** banda; 
	/* armazena o valor do pixel em cada banda da imagem 
	como as bandas são normalmente são 3 = Azul, Verde e Vermelho, criamos uma matriz tridimensional para guardar essas luzes
	note que:
	int* é um ponteiro para um int
	int** é um ponteiro para um ponteiro para um int, ou seja, aponta para uma matriz de int
	int*** é um ponteiro para um ponteiro para um ponteiro para um int, aponta para uma matriz de matrizes de int

	note que estamos alocando dinamicamente a memória, dado que não sabemos o tamanho que essa imagem vai ter
	*/
	int nx, ny;   
	/* números de pixels nos eixos x (horizontal, colunas da matriz) e y (vertical, linhas da matriz) */
	int nbandas;  
	/* número de bandas */
} Imagem;

Imagem* CriaImagem(int nx, int ny, int nbandas);
void* DestroiImagem(Imagem** img);
Imagem* LeImagem(char* nome_img);
void    GravaImagem(Imagem* img, char* nome_img);

#endif