#######################################################################################
#                                                                                     #
#                             Proyecto 2 de Sistemas Operativos                       #
#                                                                                     #
#######################################################################################

# Banderas
#FIT = -DFIRST_FIT
FIT = -DNEXT_FIT
#FIT = -DBEST_FIT 
#FIT = -DWORST_FIT
CFLAGS = -O3 $(FIT)

# Objetivo del Makefile
.PHONY: all
all: proy2


#######################################################################################
#                                                                                     #
#                               Proceso de compilación                                #
#                                                                                     #
#######################################################################################
proy2: main.o custom_malloc.o custom_malloc.h
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c
custom_malloc.o: custom_malloc.c


#######################################################################################
#                                                                                     #
#                                 Otras opciones                                      #
#                                                                                     #
#######################################################################################

.PHONY: clean
clean:
	$(RM) *.o proy2
