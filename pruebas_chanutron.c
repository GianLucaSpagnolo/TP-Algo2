#include <string.h>
#include "pa2m.h"
#include "src/tp1.h"
#include "src/pokemon.h"
#include "src/pokemon_privado.h"

typedef struct aux {
	pokemon_t *pokemones[10];
	size_t tope;
} aux_t;

pokemon_t crear_pokemon(size_t id, size_t salud, const char *nombre_entrenador,
			char *nombre)
{
	pokemon_t p;
	p.id = id;
	p.salud = salud;
	strcpy(p.nombre, nombre);
	strcpy(p.nombre_entrenador, nombre_entrenador);
	return p;
}

pokemon_t *crear_pokemon_p(size_t id, size_t salud,
			   const char *nombre_entrenador, char *nombre)
{
	pokemon_t p = crear_pokemon(id, salud, nombre_entrenador, nombre);
	return pokemon_copiar(&p);
}

// ~ POKEMON ~
void pruebas_de_pokemon()
{
	pa2m_afirmar(pokemon_crear_desde_string("") == NULL,
		     "No se puede crear un pokemon con un string vacío");

	pa2m_afirmar(pokemon_crear_desde_string(NULL) == NULL,
		     "No se puede crear un pokemon con un string NULL");

	pa2m_afirmar(
		pokemon_crear_desde_string("1,2,3") == NULL,
		"No se puede crear un pokemon con un string de formato inválido");

	pokemon_t *p = pokemon_crear_desde_string("5,Pikachu,22,Lucas");
	pa2m_afirmar(
		p != NULL,
		"Se puede crear un pokemon con el string '5,Pikachu,22,Lucas'");

	pa2m_afirmar(pokemon_id(p) == 5, "El id del pokemon es 5");
	pa2m_afirmar(pokemon_salud(p) == 22, "La salud del pokemon es 22");
	pa2m_afirmar(strcmp(pokemon_entrenador(p), "Lucas") == 0,
		     "El nombre del entrenador es Lucas");
	pa2m_afirmar(strcmp(pokemon_nombre(p), "Pikachu") == 0,
		     "El nombre del pokemon creado es Pikachu");

	pokemon_t *p2 = pokemon_copiar(p);
	pa2m_afirmar(p2 != NULL, "Puedo copiar un pokemon");
	pa2m_afirmar(pokemon_son_iguales(p, p2),
		     "El pokemon copiado es igual al original");
	pa2m_afirmar(p != p2,
		     "La dirección de memoria de ambos pokemon es diferente");

	pokemon_t p3 = crear_pokemon(1, 10, "Lucas", "Pikachu");
	pa2m_afirmar(
		pokemon_son_iguales(p, &p3) == false,
		"Si comparo el pokemon con otro diferente, la funcion de comparación da false");

	pokemon_destruir(p);
	pokemon_destruir(p2);
}

// ~ CREACIÓN Y DESTRUCCIÓN ~
void pruebas_creacion_y_destruccion()
{
	pa2m_afirmar(
		hospital_crear_desde_archivo(NULL) == NULL,
		"No se puede crear un hospital con un archivo de nombre null");

	pa2m_afirmar(
		hospital_crear_desde_archivo("//////") == NULL,
		"No se puede crear un hospital con un archivo inexistente");

	pa2m_afirmar(hospital_crear_desde_archivo("ejemplos/invalido") == NULL,
		     "No se puede crear un hospital con un archivo invalido");

	pokemon_t pokemones[] = { crear_pokemon(1, 20, "Lucas", "Pikachu"),
				  crear_pokemon(2, 35, "Abril", "Lapras"),
				  crear_pokemon(3, 2, "Nico", "Jynx") };

	hospital_t *hospital =
		hospital_crear_desde_archivo("ejemplos/hospital1.txt");

	pa2m_afirmar(
		hospital != NULL,
		"Se crea un hospital correctamente a partir del archivo hospital1");

	pa2m_afirmar(hospital_cantidad_pokemones(hospital) == 3,
		     "La cantidad de pokemones es 3");

	pa2m_afirmar(pokemon_son_iguales(hospital_obtener_pokemon(hospital, 0),
					 pokemones + 2),
		     "El primer pokemon es Jynx");

	pa2m_afirmar(pokemon_son_iguales(hospital_obtener_pokemon(hospital, 1),
					 pokemones + 0),
		     "El segundo pokemon es Pikachu");

	pa2m_afirmar(pokemon_son_iguales(hospital_obtener_pokemon(hospital, 2),
					 pokemones + 1),
		     "El tercer pokemon es Lapras");

	hospital_destruir(hospital);
}

// ~ REVISAR POKEMONES ~
// Iterador Interno

bool revisar_totalmente(pokemon_t *poke, void *aux)
{
	return true;
}

