#include "queue.h"

int default_value_function(GQueue* Q, int p) {
    int* value = (int*)Q->L.value_data;
    return value[p];
}

GQueue* CreateGQueue(int nbuckets, int nelems, int* value)
{
    GQueue* Q = CreateGQueueWithCostFunction(nbuckets, nelems, value,
        default_value_function);
    Q->L.value = value;

    return Q;
}

GQueue* CreateGQueueWithCostFunction(int nbuckets, int nelems, void* value, int (*cost_function)(GQueue* Q, int p)) {
    GQueue* Q = NULL;

    Q = (GQueue*)applyAlloc(1, sizeof(GQueue));

    if (Q != NULL)
    {
        Q->C.first = (int*)applyAlloc((nbuckets + 1), sizeof(int));
        Q->C.last = (int*)applyAlloc((nbuckets + 1), sizeof(int));
        Q->C.nbuckets = nbuckets;
        if ((Q->C.first != NULL) && (Q->C.last != NULL))
        {
            Q->L.elem = (GQNode*)applyAlloc(nelems, sizeof(GQNode));
            Q->L.nelems = nelems;
            Q->L.value = NULL;
            Q->L.value_data = value;
            Q->L.value_function = cost_function;
            if (Q->L.elem != NULL)
            {
                ResetGQueue(Q);
            }
            else
                printf("error in CreateGQueueWithCostFunction \n");
        }
        else
            printf("error in CreateGQueueWithCostFunction \n");
    }
    else
        printf("error in CreateGQueueWithCostFunction \n");

    /* default */

    SetTieBreak(Q, FIFOBREAK);
    SetRemovalPolicy(Q, MINVALUE);

    return(Q);
}

void ResetGQueue(GQueue* Q)
{
    int i;

    Q->C.minvalue = IFT_INFINITY_INT;
    Q->C.maxvalue = IFT_INFINITY_INT_NEG;
    /* No need for that, since the programmer might have changed them  */
    //    iftSetTieBreak(Q,FIFOBREAK);
    //    iftSetRemovalPolicy(Q,MINVALUE);
    for (i = 0; i < Q->C.nbuckets + 1; i++)
        Q->C.first[i] = Q->C.last[i] = IFT_NIL;

    for (i = 0; i < Q->L.nelems; i++)
    {
        Q->L.elem[i].next = Q->L.elem[i].prev = IFT_NIL;
        Q->L.elem[i].color = IFT_WHITE;
    }

}

int EmptyGQueue(GQueue* Q)
{
    int last, current;

    if (Q->C.removal_policy == MINVALUE)
        current = SafeMod(Q->C.minvalue, Q->C.nbuckets);
    else
        current = Q->C.nbuckets - 1 - (SafeMod(Q->C.maxvalue, Q->C.nbuckets));

    if (Q->C.first[current] != IFT_NIL)
        return 0;

    last = current;

    current = SafeMod(current + 1, Q->C.nbuckets);

    while ((Q->C.first[current] == IFT_NIL) && (current != last))
    {
        current = SafeMod(current + 1, Q->C.nbuckets);
    }

    if (Q->C.first[current] == IFT_NIL)
    {
        if (Q->C.first[Q->C.nbuckets] == IFT_NIL)
        {
            //Changed by Falcao and Nikolas
            // iftResetGQueue(Q);
            return(1);
        }
    }

    return (0);
}

