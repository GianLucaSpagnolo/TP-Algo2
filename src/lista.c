#include "lista.h"
#include <stddef.h>
#include <stdlib.h>

typedef struct nodo {
	void *elemento;
	struct nodo *siguiente;
} nodo_t;

struct lista {
	nodo_t *nodo_inicio;
	nodo_t *nodo_fin;
	size_t cantidad;
};

struct lista_iterador {
	nodo_t *nodo_it;
	size_t posicion;
};

/**
 * Crea la lista reservando la memoria necesaria.
 * Devuelve un puntero a la lista creada o NULL en caso de error.
 */
lista_t *lista_crear()
{
	lista_t *lista = malloc(sizeof(lista_t));
	if (!lista)
		return NULL;
	lista->nodo_inicio = NULL;
	lista->nodo_fin = NULL;
	lista->cantidad = 0;
	return lista;
}

/**
 * Recibe una lista un nodo, e inserta el nodo al final de la lista.
 * Se toma en consideracion los casos cuando la lista esta vacia, y cuando no.
 */
void insertar_elemento_al_final(lista_t *lista, nodo_t *nuevo_nodo)
{
	if (lista->cantidad == 0) {
		lista->nodo_inicio = nuevo_nodo;
		lista->nodo_fin = nuevo_nodo;
	} else {
		lista->nodo_fin->siguiente = nuevo_nodo;
		lista->nodo_fin = nuevo_nodo;
	}
}

/**
 * Crea un nodo reservando la memoria necesaria.
 * Devuelve un puntero al nodo creado, o NULL en caso de error.
 */
nodo_t *crear_nuevo_nodo()
{
	nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));
	if (!nuevo_nodo)
		return NULL;
	nuevo_nodo->siguiente = NULL;
	return nuevo_nodo;
}

/**
 * Inserta un elemento al final de la lista.
 *
 * Devuelve NULL si no pudo insertar el elemento a causa de un error, o la lista en caso de exito.
 */
lista_t *lista_insertar(lista_t *lista, void *elemento)
{
	if (!lista)
		return NULL;
	nodo_t *nuevo_nodo = crear_nuevo_nodo();
	if (!nuevo_nodo)
		return NULL;
	nuevo_nodo->elemento = elemento;

	insertar_elemento_al_final(lista, nuevo_nodo);
	lista->cantidad++;
	return lista;
}

/**
 * Inserta un elemento en la posicion indicada, donde 0 es insertar
 * como primer elemento y 1 es insertar luego del primer elemento.  
 * En caso de no existir la posicion indicada, lo inserta al final.
 *
 * Devuelve NULL si no pudo insertar el elemento a causa de un error, o la lista en caso de exito.
 */
lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if (!lista)
		return NULL;
	nodo_t *nuevo_nodo = crear_nuevo_nodo();
	nuevo_nodo->elemento = elemento;

	if (lista->cantidad == 0 || posicion >= lista->cantidad)
		insertar_elemento_al_final(lista, nuevo_nodo);
	if (lista->cantidad > 0 && posicion == 0) {
		nuevo_nodo->siguiente = lista->nodo_inicio;
		lista->nodo_inicio = nuevo_nodo;
	}
	if (lista->cantidad > 0 && posicion < lista->cantidad && posicion > 0) {
		nodo_t *nodo_actual = lista->nodo_inicio;
		for (size_t i = 0; i < posicion - 1; i++)
			nodo_actual = nodo_actual->siguiente;
		nuevo_nodo->siguiente = nodo_actual->siguiente;
		nodo_actual->siguiente = nuevo_nodo;
	}
	lista->cantidad++;
	return lista;
}

/**
 * Elimina el ultimo nodo de la lista, borra su memoria reservada y devuelve el elemento que tenia.
 * Se tiene en consideracion cuando la lista tiene un elemento, o cuando tiene mas.
*/
void *eliminar_elemento_al_final(lista_t *lista)
{
	void *elemento;
	if (lista->cantidad == 1) {
		lista->nodo_inicio = NULL;
		elemento = lista->nodo_fin->elemento;
		free(lista->nodo_fin);
		lista->nodo_fin = NULL;
	} else {
		nodo_t *nodo_actual = lista->nodo_inicio;
		for (size_t i = 0; i < lista->cantidad - 2; i++) {
			nodo_actual = nodo_actual->siguiente;
		}
		nodo_actual->siguiente = NULL;
		elemento = lista->nodo_fin->elemento;
		free(lista->nodo_fin);
		lista->nodo_fin = nodo_actual;
	}
	lista->cantidad--;
	return elemento;
}

