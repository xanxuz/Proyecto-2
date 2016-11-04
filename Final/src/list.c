#include <list.h>

BLOCK * FIRST = NULL;
BLOCK * LAST = NULL;


/**
/* Inicializa el primer bloque de memoria con el tamaño indicado. Si se
/* va a usar el siguiente ajuste se inicializa también el puntero al
/* último bloque usado
 */
void set_first_block (void * ptr, size_t size) {
	FIRST = ptr;
	FIRST->free = 1;
	FIRST->size = size;
	FIRST->next = NULL;
	
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
		
		NEXT = (void *)((char *)block + HEAD_SIZE + size);
		(NEXT)->size = SIZE - size - HEAD_SIZE;
		(NEXT)->next = next;
		(NEXT)->free = 1;
		
		SIZE = size;
		FREE = 0;
	}
}

/**
/* Recorre la lista de bloques colapsando en uno sólo aquellos
/* que estén libres y se encuentren adyacentes.
 */
void merge () {
	BLOCK *block = FIRST;
	
	while (NEXT) {
		if ((NEXT)->free) {
			SIZE += (NEXT)->size + HEAD_SIZE;
			NEXT = (NEXT)->next;
		}
		else block = NEXT;
	}
}
