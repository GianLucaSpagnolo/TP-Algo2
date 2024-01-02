#include <string.h>
#include <stdlib.h>

#include "hash.h"

#define FACTOR_CARGA_MAXIMO 0.7
#define CAPACIDAD_MINIMA 3

/**
 * Estructura de nodo que almacena un par clave - valor que se ha insertado
 * en el vector de hash, y que se encuentra simplemente enlazado a otro nodo
 * siguiente que puede ser almacenado en la misma posicion del vector.
*/
typedef struct nodo {
	char *clave;
	void *valor;
	struct nodo *siguiente;
} nodo_t;

/**
 * Estructura principal que contiene el vector de Hash con uno o mas nodos en  
 * cada posicion, y tambien lleva la cuenta de la capacidad maxima y de la cantidad
 * de elementos en el vector de hash.
*/
struct hash {
	nodo_t **vector;
	size_t capacidad;
	size_t cantidad;
};

/*
 * Crea el hash reservando la memoria necesaria para el.
 *
 * Capacidad indica la capacidad inicial con la que se crea el hash. La
 * capacidad inicial no puede ser menor a 3. Si se solicita una capacidad menor,
 * el hash se creará con una capacidad de 3.
 *
 * Devuelve un puntero al hash creado o NULL en caso de no poder crearlo.
 */
hash_t *hash_crear(size_t capacidad)
{
	hash_t *hash_creado = calloc(1, sizeof(hash_t));
	if (!hash_creado)
		return NULL;
	if (capacidad < CAPACIDAD_MINIMA)
		capacidad = CAPACIDAD_MINIMA;
	hash_creado->capacidad = capacidad;
	hash_creado->vector = calloc(1, sizeof(nodo_t *) * capacidad);
	if (!hash_creado->vector) {
		free(hash_creado);
		return NULL;
	}
	return hash_creado;
}

/*
 * Crea un nodo reservando la memoria necesaria para el.
 *
 * El nodo incluye una clave copiada a la enviada por parametro (para evitar 
 * problemas en caso de que el usuario cambie el contenido del puntero), un valor 
 * y un puntero a un nodo siguiente enlazado.
 *
 * Devuelve un puntero al nodo creado o NULL en caso de no poder crearlo.
 */
nodo_t *nodo_crear(const char *k, void *v)
{
	nodo_t *nodo_creado = calloc(1, sizeof(nodo_t));
	if (!nodo_creado)
		return NULL;
	nodo_creado->clave = calloc(1, sizeof(char) * strlen(k) + 1);
	if (!nodo_creado->clave) {
		free(nodo_creado);
		return NULL;
	}
	strcpy(nodo_creado->clave, k);
	nodo_creado->valor = v;
	return nodo_creado;
}

/**
 * Funcion Hash DJB2
 * 
 * Recibe un string de una clave, y la transforma en un numero asociado, que
 * corresponde a una posicion en el vector de hash.
*/
unsigned long funcion_hash(const char *str)
{
	unsigned long posicion = 5381;
	int c;
	while ((c = *str++))
		posicion = ((posicion << 5) + posicion) + (unsigned long)c;
	return posicion;
}

/**
 * Funcion utilizada al realizar un rehash, que inserta cada nodo enviado por
 * parametro al vector de hash del hash utilizado, en cada posicion
 * correspondiente.
 * 
 * Esta funcion no verifica que existan dos nodos con misma clave ya que todos
 * los elementos fueron previamente insertados al vector de hash anterior, 
 * pasando por dicha verificacion y modificando el valor en cada caso.
*/
hash_t *insertar_nodo_en_vector(hash_t *hash, nodo_t *nodo)
{
	unsigned long posicion =
		funcion_hash(nodo->clave) % (unsigned long)(hash->capacidad);
	nodo->siguiente = hash->vector[(int)posicion];
	hash->vector[(int)posicion] = nodo;
	return hash;
}

