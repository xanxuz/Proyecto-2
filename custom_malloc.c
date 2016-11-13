#if !defined(FIRST_FIT) && !defined(NEXT_FIT) && !defined(BEST_FIT) && !defined(WORST_FIT)
	#error no se ha definido un algoritmo de ajuste valido
#endif

#include "custom_malloc.h"


/**
/* Inicializa la memoria y el primer bloque con el tamaño libre,
/* el tamaño libre es el tamaño de la memoria menos el tamaño del
/* encabezado del bloque, es decir, el tamaño del struct block.
 */
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

/**
/* Recorre la lista de bloques e imprime la información del
/* bloque actual si éste es un bloque libre.
 */
void print_free_list () {
	BLOCK * block = FIRST;
	size_t size = 0;
	int i = 0;
	
	printf("Bloques libres:\n");
	
	while (block) {
		if (FREE) {
			printf("%2d) Address = 0x%08X\tNext = 0x%08X\tSize = %9lu B\n", i + 1, DIR(block), DIR(NEXT), SIZE);
			i++;
		}
		
		block = NEXT;
	}
	
	/**
	/* Descomentar para imprimir bloques usados
	 *//*
	printf("Bloques usados:\n");
	block = FIRST;
	while (block) {
		if (!FREE) {
			printf("%2d) Address = 0x%08X\tNext = 0x%08X\tSize = %9lu B\n", i + 1, DIR(block), DIR(NEXT), SIZE);
			i++;
		}
		
		block = NEXT;
	}
	 */
}

/**
/* Implementación del algoritmo del primer ajuste.
/* Busca el primer bloque libre con tamaño suficiente desde
/* el principio.
/* Si llego al final y no consigo, nada retorno NULL.
 */
#if defined(FIRST_FIT) && !defined(NEXT_FIT) && !defined(BEST_FIT) && !defined(WORST_FIT)
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
/* Busca el primer bloque libre con tamaño suficiente desde el
/* bloque siguiente al último que se usó.
/* Guardo el bloque siguiente al que voy a usar y si llego al
/* último bloque comienzo desde el primero.
/* Si llego otra vez al bloque donde inicia la búsqueda,
/* retorno NULL.
 */
#elif !defined(FIRST_FIT) && defined(NEXT_FIT) && !defined(BEST_FIT) && !defined(WORST_FIT)
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
/* Tomo como mejor tamaño el mayor tamaño posible y recorro todos
/* los bloques buscando el bloque libre que tenga el menor
/* tamaño suficiente.
/* Retorno el mejor bloque, si no se consigue será NULL.
 */
#elif !defined(FIRST_FIT) && !defined(NEXT_FIT) && defined(BEST_FIT) && !defined(WORST_FIT)
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
/* Tomo como peor tamaño el mayor tamaño solicitado y recorro todos
/* los bloques buscando el bloque libre que tenga el mayor tamaño.
/* Actualizo el peor tamaño por cada candidato que consiga y 
/* retorno el mejor peor, si no se consigue será NULL.
 */
