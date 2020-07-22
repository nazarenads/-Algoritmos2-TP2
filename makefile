EXEC = tp2
CFILES = zyxcba.c heap.c abb.c lista.c hash.c cola.c pila.c strutil.c paciente.c doctor.c csv.c turno.c
HFILES = heap.h abb.h lista.h hash.h cola.h pila.h strutil.h paciente.h doctor.h csv.h turno.h

CC = gcc
CFLAGS = -g -O2 -std=c99 -Wall -Werror -pedantic -Wformat=2 -Wshadow -Wpointer-arith -Wunreachable-code -Wconversion -Wno-sign-conversion -Wbad-function-cast
VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 -v

all: $(EXEC)

run: all
	./$(EXEC)

clean:
	rm -f $(EXEC)

valgrind: all
	valgrind $(VFLAGS) ./$(EXEC)

$(EXEC): $(OFILES) $(CFILES) $(HFILES)
	$(CC) $(CFLAGS) $(OFILES) $(CFILES) -o $(EXEC)