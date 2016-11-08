#include "custom_malloc.h"


static void set_initial_memory () {
	if(mem_start == NULL && mem_end == NULL) {
		mem_start = sbrk(0); /* Esto retorna la dirección actual del borde del segmento de datos. */
		sbrk(MEM_SIZE);     /* Esto mueve el borde del segmento de datos en MEM_SIZE bytes. */
		mem_end = sbrk(0);

		set_first_block(mem_start, MEM_SIZE - HEAD_SIZE);

		printf("\n********************************************************************************\n");
		printf("CUSTOM cmalloc FAMILY INITIALIZATION\n");
		printf("\tData segment start = 0x%08X\n", DIR(mem_start));
		printf("\tData segment end   = 0x%08X\n", DIR(mem_end));
		printf("\tData segment size  = %ld bytes\n", mem_end - mem_start);
		printf("********************************************************************************\n\n");
	}
}

void print_free_list () {
	BLOCK * block = FIRST;
	size_t size = 0;
	int cont = 0;
	
	printf("List of free blocks:\n");
	while (block) {
		if (FREE) {
			printf("Adress = 0x%08X\tSize = %9lu\tNext block = 0x%08X\n", DIR(block), SIZE, DIR(NEXT));
			size += SIZE;
			 cont++;
		}
		
		block = NEXT;
	}
	
	printf("\tblocks = % 2d\t\tbytes = %lu\n", cont, size);
	return;
}

/**
/* Implementación del algoritmo del primer ajuste.
 */
#if defined(FIRST_FIT)
BLOCK * get_free_block (size_t size) {
	BLOCK * block = FIRST;
	
	while (block) {
		if (FREE && SIZE >= size) return block;
		block = NEXT;
	}

	return NULL;
}

/**
/* Implementación del algoritmo del siguiente ajuste.
 */
#elif defined(NEXT_FIT)
BLOCK * get_free_block (size_t size) {
	if (!LAST) LAST = FIRST; 
	BLOCK * block = LAST;
	
	do {
		if (FREE && SIZE >= size) {
			LAST = NEXT;
			return block;
		}

		block = NEXT;

		if (!block) block = FIRST;
	} while (block != LAST);

	return NULL;
}

/**
/* Implementación del algoritmo del mejor ajuste.
 */
#elif defined(BEST_FIT)
BLOCK * get_free_block (size_t size) {
	BLOCK * block = FIRST;
	BLOCK * ptr = NULL;
	size_t best = MEM_SIZE - HEAD_SIZE;

	while (block) {
		if (FREE && SIZE >= size && SIZE <= best) {
			best = SIZE;
			ptr = block;
		}

		block = NEXT;
	}

	return ptr;
}

/**
/* Implementación del algoritmo del peor ajuste.
 */
#elif defined(WORST_FIT)
BLOCK * get_free_block (size_t size) {
	BLOCK * block = FIRST;
	BLOCK * ptr = NULL;
	size_t worst = 0;

	while (block) {
		if (FREE && SIZE > worst) {
			worst = SIZE;
			ptr = block;
		}

		block = NEXT;
	}

	return ptr;
}

/**
/* Si no se especifica un algoritmo de ajuste se muestra un error
/* y detiene la compilación.
 */
#else
	#error no se ha especificado un algoritmo de ajuste
#endif

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

		NEXT = (block + 1) + size;
		(NEXT)->size = SIZE - size - HEAD_SIZE;
		(NEXT)->next = next;
		(NEXT)->free = 1;

		SIZE = size;
		FREE = 0;
	}
}

/**
/* Implementación personalizada de malloc.
 */
void * cmalloc (size_t size) {
	set_initial_memory(); /* NO QUITAR. */
	printf("Calling cmalloc with size = %lu\n", size);
	
	if (!size) return NULL;
	
	BLOCK * block = get_free_block(size);
	if (!block) return NULL;

	trunk(block, size);
	return (block + 1);
}

/**
/* Implementación personalizada de calloc.
 */
void * ccalloc (size_t nmemb, size_t size) {
	set_initial_memory(); /* NO QUITAR. */
	printf("Calling ccalloc with nmemb = %lu and size = %lu\n", nmemb, size);

	size *= nmemb;
	void *ptr = cmalloc(size);
	memset(ptr, 0, size);
	return ptr;
}

/**
/* Implementación personalizada de realloc.
 */
void * crealloc(void * ptr, size_t size) {
	set_initial_memory(); /* NO QUITAR. */
	printf("Calling crealloc with ptr= 0x%08X and size = %lu\n", DIR(ptr), size);
	
	if (!size) {
		cfree(ptr);
		return NULL;
	}

	if (!ptr) {
		if (size > 0) return cmalloc(size);
		return NULL;
	}

	BLOCK * block = (BLOCK *)ptr - 1;
	if (SIZE == size) return ptr;
	if (SIZE > size) {
		trunk(block, size);
		return (block + 1);
	}
	if ((NEXT)->free && SIZE + (NEXT)->size + HEAD_SIZE >= size) {
		NEXT->size -= size - SIZE;
		NEXT = (block + 1) + size;
		SIZE = size;
	} else {
		BLOCK * old = (block + 1);
		cfree(old);

		block = cmalloc(size);
		if (!block) block = cmalloc((old - 1)->size);

		memcpy(block, ptr, (old - 1)->size);
	}
	
	return block;
}

/**
/* Implementación personalizada de free.
 */
void cfree(void *ptr) {
	set_initial_memory(); /* NO QUITAR. */
	printf("Calling free with ptr = 0x%08X\n", DIR(ptr));

	if (!ptr) return;
	if (ptr < mem_start || ptr >= mem_end) KILL;

	BLOCK * target = (BLOCK *)ptr - 1;
	BLOCK * block = FIRST;

	while (block != target) {
		if (NEXT) block = NEXT;
		else {
			block = NULL;
			break;
		}
	}

	if (!block || FREE) KILL;
	FREE = 1;
	merge();
}

/**
/* Recorre la lista de bloques colapsando en uno sólo aquellos
/* que estén libres y se encuentren adyacentes.
 */
void merge () {
	BLOCK *block = FIRST;

	while (NEXT) {
		if (FREE && (NEXT)->free) {
			SIZE += (NEXT)->size + HEAD_SIZE;
			NEXT = (NEXT)->next;
		}
		else block = NEXT;
	}
}
