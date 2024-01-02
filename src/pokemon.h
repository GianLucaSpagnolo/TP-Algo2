#ifndef POKEMON_H_
#define POKEMON_H_
#include <stdlib.h>
#include <stdbool.h>

typedef struct _pkm_t pokemon_t;

/**
 * Dada una línea de texto en formato CSV de la forma
 *
 * <ID>,<NOMBRE>,<SALUD>,<NOMBRE ENTRENADOR>
 *
 * Crea un pokemon con esos datos y lo devuelve.
 *
 * En caso de que el formato sea incorrecto, devuelve NULL.
 */
pokemon_t *pokemon_crear_desde_string(const char *string);

/**
 * Crea una copia del pokemon (reserva memoria para el mismo).
 *
 * Devuelve el pokemon creado o NULL en caso de error.
 */
pokemon_t *pokemon_copiar(pokemon_t *poke);

/**
 * Dados dos pokemones, la funcion devuelve true si son iguales, es decir, todos sus atributos son identicos
 * o false en caso contrario.
 */
bool pokemon_son_iguales(pokemon_t *pokemon1, pokemon_t *pokemon2);

/**
 * Devuelve el nombre del pokemon o NULL en caso de no existir el pokemon.
 */
char *pokemon_nombre(pokemon_t *pokemon);

/**
 * Devuelve el nombre del entrenador asociado al pokemon o NULL en caso de no existir el pokemon.
 */
char *pokemon_entrenador(pokemon_t *pokemon);

/**
 * Devuelve la salud del pokemon o 0 en caso de error.
 */
size_t pokemon_salud(pokemon_t *pokemon);

/**
 * Devuelve el id de un pokemon o 0 en caso de error
 */
size_t pokemon_id(pokemon_t *pokemon);

/**
 * Libera la memoria asociada al pokemon.
 */
void pokemon_destruir(pokemon_t *pkm);

#endif // POKEMON_H_
