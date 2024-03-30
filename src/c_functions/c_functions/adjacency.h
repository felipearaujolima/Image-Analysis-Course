#ifndef _ADJACENCY_H_
#define _ADJACENCY_H_

#include <malloc.h>

typedef struct _adjacency { /* criando uma estrutura do tipo Adjacencia */
	int* dx, * dy; /* deslocamentos em x e y */
	int  nadj;    /* número de adjacentes */
} Adjacency;

Adjacency* CreateAdjacency(int nadj);
void        DestroyAdjacency(Adjacency** adj);
Adjacency* Rectangular(int w, int h);

#endif
