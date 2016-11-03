#pragma once
#ifndef CUSTOM_MALLOC_H
#define CUSTOM_MALLOC_H

#include <stdio.h>  /* Imports printf. */
#include <stdlib.h> /* Imports the malloc family of functions. */
#include <signal.h> /* Imports kill. */
#include <unistd.h> /* Imports getpid. */
#include <string.h> /* Imports memset. */

/* Undefine the malloc family so that we can then reimplement it. */
#undef malloc
#undef calloc
#undef realloc
#undef free

#define MEM_SIZE 314572800 /* 300 MB. */
#define BLOCK struct BLOCK /* Estructura de bloque */

BLOCK { /* Información de cada bloque de memoria */
	char free;
	size_t size;
	BLOCK *next;
};
#define HEAD_SIZE (sizeof(BLOCK)) /* Tamaño que ocupa la información de cada bloque */

static void * mem_start = NULL;
static void * mem_end = NULL;
static BLOCK * ind = NULL; /* Bloque de memoria índice */

void align_block (BLOCK *block, size_t size) { /* Ajusta el tamaño de un bloque al solicitado */
  if (block->size + HEAD_SIZE > size) {
    BLOCK *next = block->next; /* Guardo el puntero al bloque adyacente */
    BLOCK *rest = (void *)(block + 1) + block->size; /* Ubicación del bloque nuevo */
    block->next = rest; /* El bloque a modificar apunta al bloque nuevo */
    rest->next = next; /* El bloque nuevo apunta al que estaba adyacente */
    rest->free = 1; /* Se marca libre */
    rest->size = block->size - HEAD_SIZE - size ; /* Tamaño del bloque nuevo */
    block->size = size; /* Tamaño del bloque actualizado */ 
  }
  else kill(getpid(), SIGSEGV); /* Error, incongruencia de tamaños */
}

void merge_blocks () { /* Recorre la lista colapsando bloques libres contiguos */
  BLOCK *first = mem_start;
}

#ifdef FIRST_FIT /* Algoritmo de primer ajuste */
BLOCK *get_block (size_t size) {
  return NULL;
}

#elif defined(BEST_FIT) /* Algoritmo de mejor ajuste */
BLOCK *get_block (size_t size) {
  return NULL;
}

#elif defined(WORST_FIT) /* Algoritmo de peor ajuste */
BLOCK *get_block (size_t size) {
  return NULL;
}

#elif defined(NEXT_FIT) /* Algoritmo de siguiente ajuste */
BLOCK *get_block (size_t size) {
  return NULL;
}

#else
#error no se definió un algoritmo de ajuste
#endif

static void set_initial_memory() {
  if(mem_start == NULL && mem_end == NULL) {
    mem_start = sbrk(0); /* Esto retorna la dirección actual del borde del segmento de datos. */
    sbrk(MEM_SIZE);     /* Esto mueve el borde del segmento de datos en MEM_SIZE bytes. */
    mem_end = sbrk(0);
    
    ind = mem_start; /* Se registra el bloque de memoria inicial */
    ind->free = 1;
    ind->size = MEM_SIZE; /* Es el tamaño de la memoria menos el espacio que ocupa la información */
    ind->next = NULL;
    
    printf("\n********************************************************************************\n");
    printf("CUSTOM MALLOC FAMILY INITIALIZATION\n");
    printf("\tData segment start = 0x%X\n", mem_start);
    printf("\tData segment end   = 0x%X\n", mem_end);
    printf("\tData segment size  = %d bytes\n", mem_end - mem_start);
    printf("********************************************************************************\n\n");
  }
}

/**
 * Print the contents of the free BLOCKs list.
 */
void print_free_list() {

}

/**
 * Custom malloc reimplementation.
 */
void * malloc(size_t size) {
  set_initial_memory(); /* NO QUITAR. */
  printf("Calling malloc with size = %lld\n", size);
  
  if (!size) return NULL; /* Caso borde, no se retorna ningún bloque */
  BLOCK *block = get_block(size); /* Se busca un bloque que se pueda usar según un algoritmo */
  if (!block) return NULL; /* Si no se consigue, no se retorna ningún bloque */
  align_block(block, size); /* Ajusta el tamaño del bloque al solicitado */
  return block + 1; /* Se retorna la dirección que sigue a la información del bloque */
}

/**
 * Custom calloc reimplementation.
 */
void * calloc(size_t nmemb, size_t size) {
  set_initial_memory(); /* NO QUITAR. */
  printf("Calling calloc with nmemb = %lld and size = %lld\n", nmemb, size);
  
  size += nmemb; /* Tamaño necesario */
  void *ptr = malloc(size); /* Dirección del bloque a usar */
  memset(ptr, 0, size); /* Se asigna cero en el espacio necesario */
  return ptr; /* Retorno la dirección del bloque que se usará */
}

/**
 * Custom realloc reimplementation.
 */
void * realloc(void * ptr, size_t size) {
  set_initial_memory(); /* NO QUITAR. */
  printf("Calling realloc with ptr= 0x%X and size = %lld\n", ptr, size);
  
  if (!size) {
    free(ptr);
    return NULL;
  }
  
  if (!ptr) { /* Puntero nulo */
    if (size > 0) return malloc(size); /* Malloc con tamaño mayor que cero */
    return NULL; /* Nulo con tamaño menor o igual que cero */
  }
  
  BLOCK *block = ptr;
  if (block->size == size) return ptr; /* Si el tamaño solicitado es el mismo no hay cambios */
  if (block->size > size) align_block(block, size); /* Si el tamaño solicitado es menor se reduce el bloque */
  else {
    /* Busco bloque adyacente libre, si no es lo suficientemente grande busco uno nuevo */
  }
  
  return block;
}

/**
 * Custom free reimplementation.
 */
void free(void *ptr) {
  set_initial_memory(); /* NO QUITAR. */
  printf("Calling free with ptr = 0x%X\n", ptr);
  
  if (!ptr) return; /* Caso borde, puntero nulo no hace nada */
  if (ptr < mem_start || ptr >= mem_end) kill(getpid(), SIGSEGV); /* Error, puntero inválido */
  
  BLOCK *block = (BLOCK *) ptr - 1;
  if (block->free) kill(getpid(), SIGSEGV); /* Error, bloque libre */
  block->free = 1; /* Se libera el bloque */
  merge_blocks(); /* Se colapsan bloques libres adyacentes */
}

#endif
