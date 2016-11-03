##
# Solo debe estar descomentada UNA de las siguientes lineas FIT al compilar.
##
#FIT = -DFIRST_FIT
#FIT = -DBEST_FIT 
#FIT = -DWORST_FIT
FIT = -DNEXT_FIT

CFLAGS = $(FIT) -w

.PHONY: all
all: proy2

proy2: main.o
	$(CC) -o $@ $<

main.o: main.c custom_malloc.h

.PHONY: clean
clean:
	$(RM) *.o proy2
