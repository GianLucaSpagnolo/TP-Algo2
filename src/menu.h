#ifndef __MENU_H__
#define __MENU_H__

#include "hash.h"
#include "lista.h"

#include <stdbool.h>
#include <stddef.h>

typedef struct opcion opcion_t;
typedef struct menu menu_t;

/**
 * Crea un menu, con un nombre enviado por parametro, e inicializa la 
 * cantidad de opciones que tiene (en cero) y el hash de las opciones 
 * que va a manejar el menu.
 * 
 * Devuelve el menu creado o NULL en caso de error.
*/
menu_t *menu_crear(char *nombre_menu);

/**
 * Agrega una opcion al menu, insertando toda la informacion de la 
 * opcion en el hash de opciones.
 * 
 * Cada opcion tiene un string con la tecla asociada a la opcion, otro 
 * string con una descripcion de lo que realiza la opcion, una funcion 
 * asociada a la opcion y un contexto (opcional) que el usuario puede 
 * enviar por parametro al ejecutar la opcion.
 * 
 * Inserta toda esta informacion como una opcion dentro del hash de 
 * opciones del menu enviado por parametro. Si ya existia una tecla 
 * asociada a una funcion en el hash de opciones, se reemplaza el 
 * contenido de la opcion en el hash de opciones, bajo la misma tecla 
 * asociada, y se libera la memoria asiganada de la opcion anterior.
 * 
 * Devuelve false en caso de que no se haya podido insertar la opcion 
 * en el hash por algun error o en caso de que alguno de los parametros 
 * obligatorios sean NULL, o devuelve true si no han habido problemas 
 * al agregar la opcion.
*/
bool menu_agregar_opcion(menu_t *menu, char *tecla, char *descripcion,
			 bool (*f)(void *, void *), void *contexto);

/**
 * Elimina una opcion del menu, recibiendo por parametro una tecla 
 * para eliminar la opcion insertada del hash de opciones, y liberando 
 * la memoria reservada por esa opcion. 
 * 
 * Devuelve true en caso de exito, o false en caso de error o si no 
 * existe la opcion a eliminar en el hash.
*/
bool menu_eliminar_opcion(menu_t *menu, char *tecla);

/**
 * Ejecuta la funcion asociada a una opcion del menu.
 * Recibe una tecla por parametro para encontrar la opcion del 
 * hash de opciones, y un dato que se envia por parametro al 
 * ejecutar la funcion asociada junto al contexto de la opcion 
 * determinada.
 * 
 * Devuelve false en caso de error, o devuelve el resultado de 
 * la ejecucion de la funcion con el dato y el contexto.
*/
bool menu_ejecutar_opcion(menu_t *menu, char *tecla, void *dato);

/**
 * Encuentra la descripcion de una opcion agregada al menu. 
 * Recibe una tecla por parametro para encontrar la opcion del 
 * hash de opciones, y devuelve la descripcion de la opcion del 
 * menu o NULL en caso de error, o en caso de que la opcion no se 
 * encuentre en el menu.
*/
char *menu_obtener_descripcion(menu_t *menu, char *tecla);

/**
 * Devuelve la cantidad de opciones que el usuario ha insertado 
 * en el menu, o 0 si el menu es NULL.
*/
size_t menu_cantidad_opciones(menu_t *menu);

/**
 * Devuelve el string del nombre asociado al menu en el momento 
 * de su creacion, o NULL en caso de error.
*/
char *menu_nombre(menu_t *menu);

/**
 * Crea y completa una lista con toda la informacion mas 
 * importante de cada opcion del menu.
 * 
 * La lista de ayuda creada va recibiendo cada opcion del 
 * menu (en el orden que el iterador interno recorre el hash 
 * de opciones) e inserta la tecla asociada a la opcion al final 
 * de la lista, e inserta el string de la descripcion de la opcion 
 * luego de la teclia insertada al final.
 * 
 * Por ejemplo, si un menu tiene las siguientes opciones:
 * Opcion1 - Opcion2 - Opcion3 
 * La lista va a contener cada tecla y descripcion en el 
 * siguiente orden:
 * Tecla1 - Descripcion1 - Tecla2 - Descripcion2 - Tecla3 - Descripcion3
 * 
 * Devuelve la lista de ayuda creada o NULL en caso de error.
*/
lista_t *menu_mostrar_ayuda(menu_t *menu);

/**
 * Funcion destructora.
 * Libera toda la memoria asignada por el menu y por todas las 
 * opciones que han sido agregadas al menu.
*/
void menu_destruir(menu_t *menu);

/**
 * Funcion destructora para strings.
 * Ademas de liberar toda la memoria asignada por el menu y por todas 
 * las opciones que han sido agregadas al menu, tambien libera la memoria 
 * que el usuario haya reservado previamente a agregar cada opcion al 
 * menu, correspondiente a los strings de la tecla asociada y de la 
 * descripcion de cada opcion.
*/
void menu_destruir_con_strings(menu_t *menu);

/**
 * Funcion destructora para lista de ayuda.
 * Ademas de liberar la memoria asignada por el menu y por todas 
 * las opciones que han sido agregadas al menu, tambien libera 
 * la memoria asignada a la lista de ayuda creada.
 * 
 * Si se desea destruir unicamente la lista de ayuda, puede 
 * realizarse enviando por parametro NULL como menu.
*/
void menu_destruir_con_lista_ayuda(menu_t *menu, lista_t *ayuda);

#endif /* __MENU_H__ */
