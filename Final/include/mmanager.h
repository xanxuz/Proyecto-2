#ifndef MMANAGER_H
#define MMANAGER_H

#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <list.h>
#include <fit.h>


/**
/* Casteo de la dirección a la que apunta un puntero
/* al tipo correcto para evitar advertencias al usarlo
/* con %X en printf
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
/* Base y tope del heap.
 */
static void * mem_start = NULL;
static void * mem_end = NULL;


/**
/* Reserva la MEM_SIZE bytes para la ejecución del programa.
 */
static void set_initial_memory ();

/**
 * Imprime el contenido de la lista de bloques libres.
 */
void print_free_list ();

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
