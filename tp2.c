#include <string.h>
#include <stdio.h>

#include "src/tp1.h"
#include "src/menu.h"

#define MAXIMO_CARACTERES 256
#define CAPACIDAD_DEFAULT_HASH_HOSPITALES 10

typedef struct wrapper_hospital {
	char *id;
	char *nombre;
	hospital_t *hospital;
} wrapper_hospital_t;

typedef struct hash_hospital {
	hash_t *hash;
	wrapper_hospital_t *hospital_activo;
	size_t id_generador;
	size_t cantidad;
} hash_hospital_t;

void registrar_entrada_usuario(char str[])
{
	printf("« COMANDO » ");
	scanf("%255s", str);
}

void mostrar_mensaje_determinado(char *src)
{
	FILE *archivo = fopen(src, "r");
	char linea[MAXIMO_CARACTERES];
	while (fscanf(archivo, "%[^\n]\n", linea) == 1)
		printf("%s\n", linea);
	printf("\n");
	fclose(archivo);
}

bool mi_menu_salir(void *dato, void *contexto)
{
	return false;
}

bool mi_menu_ayuda(void *dato, void *contexto)
{
	menu_t *menu = (menu_t *)contexto;
	lista_t *ayuda = menu_mostrar_ayuda(menu);
	if (!ayuda)
		return false;
	printf("\n[ %s ]\n", menu_nombre(menu));
	printf("\n[ Los comandos permitidos son: ]\n\n");
	for (size_t i = 0; i < lista_tamanio(ayuda) / 2; i++)
		printf("• %s - %s\n",
		       (char *)lista_elemento_en_posicion(ayuda, i * 2),
		       (char *)lista_elemento_en_posicion(ayuda, i * 2 + 1));
	printf("\n");
	menu_destruir_con_lista_ayuda(NULL, ayuda);
	return true;
}

bool liberar_memoria_hospital(wrapper_hospital_t *hospital)
{
	if (hospital->id != NULL)
		free(hospital->id);
	if (hospital->nombre != NULL)
		free(hospital->nombre);
	if (hospital->hospital != NULL)
		hospital_destruir(hospital->hospital);
	free(hospital);
	return false;
}

bool mi_menu_cargar(void *dato, void *contexto)
{
	hash_hospital_t *principal = (hash_hospital_t *)dato;
	char src[MAXIMO_CARACTERES];
	printf("\n[ Ingrese la direccion del archivo. Para cancelar, escriba « N » ]: ");
	scanf("%255s", src);
	char cancelar[] = "N";
	if (strcmp(src, cancelar) == 0) {
		printf("\n");
		return true;
	}

	FILE *archivo = fopen(src, "r");
	if (!archivo) {
		printf("\n[ No se ha encontrado un archivo en esa direccion ]\n\n");
		return true;
	}
	fclose(archivo);

	wrapper_hospital_t *hospital = calloc(1, sizeof(wrapper_hospital_t));
	if (!hospital)
		return false;
	hospital->hospital = hospital_crear_desde_archivo(src);
	if (!hospital->hospital)
		return liberar_memoria_hospital(hospital);

	char hospital_id[MAXIMO_CARACTERES];
	sprintf(hospital_id, "%zu", principal->id_generador);
	hospital->id = calloc(1, sizeof(char) * MAXIMO_CARACTERES);
	if (!hospital->id)
		return liberar_memoria_hospital(hospital);
	strcpy(hospital->id, hospital_id);

	hospital->nombre = calloc(1, sizeof(char) * MAXIMO_CARACTERES);
	if (!hospital->nombre)
		return liberar_memoria_hospital(hospital);
	strcpy(hospital->nombre, src);

	hash_t *aux = hash_insertar(principal->hash, hospital->id,
				    (void *)hospital, NULL);
	if (!aux)
		return liberar_memoria_hospital(hospital);
	principal->id_generador++;
	principal->cantidad++;
	printf("\n[ El hospital ha sido cargado exitosamente ]\n\n");
	return true;
}