bool buscar_id(pokemon_t *poke, void *id)
{
	if (poke->id == *(size_t *)id)
		return false;
	return true;
}

void iterador_parametros_nulos_resulta_en_error()
{
	hospital_t *hospital =
		hospital_crear_desde_archivo("ejemplos/hospital1.txt");

	int aux = 8;

	pa2m_afirmar(hospital_a_cada_pokemon(NULL, revisar_totalmente, &aux) ==
			     0,
		     "No es posible revisar un hospital nulo");
	pa2m_afirmar(hospital_a_cada_pokemon(hospital, NULL, &aux) == 0,
		     "No es posible revisar un hospital con una función nula");

	pa2m_afirmar(
		hospital_a_cada_pokemon(hospital, revisar_totalmente, NULL) ==
			3,
		"Es posible revisar un hospital valido con una funcion valida y un aux nulo");

	hospital_destruir(hospital);
}

void se_puede_revisar_todo_el_hospital()
{
	hospital_t *hospital =
		hospital_crear_desde_archivo("ejemplos/hospital1.txt");

	pa2m_afirmar(hospital_a_cada_pokemon(hospital, revisar_totalmente,
					     NULL) == 3,
		     "Es posible revisar todos los pokemones del hospital");

	hospital_destruir(hospital);
}

void se_puede_revisar_parte_del_hospital()
{
	//parámetros del archivo hospital1
	size_t id_pikachu = 1;
	size_t id_lapras = 2;
	size_t id_jynx = 3;
	size_t inexistente_id = 18;

	hospital_t *hospital =
		hospital_crear_desde_archivo("ejemplos/hospital1.txt");

	pa2m_afirmar(
		hospital_a_cada_pokemon(hospital, buscar_id, &id_pikachu) == 2,
		"Es posible buscar el primer poke con el iterador interno");
	pa2m_afirmar(
		hospital_a_cada_pokemon(hospital, buscar_id, &id_lapras) == 3,
		"Es posible buscar un poke del medio con el iterador interno");
	pa2m_afirmar(
		hospital_a_cada_pokemon(hospital, buscar_id, &id_jynx) == 1,
		"Es posible buscar el ultimo poke con el iterador interno");
	pa2m_afirmar(
		hospital_a_cada_pokemon(hospital, buscar_id, &inexistente_id) ==
			3,
		"Es posible buscar un poke que no esta con el iterador interno");

	hospital_destruir(hospital);
}

void pruebas_iterador_interno()
{
	pa2m_nuevo_grupo("Parámetros nulos");
	iterador_parametros_nulos_resulta_en_error();

	pa2m_nuevo_grupo("Iterar el hospital es posible");
	se_puede_revisar_todo_el_hospital();
	se_puede_revisar_parte_del_hospital();
}

// ~ EMERGENCIAS ~

void ambulancia_parametros_nulos_resulta_en_error()
{
	pokemon_t *pokes_ambulancia[3];

	hospital_t *hospital =
		hospital_crear_desde_archivo("ejemplos/hospital1.txt");

	pa2m_afirmar(
		hospital_aceptar_emergencias(NULL, pokes_ambulancia, 2) ==
			ERROR,
		"No se puede ingresar pokes por ambulancia a un hospital nulo");
	pa2m_afirmar(
		hospital_aceptar_emergencias(hospital, NULL, 0) == ERROR,
		"No se puede ingresar pokes nulos por ambulancia a un hospital válido");

	hospital_destruir(hospital);
}

bool pokemones_a_vector(pokemon_t *poke, void *aux_)
{
	aux_t *aux = aux_;
	aux->pokemones[aux->tope] = poke;
	aux->tope++;
	return true;
}

void hospital_puede_recibir_ambulancia_vacia()
{
	pokemon_t *pokes_ambulancia[3];

	pokemon_t pokes_hospital[] = { crear_pokemon(1, 20, "Lucas", "Pikachu"),
				       crear_pokemon(2, 35, "Abril", "Lapras"),
				       crear_pokemon(3, 2, "Nico", "Jynx") };

	hospital_t *hospital =
		hospital_crear_desde_archivo("ejemplos/hospital1.txt");

	pa2m_afirmar(hospital_aceptar_emergencias(hospital, pokes_ambulancia,
						  0) == EXITO,
		     "El hospital acepto la ambulancia vacia");

	pa2m_afirmar(hospital_cantidad_pokemones(hospital) == 3,
		     "La cantidad de pokemones es la original");

	aux_t aux;
	aux.tope = 0;
	pa2m_afirmar(
		hospital_a_cada_pokemon(hospital, pokemones_a_vector, &aux) ==
			3,
		"Se iteran todos los pokemones correctamente y se guardan en un vector");

	pa2m_afirmar(
		pokemon_son_iguales(aux.pokemones[0], pokes_hospital + 2),
		"El primer pokemon en el vector es el mismo que se encuentra al principio en el hospital");

	pa2m_afirmar(
		pokemon_son_iguales(aux.pokemones[1], pokes_hospital + 0),
		"El segundo y ultimo pokemon en el vector es el mismo que se encuentra al principio en el hospital");

	pa2m_afirmar(
		pokemon_son_iguales(aux.pokemones[2], pokes_hospital + 1),
		"El segundo y ultimo pokemon en el vector es el mismo que se encuentra al principio en el hospital");

	hospital_destruir(hospital);
}

