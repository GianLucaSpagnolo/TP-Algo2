#include <stdlib.h>
#include <string.h>

#include "menu.h"

#define CAPACIDAD_DEFAULT 15

/**
 * Estructura de cada opcion que contiene el hash de opciones del menu.
 * Contiene un string con la tecla asociada a la activacion de la 
 * opcion, otro string de una descripcion de lo que realiza la opcion, 
 * una funcion asociada a la opcion que se ejecuta cuando se activa 
 * la opcion, y un contexto (opcional para el usuario) que se envia 
 * por parametro al ejecutar la funcion asociada a la opcion.
*/
struct opcion {
	char *tecla;
	char *descripcion;
	bool (*ejecutar)(void *, void *);
	void *contexto;
};

/**
 * Estructura del menu
 * Tiene un nombre asociado que es asignado al crear el menu, un 
 * contador de la cantidad de opciones que tiene, y un hash que 
 * incluye todas las opciones que el usuario va agregando al menu.
*/
struct menu {
	char *nombre_menu;
	size_t cantidad;
	hash_t *opciones;
};

/**
 * Reserva memoria para crear e inicializar el menu, con el nombre 
 * enviado por parametro, incluyendo el hash de opciones con una 
 * capacidad moderada.
*/
menu_t *menu_crear(char *nombre)
{
	menu_t *menu_creado = calloc(1, sizeof(menu_t));
	if (!menu_creado)
		return NULL;

	menu_creado->opciones = hash_crear(CAPACIDAD_DEFAULT);
	if (!menu_creado->opciones) {
		free(menu_creado);
		return NULL;
	}

	menu_creado->nombre_menu = nombre;
	return menu_creado;
}

/**
 * Funcion utilizada por menu_agregar_opcion() para crear e 
 * inicializar una nueva opcion que se va a asignar en el menu, 
 * asignando cada parametro correspondiente para guardar la 
 * informacion de la opcion.
*/
opcion_t *opcion_crear(char *tecla, char *descripcion,
		       bool (*f)(void *, void *), void *contexto)
{
	opcion_t *opcion_creada = calloc(1, sizeof(opcion_t));
	if (!opcion_creada)
		return NULL;

	opcion_creada->tecla = tecla;
	opcion_creada->descripcion = descripcion;
	opcion_creada->ejecutar = f;
	opcion_creada->contexto = contexto;
	return opcion_creada;
}

/**
 * Agrega una opcion al menu.
 * Para ello crea una opcion con la informacion enviada por 
 * parametro, y la inserta en el hash de opciones.
 * 
 * Tambien asigna memoria para poder verificar si ya existia 
 * previamente una opcion en el hash con la misma tecla 
 * asignada. Si es ese el caso, reemplaza la opcion de la 
 * tecla por la nueva opcion creada, y libera la memoria 
 * de la opcion anterior. 
 * Al final de la ejecucion libera la memoria reservada para 
 * verificar si existia una opcion previamente, y si no existia 
 * una opcion con aquella tecla asignada aumenta el contador de 
 * la cantidad de opciones del menu.
*/
bool menu_agregar_opcion(menu_t *menu, char *tecla, char *descripcion,
			 bool (*f)(void *, void *), void *contexto)
{
	if (!menu || !tecla || !descripcion || !f)
		return false;
	opcion_t *opcion = opcion_crear(tecla, descripcion, f, contexto);
	if (!opcion)
		return false;

	void **opcion_anterior = calloc(1, sizeof(opcion_t *));
	hash_t *aux =
		hash_insertar(menu->opciones, tecla, opcion, opcion_anterior);
	if (!aux) {
		free(opcion_anterior);
		return false;
	}
	menu->opciones = aux;

	if (*opcion_anterior != NULL)
		free(*opcion_anterior);
	else
		menu->cantidad++;
	free(opcion_anterior);
	return true;
}

/**
 * Elimina una opcion del menu.
 * Busca en el hash de opciones del menu si existe una opcion 
 * con una tecla asociada enviada por parametro. De existir, 
 * elimina la opcion del hash, libera la memoria asignada a la 
 * opcion y resta en uno el contador de la cantidad de opciones 
 * en el menu. De no existir, devuelve false.
*/
bool menu_eliminar_opcion(menu_t *menu, char *tecla)
{
	if (!menu_cantidad_opciones(menu) || !tecla)
		return false;
	opcion_t *opcion = hash_quitar(menu->opciones, tecla);
	if (!opcion)
		return false;

	free(opcion);
	menu->cantidad--;
	return true;
}

/**
 * Ejecuta una opcion del menu.
 * Busca en el hash de opciones del menu si existe una opcion 
 * con una tecla asociada enviada por parametro. De existir, 
 * ejecuta la funcion de aquella opcion enviando como primer 
 * parametro el dato entregado por el usuario al ejecutar la 
 * opcion, mientras que como segundo parametro envia el contexto 
 * que el usuario asigno a la opcion al agregarla al menu.
*/
bool menu_ejecutar_opcion(menu_t *menu, char *tecla, void *dato)
{
	if (!menu_cantidad_opciones(menu) || !tecla)
		return false;
	opcion_t *opcion = hash_obtener(menu->opciones, tecla);
	if (!opcion)
		return false;
	return opcion->ejecutar(dato, opcion->contexto);
}

