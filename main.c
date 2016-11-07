#include <stdio.h>

#include "custom_malloc.h"

void memcheck(void *ptr, const char* name) {
	printf("%s: 0x%X", name, DIR(ptr));
	!((BLOCK *)ptr - 1)->free ? printf("\n") : printf(" (*)\n");
	printf("\t%s->next: 0x%X\n", name, DIR(((BLOCK *)ptr - 1)->next + HEAD_SIZE));
	printf("\tnext begin: 0x%X\n", DIR(((BLOCK *)ptr - 1)->next));
}

int main(void) {
	int * array1, * array2, * array3, * array4;
	cfree(NULL);
	printf("\n");
	print_free_list();
	printf("\n\n");
	
	array1 = (int *) cmalloc(50 * sizeof(int));
	array2 = (int *) ccalloc(20, sizeof(int));
	array3 = (int *) ccalloc(30, sizeof(int));
	array4 = (int *) cmalloc(10 * sizeof(int));
	
	printf("\n");
	memcheck(array1, "array1");
	memcheck(array2, "array2");
	memcheck(array3, "array4");
	memcheck(array3, "array4");
	
	printf("\n");
	print_free_list();
	printf("\n\n");
	
	
	cfree(array2);
	
	printf("\n");
	memcheck(array1, "array1");
	memcheck(array2, "array2");
	memcheck(array3, "array4");
	memcheck(array3, "array4");
	
	printf("\n");
	print_free_list();
	printf("\n\n");
	
	
	array3 = (int *) crealloc(array3, 40 * sizeof(int));
	
	printf("\n");
	memcheck(array1, "array1");
	memcheck(array2, "array2");
	memcheck(array3, "array4");
	memcheck(array3, "array4");
	
	printf("\n");
	print_free_list();
	printf("\n\n");
	
	
	cfree(array1);
	printf("\n");
	print_free_list();
	printf("\n\n");
	
	//cfree(array2);
	//printf("\n");
	//print_free_list();
	//printf("\n\n");
	
	cfree(array3);
	printf("\n");
	print_free_list();
	printf("\n\n");
	
	cfree(array4);
	printf("\n");
	print_free_list();
	
	
	
	return EXIT_SUCCESS;
}