/**
 * Funcion rehash utilizada al intentar insertar un elemento al hash, cuando 
 * la cantidad de elementos dentro del hash supera un limite previamente
 * establecido (en este caso, por FACTOR_CARGA_MAXIMO), lo que indica que se
 * debe expandir el vector de hash al doble de la capacidad original.
 * 
 * Para ello, crea un nuevo vector de hash con el doble de capacidad, e
 * inserta todos los nodos en este nuevo vector en cada posicion que les
 * corresponderia con esta nueva capacidad, y elimina el vector anterior.
*/
hash_t *rehash(hash_t *hash)
{
	nodo_t **vector_viejo = hash->vector;
	hash->vector = calloc(1, sizeof(nodo_t *) * hash->capacidad * 2);
	if (!hash->vector)
		return NULL;
	int capacidad_anterior = (int)(hash->capacidad);
	hash->capacidad *= 2;
	for (int i = 0; i < capacidad_anterior; i++) {
		nodo_t *nodo_actual = vector_viejo[i];
		while (nodo_actual) {
			nodo_t *nodo_siguiente = nodo_actual->siguiente;
			hash = insertar_nodo_en_vector(hash, nodo_actual);
			nodo_actual = nodo_siguiente;
		}
	}
	free(vector_viejo);
	return hash;
}

/**
 * Funcion utilizada por hash_insertar() que crea un nodo con el par
 * clave - valor enviados por parametro, y lo inserta en la posicion del
 * vector especifica, y en caso de existir nodos en esa posicion coloca
 * al nodo existente como siguiente del nodo creado.
*/
hash_t *crear_e_insertar_nodo(hash_t *hash, const char *clave, void *elemento,
			      int posicion)
{
	nodo_t *nodo_creado = nodo_crear(clave, elemento);
	if (!nodo_creado)
		return NULL;
	nodo_creado->siguiente = hash->vector[posicion];
	hash->vector[posicion] = nodo_creado;
	hash->cantidad++;
	return hash;
}

/**
 * Funcion utilizada por hash_insertar() que verifica la existencia de la
 * clave a insertar en el vector de hash. 
 * 
 * Si existe la clave, guarda su valor en *anterior si anterior no es NULL, 
 * cambia el valor de aquel nodo por el valor a insertar, y devuelve true 
 * para verificar que ha sido insertado.
 * Si no existe la clave, devuelve false.
*/
bool insertar_en_vector_clave_repetida(hash_t *hash, const char *clave,
				       void *elemento, int posicion,
				       void **anterior)
{
	nodo_t *nodo_actual = hash->vector[posicion];
	while (nodo_actual) {
		if (strcmp(nodo_actual->clave, clave) == 0) {
			if (anterior)
				*anterior = nodo_actual->valor;
			nodo_actual->valor = elemento;
			return true;
		}
		nodo_actual = nodo_actual->siguiente;
	}
	return false;
}

/*
 * Inserta o actualiza un elemento en el hash asociado a la clave dada.
 *
 * Si la clave ya existía y se reemplaza el elemento, se almacena un puntero al
 * elemento reemplazado en *anterior, si anterior no es NULL.
 *
 * Si la clave no existía y anterior no es NULL, se almacena NULL en *anterior.
 *
 * La función almacena una copia de la clave provista por el usuario,
 *
 * Nota para los alumnos: Recordar que si insertar un elemento provoca
 * que el factor de carga exceda cierto umbral, SE DEBE AJUSTAR EL
 * TAMAÑO DE LA TABLA PARA EVITAR FUTURAS COLISIONES.
 *
 * Devuelve el hash si pudo guardar el elemento o NULL si no pudo.
 */
hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if (!hash || !clave)
		return NULL;
	float carga = (float)(hash->cantidad) / (float)(hash->capacidad);
	if (carga > FACTOR_CARGA_MAXIMO) {
		hash = rehash(hash);
		if (!hash)
			return NULL;
	}
	unsigned long posicion =
		funcion_hash(clave) % (unsigned long)(hash->capacidad);

	if (anterior)
		*anterior = NULL;
	if (!hash->vector[(int)posicion])
		return crear_e_insertar_nodo(hash, clave, elemento,
					     (int)posicion);
	bool hay_clave_repetida = insertar_en_vector_clave_repetida(
		hash, clave, elemento, (int)posicion, anterior);
	if (hay_clave_repetida)
		return hash;
	return crear_e_insertar_nodo(hash, clave, elemento, (int)posicion);
}

/*
 * Quita un elemento del hash y lo devuelve.
 *
 * Si no encuentra el elemento o en caso de error devuelve NULL
 */
