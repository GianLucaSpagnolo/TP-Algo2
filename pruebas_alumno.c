#include "pa2m.h"
#include "src/menu.h"
#include "src/lista.h"

#include <stdlib.h>
#include <string.h>

void pruebas_inicializacion_menu_y_casos_borde()
{
	pa2m_afirmar(menu_cantidad_opciones(NULL) == 0,
		     "La cantidad de opciones de un menu inexistente es 0.");
	pa2m_afirmar(menu_nombre(NULL) == NULL,
		     "El nombre de un menu inexistente es NULL.");
	char tecla1[2] = "S";
	pa2m_afirmar(
		menu_obtener_descripcion(NULL, tecla1) == NULL,
		"Buscar la descripcion de una opcion con un menu inexistente es NULL.");
	char nombre1[6] = "menu1";
	menu_t *menu_creado = menu_crear(nombre1);
	pa2m_afirmar(menu_creado != NULL,
		     "El menu creado es diferente de NULL.");
	pa2m_afirmar(menu_cantidad_opciones(menu_creado) == 0,
		     "La cantidad de elementos de un menu vacio es 0.");
	pa2m_afirmar(menu_nombre(menu_creado) == nombre1,
		     "Se puede verificar el nombre de un menu vacio.");
	pa2m_afirmar(
		menu_obtener_descripcion(menu_creado, tecla1) == NULL,
		"Buscar la descripcion de una opcion con un menu vacio es NULL.");
	menu_destruir(menu_creado);
}

bool funcion_vacia(void *elemento1, void *elemento2)
{
	return false;
}

void pruebas_agregar_opcion_casos_borde()
{
	char nombre1[6] = "menu2";
	menu_t *menu = menu_crear(nombre1);
	char tecla1[2] = "D";
	char descripcion1[8] = "Prueba 1";
	pa2m_afirmar(
		menu_agregar_opcion(NULL, tecla1, descripcion1, funcion_vacia,
				    NULL) == false,
		"Agregar una opcion en el menu con un menu NULL devuelve false.");
	pa2m_afirmar(
		menu_agregar_opcion(menu, NULL, descripcion1, funcion_vacia,
				    NULL) == false,
		"Agregar una opcion en el menu con una tecla NULL devuelve false.");
	pa2m_afirmar(
		menu_agregar_opcion(menu, tecla1, NULL, funcion_vacia, NULL) ==
			false,
		"Agregar una opcion en el menu con una descripcion NULL devuelve false.");
	pa2m_afirmar(
		menu_agregar_opcion(menu, tecla1, descripcion1, NULL, NULL) ==
			false,
		"Agregar una opcion en el menu con una funcion NULL devuelve false.\n");
	menu_destruir(menu);
}

void pruebas_agregar_varias_opciones()
{
	char nombre[6] = "menu3";
	menu_t *menu = menu_crear(nombre);
	char tecla1[2] = "A";
	char descripcion1[8] = "Prueba 1";
	pa2m_afirmar(menu_agregar_opcion(menu, tecla1, descripcion1,
					 funcion_vacia, NULL) == true,
		     "Al agregar una opcion valida se devuelve true.");
	char tecla2[2] = "B";
	char descripcion2[8] = "Prueba 2";
	char tecla3[2] = "C";
	char descripcion3[8] = "Prueba 3";
	menu_agregar_opcion(menu, tecla2, descripcion2, funcion_vacia, NULL);
	pa2m_afirmar(menu_agregar_opcion(menu, tecla3, descripcion3,
					 funcion_vacia, NULL) == true,
		     "Al agregar otra opcion valida se devuelve true.");
	pa2m_afirmar(menu_cantidad_opciones(menu) == 3,
		     "La cantidad de las opciones se actualiza correctamente");
	pa2m_afirmar(
		menu_obtener_descripcion(menu, tecla1) == descripcion1 &&
			menu_obtener_descripcion(menu, tecla2) ==
				descripcion2 &&
			menu_obtener_descripcion(menu, tecla3) == descripcion3,
		"Se pueden obtener las descripciones de las tres opciones agregadas.");
	menu_destruir(menu);
}