/**
 * Devuelve la descripcion de una opcion del menu.
 * Busca en el hash de opciones del menu si existe una opcion 
 * con una tecla asociada enviada por parametro. De existir, 
 * devuelve la descripcion de aquella opcion que ha sido 
 * asignada en el momento que el usuario agrego la opcion al menu.
*/
char *menu_obtener_descripcion(menu_t *menu, char *tecla)
{
	if (!menu_cantidad_opciones(menu) || !tecla)
		return NULL;
	opcion_t *opcion = hash_obtener(menu->opciones, tecla);
	if (!opcion)
		return NULL;
	return opcion->descripcion;
}

/**
 * Devuelve la cantidad de opciones que el usuario ha insertado 
 * en el menu, o 0 si el menu es NULL.
*/
size_t menu_cantidad_opciones(menu_t *menu)
{
	return (!menu) ? 0 : menu->cantidad;
}

/**
 * Devuelve el string del nombre asociado al menu en el momento 
 * de su creacion, o NULL en caso de error.
*/
char *menu_nombre(menu_t *menu)
{
	return (!menu) ? NULL : menu->nombre_menu;
}

/**
 * Funcion utilizada por menu_mostrar_ayuda() al recorrer cada 
 * opcion del hash de opciones del menu con el iterador interno. 
 * 
 * Inserta en la lista primero la tecla asociada a la opcion, 
 * y luego la descripcion de la opcion.
*/
bool clave_y_descripcion_a_lista(const char *clave, void *valor, void *lista)
{
	lista_t *ayuda = (lista_t *)lista;
	opcion_t *opcion = (opcion_t *)valor;
	void *tecla = opcion->tecla;
	lista_t *aux1 = lista_insertar(ayuda, tecla);
	if (!aux1)
		return false;

	void *descripcion = opcion->descripcion;
	lista_t *aux2 = lista_insertar(ayuda, descripcion);
	if (!aux2)
		return false;
	return true;
}

/**
 * Crea y entrega una lista con todas las teclas y descripciones 
 * asignadas a cada opcion agregada al menu.
 * 
 * Para ir agregando cada tecla y descripcion asociados se utiliza 
 * el iterador interno del hash con el hash de opciones del menu, 
 * donde recorre cada opcion del menu y va insertando la tecla y 
 * la descripcion de cada opcion.
 * 
 * A medida que se recorre cada opcion del menu, primero se 
 * inserta la tecla y luego la descripcion, de modo que en la 
 * lista en cada posicion par hay una tecla, y en cada posicion 
 * impar hay una descripcion.
 * 
 * Al finalizar, verifica si la cantidad de elementos en la lista 
 * corresponde con la cantidad de teclas y descripciones entre todas 
 * las opciones del menu, para verificar algun error al insertar 
 * cada elemento en la lista. De ser ese el caso, libera la memoria 
 * de la lista y devuelve NULL.
*/
lista_t *menu_mostrar_ayuda(menu_t *menu)
{
	if (!menu_cantidad_opciones(menu))
		return NULL;
	lista_t *ayuda = lista_crear();
	if (!ayuda)
		return NULL;

	hash_con_cada_clave(menu->opciones, clave_y_descripcion_a_lista, ayuda);
	if (lista_tamanio(ayuda) != menu->cantidad * 2) {
		lista_destruir(ayuda);
		return NULL;
	}
	return ayuda;
}

/**
 * Funcion destructora utilizada por menu_destruir() al ejecutar 
 * hash_destruir_todo() para liberar la memoria asignada a cada 
 * opcion del menu.
*/
void funcion_destructor(void *opcion)
{
	free(opcion);
}

/**
 * Funcion destructora.
 * Libera la memoria correspondiente a cada opcion que haya sido 
 * agregada al menu, y libera la memoria asignada al menu al final.
*/
void menu_destruir(menu_t *menu)
{
	if (!menu)
		return;
	hash_destruir_todo(menu->opciones, funcion_destructor);
	free(menu);
}

/**
 * Funcion destructora utilizada por menu_destruir_con_strings() al 
 * ejecutar hash_destruir_todo() para liberar la memoria asignada a 
 * cada opcion del menu; y ademas toda la memoria que el usuario haya 
 * reservado para la tecla asociada y la descripcion de cada opcion 
 * antes de que haya agregado la opcion al menu.
*/
void funcion_destructor_strings(void *dato)
{
	opcion_t *opcion = (opcion_t *)dato;
	free(opcion->tecla);
	free(opcion->descripcion);
	free(opcion);
}

/**
 * Funcion destructora que ademas de liberar toda la memoria 
 * correspondiente al menu y a sus opciones, tambien libera la 
 * memoria que el usuario haya reservado previamente de agregar 
 * cada opcion al menu, para el string correspondiente a la tecla 
 * asociada y a la descripcion de cada opcion.
*/
void menu_destruir_con_strings(menu_t *menu)
{
	if (!menu)
		return;
	hash_destruir_todo(menu->opciones, funcion_destructor_strings);
	free(menu);
}

/**
 * Funcion destructora que ademas de liberar toda la memoria 
 * correspondiente al menu y a sus opciones, tambien libera la 
 * memoria de la lista de ayuda que el usuario haya creado 
 * previamente, llamando a lista_destruir().
*/
void menu_destruir_con_lista_ayuda(menu_t *menu, lista_t *ayuda)
{
	if (ayuda != NULL)
		lista_destruir(ayuda);
	menu_destruir(menu);
}