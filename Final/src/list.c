#include "../include/list.h"


/**
/* Inicializa el primer bloque de memoria con el tamaño indicado. Si se
/* va a usar el siguiente ajuste se inicializa también el puntero al
/* último bloque usado
 */
void set_first_block (void * ptr) {
	FIRST = ptr;
	
	#ifdef NEXT_FIT
	last = FIRST;
	#endif
}

/**
/* Ajusta el bloque al tamaño indicado y crea un bloque libre
/* adyacente al bloque modificado con los bytes sobrantes.
/* El nuevo tamaño debe ser menor o igual al tamaño actual.
 */
void trunk (BLOCK *block, size_t size) { /* Ajusta el tamaño de un bloque al solicitado */
	if (SIZE > size) {
		if (SIZE - size <= HEAD_SIZE) return;
		
		BLOCK * next = NEXT;
		BLOCK * rest = (block + 1) + size;
		
		NEXT = rest;
		rest->next = NEXT;
		
		rest->size = SIZE - size - HEAD_SIZE;
		SIZE = size;
	}
}

/**
/* Recorre la lista de bloques colapsando en uno sólo aquellos
/* que estén libres y se encuentren adyacentes.
 */
void merge () {
	BLOCK *block = FIRST;
	
	while (block) {
		if ((NEXT)->free) {
			SIZE += (NEXT)->size + HEAD_SIZE;
			NEXT = (NEXT)->next;
		}
		else block = NEXT;
	}
}
