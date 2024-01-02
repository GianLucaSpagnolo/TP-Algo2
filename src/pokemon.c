#include "pokemon.h"
#include <string.h>
#include <stdio.h>
#include "pokemon_privado.h"

/**
 * Dada una línea de texto en formato CSV de la forma
 *
 * <ID>,<NOMBRE>,<SALUD>,<NOMBRE ENTRENADOR>
 *
 * Crea un pokemon con esos datos y lo devuelve.
 *
 * En caso de que el formato sea incorrecto, devuelve NULL.
 */
pokemon_t *pokemon_crear_desde_string(const char *string)
{
	if (!string)
		return NULL;
	pokemon_t *pokemon_creado = calloc(1, sizeof(pokemon_t));
	if (!pokemon_creado)
		return NULL;

	if (sscanf(string, "%zu,%[^,],%zu,%[^,]", &pokemon_creado->id,
		   pokemon_creado->nombre, &pokemon_creado->salud,
		   pokemon_creado->nombre_entrenador) == 4)
		return pokemon_creado;
	free(pokemon_creado);
	return NULL;
}

/**
 * Crea una copia del pokemon (reserva memoria para el mismo).
 *
 * Devuelve el pokemon creado o NULL en caso de error.
 */
pokemon_t *pokemon_copiar(pokemon_t *poke)
{
	if (!poke)
		return NULL;
	pokemon_t *pokemon_copiado = calloc(1, sizeof(pokemon_t));
	if (!pokemon_copiado)
		return NULL;

	pokemon_copiado->id = poke->id;
	pokemon_copiado->salud = poke->salud;
	strcpy(pokemon_copiado->nombre, poke->nombre);
	strcpy(pokemon_copiado->nombre_entrenador, poke->nombre_entrenador);
	return pokemon_copiado;
}

/**
 * Dados dos pokemones, la funcion devuelve true si son iguales, es decir, todos sus atributos son identicos
 * o false en caso contrario.
 */
bool pokemon_son_iguales(pokemon_t *pokemon1, pokemon_t *pokemon2)
{
	if (!pokemon1 || !pokemon2)
		return false;
	return (pokemon1->id == pokemon2->id &&
		pokemon1->salud == pokemon2->salud &&
		strcmp(pokemon1->nombre, pokemon2->nombre) == 0 &&
		strcmp(pokemon1->nombre_entrenador,
		       pokemon2->nombre_entrenador) == 0);
}

/**
 * Devuelve el nombre del pokemon o NULL en caso de no existir el pokemon.
 */
char *pokemon_nombre(pokemon_t *pokemon)
{
	return (!pokemon) ? NULL : pokemon->nombre;
}

/**
 * Devuelve el nombre del entrenador asociado al pokemon o NULL en caso de no existir el pokemon.
 */
char *pokemon_entrenador(pokemon_t *pokemon)
{
	return (!pokemon) ? NULL : pokemon->nombre_entrenador;
}

/**
 * Devuelve la salud del pokemon o 0 en caso de error.
 */
size_t pokemon_salud(pokemon_t *pokemon)
{
	return (!pokemon) ? 0 : pokemon->salud;
}

/**
 * Devuelve el id de un pokemon o 0 en caso de error
 */
size_t pokemon_id(pokemon_t *pokemon)
{
	return (!pokemon) ? 0 : pokemon->id;
}

/**
 * Libera la memoria asociada al pokemon.
 */
void pokemon_destruir(pokemon_t *pkm)
{
	if (pkm != NULL)
		free(pkm);
}