void hospital_puede_recibir_pokes_por_ambulancia()
{
	size_t cant_pokes_inicial = 3;

	pokemon_t pokes_hospital[] = { crear_pokemon(1, 20, "Lucas", "Pikachu"),
				       crear_pokemon(2, 35, "Abril", "Lapras"),
				       crear_pokemon(3, 2, "Nico", "Jynx") };

	pokemon_t *pokes_ambulancia[] = {
		crear_pokemon_p(2, 1, "Luis", "Mew"),
		crear_pokemon_p(12, 8, "Maria", "Mewtwo"),
		crear_pokemon_p(1212, 13, "Tamara", "Raichu"),
		crear_pokemon_p(172, 70, "Tamara", "Suicune")

	};

	size_t cant_pokes_ambulancia = 4;

	hospital_t *hospital =
		hospital_crear_desde_archivo("ejemplos/hospital1.txt");

	pa2m_afirmar(
		hospital_aceptar_emergencias(hospital, pokes_ambulancia,
					     cant_pokes_ambulancia) == EXITO,
		"Los pokes de la primer ambulancia se ingresan al hospital correctamente");

	pa2m_afirmar(hospital_cantidad_pokemones(hospital) ==
			     cant_pokes_ambulancia + cant_pokes_inicial,
		     "La cantidad de pokemones se actualizó correctamente");

	aux_t aux;
	aux.tope = 0;

	pa2m_afirmar(
		hospital_a_cada_pokemon(hospital, pokemones_a_vector, &aux) ==
			cant_pokes_ambulancia + cant_pokes_inicial,
		"Se iteran todos los pokemones correctamente y se guardan en un vector");

	pa2m_afirmar(pokemon_son_iguales(aux.pokemones[0], pokes_ambulancia[0]),
		     "El primer pokemon en el vector es el que deberia ser");

	pa2m_afirmar(pokemon_son_iguales(aux.pokemones[1], pokes_hospital + 2),
		     "El segundo pokemon en el vector es el que deberia ser");

	pa2m_afirmar(pokemon_son_iguales(aux.pokemones[2], pokes_ambulancia[1]),
		     "El tercer pokemon en el vector es el que deberia ser");

	pa2m_afirmar(pokemon_son_iguales(aux.pokemones[3], pokes_ambulancia[2]),
		     "El cuarto pokemon en el vector es el que deberia ser");

	pa2m_afirmar(pokemon_son_iguales(aux.pokemones[4], pokes_hospital + 0),
		     "El quinto pokemon en el vector es el que deberia ser");

	pa2m_afirmar(
		pokemon_son_iguales(aux.pokemones[5], pokes_hospital + 1),
		"El sexto y ultimo pokemon en el vector es el que deberia ser");

	pa2m_afirmar(
		pokemon_son_iguales(aux.pokemones[6], pokes_ambulancia[3]),
		"El septimo y ultimo pokemon en el vector es el que deberia ser");

	hospital_destruir(hospital);
}

void pruebas_emergencias()
{
	pa2m_nuevo_grupo("Parámetros nulos");
	ambulancia_parametros_nulos_resulta_en_error();

	pa2m_nuevo_grupo("Ambulancia vacia");
	hospital_puede_recibir_ambulancia_vacia();

	pa2m_nuevo_grupo("Agregando al hospital pokes de la ambulancia");
	hospital_puede_recibir_pokes_por_ambulancia();
}

int main()
{
	pa2m_nuevo_grupo("------------ PRUEBAS DEL TP1 ------------");

	pa2m_nuevo_grupo("PRUEBAS DE POKEMON");
	pruebas_de_pokemon();

	pa2m_nuevo_grupo("PRUEBAS DE CREACIÓN Y DESTRUCCIÓN");
	pruebas_creacion_y_destruccion();

	pa2m_nuevo_grupo("PRUEBAS DE ITERADOR INTERNO");
	pruebas_iterador_interno();

	pa2m_nuevo_grupo("PRUEBAS DE EMERGENCIAS");
	pruebas_emergencias();

	return pa2m_mostrar_reporte();
}
