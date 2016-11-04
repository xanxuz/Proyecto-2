#include "../include/mmanager.h"


static void set_initial_memory () {
	if(mem_start == NULL && mem_end == NULL) {
		mem_start = sbrk(0); /* Esto retorna la dirección actual del borde del segmento de datos. */
		sbrk(MEM_SIZE);     /* Esto mueve el borde del segmento de datos en MEM_SIZE bytes. */
		mem_end = sbrk(0);
		
		set_first_block(mem_start);
		
		printf("\n********************************************************************************\n");
		printf("CUSTOM MALLOC FAMILY INITIALIZATION\n");
		printf("\tData segment start = 0x%X\n", DIR(mem_start));
		printf("\tData segment end   = 0x%X\n", DIR(mem_end));
		printf("\tData segment size  = %ld bytes\n", mem_end - mem_start);
		printf("********************************************************************************\n\n");
	}
}

void * malloc (size_t size) {
	set_initial_memory(); /* NO QUITAR. */
	printf("Calling malloc with size = %lu\n", size);
	
	if (!size) return NULL; 
	
	BLOCK * block = get_free_block(size);
	if (!block) return NULL;
	
	trunk(block, size);
	return (block + 1);
}

void * calloc (size_t nmemb, size_t size) {
	set_initial_memory(); /* NO QUITAR. */
	printf("Calling calloc with nmemb = %lu and size = %lu\n", nmemb, size);
	
	size *= nmemb;
	void *ptr = malloc(size);
	memset(ptr, 0, size);
	return ptr;
}

void * realloc(void * ptr, size_t size) {
	set_initial_memory(); /* NO QUITAR. */
	printf("Calling realloc with ptr= 0x%X and size = %lu\n", DIR(ptr), size);
	  
	if (!size) {
		free(ptr);
		return NULL;
	}
  
	if (!ptr) {
		if (size > 0) return malloc(size);
		return NULL;
	}
	
	BLOCK * block = (BLOCK *)(ptr - 1);
	if (SIZE == size) return ptr;
	if (SIZE > size) trunk(block, size);
	else {
		if ((NEXT)->free && SIZE + (NEXT)->size + HEAD_SIZE >= size) {
			NEXT->size -= size - SIZE;
			NEXT = (block + 1) + size;
			SIZE = size;
		}
		else {
			BLOCK * old = block;
			free(block);
			
			block = malloc(size);
			if (!block) block = malloc(old->size);
			
			memcpy(block, ptr, old->size);
		}
	}
	
	return block;
}

void free(void *ptr) {
	set_initial_memory(); /* NO QUITAR. */
	printf("Calling free with ptr = 0x%X\n", DIR(ptr));
	
	if (!ptr) return; 
	if (ptr < mem_start || ptr >= mem_end) KILL;
	
	BLOCK *block = (BLOCK *)(ptr - 1);
	if (FREE) KILL;
	FREE = 1;
	merge();
}
