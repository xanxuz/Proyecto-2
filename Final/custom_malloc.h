#pragma once
#ifndef CUSTOM_MALLOC_H
#define CUSTOM_MALLOC_H

#include <stdio.h>  /* Imports printf. */
#include <stdlib.h> /* Imports the malloc family of functions. */
#include <string.h> /* Imports memset and memcpy */
#include <signal.h> /* Imports kill. */
#include <unistd.h> /* Imports getpid. */

/**
/* Indefine la familia de funciones malloc.
 */
#undef malloc
#undef calloc
#undef realloc
#undef free


/**
/* Casteo de la dirección a la que apunta un puntero
/* al tipo correcto para evitar advertencias al usarlo
/* con %X en printf.
 */
#define DIR(x) (unsigned int)((long)x)

/**
/* Memoria que se reservará durante toda la ejecución
/* del programa - 300MB.
 */
#define MEM_SIZE 314572800

/**
/* Señal SIGSEGV en caso de operar sobre punteros no inicializados
/* o fuera de la lista de bloques de memoria.
 */
#define KILL kill(getpid(), SIGSEGV)

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
/* Base y tope del heap.
 */
static void * mem_start = NULL;
static void * mem_end = NULL;

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
#define LAST last
static BLOCK * last = NULL;
#endif


/**
/* Reserva la MEM_SIZE bytes para la ejecución del programa.
 */
static void set_initial_memory ();

/**
 * Imprime el contenido de la lista de bloques libres.
 */
void print_free_list ();

/**
/* Retorna un bloque libre en función del algoritmo de ajuste indicado.
 */
BLOCK * get_free_block ();

/**
/* Inicializa el primer bloque de memoria con el tamaño indicado.
 */
void set_first_block (void *, size_t);

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

/**
 * Implementación personalizada de malloc.
 */
void * malloc (size_t);

/**
 * Implementación personalizada de calloc.
 */
void * calloc (size_t, size_t);

/**
 * Implementación personalizada de realloc.
 */
void * realloc (void *, size_t);

/**
 * Implementación personalizada de free.
 */
void free (void *);


#endif
