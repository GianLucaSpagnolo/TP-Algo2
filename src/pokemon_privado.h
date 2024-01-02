#ifndef POKEMON_PRIVADO_H_
#define POKEMON_PRIVADO_H_
#include <stdlib.h>

// Este archivo es privado de la implementación, el usuario no lo conoce. Lo
// declaramos por separado para que las pruebas puedan hacer uso de la
// estructura (lo cual no siempre es una buena idea, pero para simplificar en
// este caso lo hacemos)

#define MAX_NOMBRE 30

struct _pkm_t {
	size_t id;
	size_t salud;
	char nombre_entrenador[MAX_NOMBRE];
	char nombre[MAX_NOMBRE];
};

#endif // POKEMON_PRIVADO_H_