bool imprimir_estado_hospitales(const char *clave, void *valor, void *aux)
{
	hash_hospital_t *principal = (hash_hospital_t *)aux;
	wrapper_hospital_t *hospital = (wrapper_hospital_t *)valor;
	if (principal->hospital_activo != NULL &&
	    strcmp(principal->hospital_activo->id, hospital->id) == 0)
		printf("• [ Hospital ID: %s ] Activo\n", hospital->id);
	else
		printf("• Hospital ID: %s\n", hospital->id);
	return true;
}

bool mi_menu_estado(void *dato, void *contexto)
{
	hash_hospital_t *principal = (hash_hospital_t *)dato;
	if (!principal->cantidad) {
		printf("\n[ No hay ningun hospital cargado ]\n\n");
		return true;
	}
	printf("\n[ Listado de hospitales cargados ]\n\n");
	hash_con_cada_clave(principal->hash, imprimir_estado_hospitales,
			    (void *)principal);
	printf("\n");
	return true;
}

bool mi_menu_activar(void *dato, void *contexto)
{
	hash_hospital_t *principal = (hash_hospital_t *)dato;
	if (!principal->cantidad) {
		printf("\n[ No hay ningun hospital cargado ]\n\n");
		return true;
	}

	char src[MAXIMO_CARACTERES];
	printf("\n[ Ingrese un numero de identificacion de un hospital. Para cancelar, escriba « N » ]: ");
	scanf("%255s", src);
	char cancelar[] = "N";
	if (strcmp(src, cancelar) == 0) {
		printf("\n");
		return true;
	}

	wrapper_hospital_t *hospital =
		(wrapper_hospital_t *)hash_obtener(principal->hash, src);
	if (!hospital) {
		printf("\n[ No existe un hospital con esa ID  ]\n\n");
		return true;
	}
	principal->hospital_activo = hospital;
	printf("\n[ El hospital de ID: %s se ha activado correctamente ]\n\n",
	       principal->hospital_activo->id);
	return true;
}

bool imprimir_pokemon_simple(pokemon_t *p, void *aux)
{
	printf("• %s\n", pokemon_nombre(p));
	return true;
}

bool mi_menu_mostrar(void *dato, void *contexto)
{
	hash_hospital_t *principal = (hash_hospital_t *)dato;
	if (!principal->hospital_activo) {
		printf("\n[ No hay un hospital activo en este momento ]\n\n");
		return true;
	}
	printf("\n[ Pokemon en el hospital ID: %s ]\n\n",
	       principal->hospital_activo->id);
	hospital_a_cada_pokemon(principal->hospital_activo->hospital,
				imprimir_pokemon_simple, NULL);
	printf("\n");
	return true;
}

bool imprimir_pokemon_detallado(pokemon_t *p, void *aux)
{
	printf("[ %s ]\n", pokemon_nombre(p));

	char id[MAXIMO_CARACTERES];
	sprintf(id, "%zu", pokemon_id(p));
	printf("• ID: %s\n", id);

	char salud[MAXIMO_CARACTERES];
	sprintf(salud, "%zu", pokemon_salud(p));
	printf("• Salud: %s\n\n", salud);
	return true;
}

bool mi_menu_listar(void *dato, void *contexto)
{
	hash_hospital_t *principal = (hash_hospital_t *)dato;
	if (!principal->hospital_activo) {
		printf("\n[ No hay un hospital activo en este momento ]\n\n");
		return true;
	}
	printf("\n[ Listado de informacion de los pokemon en el hospital ID: %s ]\n\n",
	       principal->hospital_activo->id);
	hospital_a_cada_pokemon(principal->hospital_activo->hospital,
				imprimir_pokemon_detallado, NULL);
	return true;
}

