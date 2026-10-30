/**
 * @file archivos.h
 * @brief Localizacion portable de los archivos de datos del torneo.
 *
 * El programa puede ejecutarse desde la carpeta del proyecto, desde bin/ o con
 * doble clic en Windows. Esta libreria busca los datos en:
 *   1. ./datos/
 *   2. La carpeta datos/ junto al ejecutable.
 *   3. La carpeta datos/ un nivel arriba del ejecutable (bin/../datos).
 *   4. La carpeta actual (compatibilidad con la estructura anterior).
 *
 * Al escribir, elige la primera carpeta de datos que exista y, si no hay
 * ninguna, crea ./datos.
 */
#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

#define ARCHIVOS_RUTA_MAX 1024

/**
 * @brief Abre un archivo de datos buscandolo en las carpetas conocidas.
 * @param nombre Nombre del archivo, por ejemplo "pokedex.txt". Si incluye una
 *               ruta con / o \, se usa tal cual.
 * @param modo Modo de fopen ("r", "w", "a", "a+", ...).
 * @return FILE* Archivo abierto o NULL si no se pudo abrir.
 */
FILE *archivos_abrir(const char *nombre, const char *modo);

/**
 * @brief Busca un archivo de datos para lectura.
 * @param nombre Nombre del archivo.
 * @param destino Buffer donde se guarda la ruta encontrada.
 * @param tamano Capacidad del buffer.
 * @return true si el archivo existe y la ruta se guardo.
 */
bool archivos_ruta_datos(const char *nombre, char *destino, size_t tamano);

/**
 * @brief Informa por pantalla que no se pudo abrir un archivo de datos.
 * @param nombre Nombre del archivo que fallo.
 */
void archivos_error(const char *nombre);

#endif /* ARCHIVOS_H */