int RemoveGQueue(GQueue* Q)
{
    int elem = IFT_NIL, next, prev;
    int last, current;

    if (Q->C.removal_policy == MINVALUE)
        current = SafeMod(Q->C.minvalue, Q->C.nbuckets);
    else
        current = Q->C.nbuckets - 1 - SafeMod(Q->C.maxvalue, Q->C.nbuckets);

    /** moves to next element **/

    if (Q->C.first[current] == IFT_NIL)
    {
        last = current;

        current = SafeMod(current + 1, Q->C.nbuckets);

        while ((Q->C.first[current] == IFT_NIL) && (current != last))
        {
            current = SafeMod(current + 1, Q->C.nbuckets);
        }

        if (Q->C.first[current] != IFT_NIL)
        {
            if (Q->C.removal_policy == MINVALUE)
                Q->C.minvalue = Q->L.value_function(Q, Q->C.first[current]);
            else
                Q->C.maxvalue = Q->L.value_function(Q, Q->C.first[current]);
        }
        else
        {
            if (Q->C.first[Q->C.nbuckets] != IFT_NIL)
            {
                current = Q->C.nbuckets;
                if (Q->C.removal_policy == MINVALUE)
                    Q->C.minvalue = Q->L.value_function(Q, Q->C.first[current]);
                else
                    Q->C.maxvalue = Q->L.value_function(Q, Q->C.first[current]);
            }
            else
            {
                
                printf("error in remove gqueue \n");
            }
        }
    }

    if (Q->C.tiebreak == LIFOBREAK)
    {
        elem = Q->C.last[current];
        prev = Q->L.elem[elem].prev;
        if (prev == IFT_NIL)           /* there was a single element in the list */
        {
            Q->C.last[current] = Q->C.first[current] = IFT_NIL;
        }
        else
        {
            Q->C.last[current] = prev;
            Q->L.elem[prev].next = IFT_NIL;
        }
    }
    else   /* Assume FIFO policy for breaking ties */
    {
        elem = Q->C.first[current];
        next = Q->L.elem[elem].next;
        if (next == IFT_NIL)           /* there was a single element in the list */
        {
            Q->C.first[current] = Q->C.last[current] = IFT_NIL;
        }
        else
        {
            Q->C.first[current] = next;
            Q->L.elem[next].prev = IFT_NIL;
        }
    }

    Q->L.elem[elem].color = IFT_BLACK;

    return elem;
}

void DestroyGQueue(GQueue** Q)
{
    GQueue* aux = *Q;

    if (aux != NULL)
    {
        if (aux->C.first != NULL) applyFree(aux->C.first);
        if (aux->C.last != NULL) applyFree(aux->C.last);
        if (aux->L.elem != NULL) applyFree(aux->L.elem);
        applyFree(aux);
        *Q = NULL;
    }
}

GQueue* GrowGQueue(GQueue** Q, int nbuckets)
{
    GQueue* Q1 = CreateGQueueWithCostFunction(nbuckets, (*Q)->L.nelems,
        (*Q)->L.value_data,
        (*Q)->L.value_function);
    int i, bucket;

    Q1->C.minvalue = (*Q)->C.minvalue;
    Q1->C.maxvalue = (*Q)->C.maxvalue;
    Q1->C.tiebreak = (*Q)->C.tiebreak;
    Q1->C.removal_policy = (*Q)->C.removal_policy;
    for (i = 0; i < (*Q)->C.nbuckets; i++)
        if ((*Q)->C.first[i] != IFT_NIL)
        {
            bucket = SafeMod((*Q)->L.value_function((*Q), (*Q)->C.first[i]), Q1->C.nbuckets);
            Q1->C.first[bucket] = (*Q)->C.first[i];
            Q1->C.last[bucket] = (*Q)->C.last[i];
        }
    if ((*Q)->C.first[(*Q)->C.nbuckets] != IFT_NIL)
    {
        bucket = Q1->C.nbuckets;
        Q1->C.first[bucket] = (*Q)->C.first[(*Q)->C.nbuckets];
        Q1->C.last[bucket] = (*Q)->C.last[(*Q)->C.nbuckets];
    }

    for (i = 0; i < (*Q)->L.nelems; i++)
        Q1->L.elem[i] = (*Q)->L.elem[i];

    DestroyGQueue(Q);
    return(Q1);
}