bool mi_menu_destruir(void *dato, void *contexto)
{
	hash_hospital_t *principal = (hash_hospital_t *)dato;
	if (!principal->hospital_activo) {
		printf("\n[ No hay un hospital activo en este momento ]\n\n");
		return true;
	}
	wrapper_hospital_t *hospital = (wrapper_hospital_t *)hash_quitar(
		principal->hash, principal->hospital_activo->id);
	printf("\n[ El hospital activo de ID: %s se ha destruido correctamente ]\n\n",
	       hospital->id);
	liberar_memoria_hospital(hospital);
	principal->hospital_activo = NULL;
	principal->cantidad--;
	return true;
}

char *reservar_memoria_string(char *dato)
{
	char *str = calloc(1, sizeof(char) * MAXIMO_CARACTERES);
	strcpy(str, dato);
	return str;
}

void liberar_memoria_string_error(char *tecla, char *descripcion)
{
	free(tecla);
	free(descripcion);
}

menu_t *inicializar_menu(char *nombre_menu)
{
	menu_t *menu = menu_crear(nombre_menu);
	if (!menu)
		return NULL;

	char *tecla_salir = reservar_memoria_string("S");
	char *descripcion_salir =
		reservar_memoria_string("Salir/Exit: Salir del programa.");
	bool no_problema1 = menu_agregar_opcion(
		menu, tecla_salir, descripcion_salir, mi_menu_salir, NULL);
	if (!no_problema1) {
		liberar_memoria_string_error(tecla_salir, descripcion_salir);
		return NULL;
	}

	char *tecla_ayuda = reservar_memoria_string("H");
	char *descripcion_ayuda = reservar_memoria_string(
		"Ayuda/Help: Muestra un menú de ayuda con los comandos permitidos.");
	bool no_problema2 = menu_agregar_opcion(menu, tecla_ayuda,
						descripcion_ayuda,
						mi_menu_ayuda, (void *)menu);
	if (!no_problema2) {
		liberar_memoria_string_error(tecla_ayuda, descripcion_ayuda);
		return NULL;
	}

	char *tecla_cargar = reservar_memoria_string("C");
	char *descripcion_cargar = reservar_memoria_string(
		"Cargar: Pide un nombre de archivo e intenta cargarlo creando un hospital. El hospital queda identificado con un número y el nombre del archivo.");
	bool no_problema3 = menu_agregar_opcion(
		menu, tecla_cargar, descripcion_cargar, mi_menu_cargar, NULL);
	if (!no_problema3) {
		liberar_memoria_string_error(tecla_cargar, descripcion_cargar);
		return NULL;
	}

	char *tecla_estado = reservar_memoria_string("E");
	char *descripcion_estado = reservar_memoria_string(
		"Estado: Muestra un listado con los hospitales cargados (y el activo, si alguno).");
	bool no_problema4 = menu_agregar_opcion(
		menu, tecla_estado, descripcion_estado, mi_menu_estado, NULL);
	if (!no_problema4) {
		liberar_memoria_string_error(tecla_estado, descripcion_estado);
		return NULL;
	}

	char *tecla_activar = reservar_memoria_string("A");
	char *descripcion_activar = reservar_memoria_string(
		"Activar: Pide un número de identificación y activa el hospital. El resto de las operaciones se realizan sobre el hospital activo.");
	bool no_problema5 = menu_agregar_opcion(menu, tecla_activar,
						descripcion_activar,
						mi_menu_activar, NULL);
	if (!no_problema5) {
		liberar_memoria_string_error(tecla_activar,
					     descripcion_activar);
		return NULL;
	}

	char *tecla_mostrar = reservar_memoria_string("M");
	char *descripcion_mostrar = reservar_memoria_string(
		"Mostrar: Muestra un listado con los nombres de todos los pokemones en el hospital.");
	bool no_problema6 = menu_agregar_opcion(menu, tecla_mostrar,
						descripcion_mostrar,
						mi_menu_mostrar, NULL);
	if (!no_problema6) {
		liberar_memoria_string_error(tecla_mostrar,
					     descripcion_mostrar);
		return NULL;
	}

	char *tecla_listar = reservar_memoria_string("L");
	char *descripcion_listar = reservar_memoria_string(
		"Listar: Muestra un listado detallado de todos los pokemones en el hospital.");
	bool no_problema7 = menu_agregar_opcion(
		menu, tecla_listar, descripcion_listar, mi_menu_listar, NULL);
	if (!no_problema7) {
		liberar_memoria_string_error(tecla_listar, descripcion_listar);
		return NULL;
	}

	char *tecla_destruir = reservar_memoria_string("D");
	char *descripcion_destruir = reservar_memoria_string(
		"Destruir: Destruye el hospital activo.");
	bool no_problema8 = menu_agregar_opcion(menu, tecla_destruir,
						descripcion_destruir,
						mi_menu_destruir, NULL);
	if (!no_problema8) {
		liberar_memoria_string_error(tecla_destruir,
					     descripcion_destruir);
		return NULL;
	}
	return menu;
}