/**
 * Quita de la lista el elemento que se encuentra en la ultima posición.
 *
 * Devuelve el elemento removido de la lista o NULL en caso de error.
 */
void *lista_quitar(lista_t *lista)
{
	if (!lista || lista->cantidad == 0)
		return NULL;
	return eliminar_elemento_al_final(lista);
}

/**
 * Quita de la lista el elemento que se encuentra en la posición
 * indicada, donde 0 es el primer elemento de la lista.
 *
 * En caso de no existir esa posición se intentará borrar el último
 * elemento.
 *
 * Devuelve el elemento removido de la lista o NULL en caso de error.
 *
 */
void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	if (!lista || lista->cantidad == 0)
		return NULL;

	if (lista->cantidad == 1 || posicion >= lista->cantidad - 1) {
		return eliminar_elemento_al_final(lista);
	}
	void *elemento;
	nodo_t *nodo_actual = lista->nodo_inicio;
	if (posicion == 0) {
		lista->nodo_inicio = nodo_actual->siguiente;
		elemento = nodo_actual->elemento;
		free(nodo_actual);
	} else {
		for (size_t i = 0; i < posicion - 1; i++) {
			nodo_actual = nodo_actual->siguiente;
		}
		nodo_t *nodo_aux = nodo_actual->siguiente;
		nodo_actual->siguiente = nodo_aux->siguiente;
		elemento = nodo_aux->elemento;
		free(nodo_aux);
	}
	lista->cantidad--;
	return elemento;
}

/**
 * Devuelve el elemento en la posicion indicada, donde 0 es el primer
 * elemento.
 *
 * Si no existe dicha posicion devuelve NULL.
 */
void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if (!lista || posicion > lista->cantidad - 1 || lista->cantidad == 0)
		return NULL;

	if (posicion == lista->cantidad - 1)
		return lista->nodo_fin->elemento;
	nodo_t *nodo_actual = lista->nodo_inicio;
	for (size_t i = 0; i < posicion; i++) {
		nodo_actual = nodo_actual->siguiente;
	}
	return nodo_actual->elemento;
}

/**
 * Devuelve el primer elemento de la lista que cumple la condición
 * comparador(elemento, contexto) == 0.
 *
 * Si no existe el elemento devuelve NULL.
 */
void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{
	if (!lista || !comparador)
		return NULL;

	nodo_t *nodo_actual = lista->nodo_inicio;
	for (size_t i = 0; i < lista->cantidad; i++) {
		if (comparador(nodo_actual->elemento, contexto) == 0)
			return nodo_actual->elemento;
		nodo_actual = nodo_actual->siguiente;
	}
	return NULL;
}

/**
 * Devuelve el primer elemento de la lista o NULL si la lista se
 * encuentra vacía o no existe.
 */
void *lista_primero(lista_t *lista)
{
	if (!lista || lista->cantidad == 0)
		return NULL;
	return lista->nodo_inicio->elemento;
}

/**
 * Devuelve el último elemento de la lista o NULL si la lista se
 * encuentra vacía o no existe.
 */
void *lista_ultimo(lista_t *lista)
{
	if (!lista || lista->cantidad == 0)
		return NULL;
	return lista->nodo_fin->elemento;
}

/**
 * Devuelve true si la lista está vacía (o no existe) o false en caso contrario.
 */
bool lista_vacia(lista_t *lista)
{
	if (!lista || lista->cantidad == 0)
		return true;
	return false;
}

/**
 * Devuelve la cantidad de elementos almacenados en la lista.
 * Una lista que no existe no puede tener elementos.
 */
size_t lista_tamanio(lista_t *lista)
{
	if (!lista)
		return 0;
	return lista->cantidad;
}

