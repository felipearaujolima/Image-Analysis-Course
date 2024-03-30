#include "adjacency.h"

Adjacency* CreateAdjacency(int nadj)
{
    Adjacency* adj = (Adjacency*)calloc(1, sizeof(Adjacency)); /* criando um ponteiro com um elemento e do tamanho da estrutura adjacencia
    e mudando o ponteiro para o tipo adjacencia, para podermos corretamente utilizar... 
    veja que fazer isso é necessário, pois eu n sei qual o tamanho da adjacencia, por isso usamos o calloc */

    adj->nadj = nadj; /* o tamanho da adjacencia */
    adj->dx = (int*)calloc(nadj, sizeof(int)); /* deslocamento em x, dx eh um ponteiro que aponta para uma serie de inteiros do tamanho nadj */
    adj->dy = (int*)calloc(nadj, sizeof(int)); /* deslocamento em y */

    return(adj);
}

void DestroyAdjacency(Adjacency** adj) /* aqui precisamos de um ponteiro para um ponteiro para uma adjacencia
fazemos isso pois queremos destruir tudo atrelado a isso */
{
    if ((*adj) != NULL) {  /* se ja n destruimos essa adjacencia */
        free((*adj)->dx); /* liberamos a memoria alocada ao dx */
        free((*adj)->dy); /* ao dy */
        free(*adj); /* e a estrutura como um todo */
        (*adj) = NULL; /* aqui colocamos o ponteiro como NULL, desativando-o por completo */
    }
}

Adjacency* Rectangular(int w, int h) /* aqui criamos a adjacencia retangular */
{
    Adjacency* adj = NULL; /* instanciamos uma estrutura vazia */

    if (w % 2 == 0) w += 1;  /* se for par, adicionamos mais um */
    if (h % 2 == 0) h += 1; /* o mesmo aqui */

    adj = CreateAdjacency(w * h); /* criamos uma adjacencia, mas note que o tamanho do grid vai ser esse */

    int i = 0; /* começamos a iterar */
    for (int dy = -h / 2; dy <= h / 2; dy++) { /* para dy = -h/2 até dy menor que h/2 */
        for (int dx = -w / 2; dx <= w / 2; dx++) { /* para dx = -w/2 até dx menor que w/2 */
            adj->dx[i] = dx; /* alocamos na posicao i o dx */
            adj->dy[i] = dy; /* alocamos na posicao o dy */
            i++;
        }
    }

    return(adj);
}
