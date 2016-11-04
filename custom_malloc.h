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

static void * mem_start = NULL;
static void * mem_end = NULL;

static void set_initial_memory() {
  if(mem_start == NULL && mem_end == NULL) {
    mem_start = sbrk(0); /* Esto retorna la dirección actual del borde del segmento de datos. */
    sbrk(MEM_SIZE);     /* Esto mueve el borde del segmento de datos en MEM_SIZE bytes. */
    mem_end = sbrk(0);
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

  return NULL;
}

/**
 * Custom calloc reimplementation.
 */
void * calloc(size_t nmemb, size_t size) {
  set_initial_memory(); /* NO QUITAR. */
  printf("Calling calloc with nmemb = %lld and size = %lld\n", nmemb, size);
  
  return NULL;
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
}

#endif
