# Informe de cambios — El Gran Torneo Pokémon

**Proyecto:** `proyecto-pokemon` (Lenguaje C, Programación I)
**Fecha del informe:** 20/10/2026
**Estado:** compila sin errores ni avisos (`gcc -Wall -Wextra pokemon.c funciones.c`)

Este documento resume todos los cambios realizados sobre el proyecto, agrupados por área. Al final hay una sección de verificación con las pruebas ejecutadas.

---

## 1. Presentación: banner con colores

### 1.1 Banner principal
Se rediseñó `imprimir_banner()` en [funciones.c](funciones.c):

- Banner **solo texto** (sin arte ASCII de Pokebola).
- Título `EL GRAN TORNEO POKEMON` en **amarillo**, con letras simples y centrado.
- Subtítulo en una **cajita centrada** con `+---+` y el texto en amarillo.
- Marco superior e inferior en **azul** (`\033[1;34m`).

### 1.2 Menú principal
El menú ahora usa `imprimir_opcion_menu()` para alinear todas las opciones con el marco del banner: número en amarillo y descripción en blanco. Además muestra un aviso cuando no hay entrenadores cargados.

### 1.3 Separador
`imprimir_separador()` dibuja una línea azul del mismo ancho que el interior del banner.

---

## 2. Pokédex con colores

Se agregaron funciones de presentación para la Pokédex:

| Función | Descripción |
|---|---|
| `color_tipo()` | Devuelve el color ANSI de cada tipo elemental (Fuego=rojo, Agua=azul, Planta=verde, Eléctrico=amarillo, etc.). |
| `imprimir_tipo_coloreado()` | Imprime el tipo con su color. |
| `imprimir_mensaje()` | Mensaje uniforme con marco, icono, título y color (para errores, avisos y éxitos). |

### 2.1 Tabla completa (`imprimir_vector_pokemon`)
- Encabezado verde con título `POKEDEX - PRIMERA GENERACION`.
- Columnas: `ID | NOMBRE | TIPO 1 | TIPO 2 | HP | ATQ | DEF | VEL`.
- **Tipos coloreados** según su elemento.
- Estadísticas con colores: HP verde, Ataque rojo, Defensa azul, Velocidad cian.
- Pie con el total de especies y la leyenda de las siglas.

### 2.2 Ficha de especie (`mostrar_estadisticas_pokemon`)
- Encabezado con el título de la ficha.
- Nombre en amarillo, número `#NNN` debajo.
- Tipos pintados con sus colores.
- Estadísticas base en dos columnas y **total base** sumado al final.

### 2.3 Búsquedas y menú
- `consultar_pokedex_id` y `consultar_pokedex_nombre` ahora muestran el banner y un ejemplo, y usan `imprimir_mensaje()` para el error "no encontrado".
- `menu_consultar_pokedex` muestra la cantidad de especies cargadas y las opciones alineadas.

---

## 3. Estructura y reglas del proyecto

### 3.1 Prototipos en el header
Se movieron **todos** los prototipos a [funciones.h](funciones.h), incluidos los de las funciones auxiliares internas. Se agregó la sección `MODULO 7: FUNCIONES AUXILIARES INTERNAS` con documentación Doxygen completa.

Funciones auxiliares ahora declaradas en el header:
`generar_equipo_entrenador`, `ejecutar_turno_ataque`, `seleccionar_siguiente`, `validar_participantes`, `buscar_entrenador_en_grupo`, `comparar_tabla_grupo`, `mostrar_etiqueta_clasificado`, `mostrar_etiqueta_cruce`, `es_id_entrenador`, `buscar_nombre_entrenador`, `color_tipo`, `imprimir_tipo_coloreado`, `imprimir_mensaje`.