void InsertGQueue(GQueue** Q, int elem)
{
    int bucket, minvalue = (*Q)->C.minvalue, maxvalue = (*Q)->C.maxvalue;

    if (((*Q)->L.value_function((*Q), elem) == IFT_INFINITY_INT) || ((*Q)->L.value_function((*Q), elem) == IFT_INFINITY_INT_NEG))
        bucket = (*Q)->C.nbuckets;
    else
    {
        if ((*Q)->L.value_function((*Q), elem) < minvalue)
            minvalue = (*Q)->L.value_function((*Q), elem);
        if ((*Q)->L.value_function((*Q), elem) > maxvalue)
            maxvalue = (*Q)->L.value_function((*Q), elem);
        if ((maxvalue - minvalue) > ((*Q)->C.nbuckets - 1))
        {
            (*Q) = GrowGQueue(Q, 2 * (maxvalue - minvalue) + 1);
            fprintf(stdout, "Warning: Doubling queue size\n");
        }
        if ((*Q)->C.removal_policy == MINVALUE)
        {
            bucket = SafeMod((*Q)->L.value_function((*Q), elem), (*Q)->C.nbuckets);
        }
        else
        {
            bucket = (*Q)->C.nbuckets - 1 - (SafeMod((*Q)->L.value_function((*Q), elem), (*Q)->C.nbuckets));
        }
        (*Q)->C.minvalue = minvalue;
        (*Q)->C.maxvalue = maxvalue;
    }
    if ((*Q)->C.first[bucket] == IFT_NIL)
    {
        (*Q)->C.first[bucket] = elem;
        (*Q)->L.elem[elem].prev = IFT_NIL;
    }
    else
    {
        (*Q)->L.elem[(*Q)->C.last[bucket]].next = elem;
        (*Q)->L.elem[elem].prev = (*Q)->C.last[bucket];
    }

    (*Q)->C.last[bucket] = elem;
    (*Q)->L.elem[elem].next = IFT_NIL;
    (*Q)->L.elem[elem].color = IFT_GRAY;
}

void RemoveGQueueElem(GQueue* Q, int elem)
{
    int prev, next, bucket;

    if ((Q->L.value_function(Q, elem) == IFT_INFINITY_INT) || (Q->L.value_function(Q, elem) == IFT_INFINITY_INT_NEG))
        bucket = Q->C.nbuckets;
    else
    {
        if (Q->C.removal_policy == MINVALUE)
            bucket = SafeMod(Q->L.value_function(Q, elem), Q->C.nbuckets);
        else
            bucket = Q->C.nbuckets - 1 - SafeMod(Q->L.value_function(Q, elem), Q->C.nbuckets);
    }

    prev = Q->L.elem[elem].prev;
    next = Q->L.elem[elem].next;

    /* if elem is the first element */
    if (Q->C.first[bucket] == elem)
    {
        Q->C.first[bucket] = next;
        if (next == IFT_NIL) /* elem is also the last one */
            Q->C.last[bucket] = IFT_NIL;
        else
            Q->L.elem[next].prev = IFT_NIL;
    }
    else    /* elem is in the middle or it is the last */
    {
        Q->L.elem[prev].next = next;
        if (next == IFT_NIL) /* if it is the last */
            Q->C.last[bucket] = prev;
        else
            Q->L.elem[next].prev = prev;
    }

    Q->L.elem[elem].color = IFT_WHITE;

}


// FIFO

//FIFO* CreateFIFO(int n)
//{
//    FIFO* F = (FIFO*)applyAlloc(1, sizeof(FIFO));
//
//    F->FIFO = applyAllocIntArray(n);
//    F->color = applyAllocCharArray(n);
//    F->n = n;
//    F->first = F->last = 0;
//
//    return(F);
//}

FIFO* CreateFIFO(int n)
{
    FIFO* F = (FIFO*)applyAlloc(1, sizeof(FIFO));

    F->FIFO = applyAllocIntArray(n);
    F->weights = applyAllocIntArray(n);  // Allocate memory for weights
    F->color = applyAllocCharArray(n);
    F->n = n;
    F->first = F->last = 0;

    return(F);
}

