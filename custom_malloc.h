#pragma once
#ifndef CUSTOM_MALLOC_H
#define CUSTOM_MALLOC_H

#include <stdio.h>  /* Imports printf. */
#include <stdlib.h> /* Imports the malloc family of functions. */
#include <signal.h> /* Imports kill. */
#include <unistd.h> /* Imports getpid. */

/* Undefine the malloc family so that we can then reimplement it. */
#undef malloc
#undef calloc
#undef realloc
#undef free

#define MEM_SIZE 314572800 /* 300 MB. */

typedef struct { /* Información de cada bloque de memoria */
	char free;
	size_t size;
	block *next;
} block;

#define HEAD_SIZE (sizeof(block)) /* Tamaño que ocupa la información de cada bloque */


static void * mem_start = NULL;
static void * mem_end = NULL;
static block * index = NULL; /* Primer bloque de memoria */

static void set_initial_memory() {
  if(mem_start == NULL && mem_end == NULL) {
    mem_start = sbrk(0); /* Esto retorna la dirección actual del borde del segmento de datos. */
    sbrk(MEM_SIZE);     /* Esto mueve el borde del segmento de datos en MEM_SIZE bytes. */
    mem_end = sbrk(0);
    
    index = mem_start; /* Se registra el bloque de memoria inicial */
    index->free = 1;
    index->size = MEM_SIZE - HEAD_SIZE; /* Es el tamaño de la memoria menos el espacio que ocupa la información */
    index->next = NULL;
    
    printf("\n********************************************************************************\n");
    printf("CUSTOM MALLOC FAMILY INITIALIZATION\n");
    printf("\tData segment start = 0x%X\n", mem_start);
    printf("\tData segment end   = 0x%X\n", mem_end);
    printf("\tData segment size  = %d bytes\n", mem_end - mem_start);
    printf("********************************************************************************\n\n");
  }
}

/**
 * Print the contents of the free blocks list.
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
  block *target = free_block(size); /* Se busca un bloque que se pueda usar */
  if (!target) return NULL; /* Si no se consigue, no se retorna ningún bloque */
  return target + 1; /* Se retorna la dirección que sigue a la información del bloque */
}

/**
 * Custom calloc reimplementation.
 */
void * calloc(size_t nmemb, size_t size) {
  set_initial_memory(); /* NO QUITAR. */
  printf("Calling calloc with nmemb = %lld and size = %lld\n", nmemb, size);
  
  size_t size += nmemb; /* Tamaño necesario */
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
  
  return NULL;
}

/**
 * Custom free reimplementation.
 */
void free(void *ptr) {
  set_initial_memory(); /* NO QUITAR. */
  printf("Calling free with ptr = 0x%X\n", ptr);
  
  /* Valdiar puntero */
  
  if (!ptr) return;
  
  block *target = (block *) ptr - 1;
}

#ifdef FIRST_FIT /* Algoritmo de primer ajuste */

#elif defined(BEST_FIT) /* Algoritmo de mejor ajuste */

#elif defined(WORST_FIT) /* Algoritmo de peor ajuste */

#elif defined(NEXT_FIT) /* Algoritmo de siguiente ajuste */

#else
	#error "Error: No se definió un algoritmo de ajuste"
#endif

#endif