### 3.2 Motor de combate (de tandas anteriores)
- `disputar_combate`: combate por **rondas** (más veloz ataca primero, moneda si empatan), con contraataque.
- `curar_equipo`: restaura HP y estado al inicio de cada combate.
- `seleccionar_pokemon` y `seleccionar_siguiente`: selección automática del primer Pokémon disponible (un solo `return`).
- Empates: en fase de grupos valen 1 punto; en eliminatoria se desempata por **HP total restante**.
- `pokemon_derrotados`: contador para el 3.er criterio de desempate.

### 3.3 Torneo (de tandas anteriores)
- Tipo `Partido` + funciones de persistencia (`inicializar_partidos`, `cargar_partidos`, `guardar_partidos`, `buscar_partido`, `registrar_partido`, `obtener_participantes`).
- `fase_de_grupos`: 8 grupos × 4 entrenadores, round-robin (48 combates).
- `calcular_clasificacion`: ordena con `qsort` por puntos → victorias → Pokémon derrotados → nombre.
- `fase_eliminatoria`: octavos, cuartos, semifinal, 3.er lugar y final (combates 49–64).
- `cargar_resultados_archivo`: carga resultados desde un archivo de texto, validando línea por línea.

### 3.4 Menús nuevos (de tandas anteriores)
- **Crear equipo** ahora es submenú: crear todos los equipos de una vez (`crear_equipos_todos`) o seleccionar manualmente.
- **Cargar resultados** ahora es submenú: por teclado o desde archivo.
- **Lectura segura**: `leer_entero` y `leer_texto` con `fgets` (evitan bucles infinitos con entradas inválidas).

---

## 4. Archivos de datos

### 4.1 `pokedex.txt`
- Se limpió a **150 especies** (se eliminó el registro de prueba `152 pedro` y se renumeró el final).
- IDs únicos del `#001` al `#150`, sin líneas mal formadas.

### 4.2 `entrenadores.txt`
- Se migró al formato de **9 campos** (se agregó `pokemon_derrotados`).
- Actualmente los **32 entrenadores tienen equipo** (`true`), resultado de la corrida de "crear todos los equipos".

### 4.3 `equipos_pokemon.txt`
- **32 equipos** (224 líneas: encabezado + 6 Pokémon por entrenador), generados con el algoritmo de backtracking.

### 4.4 `resultados_combates.txt`
- Formato nuevo con encabezado y un combate registrado de prueba.

---

## 5. Verificación

Pruebas ejecutadas y verificadas:

| Prueba | Resultado |
|---|---|
| Compilación `gcc -Wall -Wextra` | ✅ sin errores ni avisos |
| Banner principal (título + subtítulo) | ✅ alineado y con colores |
| Ficha de Pikachu (#025) | ✅ tipos y estadísticas correctos |
| Tabla completa de la Pokédex (150 especies) | ✅ con tipos coloreados |
| Creación masiva de 32 equipos | ✅ 224 líneas, 0 duplicados |
| Segunda corrida de creación masiva | ✅ no duplica, avisa que ya existen |
| Fase de grupos completa | ✅ 48/48 combates |
| Fase eliminatoria | ✅ 16/16 combates (49–64) |
| Campeón | ✅ se muestra correctamente |
| Carga de resultados desde archivo | ✅ válidos registrados, inválidos rechazados con motivo |
| Persistencia (reabrir y recuperar resultados) | ✅ |

---

## 6. Notas para el usuario

1. **Recompilar**: el `torneo.exe` del repo fue recompilado, pero si haces cambios hay que volver a compilar con `gcc -Wall -Wextra pokemon.c funciones.c`.
2. **Datos de prueba**: la corrida de "crear todos los equipos" dejó los 32 entrenadores con equipo en los archivos del repo. Si quieres empezar el torneo desde cero, vacía `resultados_combates.txt` y pon los contadores de `entrenadores.txt` en 0.
3. **`dxdiag-report.txt`**: sigue siendo un archivo de tu PC (no del proyecto) que está sin rastrear. Conviene no subirlo al repositorio.
