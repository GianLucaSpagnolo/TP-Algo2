<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

## TP2

Ya estamos cerca del final de la materia. Implementaste el TP1 y varios TDAs. Ahora el objetivo es aplicar todo lo aprendido para mejorar tu TP1.


## Desarrollo

- Utilice el `tp1.c` ya implementado en el TP1. Explique cómo se podría reemplazar el vector dinámico de pokemones en la estructura `hospital_t` por algún otro TDA visto en la materia. ¿Que TDA conviene utilizar? ¿Que problema habría la reemplazar dicha estructura? ¿Habría algún problema con las pruebas? Justificar la respuesta. Selecciones por lo menos 2 TDA de los vistos en la materia y arme una tabla con la complejidad de cada una de las operaciones de `tp1.h` si se utilizara dicho TDA. (Por ejemplo, si usaramos una lista enlazada, la complejidad de crear el hospital es tanto por tal cosa, de recorrer el hospital es tal otra complejidad, etc).

- Cree una aplicación (`tp2.c`) que haga uso de menúes para la interacción con el usuario. Se deja a cargo del alumno la presentación visual del trabajo, pero se deben incorporar las siguientes opciones:
  - S (salir/exit): Sale del programa (obviamente que libera toda la memoria).
  - H (ayuda/help): Muestra un menú de ayuda con los comandos disponibles.
  - C (cargar): Pide un nombre de archivo e intenta cargarlo creando un hospital. El hospital queda identificado con un número y el nombre del archivo.
  - E (estado): Muestra un listado con los hospitales cargados (y el activo, si alguno).
  - A (activar): Pide un número de identificación y activa el hospital. El resto de las operaciones se realizan sobre el hospital activo.
  - M (mostrar): Muestra un listado con los nombres de todos los pokemones en el hospital.
  - L (listar): Muestra un listado detallado de todos los pokemones en el hospital.
  - D (destruir): Destruye el hospital activo.

- Proponga uno o mas `TDAs` que faciliten la creación de los menúes interactivos por consola (por ejemplo, `TDA Menu` o `TDA Opcion`). Estos `TDA` pueden hacer uso de los `TDAs` implementados en la materia y no tienen permitido usar vectores dinámicos o estáticos (si se permiten strings, obvio). Defina la interfaz y explique las operaciones del `TDA`.

- Incluya pruebas para el `TDA` creado.

- Tenga en cuenta que el usuario no tiene por qué saber cómo manejar su aplicación ni saber qué está sucediendo. Muestre por pantalla la información apropiada para guiar al usuario sin ser molesto.

- Incluya en el informe todas las suposiciones que se hayan tomado (cosas que no queden claras en el enunciado por ejemplo) y diagramas de memoria explicando cómo se almacenan los datos.