void pruebas_agregar_opcion_reemplazo()
{
	char nombre[6] = "menu4";
	menu_t *menu = menu_crear(nombre);
	char tecla1[2] = "F";
	char descripcion1[8] = "Prueba 1";
	menu_agregar_opcion(menu, tecla1, descripcion1, funcion_vacia, NULL);
	char descripcion2[8] = "HUHAIFO";
	pa2m_afirmar(
		menu_agregar_opcion(menu, tecla1, descripcion2, funcion_vacia,
				    NULL) == true,
		"Al agregar una opcion cuya tecla existe, se devuelve true.");
	pa2m_afirmar(
		menu_obtener_descripcion(menu, tecla1) == descripcion2,
		"Al agregar una opcion cuya tecla existe, el contenido de la opcion cambia.");
	pa2m_afirmar(
		menu_cantidad_opciones(menu) == 1,
		"Al agregar una opcion cuya tecla existe, la cantidad de opciones es la misma.");
	menu_destruir(menu);
}

void pruebas_eliminar_opcion_casos_borde()
{
	char nombre1[6] = "menu5";
	menu_t *menu = menu_crear(nombre1);
	char tecla1[2] = "P";
	char descripcion1[8] = "Prueba 1";
	pa2m_afirmar(menu_eliminar_opcion(menu, tecla1) == false,
		     "Eliminar una opcion de un menu vacio devuelve false.");
	menu_agregar_opcion(menu, tecla1, descripcion1, funcion_vacia, NULL);
	pa2m_afirmar(
		menu_eliminar_opcion(NULL, tecla1) == false,
		"Eliminar una opcion del menu con un menu NULL devuelve false.");
	pa2m_afirmar(
		menu_eliminar_opcion(menu, NULL) == false,
		"Eliminar una opcion del menu con una tecla NULL devuelve false.");
	char tecla2[2] = "U";
	pa2m_afirmar(
		menu_eliminar_opcion(menu, tecla2) == false,
		"Eliminar una opcion inexistente del menu devuelve false.\n");
	menu_destruir(menu);
}

void pruebas_eliminar_unica_opcion()
{
	char nombre1[6] = "menu6";
	menu_t *menu = menu_crear(nombre1);
	char tecla1[2] = "K";
	char descripcion1[8] = "Prueba 1";
	menu_agregar_opcion(menu, tecla1, descripcion1, funcion_vacia, NULL);
	pa2m_afirmar(menu_eliminar_opcion(menu, tecla1) == true,
		     "Eliminar la unica opcion de un menu devuelve true.");
	pa2m_afirmar(
		menu_obtener_descripcion(menu, tecla1) == NULL,
		"Ya no se puede obtener la descripcion de la opcion eliminada.");
	pa2m_afirmar(menu_cantidad_opciones(menu) == 0,
		     "La cantidad de opciones del menu pasa a ser 0.");
	menu_destruir(menu);
}

void pruebas_eliminar_varias_opciones()
{
	char nombre1[6] = "menu7";
	menu_t *menu = menu_crear(nombre1);
	char tecla1[2] = "L";
	char descripcion1[8] = "Prueba 1";
	char tecla2[2] = "M";
	char descripcion2[8] = "Prueba 2";
	char tecla3[2] = "N";
	char descripcion3[8] = "Prueba 3";
	char tecla4[2] = "Ñ";
	char descripcion4[8] = "Prueba 4";
	char tecla5[2] = "O";
	char descripcion5[8] = "Prueba 5";
	menu_agregar_opcion(menu, tecla1, descripcion1, funcion_vacia, NULL);
	menu_agregar_opcion(menu, tecla2, descripcion2, funcion_vacia, NULL);
	menu_agregar_opcion(menu, tecla3, descripcion3, funcion_vacia, NULL);
	menu_agregar_opcion(menu, tecla4, descripcion4, funcion_vacia, NULL);
	menu_agregar_opcion(menu, tecla5, descripcion5, funcion_vacia, NULL);
	pa2m_afirmar(menu_eliminar_opcion(menu, tecla4) == true &&
			     menu_eliminar_opcion(menu, tecla2) == true &&
			     menu_eliminar_opcion(menu, tecla3) == true,
		     "Eliminar varias opciones de un menu devuelve true.");
	pa2m_afirmar(
		menu_obtener_descripcion(menu, tecla4) == NULL &&
			menu_obtener_descripcion(menu, tecla2) == NULL &&
			menu_obtener_descripcion(menu, tecla3) == NULL,
		"Ya no se puede obtener la descripcion de las opciones eliminadas.");
	pa2m_afirmar(
		menu_cantidad_opciones(menu) == 2,
		"La cantidad de opciones del menu se actualiza correctamente.");
	pa2m_afirmar(
		menu_eliminar_opcion(menu, tecla3) == false,
		"Al intentar eliminar una opcion previamente eliminada devuelve false.");
	menu_destruir(menu);
}

