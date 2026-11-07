/**
 * @file archivos.c
 * @brief Busqueda y apertura portable de los archivos de datos.
 */
#include "archivos.h"
#include "consola.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdint.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    #include <direct.h>
#else
    #include <unistd.h>
#endif

#if defined(__APPLE__)
    #include <mach-o/dyld.h>
#endif

static char directorio_ejecutable[ARCHIVOS_RUTA_MAX] = ".";
static bool directorio_calculado = false;

/** @brief Quita el nombre del archivo a una ruta, dejando la carpeta. */
static void quitar_nombre_archivo(char *ruta)
{
    char *ultima_barra = strrchr(ruta, '/');
    char *ultima_barra_inversa = strrchr(ruta, '\\');
    char *corte = ultima_barra;

    if (ultima_barra_inversa != NULL &&
        (corte == NULL || ultima_barra_inversa > corte)) {
        corte = ultima_barra_inversa;
    }

    if (corte != NULL) {
        if (corte == ruta) {
            corte[1] = '\0';
        } else {
            *corte = '\0';
        }
    }
}

/** @brief Obtiene una sola vez la carpeta donde vive el ejecutable. */
static void calcular_directorio_ejecutable(void)
{
#if defined(_WIN32) || defined(_WIN64)
    char ruta[ARCHIVOS_RUTA_MAX];
    DWORD leidos;
#elif defined(__linux__)
    char ruta[ARCHIVOS_RUTA_MAX];
    ssize_t leidos;
#elif defined(__APPLE__)
    char ruta[ARCHIVOS_RUTA_MAX];
    uint32_t tamano;
#endif

    if (!directorio_calculado) {
        directorio_calculado = true;

#if defined(_WIN32) || defined(_WIN64)
        leidos = GetModuleFileNameA(NULL, ruta, (DWORD)sizeof(ruta));

        if (leidos > 0 && leidos < (DWORD)sizeof(ruta)) {
            ruta[leidos] = '\0';
            quitar_nombre_archivo(ruta);
            if (ruta[0] != '\0') {
                snprintf(directorio_ejecutable, sizeof(directorio_ejecutable),
                         "%s", ruta);
            }
        }
#elif defined(__linux__)
        leidos = readlink("/proc/self/exe", ruta, sizeof(ruta) - 1);

        if (leidos > 0) {
            ruta[leidos] = '\0';
            quitar_nombre_archivo(ruta);
            if (ruta[0] != '\0') {
                snprintf(directorio_ejecutable, sizeof(directorio_ejecutable),
                         "%s", ruta);
            }
        }
#elif defined(__APPLE__)
        tamano = (uint32_t)sizeof(ruta);

        if (_NSGetExecutablePath(ruta, &tamano) == 0) {
            quitar_nombre_archivo(ruta);
            if (ruta[0] != '\0') {
                snprintf(directorio_ejecutable, sizeof(directorio_ejecutable),
                         "%s", ruta);
            }
        }
#endif
    }
}

/** @brief Indica si una ruta es una carpeta existente. */
static bool existe_directorio(const char *ruta)
{
    bool existe;

#if defined(_WIN32) || defined(_WIN64)
    DWORD atributos = GetFileAttributesA(ruta);

    existe = atributos != INVALID_FILE_ATTRIBUTES &&
             (atributos & FILE_ATTRIBUTE_DIRECTORY) != 0;
#else
    struct stat informacion;

    existe = stat(ruta, &informacion) == 0 && S_ISDIR(informacion.st_mode);
#endif

    return existe;
}

/** @brief Indica si un archivo se puede abrir para lectura. */
static bool existe_archivo(const char *ruta)
{
    FILE *archivo;
    bool existe = false;

    archivo = fopen(ruta, "rb");

    if (archivo == NULL) {
        existe = false;
    } else {
        fclose(archivo);
        existe = true;
    }

    return existe;
}

/** @brief Crea una carpeta (si ya existe no es un error). */
static bool crear_directorio(const char *ruta)
{
    bool creado;

#if defined(_WIN32) || defined(_WIN64)
    int resultado = _mkdir(ruta);
#else
    int resultado = mkdir(ruta, 0755);
#endif

    creado = (resultado == 0) || (errno == EEXIST);

    return creado;
}

/** @brief Une una carpeta y un nombre de archivo en un buffer. */
static void unir_ruta(char *destino, size_t tamano,
                      const char *carpeta, const char *nombre)
{
    if (carpeta == NULL || carpeta[0] == '\0') {
        snprintf(destino, tamano, "%s", nombre);
    } else {
        snprintf(destino, tamano, "%s/%s", carpeta, nombre);
    }
}

/** @brief Prueba una ruta de lectura; si existe la devuelve. */
static bool probar_lectura(const char *carpeta, const char *nombre,
                           char *destino, size_t tamano)
{
    char ruta[ARCHIVOS_RUTA_MAX];
    bool encontrada = false;

    unir_ruta(ruta, sizeof(ruta), carpeta, nombre);
    if (existe_archivo(ruta)) {
        snprintf(destino, tamano, "%s", ruta);
        encontrada = true;
    }

    return encontrada;
}

