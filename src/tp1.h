//Este archivo NO SE PUEDE MODIFICAR

#ifndef TP1_H_
#define TP1_H_

#include <stdlib.h>

#include "pokemon.h"

#define ERROR -1
#define EXITO 0

typedef struct _hospital_pkm_t hospital_t;

/**
 * Lee un archivo con pokemones y crea un hospital con esos pokemones.
 *
 * Si alguna de las líneas del archivo se encuentra mal formateada, debe ser considerado un error.
 *
 * Si el archivo no contiene al menos un pokemon, se debe devolver NULL
 *
 * Devuelve NULL en caso de no poder crearlo.
 */
hospital_t *hospital_crear_desde_archivo(const char *nombre_archivo);

/**
 * Devuelve la cantidad de pokemon que son atendidos actualmente en el hospital.
 */
size_t hospital_cantidad_pokemones(hospital_t *hospital);

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
			       void *aux);

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
				 size_t cant_pokes_ambulancia);

/**
 * Devuelve el pokemon con la prioridad indicada (siendo 0 la mas alta prioridad, el pokemon con menos salúd).
 *
 * Si no existe la prioridad indicada devuelve NULL
 */
pokemon_t *hospital_obtener_pokemon(hospital_t *hospital, size_t prioridad);

/**
 *  Libera el hospital y toda la memoria utilizada por el mismo.
 */
void hospital_destruir(hospital_t *hospital);

#endif // TP1_H_
