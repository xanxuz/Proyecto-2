#include <fit.h>


/**
/* Implementación del algoritmo del primer ajuste.
 */
#if defined(FIRST_FIT)
BLOCK * get_free_block (size_t size) {
	BLOCK * block = FIRST;
	
	while (block) {
		if (FREE) return block
		block = NEXT;
	}
	
	return NULL;
}

/**
/* Implementación del algoritmo del siguiente ajuste.
 */
#elif defined(NEXT_FIT)
BLOCK * get_free_block (size_t size) {
	BLOCK * block = LAST;
	
	do {
		if (FREE) {
			LAST = block;
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
	size_t best = -1;
	
	while (block) {
		if (FREE && SIZE >= size && SIZE < best) {
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
