#include <stdio.h>

#include "custom_malloc.h"

int main(void) {
  int i, * array1, *array2, *array3, *array4;
  
  array1 = (int *) ccalloc(50, sizeof(int));
  for(i = 0; i < 50; i++) array1[i] = 1;
  
  array2 = (int *) cmalloc(30 * sizeof(int));
  for(i = 0; i < 30; i++) array2[i] = 2;
  
  array3 = (int *) cmalloc(20 * sizeof(int));
  for(i = 0; i < 20; i++) array3[i] = 3;
  
  array4 = (int *) cmalloc(25 * sizeof(int));
  for(i = 0; i < 25; i++) array4[i] = 4;
  
  
  printf("\n");
  printf("array1:"); for(i = 0; i < 50; i++) printf(" %d", array1[i]); printf("\n");
  printf("array2:"); for(i = 0; i < 30; i++) printf(" %d", array2[i]); printf("\n");
  printf("array3:"); for(i = 0; i < 20; i++) printf(" %d", array3[i]); printf("\n");
  printf("array4:"); for(i = 0; i < 25; i++) printf(" %d", array4[i]); printf("\n");
  
  printf("\n");
  print_free_list();
  
  
  /////////////////////////////DEAD ZONE////////////////////////////////////////////
  cfree(array1);
  cfree(array3);
  
  printf("\n");
  print_free_list();
  
  
  
  cfree(array2);
  
  printf("\n");
  print_free_list();
  
  printf("\n");
  //cfree(array1);
  //cfree(array2);
  //cfree(array3);
  cfree(array4);
  
  return EXIT_SUCCESS;
}