void pruebas_obtener_descripcion_casos_borde()
{
	char nombre1[6] = "menu8";
	menu_t *menu = menu_crear(nombre1);
	char tecla1[2] = "E";
	char descripcion1[8] = "Prueba 1";
	menu_agregar_opcion(menu, tecla1, descripcion1, funcion_vacia, NULL);
	char tecla2[2] = "R";
	pa2m_afirmar(
		menu_obtener_descripcion(menu, NULL) == NULL,
		"Buscar la descripcion de una opcion con una tecla NULL es NULL.");
	pa2m_afirmar(
		menu_obtener_descripcion(menu, tecla2) == NULL,
		"Buscar la descripcion de una opcion con una tecla no agregada es NULL.");
	pa2m_afirmar(
		menu_obtener_descripcion(menu, tecla1) == descripcion1,
		"Buscar la descripcion de una opcion con una tecla agregada devuelve su descripcion.");
	menu_destruir(menu);
}

void pruebas_agregar_eliminar_obtener_opcion_en_conjunto()
{
	char nombre1[6] = "menu9";
	menu_t *menu = menu_crear(nombre1);
	char tecla1[2] = "U";
	char descripcion1[8] = "Prueba 1";
	char tecla2[2] = "V";
	char descripcion2[8] = "Prueba 2";
	char tecla3[2] = "W";
	char descripcion3[8] = "Prueba 3";
	char tecla4[2] = "X";
	char descripcion4[8] = "Prueba 4";
	char tecla5[2] = "Y";
	char descripcion5[8] = "Prueba 5";
	pa2m_afirmar(menu_nombre(menu) == nombre1,
		     "El nombre del menu creado es el correcto.");
	menu_agregar_opcion(menu, tecla1, descripcion1, funcion_vacia, NULL);
	pa2m_afirmar(menu_obtener_descripcion(menu, tecla1) == descripcion1,
		     "Se ha insertado la primera opcion correctamente.");
	menu_agregar_opcion(menu, tecla2, descripcion2, funcion_vacia, NULL);
	pa2m_afirmar(menu_obtener_descripcion(menu, tecla2) == descripcion2,
		     "Se ha insertado la segunda opcion correctamente.");
	menu_agregar_opcion(menu, tecla3, descripcion3, funcion_vacia, NULL);
	pa2m_afirmar(menu_obtener_descripcion(menu, tecla3) == descripcion3,
		     "Se ha insertado la tercera opcion correctamente.");
	menu_agregar_opcion(menu, tecla4, descripcion4, funcion_vacia, NULL);
	pa2m_afirmar(menu_obtener_descripcion(menu, tecla4) == descripcion4,
		     "Se ha insertado la cuarta opcion correctamente.");
	menu_agregar_opcion(menu, tecla5, descripcion5, funcion_vacia, NULL);
	pa2m_afirmar(menu_obtener_descripcion(menu, tecla5) == descripcion5,
		     "Se ha insertado la quinta opcion correctamente.");
	pa2m_afirmar(menu_cantidad_opciones(menu) == 5,
		     "La cantidad de opciones se actualiza correctamente.");
	menu_eliminar_opcion(menu, tecla1);
	pa2m_afirmar(
		menu_obtener_descripcion(menu, tecla1) == NULL,
		"La primera opcion no se encuentra en el menu despues de haberla eliminado.");
	pa2m_afirmar(menu_cantidad_opciones(menu) == 4,
		     "La cantidad de opciones se actualiza correctamente.");
	menu_agregar_opcion(menu, tecla1, descripcion1, funcion_vacia, NULL);
	pa2m_afirmar(
		menu_obtener_descripcion(menu, tecla1) == descripcion1,
		"Despues de haberla insertado denuevo, la primera opcion se encuentra en el menu.");
	pa2m_afirmar(menu_cantidad_opciones(menu) == 5,
		     "La cantidad de opciones se actualiza correctamente.");
	char descripcion6[11] = "Antiprueba";
	menu_agregar_opcion(menu, tecla2, descripcion6, funcion_vacia, NULL);
	pa2m_afirmar(
		menu_obtener_descripcion(menu, tecla2) == descripcion6,
		"Al insertar una tecla ya existente, se modifica el contenido de la opcion.");
	pa2m_afirmar(menu_cantidad_opciones(menu) == 5,
		     "La cantidad de opciones se mantiene igual.");
	menu_eliminar_opcion(menu, tecla2);
	pa2m_afirmar(
		menu_obtener_descripcion(menu, tecla2) == NULL,
		"Al eliminar la opcion modificada, ya no se encuentra en el menu.");
	pa2m_afirmar(menu_cantidad_opciones(menu) == 4,
		     "La cantidad de opciones se actualiza correctamente.");
	menu_destruir(menu);
}

