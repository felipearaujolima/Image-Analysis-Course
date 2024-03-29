#ifndef _ADJACENCIA_BASICS_H_
#define _ADJACENCIA_BASICS_H_

#include <malloc.h>

typedef struct _adjacencia { /* criando uma estrutura do tipo Adjacencia */
	int* dx, * dy; /* deslocamentos em x e y */
	int  nadj;    /* número de adjacentes */
} Adjacencia;

Adjacencia* CriaAdjacencia(int nadj);
void        DestroiAdjacencia(Adjacencia** adj);
Adjacencia* Retangular(int w, int h);

#endif
