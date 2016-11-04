#######################################################################################
#                                                                                     #
#                             Proyecto 2 de Sistemas Operativos                       #
#                                                                                     #
#######################################################################################

# Directorios
LIB=./lib/
OBJ=./obj/
BIN=./bin/
SRC=./src/
INCL=./include/

# Requisitos
HEADERS=$(INCL)fit.h $(INCL)list.h $(INCL)mmanager.h
#DFIT=-DFIRST_FIT
DFIT=-DNEXT_FIT
#DFIT=-DBEST_FIT 
#DFIT=-DWORST_FIT

# Objetivos
FIT=$(OBJ)fit.o
LIST=$(OBJ)list.o
MMANAGER=$(OBJ)mmanager.o
MLIB=$(LIB)libmmanager.a
MAIN=main

# Banderas
AR=ar src
FLAGS=-O3 $(DFIT)
INCLUDE=-I$(INCL)
LINK=-L$(LIB) -lmmanager

# cc -O3 -DNEXT_FIT -L./lib/ -lmmanager -I./include/ -o ./bin/main ./src/main.c

# Objetivo del Makefile
# Crea el ejecutable
.PHONY: all
all: $(BIN)$(MAIN)

# Crear librería estática
.PHONY: lib
lib: $(MLIB)


#######################################################################################
#                                                                                     #
#                               Proceso de compilación                                #
#                                                                                     #
#######################################################################################
$(BIN)$(MAIN): $(SRC)main.c $(MLIB) $(HEADERS)
	$(CC) -O3 -o $@ $< $(INCLUDE) $(LINK)


#######################################################################################
#                                                                                     #
#                      Compilación de la librería estática                            #
#                                                                                     #
#######################################################################################

$(MLIB): $(FIT) $(LIST) $(MMANAGER)
	$(AR) $@ $^

#######################################################################################

# Compila los algoritmos de ajuste
$(FIT): $(SRC)fit.c $(HEADERS)
	$(CC) -O3 -c -o $@ $< $(DFIT)

# Compila las estructuras y funciones sobre bloques de memoria
$(LIST): $(SRC)list.c $(HEADERS)
	$(CC) -O3 -c -o $@ $< $(DFIT)

# Compila las funciones que gestionan la memoria
$(MMANAGER): $(SRC)mmanager.c $(HEADERS)
	$(CC) -O3 -c -o $@ $<


#######################################################################################
#                                                                                     #
#                                 Otras opciones                                      #
#                                                                                     #
#######################################################################################

.PHONY: clean
clean:
	$(RM) $(OBJ)*.o $(BIN)$(MAIN) $(MLIB)