bool archivos_ruta_datos(const char *nombre, char *destino, size_t tamano)
{
    char carpeta[ARCHIVOS_RUTA_MAX];
    bool encontrada = false;

    calcular_directorio_ejecutable();

    if (nombre == NULL || destino == NULL || tamano == 0) {
        encontrada = false;
    } else {
        /* Ruta por defecto: util para mostrar un mensaje de error entendible. */
        snprintf(destino, tamano, "./datos/%s", nombre);

        if (probar_lectura("./datos", nombre, destino, tamano) ||
            probar_lectura(".", nombre, destino, tamano) ||
            probar_lectura(directorio_ejecutable, nombre, destino, tamano)) {
            encontrada = true;
        } else {
            snprintf(carpeta, sizeof(carpeta), "%s/../datos", directorio_ejecutable);
            if (probar_lectura(carpeta, nombre, destino, tamano)) {
                encontrada = true;
            } else {
                snprintf(carpeta, sizeof(carpeta), "%s/datos", directorio_ejecutable);
                if (probar_lectura(carpeta, nombre, destino, tamano)) {
                    encontrada = true;
                } else {
                    snprintf(carpeta, sizeof(carpeta), "%s/..", directorio_ejecutable);
                    if (probar_lectura(carpeta, nombre, destino, tamano)) {
                        encontrada = true;
                    }
                }
            }
        }
    }

    return encontrada;
}

FILE *archivos_abrir(const char *nombre, const char *modo)
{
    char ruta[ARCHIVOS_RUTA_MAX];
    char carpetas[5][ARCHIVOS_RUTA_MAX];
    int cantidad = 0;
    int i;
    FILE *archivo = NULL;
    FILE *resultado = NULL;

    if (nombre == NULL || modo == NULL) {
        resultado = NULL;
    } else if (strchr(nombre, '/') != NULL || strchr(nombre, '\\') != NULL) {
        /* Una ruta explicita (por ejemplo, la que escribe el usuario) se respeta. */
        resultado = fopen(nombre, modo);
    } else if (modo[0] == 'r') {
        if (archivos_ruta_datos(nombre, ruta, sizeof(ruta))) {
            resultado = fopen(ruta, modo);
        }
    } else {
        /* Escritura: elegir la primera carpeta de datos que exista. */
        calcular_directorio_ejecutable();

        snprintf(carpetas[cantidad++], ARCHIVOS_RUTA_MAX, "./datos");
        snprintf(carpetas[cantidad++], ARCHIVOS_RUTA_MAX, "%s/../datos",
                 directorio_ejecutable);
        snprintf(carpetas[cantidad++], ARCHIVOS_RUTA_MAX, "%s/datos",
                 directorio_ejecutable);
        snprintf(carpetas[cantidad++], ARCHIVOS_RUTA_MAX, ".");
        snprintf(carpetas[cantidad++], ARCHIVOS_RUTA_MAX, "%s",
                 directorio_ejecutable);

        /* Si el archivo ya existe, se reescribe en la misma carpeta de donde
         * se leyo, para no separar los datos entre la raiz y datos/. */
        for (i = 0; i < cantidad && resultado == NULL; i++) {
            if (existe_directorio(carpetas[i])) {
                unir_ruta(ruta, sizeof(ruta), carpetas[i], nombre);
                if (existe_archivo(ruta)) {
                    archivo = fopen(ruta, modo);
                    if (archivo != NULL) {
                        resultado = archivo;
                    }
                }
            }
        }

        for (i = 0; i < cantidad && resultado == NULL; i++) {
            if (existe_directorio(carpetas[i])) {
                unir_ruta(ruta, sizeof(ruta), carpetas[i], nombre);
                archivo = fopen(ruta, modo);
                if (archivo != NULL) {
                    resultado = archivo;
                }
            }
        }

        /* Ninguna existia: crear ./datos y reintentar. */
        if (resultado == NULL && crear_directorio("./datos")) {
            unir_ruta(ruta, sizeof(ruta), "./datos", nombre);
            resultado = fopen(ruta, modo);
        }
    }

    return resultado;
}

void archivos_error(const char *nombre)
{
    char ruta[ARCHIVOS_RUTA_MAX];

    if (nombre == NULL) {
        nombre = "(desconocido)";
    }

    if (!archivos_ruta_datos(nombre, ruta, sizeof(ruta))) {
        snprintf(ruta, sizeof(ruta), "./datos/%s", nombre);
    }

    consola_printf("\n\033[1;31m[!] ERROR DE ARCHIVO:\033[0m no se pudo abrir\n");
    consola_printf("    %s\n", ruta);
    consola_printf("    Verifique que la carpeta 'datos' exista y tenga sus archivos.\n\n");
}