void *hash_quitar(hash_t *hash, const char *clave)
{
	if (!hash_cantidad(hash) || !clave)
		return NULL;
	unsigned long posicion =
		funcion_hash(clave) % (unsigned long)(hash->capacidad);
	void *elemento;
	nodo_t *nodo_actual = hash->vector[(int)posicion];
	nodo_t *nodo_anterior = NULL;
	while (nodo_actual) {
		if (strcmp(nodo_actual->clave, clave) == 0) {
			if (!nodo_anterior)
				hash->vector[(int)posicion] =
					nodo_actual->siguiente;
			else
				nodo_anterior->siguiente =
					nodo_actual->siguiente;
			elemento = nodo_actual->valor;
			free(nodo_actual->clave);
			free(nodo_actual);
			hash->cantidad--;
			return elemento;
		}
		nodo_anterior = nodo_actual;
		nodo_actual = nodo_actual->siguiente;
	}
	return NULL;
}

/*
 * Devuelve un elemento del hash con la clave dada o NULL si dicho
 * elemento no existe (o en caso de error).
 */
void *hash_obtener(hash_t *hash, const char *clave)
{
	if (!hash_cantidad(hash) || !clave)
		return NULL;
	unsigned long posicion =
		funcion_hash(clave) % (unsigned long)(hash->capacidad);
	nodo_t *nodo_actual = hash->vector[(int)posicion];
	while (nodo_actual) {
		if (strcmp(nodo_actual->clave, clave) == 0)
			return nodo_actual->valor;
		nodo_actual = nodo_actual->siguiente;
	}
	return NULL;
}

/*
 * Devuelve true si el hash contiene un elemento almacenado con la
 * clave dada o false en caso contrario (o en caso de error).
 */
bool hash_contiene(hash_t *hash, const char *clave)
{
	if (!hash_cantidad(hash) || !clave)
		return false;
	unsigned long posicion =
		funcion_hash(clave) % (unsigned long)(hash->capacidad);
	nodo_t *nodo_actual = hash->vector[(int)posicion];
	while (nodo_actual) {
		if (strcmp(nodo_actual->clave, clave) == 0)
			return true;
		nodo_actual = nodo_actual->siguiente;
	}
	return false;
}

/*
 * Devuelve la cantidad de elementos almacenados en el hash o 0 en
 * caso de error.
 */
size_t hash_cantidad(hash_t *hash)
{
	if (!hash)
		return 0;
	return hash->cantidad;
}

/*
 * Destruye el hash liberando la memoria reservada.
 */
void hash_destruir(hash_t *hash)
{
	hash_destruir_todo(hash, NULL);
}

/*
 * Destruye el hash liberando la memoria reservada y asegurandose de
 * invocar la funcion destructora con cada elemento almacenado en el
 * hash.
 */
void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash)
		return;
	for (int i = 0; i < (int)(hash->capacidad); i++) {
		nodo_t *nodo_actual = hash->vector[i];
		while (nodo_actual) {
			nodo_t *nodo_siguiente = nodo_actual->siguiente;
			if (destructor)
				destructor(nodo_actual->valor);
			free(nodo_actual->clave);
			free(nodo_actual);
			nodo_actual = nodo_siguiente;
		}
	}
	free(hash->vector);
	free(hash);
}

/*
 * Recorre cada una de las claves almacenadas en la tabla de hash e invoca a la
 * función f, pasandole como parámetros la clave, el valor asociado a la clave y
 * el puntero auxiliar.
 *
 * Mientras que queden mas claves o la funcion retorne true, la
 * iteración continúa. Cuando no quedan mas claves o la función
 * devuelve false, la iteración se corta y la función principal
 * retorna.
 *
 * Devuelve la cantidad de claves totales iteradas (la cantidad de
 * veces que fue invocada la función) o 0 en caso de error.
 *
 */
size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux),
			   void *aux)
{
	size_t n = 0;
	if (!hash_cantidad(hash) || !f)
		return n;
	for (int i = 0; i < (int)(hash->capacidad); i++) {
		nodo_t *nodo_actual = hash->vector[i];
		bool continuar = true;
		while (nodo_actual) {
			continuar =
				f(nodo_actual->clave, nodo_actual->valor, aux);
			n++;
			if (!continuar)
				break;
			nodo_actual = nodo_actual->siguiente;
		}
		if (!continuar)
			break;
	}
	return n;
}