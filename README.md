# El Gran Torneo Pokémon

Sistema en **lenguaje C** que administra el **Gran Torneo Pokémon** de la Primera Generación: Pokédex, entrenadores, equipos, fase de grupos, fase eliminatoria, clasificación y campeón.

> Proyecto de Programación I — Universidad de Carabobo (Facultad Experimental de Ciencias y Tecnología).

---

## Índice

1. [Descripción](#descripción)
2. [Requisitos](#requisitos)
3. [Compilación y ejecución](#compilación-y-ejecución)
4. [Menú principal](#menú-principal)
5. [Reglas del torneo](#reglas-del-torneo)
6. [Fórmulas usadas](#fórmulas-usadas)
7. [Archivos de datos](#archivos-de-datos)
8. [Estructura del código](#estructura-del-código)
9. [Ejemplo de uso rápido](#ejemplo-de-uso-rápido)

---

## Descripción

El sistema permite:

- Consultar la **Pokédex** (150 especies de la Primera Generación).
- Registrar **entrenadores** (máximo 32).
- Crear **equipos** de 6 Pokémon, de uno en uno o **todos de una vez**, usando un algoritmo de **backtracking** que garantiza al menos 4 tipos elementales distintos y un nivel total máximo de 300.
- Disputar la **fase de grupos** (8 grupos de 4 entrenadores, *round-robin*).
- Calcular la **clasificación** con criterios de desempate.
- Disputar la **fase eliminatoria** (octavos, cuartos, semifinal, tercer lugar y final).
- Cargar resultados **por teclado o desde un archivo**, con validación.
- Mostrar el **historial de combates**, los **resultados** y al **campeón**.

---

## Requisitos

- Compilador de C: **gcc** (MinGW en Windows) o cualquier GCC/Clang moderno.
- **GNU Make**: `make` en Linux/macOS o `mingw32-make` en Windows.
- Terminal con UTF-8. En Windows 10/11 la consola se configura automáticamente;
  si el terminal no soporta ANSI, el programa sigue funcionando en blanco y negro.

---

## Compilación y ejecución

### Windows (MinGW)

```bat
mingw32-make
mingw32-make run
```

### Linux / macOS

```bash
make
make run
```

El ejecutable queda en `bin/torneo.exe` (Windows) o `bin/torneo` (Linux/macOS).
Los objetos intermedios van a `obj/`. Para limpiar:

```bash
mingw32-make clean     # Windows
make clean             # Linux/macOS
```

### ¿Dónde están los datos?

Todos los archivos de datos viven en **`datos/`**. El programa los encuentra
aunque se ejecute desde la raíz del proyecto, desde `bin/` o con doble clic en
Windows: `src/archivos.c` prueba `./datos/`, la carpeta del ejecutable y
`../datos/`, y mantiene compatibilidad con la estructura anterior (archivos en
la raíz).

### Pantalla de bienvenida

Al iniciar se muestra una imagen pixelada con el texto **TORNEO POKEMON** y
**PRESIONA ENTER**. La imagen se construye en una matriz de 78x44 píxeles
(`include/pixel_art.h` / `src/pixel_art.c`) y se dibuja usando bloques de color
ANSI; luego aparece el menú principal.

![Vista previa de la pantalla de bienvenida](docs/pantalla_bienvenida.png)

---

## Menú principal

El menú se dibuja como una tabla de dos columnas: número y módulo, con una
división horizontal por cada opción y el título dentro del mismo cuadro. Las
líneas son continuas y se limpia la pantalla al cambiar de módulo.

```
 1. Consultar Pokedex
 2. Registrar entrenador
 3. Crear equipo
 4. Consultar entrenadores
 5. Consultar equipos
 6. Cargar resultados
 7. Consultar clasificacion
 8. Consultar enfrentamientos
 9. Consultar historial de combates
10. Mostrar resultados del torneo
11. Mostrar campeon
12. Recargar base de datos
13. Disputar fase de grupos
14. Disputar fase eliminatoria
15. Salir
```

### Detalle de cada opción

| Opción | Descripción |
|---|---|
| **1** | Submenú de la Pokédex: mostrar completa, buscar por número o por nombre. |
| **2** | Registra un entrenador nuevo (valida ID único, máximo 32). |
| **3** | Submenú de equipos: crear **todos los que faltan** de una vez o elegir uno manualmente. |
| **4** | Lista entrenadores y muestra la tarjeta del que se indique por ID. |
| **5** | Muestra el equipo del entrenador indicado. |
| **6** | Submenú: cargar un resultado por teclado o varios desde un archivo de texto. |
| **7** | Clasificación de los 8 grupos (puntos, victorias, derrotas, Pokémon derrotados). |
| **8** | Cuadro de enfrentamientos de la fase eliminatoria. |
| **9** | Historial detallado de combates (`historial_detallado.txt`). |
| **10** | Todos los resultados del torneo agrupados por etapa. |
| **11** | Campeón, subcampeón, tercer y cuarto lugar. |
| **12** | Recarga los entrenadores y equipos desde disco. |
| **13** | Disputa la fase de grupos completa (48 combates). |
| **14** | Disputa la fase eliminatoria (16 combates). |

---

## Reglas del torneo

### Fase de grupos

- **32 entrenadores**, divididos en **8 grupos** de 4 (A a H), asignados por orden de carga.
- Dentro de cada grupo se juega *round-robin*: cada entrenador se enfrenta una vez contra los otros tres. Son **6 combates por grupo**, 48 en total.
- Numeración: Grupo A = combates 1-6, Grupo B = 7-12, ..., Grupo H = 43-48.

**Puntuación:**

| Resultado | Puntos |
|---|---|
| Victoria | 3 |
| Empate | 1 |
| Derrota | 0 |

**Criterios de desempate** (en este orden):

1. Mayor cantidad de puntos.
2. Mayor cantidad de victorias.
3. Mayor cantidad de Pokémon derrotados.
4. (Desempate adicional) Orden alfabético por nombre.

Clasifican los **2 primeros de cada grupo**: 1A, 2A, 1B, 2B, ..., 1H, 2H (16 clasificados).

### Fase eliminatoria

16 combates según el cuadro oficial:

| Ronda | Combates |
|---|---|
| Octavos de final | 49-56 |
| Cuartos de final | 57-60 |
| Semifinal | 61-62 |
| Tercer lugar | 63 |
| Final | 64 |

Resultados finales:

- **Campeón** = ganador del combate 64.
- **Subcampeón** = perdedor del combate 64.
- **Tercer lugar** = ganador del combate 63.
- **Cuarto lugar** = perdedor del combate 63.

En esta fase **no hay empates**: si un combate agota las 100 rondas sin K.O., gana el equipo con más **HP total restante**.

---

## Fórmulas usadas

### Estadísticas de un ejemplar (nivel 50)

Al crear un equipo, los ejemplares se generan a partir de las estadísticas base de su especie:

```
HP        = (HP_base * nivel / 50) + nivel + 10
Ataque    = (Ataque_base * nivel / 50) + 5
Defensa   = (Defensa_base * nivel / 50) + 5
Velocidad = (Velocidad_base * nivel / 50) + 5
```

### Daño en combate

```
Daño = (((2 * nivel / 5 + 2) * PODER_ATAQUE * (ataque / defensa)) / 50 + 2) * multiplicador_tipos
```

- `PODER_ATAQUE` = 50 (fijo).
- El `multiplicador_tipos` sale de la **tabla de efectividad 18×18** (tipos atacante vs. tipos defensores), multiplicando por cada tipo del defensor (1.0 normal, 2.0 muy efectivo, 0.5 poco efectivo, 0.0 sin efecto).
- Un ataque que no sea "sin efecto" siempre hace mínimo **1 de daño**.

### Regla de combate

- En cada ronda, ataca primero el Pokémon con **mayor velocidad**. Si empatan, se lanza una **moneda**.
- Si el defensor sobrevive, **contraataca**.
- Cuando un Pokémon llega a 0 de HP, entra el siguiente disponible del equipo.
- Si se agotan **100 rondas** sin K.O., el combate es **empate** (en grupos) o se desempata por HP restante (en eliminatoria).

---

## Archivos de datos

Todos los archivos se leen y guardan en la carpeta **`datos/`**.

### `pokedex.txt`

Una especie por línea, con 7 u 8 campos:

```
001 Bulbasaur Planta Veneno 45 49 49 45
004 Charmander Fuego 39 52 43 65
```

| Campo | Descripción |
|---|---|
| 1 | Número de la Pokédex (001-150) |
| 2 | Nombre |
| 3 | Tipo primario |
| 4 | Tipo secundario (se omite si tiene un solo tipo) |
| 5-8 | HP base, Ataque base, Defensa base, Velocidad base |

### `entrenadores.txt`

Un entrenador por línea, 9 campos:

```
001 Saul 6 0 0 0 0 0 true
```

| Campo | Descripción |
|---|---|
| 1 | ID (único) |
| 2 | Nombre |
| 3 | Cantidad de Pokémon (informativo) |
| 4-6 | Victorias, empates, derrotas |
| 7 | Puntuación |
| 8 | Pokémon derrotados (criterio de desempate) |
| 9 | Tiene equipo (`true` / `false`) |

> El lector también acepta el formato viejo de 8 campos (sin el campo 8), para no romper archivos antiguos.

### `equipos_pokemon.txt`

Encabezado con el ID y nombre del entrenador, seguido de 6 líneas (una por Pokémon):

```
001 Saul
PKM1 Clefable 50 155 155 75 78 65 0 1
PKM2 Poliwrath 50 150 150 90 100 75 0 1
...
```

| Campo del Pokémon | Descripción |
|---|---|
| 1 | ID del ejemplar (PKM1-PKM6) |
| 2 | Nombre de la especie |
| 3 | Nivel |
| 4-5 | HP actual, HP máximo |
| 6-8 | Ataque, defensa, velocidad |
| 9 | Experiencia |
| 10 | Estado (1 = disponible, 0 = derrotado) |

### `resultados_combates.txt`

Resultados oficiales del torneo. Lo reescribe el programa al guardar:

```
# Numero Etapa Entrenador1 Entrenador2 Resultado Ganador
1 Fase_de_Grupos 001 004 Victoria 001
26 Fase_de_Grupos 018 019 Empate -
```

### `historial_detallado.txt`

Historial de combates con el detalle ronda a ronda (lo genera el motor de combate).

---

## Estructura del código

| Archivo | Contenido |
|---|---|
| `Makefile` | Compilación y ejecución multiplataforma (Windows/Linux/macOS). |
| `src/main.c` | `main()`: bienvenida, menú principal y flujo del programa. |
| `include/funciones.h` | Estructuras, constantes y prototipos (con documentación Doxygen). |
| `src/funciones.c` | Pokédex, entrenadores, equipos/backtracking, combate y torneo. |
| `include/consola.h`, `src/consola.c` | Salida portable: ANSI en Windows, UTF-8, limpiar pantalla y pausas. |
| `include/archivos.h`, `src/archivos.c` | Búsqueda de los datos en `datos/` sin depender del directorio actual. |
| `include/pixel_art.h`, `src/pixel_art.c` | Matriz de píxeles, fuente 3x5, Pokebola y pantalla de bienvenida. |
| `datos/` | `pokedex.txt`, `entrenadores.txt`, `equipos_pokemon.txt`, resultados e historial. |
| `bin/` | Ejecutable generado por `make`. |
| `obj/` | Archivos objeto generados por `make`. |
| `docs/` | Vista previa de la pantalla de bienvenida. |

### Principales estructuras

- `especies_pokemons`: datos base de una especie de la Pokédex.
- `EjemplarPokemon`: Pokémon físico de un entrenador (stats calculados, HP, estado).
- `Entrenador`: participante con su equipo y estadísticas del torneo.
- `Partido`: un combate del torneo (número, etapa, participantes, resultado).
- `TablaGrupo`: fila de la clasificación de un grupo.

### Funciones destacadas

- **Combate**: `disputar_combate`, `calcular_dano`, `calcular_multiplicador_tipos`, `curar_equipo`, `seleccionar_pokemon`.
- **Equipos**: `crear_equipo_pokemon` (backtracking con returns tempranos), `generar_equipo_entrenador`, `crear_equipos_todos`, `validar_variedad_tipos`.
- **Torneo**: `fase_de_grupos`, `fase_eliminatoria`, `calcular_clasificacion`, `obtener_participantes`, `registrar_partido`.
- **Presentación**: `imprimir_banner`, `imprimir_opcion_menu`, `color_tipo`, `imprimir_mensaje`, `limpiar_consola`, `consola_printf`, `pantalla_bienvenida`.

---

## Ejemplo de uso rápido

1. Registra (o verifica) los 32 entrenadores en `datos/entrenadores.txt`.
2. Opción **3 → 1**: crea los equipos de todos los que falten de una vez.
3. Opción **13**: disputa la fase de grupos (48 combates).
4. Opción **7**: revisa la clasificación.
5. Opción **14**: disputa la fase eliminatoria (16 combates).
6. Opción **11**: mira al campeón.

También puedes cargar resultados a mano con la opción **6**, escribiendo el número de combate y el ID del ganador, o desde un archivo con el formato `numero_combate id_ganador`.

---

## Correcciones recientes

- **Entrenadores que no aparecían**: el archivo usaba el formato viejo de 8
  campos y el lector intentaba convertir `true` como entero con `sscanf`; eso
  devolvía 7 conversiones y descartaba todas las líneas. Ahora se leen los
  campos como palabras y se aceptan los formatos de 8 y 9 campos.
- **Backtracking**: `crear_equipo_pokemon` y `validar_variedad_tipos` usan
  `return` tempranos en lugar de banderas y condiciones compuestas en los
  ciclos. El comportamiento del algoritmo es el mismo.
- **Consola portable**: nueva librería `consola` que activa ANSI/UTF-8 en
  Windows, limpia la pantalla sin depender de `cls`/`clear` y filtra los
  colores cuando la salida no es un terminal.
- **Rutas portables**: nueva librería `archivos` que localiza `datos/` desde la
  raíz, desde `bin/` o con doble clic.
- **Pantalla pixelada** antes del menú y **Makefile** multiplataforma.
- **Menú principal tipo tabla**: las opciones están dentro de un cuadro con
  divisiones por módulo, líneas continuas y columnas para el número y el nombre.
  La pantalla se limpia al pasar de la bienvenida al menú y al cambiar de
  módulo, pero se conserva el historial necesario dentro de cada operación.

---

## Documentación

- [INFORME_CAMBIOS.md](INFORME_CAMBIOS.md): resumen de todos los cambios realizados y su verificación.
- [Proyecto- 2026.pdf](Proyecto-%202026.pdf): enunciado original del proyecto.
