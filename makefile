VALGRIND_FLAGS=--leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes
VALGRIND_FLAGS_TP2=--leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all
CFLAGS =-std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0 -g
CC = gcc

all: clean valgrind-chanutron tp2

valgrind-chanutron: pruebas_chanutron
	valgrind $(VALGRIND_FLAGS) ./pruebas_chanutron

pruebas_chanutron: src/*.c pruebas_chanutron.c
	$(CC) $(CFLAGS) src/*.c pruebas_chanutron.c -o pruebas_chanutron

valgrind-alumno: pruebas_alumno
	valgrind $(VALGRIND_FLAGS) ./pruebas_alumno

pruebas_alumno: src/*.c pruebas_alumno.c
	$(CC) $(CFLAGS) src/*.c pruebas_alumno.c -o pruebas_alumno

valgrind-hospital: pruebas_hospital
	valgrind $(VALGRIND_FLAGS_TP2) ./pruebas_hospital

pruebas_hospital: src/*.c tp2.c
	$(CC) $(CFLAGS) src/*.c tp2.c -o pruebas_hospital


tp2: src/*.c tp2.c
	$(CC) $(CFLAGS) src/*.c tp2.c -o tp2

clean:
	rm -f pruebas_alumno pruebas_chanutron tp2