void funcion_destructora_hospitales(void *valor)
{
	wrapper_hospital_t *hospital = (wrapper_hospital_t *)valor;
	hospital_destruir(hospital->hospital);
	free(hospital->nombre);
	free(hospital->id);
	free(hospital);
}

void terminar_ejecucion(menu_t *menu, hash_hospital_t *hash_hospital)
{
	if (menu != NULL)
		menu_destruir_con_strings(menu);
	if (hash_hospital != NULL) {
		hash_destruir_todo(hash_hospital->hash,
				   funcion_destructora_hospitales);
		free(hash_hospital);
	}
}

int main(int argc, char *argv[])
{
	system("clear");

	char nombre_menu[] = "Menu - Hospital Pokemon";
	menu_t *menu = inicializar_menu(nombre_menu);
	hash_hospital_t *hash_hospitales = calloc(1, sizeof(hash_hospital_t));
	hash_hospitales->hash = hash_crear(CAPACIDAD_DEFAULT_HASH_HOSPITALES);

	char src_mensaje_menu_principal[] = "mensajes/mensaje_principal.txt";
	char src_mensaje_error[] = "mensajes/mensaje_error.txt";
	char src_mensaje_error_archivo[] = "mensajes/mensaje_error_archivo.txt";
	char src_mensaje_comando_incorrecto[] =
		"mensajes/mensaje_comando_incorrecto.txt";
	char src_mensaje_finalizacion[] = "mensajes/mensaje_finalizacion.txt";

	if (!menu || !hash_hospitales || !hash_hospitales->hash) {
		mostrar_mensaje_determinado(src_mensaje_error);
		terminar_ejecucion(menu, hash_hospitales);
		return 201;
	}

	mostrar_mensaje_determinado(src_mensaje_menu_principal);

	hash_hospitales->id_generador++;
	char tecla_salir[] = "S";
	char tecla_cargar[] = "C";
	bool funcionamiento = true;
	while (funcionamiento) {
		char entrada[MAXIMO_CARACTERES];
		registrar_entrada_usuario(entrada);
		if (!menu_obtener_descripcion(menu, entrada))
			mostrar_mensaje_determinado(
				src_mensaje_comando_incorrecto);
		else
			funcionamiento = menu_ejecutar_opcion(
				menu, entrada, (void *)hash_hospitales);

		if (!funcionamiento && strcmp(tecla_cargar, entrada) == 0) {
			terminar_ejecucion(menu, hash_hospitales);
			mostrar_mensaje_determinado(src_mensaje_error_archivo);
			return 203;
		}
		if (!funcionamiento && strcmp(tecla_salir, entrada) != 0) {
			terminar_ejecucion(menu, hash_hospitales);
			mostrar_mensaje_determinado(src_mensaje_error);
			return 201;
		}
	}
	terminar_ejecucion(menu, hash_hospitales);
	mostrar_mensaje_determinado(src_mensaje_finalizacion);
	return 0;
}