bool funcion_mayor_a_contexto(void *dato, void *contexto)
{
	if (!dato || !contexto)
		return false;
	int *numero1 = (int *)dato;
	int *numero2 = (int *)contexto;
	if (*numero1 > *numero2)
		return true;
	return false;
}

void pruebas_ejecutar_opcion_casos_borde()
{
	char nombre1[7] = "menu10";
	menu_t *menu = menu_crear(nombre1);
	char tecla1[2] = "I";
	char descripcion1[8] = "Prueba 1";
	int contexto = 20;
	int dato = 10;
	menu_agregar_opcion(menu, tecla1, descripcion1,
			    funcion_mayor_a_contexto, &contexto);
	pa2m_afirmar(menu_ejecutar_opcion(NULL, tecla1, &dato) == false,
		     "Ejecutar una opcion con un menu NULL devuelve false.");
	pa2m_afirmar(menu_ejecutar_opcion(menu, NULL, &dato) == false,
		     "Ejecutar una opcion con una tecla NULL devuelve false.");
	pa2m_afirmar(
		menu_ejecutar_opcion(menu, tecla1, NULL) == false,
		"Ejecutar una opcion con un dato NULL depende de la funcion asignada a la opcion.");
	char tecla2[2] = "O";
	char descripcion2[8] = "Prueba 2";
	menu_agregar_opcion(menu, tecla2, descripcion2,
			    funcion_mayor_a_contexto, NULL);
	pa2m_afirmar(
		menu_ejecutar_opcion(menu, tecla1, &dato) == false,
		"Ejecutar una opcion con un contexto de la opcion NULL depende de la funcion asignada a la opcion.\n");
	menu_destruir(menu);
}

void pruebas_ejecutar_opcion_validos()
{
	char nombre1[7] = "menu11";
	menu_t *menu = menu_crear(nombre1);
	char tecla1[2] = "R";
	char descripcion1[8] = "Prueba 1";
	int contexto1 = 20;
	char tecla2[2] = "S";
	char descripcion2[8] = "Prueba 2";
	int contexto2 = 2;
	char tecla3[2] = "T";
	char descripcion3[8] = "Prueba 3";
	int contexto3 = 5;
	int dato = 10;
	menu_agregar_opcion(menu, tecla1, descripcion1,
			    funcion_mayor_a_contexto, &contexto1);
	menu_agregar_opcion(menu, tecla2, descripcion2,
			    funcion_mayor_a_contexto, &contexto2);
	menu_agregar_opcion(menu, tecla3, descripcion3,
			    funcion_mayor_a_contexto, &contexto3);
	pa2m_afirmar(
		menu_ejecutar_opcion(menu, tecla1, &dato) == false,
		"Se ejecuta correctamente la funcion de la primer opcion.");
	pa2m_afirmar(
		menu_ejecutar_opcion(menu, tecla2, &dato) == true,
		"Se ejecuta correctamente la funcion de la segunda opcion.");
	pa2m_afirmar(
		menu_ejecutar_opcion(menu, tecla3, &dato) == true,
		"Se ejecuta correctamente la funcion de la tercera opcion.");
	menu_destruir(menu);
}

void pruebas_ayuda_casos_borde()
{
	char nombre1[7] = "menu12";
	menu_t *menu = menu_crear(nombre1);
	pa2m_afirmar(menu_mostrar_ayuda(NULL) == NULL,
		     "No se puede obtener la lista de ayuda con un menu NULL.");
	pa2m_afirmar(
		menu_mostrar_ayuda(menu) == NULL,
		"No se puede obtener la lista de ayuda con un menu vacio.");
	menu_destruir(menu);
}

