#include "tp1.h"

#include "pokemon.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _hospital_pkm_t {
	pokemon_t **pokemones;
	size_t cantidad_pokemon;
	size_t cantidad_entrenadores;
};

/**
 * Reserva memoria para inicializar correctamente el hospital y el vector de pokemones que
 * incluye. 
 * 
 * Devuelve un puntero al hospital creado o NULL en caso de error.
*/
hospital_t *hospital_crear()
{
	hospital_t *hospital_creado = calloc(1, sizeof(hospital_t));
	if (!hospital_creado)
		return NULL;

	hospital_creado->pokemones = malloc(sizeof(pokemon_t *));
	if (!hospital_creado->pokemones) {
		free(hospital_creado);
		return NULL;
	}
	return hospital_creado;
}

/**
 * Agranda el tamaño de la memoria asignada para el vector de pokemones del hospital, en base a
 * la nueva capacidad recibida por parametro.
 * 
 * Devuelve un puntero al nuevo vector o NULL en caso de error.
*/
pokemon_t **agrandar_vector_pokemon(pokemon_t **vector, size_t capacidad)
{
	pokemon_t **nuevo_vector =
		realloc(vector, sizeof(pokemon_t *) * capacidad);
	return nuevo_vector;
}

/**
 * Lee un archivo con pokemones y crea un hospital con esos pokemones.
 *
 * Si alguna de las líneas del archivo se encuentra mal formateada, debe ser considerado un error.
 *
 * Si el archivo no contiene al menos un pokemon, se debe devolver NULL
 *
 * Devuelve NULL en caso de no poder crearlo.
 */
hospital_t *hospital_crear_desde_archivo(const char *nombre_archivo)
{
	if (!nombre_archivo)
		return NULL;
	FILE *archivo = fopen(nombre_archivo, "r");
	if (!archivo)
		return NULL;

	hospital_t *hospital = hospital_crear();
	if (!hospital) {
		fclose(archivo);
		return NULL;
	}

	char linea[30] = { 0 };
	while (fscanf(archivo, "%[^\n]\n", linea) == 1) {
		pokemon_t *pokemon_leido = pokemon_crear_desde_string(linea);
		hospital->pokemones = agrandar_vector_pokemon(
			hospital->pokemones, hospital->cantidad_pokemon + 1);
		if (!pokemon_leido || !hospital->pokemones) {
			if (pokemon_leido)
				pokemon_destruir(pokemon_leido);
			hospital_destruir(hospital);
			fclose(archivo);
			return NULL;
		}
		hospital->cantidad_pokemon++;
		hospital->pokemones[hospital->cantidad_pokemon - 1] =
			pokemon_leido;
	}

	fclose(archivo);
	if (!hospital->cantidad_pokemon) {
		hospital_destruir(hospital);
		return NULL;
	}
	return hospital;
}

/**
 * Devuelve la cantidad de pokemon que son atendidos actualmente en el hospital.
 */
size_t hospital_cantidad_pokemones(hospital_t *hospital)
{
	return (!hospital) ? 0 : hospital->cantidad_pokemon;
}

/**
 * Funcion utilizada en hospital_a_cada_pokemon() que permite ordenar los pokemon dentro del hospital 
 * de menor a mayor salud. Se utiliza el ordenamiento por INSERCIÓN.
*/
void ordenar_pokemones_por_salud(hospital_t *hospital)
{
	for (size_t i = 1; i < hospital->cantidad_pokemon; i++) {
		pokemon_t *pokemon_aux = hospital->pokemones[i];
		size_t j = 1;
		bool anterior_mayor =
			pokemon_salud(hospital->pokemones[i - 1]) >
			pokemon_salud(hospital->pokemones[i]);
		while (anterior_mayor == true && j <= i) {
			hospital->pokemones[i - j + 1] =
				hospital->pokemones[i - j];
			if (j == i ||
			    pokemon_salud(hospital->pokemones[i - j - 1]) <
				    pokemon_salud(pokemon_aux)) {
				hospital->pokemones[i - j] = pokemon_aux;
				break;
			}
			j++;
		}
	}
}

/**
 * Aplica una función a cada uno de los pokemon almacenados en el hospital. La
 * función debe aplicarse a cada pokemon en orden de prioridad (los de menor salud primero).
 *
 * La función a aplicar recibe el pokemon y la variable auxiliar que se le paso a hospital_a_cada_pokemon
 * y devuelve true o false. Si la función devuelve true, se debe seguir aplicando la función a los próximos 
 * pokemon si quedan. Si la función devuelve false, no se debe continuar.
 *
 * Devuelve la cantidad de veces que se invocó la función (haya devuelto true o false).
 */
size_t hospital_a_cada_pokemon(hospital_t *hospital,
			       bool (*funcion)(pokemon_t *p, void *aux),
			       void *aux)
{
	size_t iteracion = 0;
	if (!hospital || !funcion)
		return iteracion;
	ordenar_pokemones_por_salud(hospital);
	for (size_t i = 0; i < hospital->cantidad_pokemon; i++) {
		iteracion++;
		if (!funcion(hospital->pokemones[i], aux))
			break;
	}
	return iteracion;
}

/**
 *  Cuando ocurre una emergencia, llegan nuevos pokemones en ambulancia, que
 *  deben ser ingresados al hospital.
 *
 *  No es necesario copiar los pokemon, tenes que quedarte con el puntero.
 *  Recorda que a partir del momento que te guardas el puntero pasas a ser
 *  responsable de la liberación de los pokemon.
 *
 *  Devuelve -1 en caso de error o 0 en caso de éxito
 */
int hospital_aceptar_emergencias(hospital_t *hospital,
				 pokemon_t **pokemones_ambulancia,
				 size_t cant_pokes_ambulancia)
{
	if (!hospital || !pokemones_ambulancia)
		return ERROR;
	pokemon_t **nuevo_vector = agrandar_vector_pokemon(
		hospital->pokemones,
		hospital->cantidad_pokemon + cant_pokes_ambulancia);
	if (!nuevo_vector)
		return ERROR;
	hospital->pokemones = nuevo_vector;
	for (size_t i = 0; i < cant_pokes_ambulancia; i++)
		hospital->pokemones[hospital->cantidad_pokemon + i] =
			pokemones_ambulancia[i];
	hospital->cantidad_pokemon += cant_pokes_ambulancia;
	return EXITO;
}

/**
 * Devuelve el pokemon con la prioridad indicada (siendo 0 la mas alta prioridad, el pokemon con menos salúd).
 *
 * Si no existe la prioridad indicada devuelve NULL
 */
pokemon_t *hospital_obtener_pokemon(hospital_t *hospital, size_t prioridad)
{
	if (!hospital || prioridad >= hospital->cantidad_pokemon)
		return NULL;
	ordenar_pokemones_por_salud(hospital);
	return hospital->pokemones[prioridad];
}

/**
 *  Libera el hospital y toda la memoria utilizada por el mismo.
 */
void hospital_destruir(hospital_t *hospital)
{
	if (!hospital)
		return;
	for (int i = 0; i < hospital->cantidad_pokemon; i++)
		pokemon_destruir(hospital->pokemones[i]);
	free(hospital->pokemones);
	free(hospital);
}