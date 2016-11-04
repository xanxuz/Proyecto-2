#ifndef LIST_H
#define LIST_H

#include <stddef.h>

/**
/* Macros auxiliares para trabajar con la estrucutura
/* de bloque de memoria.
 */
#define BLOCK struct block
#define HEAD_SIZE sizeof(BLOCK)

#define SIZE block->size
#define FREE block->free
#define NEXT block->next


/**
/* Estructura de bloque de memoria.
/* Contiene información mínima sobre el bloque.
/* Tamaño, uso (ocupado/libre) y un puntero al siguiente bloque.
 */
BLOCK {
	size_t size;
	int free;
	BLOCK * next;
};


/**
/* Primer bloque de la memoria.
 */
#define FIRST first
static BLOCK * first = NULL;

/**
/* Para el siguiente ajuste se usa un apuntador al último bloque
/* usado para comenzar la búsqueda a partir de él.
 */
#ifdef NEXT_FIT
static BLOCK * last = NULL;
#endif


/**
/* Inicializa el primer bloque de memoria con el tamaño indicado.
 */
void set_first_block (void *);

/**
/* Ajusta el bloque al tamaño indicado y crea un bloque libre
/* adyacente al bloque modificado con los bytes sobrantes.
/* El nuevo tamaño debe ser menor o igual al tamaño actual.
 */
void trunk (BLOCK *, size_t);

/**
/* Recorre la lista de bloques colapsando en uno sólo aquellos
/* que estén libres y se encuentren adyacentes
 */
void merge ();

#endif
