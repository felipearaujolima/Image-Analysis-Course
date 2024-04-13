#include "adjacency.h"

Voxel applyGetAdjacentVoxel(AdjRel* A, Voxel u, int adj)
{
    Voxel v;

    v.x = u.x + A->dx[adj];
    v.y = u.y + A->dy[adj];
    v.z = u.z + A->dz[adj];
    v.t = u.t + A->dt[adj];

    return(v);
}

AdjRel* CopyAdjacency(AdjRel* A) {
    AdjRel* B = CreateAdjRel(A->n);
    int i;
    for (i = 0; i < A->n; i++) {
        B->dx[i] = A->dx[i];
        B->dy[i] = A->dy[i];
        B->dz[i] = A->dz[i];
        B->dt[i] = A->dt[i];
    }

    return(B);
}

AdjRel* CreateAdjRel(int n) /*! \brief Allocates memory for a
					  3D adjacency relation */
{
	AdjRel* A = (AdjRel*)applyAlloc(1, sizeof(AdjRel));

	A->dx = (int*)applyAllocIntArray(n);
	A->dy = (int*)applyAllocIntArray(n);
	A->dz = (int*)applyAllocIntArray(n);
	A->dt = (int*)applyAllocIntArray(n);
	A->n = n;

	return(A);
}

void DestroyAdjRel(AdjRel** A) /*! \brief Deallocates memory for a
						 3D adjacency relation */
{
	AdjRel* aux = *A;

	if (aux != NULL) {
		if (aux->dx != NULL) applyFree(aux->dx);
		if (aux->dy != NULL) applyFree(aux->dy);
		if (aux->dz != NULL) applyFree(aux->dz);
		if (aux->dt != NULL) applyFree(aux->dt);
		applyFree(aux);
		*A = NULL;
	}
}

AdjRel* adjRectangular(int xsize, int ysize)
{
	AdjRel* A;
	int i, dx, dy, n, i0 = 0;

	n = 0;
	for (dy = -ysize / 2; dy <= ysize / 2; dy++)
		for (dx = -xsize / 2; dx <= xsize / 2; dx++)
			n++;

	A = CreateAdjRel(n);

	i = 0;
	for (dy = -ysize / 2; dy <= ysize / 2; dy++)
		for (dx = -xsize / 2; dx <= xsize / 2; dx++) {
			A->dx[i] = dx;
			A->dy[i] = dy;
			A->dz[i] = 0;
			if ((dx == 0) && (dy == 0))
				i0 = i;
			i++;
		}

	/* shift to right and place central point (origin) at first */

	for (i = i0; i > 0; i--) {
		dx = A->dx[i];
		dy = A->dy[i];
		A->dx[i] = A->dx[i - 1];
		A->dy[i] = A->dy[i - 1];
		A->dx[i - 1] = dx;
		A->dy[i - 1] = dy;
	}

	return(A);
}

AdjRel* adjCircular(float r) {
    int r0 = (int)r;
    float r2 = (int)(r * r + 0.5);

    int n = 0;
    for (int dy = -r0; dy <= r0; dy++)
        for (int dx = -r0; dx <= r0; dx++)
            if (((dx * dx) + (dy * dy)) <= r2)
                n++;

    AdjRel* A = CreateAdjRel(n);
    int i = 0;
    int i0 = 0;
    for (int dy = -r0; dy <= r0; dy++)
        for (int dx = -r0; dx <= r0; dx++)
            if (((dx * dx) + (dy * dy)) <= r2) {
                A->dx[i] = dx;
                A->dy[i] = dy;
                A->dz[i] = 0;

                if ((dx == 0) && (dy == 0))
                    i0 = i;
                i++;
            }

    // shift to right and place central pixel at first
    for (int i = i0; i > 0; i--) {
        int dx = A->dx[i];
        int dy = A->dy[i];
        A->dx[i] = A->dx[i - 1];
        A->dy[i] = A->dy[i - 1];
        A->dx[i - 1] = dx;
        A->dy[i - 1] = dy;
    }


    // sort by radius, so the 4 closest neighbors will come first
    float* dr = applyAllocFloatArray(A->n);
    //for (int i = 0; i < A->n; i++)
    //    dr[i] = A->dx[i] * A->dx[i] + A->dy[i] * A->dy[i];

    IntArray* idxs = IntRange(0, A->n - 1, 1);
    //iftFQuickSort(dr, idxs->val, 0, A->n - 1, IFT_INCREASING);
    AdjRel* Asort = CreateAdjRel(A->n);

    for (int i = 0; i < A->n; i++) {
        int idx = idxs->val[i];
        Asort->dx[i] = A->dx[idx];
        Asort->dy[i] = A->dy[idx];
        Asort->dz[i] = A->dz[idx];
    }

    applyFree(dr);
    DestroyIntArray(&idxs);
    DestroyAdjRel(&A);

    return Asort;
}