/**
 * Libera la memoria reservada por la lista.
 */
void lista_destruir(lista_t *lista)
{
	if (!lista)
		return;

	if (lista->cantidad == 1)
		free(lista->nodo_inicio);
	else {
		nodo_t *nodo_actual = lista->nodo_inicio;
		for (size_t i = 0; i < lista->cantidad; i++) {
			nodo_t *nodo_aux = nodo_actual;
			nodo_actual = nodo_actual->siguiente;
			free(nodo_aux);
		}
	}
	free(lista);
}

/**
 * Libera la memoria reservada por la lista pero además aplica la función
 * destructora dada (si no es NULL) a cada uno de los elementos presentes en la
 * lista.
 */
void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if (!lista)
		return;

	if (lista->cantidad == 1) {
		if (funcion != NULL)
			funcion(lista->nodo_inicio->elemento);
		free(lista->nodo_inicio);
	} else {
		nodo_t *nodo_actual = lista->nodo_inicio;
		for (size_t i = 0; i < lista->cantidad; i++) {
			if (funcion != NULL)
				funcion(nodo_actual->elemento);
			nodo_t *nodo_aux = nodo_actual;
			nodo_actual = nodo_actual->siguiente;
			free(nodo_aux);
		}
	}
	free(lista);
}

/**
 * Crea un iterador para una lista. El iterador creado es válido desde
 * el momento de su creación hasta que no haya mas elementos por
 * recorrer o se modifique la lista iterada (agregando o quitando
 * elementos de la lista). 
 *
 * Al momento de la creación, el iterador queda listo para devolver el
 * primer elemento utilizando lista_iterador_elemento_actual.
 *
 * Devuelve el puntero al iterador creado o NULL en caso de error.
 */
lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (!lista)
		return NULL;
	lista_iterador_t *iterador = malloc(sizeof(lista_iterador_t));
	if (!iterador)
		return NULL;
	iterador->nodo_it = lista->nodo_inicio;
	return iterador;
}

/**
 * Devuelve true si hay mas elementos sobre los cuales iterar o false
 * si no hay mas.
 */
bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	if (!iterador || !iterador->nodo_it)
		return false;
	return true;
}

/**
 * Avanza el iterador al siguiente elemento.
 * Devuelve true si pudo avanzar el iterador o false en caso de
 * que no queden elementos o en caso de error.
 *
 * Una vez llegado al último elemento, si se invoca a
 * lista_iterador_elemento_actual, el resultado siempre será NULL.
 */
bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if (!lista_iterador_tiene_siguiente(iterador))
		return false;
	iterador->nodo_it = iterador->nodo_it->siguiente;
	return iterador->nodo_it != NULL;
}

/**
 * Devuelve el elemento actual del iterador o NULL en caso de que no
 * exista dicho elemento o en caso de error.
 */
void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if (!lista_iterador_tiene_siguiente(iterador))
		return NULL;
	return iterador->nodo_it->elemento;
}

/**
 * Libera la memoria reservada por el iterador.
 */
void lista_iterador_destruir(lista_iterador_t *iterador)
{
	if (!iterador)
		return;
	free(iterador);
}

/**
 * Iterador interno. Recorre la lista e invoca la funcion con cada elemento de
 * la misma como primer parámetro. Dicha función puede devolver true si se deben
 * seguir recorriendo elementos o false si se debe dejar de iterar elementos.
 *
 * El puntero contexto se pasa como segundo argumento a la función del usuario.
 *
 * La función devuelve la cantidad de elementos iterados o 0 en caso de error
 * (errores de memoria, función o lista NULL, etc).
 *
 */
size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
			       void *contexto)
{
	size_t cant = 0;
	if (!lista || !funcion || lista->cantidad == 0)
		return cant;

	nodo_t *nodo_actual = lista->nodo_inicio;
	for (size_t i = 0; i < lista->cantidad; i++) {
		bool estado = funcion(nodo_actual->elemento, contexto);
		cant++;
		if (!estado)
			break;
		nodo_actual = nodo_actual->siguiente;
	}
	return cant;
}
