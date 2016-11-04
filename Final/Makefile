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

# Banderas
AR=ar src
CFLAGS=-O3 $(DFIT)
INCLUDE=-L$(LIB) mmanager.a

# Reglas y objetivos
FIT=$(OBJ)fit.o
LIST=$(OBJ)list.o
MMANAGER=$(OBJ)mmanager.o
MLIB=$(LIB)mmanager.a
MAIN=main

# Objetivo del Makefile
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

# Crea el ejecutable
$(BIN)$(MAIN): $(SRC)$(MAIN) $(MLIB) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $<

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
	$(CC) $(CFLAGS) -c -o $@ $<

# Compila las estructuras y funciones sobre bloques de memoria
$(LIST): $(SRC)list.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

# Compila las funciones que gestionan la memoria
$(MMANAGER): $(SRC)mmanager.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<


#######################################################################################
#                                                                                     #
#                                 Otras opciones                                      #
#                                                                                     #
#######################################################################################

.PHONY: clean
clean:
	$(RM) $(OBJ)*.o $(BIN)$(MAIN) $(MLIB)