void pruebas_ayuda_unica_opcion()
{
	char nombre1[7] = "menu13";
	menu_t *menu = menu_crear(nombre1);
	char tecla1[2] = "G";
	char descripcion1[8] = "Prueba 1";
	menu_agregar_opcion(menu, tecla1, descripcion1,
			    funcion_mayor_a_contexto, NULL);
	lista_t *ayuda = menu_mostrar_ayuda(menu);
	pa2m_afirmar(
		ayuda != NULL,
		"Se entrega una lista al tener un menu con una opcion valida.");
	void *elemento1 = lista_elemento_en_posicion(ayuda, 0);
	void *elemento2 = lista_elemento_en_posicion(ayuda, 1);
	pa2m_afirmar(
		(char *)elemento1 == tecla1 &&
			(char *)elemento2 == descripcion1 &&
			lista_tamanio(ayuda) == 2,
		"La lista de ayuda contiene la tecla y la descripcion de la unica opcion del menu.");
	menu_destruir_con_lista_ayuda(menu, ayuda);
}

void pruebas_ayuda_varias_opciones()
{
	char nombre1[7] = "menu14";
	menu_t *menu = menu_crear(nombre1);
	char tecla1[2] = "H";
	char descripcion1[8] = "Prueba 1";
	char tecla2[2] = "I";
	char descripcion2[8] = "Prueba 2";
	char tecla3[2] = "J";
	char descripcion3[8] = "Prueba 3";
	menu_agregar_opcion(menu, tecla1, descripcion1,
			    funcion_mayor_a_contexto, NULL);
	menu_agregar_opcion(menu, tecla2, descripcion2,
			    funcion_mayor_a_contexto, NULL);
	menu_agregar_opcion(menu, tecla3, descripcion3,
			    funcion_mayor_a_contexto, NULL);
	lista_t *ayuda = menu_mostrar_ayuda(menu);
	void *elemento1 = lista_elemento_en_posicion(ayuda, 0);
	void *elemento2 = lista_elemento_en_posicion(ayuda, 1);
	void *elemento3 = lista_elemento_en_posicion(ayuda, 2);
	void *elemento4 = lista_elemento_en_posicion(ayuda, 3);
	void *elemento5 = lista_elemento_en_posicion(ayuda, 4);
	void *elemento6 = lista_elemento_en_posicion(ayuda, 5);
	pa2m_afirmar(
		(char *)elemento1 == tecla1 &&
			(char *)elemento2 == descripcion1 &&
			(char *)elemento3 == tecla2 &&
			(char *)elemento4 == descripcion2 &&
			(char *)elemento5 == tecla3 &&
			(char *)elemento6 == descripcion3 &&
			lista_tamanio(ayuda) == 6,
		"Al tener un menu con varias opciones, la lista de ayuda contiene cada tecla y cada descripcion en orden.");
	menu_destruir_con_lista_ayuda(menu, ayuda);
}

int main()
{
	pa2m_nuevo_grupo(
		"\nXx------------------- PRUEBAS DE TDA: MENU -------------------xX");

	pa2m_nuevo_grupo("\nPRUEBAS DE MENU: CREACIÓN");
	pruebas_inicializacion_menu_y_casos_borde();

	pa2m_nuevo_grupo("\nPRUEBAS DE MENU: AGREGAR OPCION");
	pruebas_agregar_opcion_casos_borde();
	pruebas_agregar_varias_opciones();
	pruebas_agregar_opcion_reemplazo();

	pa2m_nuevo_grupo("\nPRUEBAS DE MENU: ELIMINAR OPCION");
	pruebas_eliminar_opcion_casos_borde();
	pruebas_eliminar_unica_opcion();
	pruebas_eliminar_varias_opciones();

	pa2m_nuevo_grupo("\nPRUEBAS DE MENU: PRUEBAS EN CONJUNTO");
	pruebas_obtener_descripcion_casos_borde();
	pruebas_agregar_eliminar_obtener_opcion_en_conjunto();

	pa2m_nuevo_grupo("\nPRUEBAS DE MENU: EJECUTAR OPCION");
	pruebas_ejecutar_opcion_casos_borde();
	pruebas_ejecutar_opcion_validos();

	pa2m_nuevo_grupo("\nPRUEBAS DE MENU: AYUDA");
	pruebas_ayuda_casos_borde();
	pruebas_ayuda_unica_opcion();
	pruebas_ayuda_varias_opciones();

	return pa2m_mostrar_reporte();
}