#elif !defined(FIRST_FIT) && !defined(NEXT_FIT) && !defined(BEST_FIT) && defined(WORST_FIT)
BLOCK * get_free_block (size_t size) {
	BLOCK * block = FIRST;
	BLOCK * ptr = NULL;
	size_t worst = size;
	
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
	#error se ha definido mas de un ajuste
#endif

/**
/* Inicializa el primer bloque de memoria con el tamaño indicado. Si se
/* va a usar el siguiente ajuste se inicializa también el puntero al
/* último bloque usado.
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
void trunk (BLOCK *block, size_t size) {
	if (SIZE == size) {
		FREE = 0;
		return;
	}
	
	
	BLOCK * next = NEXT;
	
	NEXT = (void *)(block + 1) + size;
	(NEXT)->size = SIZE - size - HEAD_SIZE;
	(NEXT)->next = next;
	(NEXT)->free = 1;

	SIZE = size;
	FREE = 0;
	
	merge();
}

/**
/* Busca un bloque libre segun el algoritmo de ajuste indicado y
/* trunca el bloque nuevo en el caso que sea posible.
/* CASOS BORDE:
/* Si el tamaño es cero retorna NULL.
/* Si el algoritmo de ajuste no consigue un bloque, retorna NULL.
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
/* Usa cmalloc para hallar el bloque nuevo y lo inicializa con ceros.
/* CASOS BORDE:
/* Si cmalloc no consigue un bloque, se retorna NULL.
 */
void * ccalloc (size_t nmemb, size_t size) {
	set_initial_memory(); /* NO QUITAR. */
	printf("Calling ccalloc with nmemb = %lu and size = %lu\n", nmemb, size);

	size *= nmemb;
	void *ptr = cmalloc(size);
	if (!ptr) return NULL;
	
	memset(ptr, 0, size);
	return ptr;
}

/**
/* Si el tamaño nuevo es menor al del bloque actual, se trunca el
/* bloque.
/* Si el tamaño es mayor, el bloque adyacente está libre y el tamaño
/* nuevo es menor que la suma del bloque actual y el adyacente, se
/* expande el bloque actual y trunca el adyacente.
/* Si no es ninguno de los casos anteriores, se hace free al bloque
/* actual, busca un nuevo bloque con el algoritmo de juste indicado y
/* se copia el contenido del bloque anterior al bloque nuevo.
/* CASOS BORDE:
/* Si el tamaño nuevo es igual al bloque actual no se hace nada.
/* Si el tamaño nuevo es cero, se aplica cfree al bloque indicado.
/* Si el puntero es NULL, se aplica cmalloc con el tamaño nuevo
/* y retorna el nuevo bloque.
/* Si no se consigue un nuevo bloque para realocar, se retorna NULL.
 */
void * crealloc(void * ptr, size_t size) {
	set_initial_memory(); /* NO QUITAR. */
	printf("Calling crealloc with ptr= 0x%08X and size = %lu\n", DIR(ptr), size);
	
	if (!size) {
		cfree(ptr);
		return NULL;
	}

	if (!ptr) return cmalloc(size);
	

	BLOCK * block = (BLOCK *)ptr - 1;
	if (SIZE == size) return ptr;
	if (SIZE > size) {
		trunk(block, size);
		return (block + 1);
	}
	if ((NEXT)->free && SIZE + (NEXT)->size + HEAD_SIZE >= size) {
		BLOCK * next = NEXT;

		NEXT = (void *)(block + 1) + size;
		(NEXT)->size -= size - SIZE - HEAD_SIZE;
		(NEXT)->free = 1;
		
		SIZE = size;
		
		return (block + 1);
	}
	
	BLOCK * old = (block + 1);
	cfree(block + 1);
	
	block = cmalloc(size);
	if (!block) return NULL;
	
	memcpy(block, ptr, (old - 1)->size);
	return block;
}

/**
/* Busco el bloque libre al que apunta ptr (si está en medio
/* de un bloque lo trunco) y marco el bloque como libre.
/* CASOS BORDE:
/* Si ptr apunta a una zona fuera del heap se envía SIGSEGV
/* Si el bloque encontrado ya está libre se envía SIGSEGV
 */
void cfree(void *ptr) {
	set_initial_memory(); /* NO QUITAR. */
	printf("Calling free with ptr = 0x%08X\n", DIR(ptr));

	if (!ptr) return;
	if (ptr < mem_start || ptr >= mem_end) KILL;

	BLOCK * target = (BLOCK *)ptr - 1;
	BLOCK * block = FIRST;
	BLOCK * aux = NULL;

	while (block < target && block != NULL) {
		aux = block;
		block = NEXT;
		
		if (block > target) {
			BLOCK * next = block;
			block = aux;
			
			if (FREE) KILL;
			
			
			NEXT = ptr;
			SIZE = (size_t)(NEXT) - (size_t)(block + 1);
			
			block = NEXT;
			NEXT = next;
			SIZE = (size_t)(NEXT) - (size_t)(block + 1);
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
