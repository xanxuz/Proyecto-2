#include <stdio.h>

#include "custom_malloc.h"

void memcheck (void * ptr) {
	BLOCK * block = (BLOCK *)ptr - 1;
	int cont = 1;
	
	while (block) {
		printf("%2d) 0x%X -> ", cont++, DIR(block));
		NEXT ? printf("0x%X", DIR(NEXT)) : printf("NULL\t");
		printf("\t%9lu B", SIZE);
		FREE ? printf(" (*)\n") : printf("\n");
		
		block = NEXT;
	}
}

int main(void) {
	int i, * array1, * array2, * array3, * array4;
	cfree(NULL);
	printf("\n");
	print_free_list();
	printf("\n\n");
	
	array1 = (int *) cmalloc(30 * sizeof(int));
	for (i = 0; i < 30; i++) array1[i] = 1;
	
	array2 = (int *) ccalloc(20, sizeof(int));
	for (i = 0; i < 20; i++) array2[i] = 2;
	
	array3 = (int *) ccalloc(10, sizeof(int));
	for (i = 0; i < 10; i++) array3[i] = 3;
	
	array4 = (int *) cmalloc(15 * sizeof(int));
	for (i = 0; i < 15; i++) array4[i] = 4;
	
	
	
	printf("\n");
	printf("array1"); for (i = 0; i < 30; i++) printf(" %d", array1[i]); printf("\n");
	printf("array2"); for (i = 0; i < 20; i++) printf(" %d", array2[i]); printf("\n");
	printf("array3"); for (i = 0; i < 10; i++) printf(" %d", array3[i]); printf("\n");
	printf("array4"); for (i = 0; i < 15; i++) printf(" %d", array4[i]); printf("\n");
	
	printf("\n");
	memcheck(array1);
	
	printf("\n");
	print_free_list();
	printf("\n\n");
	
	
	cfree(array2);
	
	printf("\n");
	printf("array1"); for (i = 0; i < 30; i++) printf(" %d", array1[i]); printf("\n");
	printf("array2"); for (i = 0; i < 20; i++) printf(" %d", array2[i]); printf("\n");
	printf("array3"); for (i = 0; i < 10; i++) printf(" %d", array3[i]); printf("\n");
	printf("array4"); for (i = 0; i < 15; i++) printf(" %d", array4[i]); printf("\n");
	
	printf("\n");
	memcheck(array1);
	
	printf("\n");
	print_free_list();
	printf("\n\n");
	
	
	array3 = (int *) crealloc(array3, 25 * sizeof(int));
	
	printf("\n");
	printf("array1"); for (i = 0; i < 30; i++) printf(" %d", array1[i]); printf("\n");
	printf("array2"); for (i = 0; i < 20; i++) printf(" %d", array2[i]); printf("\n");
	printf("array3"); for (i = 0; i < 25; i++) printf(" %d", array3[i]); printf("\n");
	printf("array4"); for (i = 0; i < 15; i++) printf(" %d", array4[i]); printf("\n");
	
	printf("\n");
	memcheck(array1);
	
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
