##
# Solo debe estar descomentada UNA de las siguientes lineas CFLAGS al compilar.
##
CFLAGS = -DFIRST_FIT
#CFLAGS = -DBEST_FIT
#CFLAGS = -DWORST_FIT
#CFLAGS = -DNEXT_FIT

.PHONY: all
all: proy2

proy2: main.o
	$(CC) -o $@ $<

main.o: main.c custom_malloc.h

.PHONY: clean
clean:
	$(RM) *.o proy2