//void DestroyFIFO(FIFO** F) {
//    if (F != NULL) {
//        FIFO* aux = *F;
//
//        if (aux != NULL) {
//            applyFree(aux->FIFO);
//            applyFree(aux->color);
//            applyFree(aux);
//            *F = NULL;
//        }
//    }
//}

void DestroyFIFO(FIFO** F) {
    if (F != NULL) {
        FIFO* aux = *F;

        if (aux != NULL) {
            applyFree(aux->FIFO);
            applyFree(aux->weights);  // Free memory for weights
            applyFree(aux->color);
            applyFree(aux);
            *F = NULL;
        }
    }
}

//char InsertFIFO(FIFO* F, int elem)
//{
//    if (FullFIFO(F)) {
//        printf("FIFO Full. \n");
//        return 0;
//    }
//    F->color[elem] = IFT_GRAY;
//    F->FIFO[F->last] = elem;  F->last++;
//
//    return 1;
//}

char InsertFIFO(FIFO* F, int elem, int weight)
{
    if (FullFIFO(F)) {
        printf("FIFO Full. \n");
        return 0;
    }
    F->color[elem] = IFT_GRAY;
    F->FIFO[F->last] = elem;
    F->weights[F->last] = weight;  // Store weight
    F->last++;

    // Sort the FIFO based on weights
    for (int i = F->first; i < F->last; ++i) {
        for (int j = i + 1; j < F->last; ++j) {
            if (F->weights[i] > F->weights[j]) {
                // Swap elements
                int temp = F->FIFO[i];
                F->FIFO[i] = F->FIFO[j];
                F->FIFO[j] = temp;

                // Swap weights
                temp = F->weights[i];
                F->weights[i] = F->weights[j];
                F->weights[j] = temp;
            }
        }
    }

    return 1;
}

int RemoveFIFO(FIFO* F)
{
    int node = IFT_NIL;

    if (!EmptyFIFO(F)) {
        node = F->FIFO[F->first];  F->first++;
        F->color[node] = IFT_BLACK;
    }
    else {
        printf("FIFO is empty. \n");
    }

    return node;
}

bool EmptyFIFO(FIFO* F)
{
    if (F->first == F->last) {
        // Changed by Falcao and Nikolas
        // iftResetFIFO(F);
        return(1);
    }
    else
        return(0);
}

bool FullFIFO(FIFO* F)
{
    if (F->last == F->n) {
        return(1);
    }
    else
        return(0);
}

void ResetFIFO(FIFO* F)
{
    int p;
    for (p = 0; p < F->n; p++)
        F->color[p] = IFT_WHITE;
    F->first = F->last = 0;
}

int ColorFIFO(FIFO* F, int pos)
{
    return F->color[pos];
}

bool IsInFIFO(FIFO* F, int elem)
{
    for (int i = F->first; i < F->last; ++i) {
        if (F->FIFO[i] == elem) {
            return true;
        }
    }
    return false;
}

void UpdateWeightFIFO(FIFO* F, int elem, int new_weight)
{
    int i;
    for (i = F->first; i < F->last; i++) {
        if (F->FIFO[i] == elem) {
            break;
        }
    }
    if (i < F->last) { // se o elemento foi encontrado
        F->weights[i] = new_weight; // atualizar o peso do elemento

        // reordenar a fila
        while (i > F->first && F->weights[i - 1] > new_weight) {
            // trocar o elemento e seu peso com o elemento anterior
            int temp = F->FIFO[i];
            F->FIFO[i] = F->FIFO[i - 1];
            F->FIFO[i - 1] = temp;

            temp = F->weights[i];
            F->weights[i] = F->weights[i - 1];
            F->weights[i - 1] = temp;

            i--;
        }
    }
}
