#include <stdio.h>

#include "custom_malloc.h"

int main(void) {
	int * memory;
	int i;
	
	printf("Jojójo\n");
	
	memory = (int *)cmalloc(100 * sizeof(int));
	printf("Memory allocated at 0x%X\n", DIR(memory));
	
	if (memory != NULL) {
		for (i = 0; i < 100; i++) {
			printf("memory[% 3d] = %d\n", i, memory[i]);
		}
	}
	
	print_free_list();
	cfree(memory);
	print_free_list();
	
	return EXIT_SUCCESS;
}
