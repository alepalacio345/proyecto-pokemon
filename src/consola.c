/**
 * @file consola.c
 * @brief Implementacion portable de la salida por consola.
 */
#include "consola.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    #include <io.h>
    #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
        #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
    #endif
#else
    #include <unistd.h>
#endif

static bool consola_preparada = false;
static bool colores_ansi = false;
static bool salida_utf8 = false;

/** @brief Detecta si stdout esta conectado a una consola real. */
static bool detectar_terminal(void)
{
#if defined(_WIN32) || defined(_WIN64)
    HANDLE salida = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo = 0;

    return (salida != INVALID_HANDLE_VALUE) && GetConsoleMode(salida, &modo);
#else
    return isatty(STDOUT_FILENO) == 1;
#endif
}

#if !defined(_WIN32) && !defined(_WIN64)
/**
 * @brief Comprueba si alguna variable de idioma declara UTF-8.
 *
 * Si no hay variables definidas se asume UTF-8, que es el valor habitual en
 * las distribuciones Linux y en macOS actuales.
 */
static bool idioma_es_utf8(void)
{
    static const char *variables[] = {"LC_ALL", "LC_CTYPE", "LANG"};
    const char *valor;
    bool resultado = true;
    size_t i;

    for (i = 0; i < sizeof(variables) / sizeof(variables[0]); i++) {
        valor = getenv(variables[i]);

        if (valor != NULL && valor[0] != '\0') {
            if (strstr(valor, "UTF-8") != NULL || strstr(valor, "utf-8") != NULL ||
                strstr(valor, "UTF8") != NULL || strstr(valor, "utf8") != NULL) {
                resultado = true;
                break;
            }

            if (strcmp(valor, "C") == 0 || strcmp(valor, "POSIX") == 0) {
                resultado = false;
                break;
            }
        }
    }

    return resultado;
}
#endif

bool consola_iniciar(void)
{
#if defined(_WIN32) || defined(_WIN64)
    HANDLE salida;
    DWORD modo;
    DWORD nuevo_modo;
#else
    const char *termino;
#endif

    if (!consola_preparada) {
        consola_preparada = true;

#if defined(_WIN32) || defined(_WIN64)
        salida = GetStdHandle(STD_OUTPUT_HANDLE);
        modo = 0;

        if (salida != INVALID_HANDLE_VALUE && GetConsoleMode(salida, &modo)) {
            nuevo_modo = modo | ENABLE_PROCESSED_OUTPUT |
                         ENABLE_VIRTUAL_TERMINAL_PROCESSING;

            if (SetConsoleMode(salida, nuevo_modo)) {
                colores_ansi = true;
            }
        }

        /* La pagina 65001 permite imprimir los bloques del arte en UTF-8. */
        if (SetConsoleOutputCP(CP_UTF8)) {
            salida_utf8 = true;
        }
        SetConsoleCP(CP_UTF8);
#else
        termino = getenv("TERM");

        colores_ansi = detectar_terminal() &&
                       (termino == NULL || strcmp(termino, "dumb") != 0);
        salida_utf8 = idioma_es_utf8();
#endif
    }

    return colores_ansi;
}

bool consola_usa_ansi(void)
{
    return colores_ansi;
}

bool consola_usa_utf8(void)
{
#if defined(_WIN32) || defined(_WIN64)
    /* Si la consola no soporta ANSI, el arte se dibuja en ASCII para que se
     * vea igualmente (Windows 7 y consolas antiguas). */
    return salida_utf8 && colores_ansi;
#else
    return idioma_es_utf8();
#endif
}

bool consola_es_terminal(void)
{
    return detectar_terminal();
}

/**
 * @brief Copia el texto a stdout omitiendo las secuencias CSI (colores ANSI).
 */
static void imprimir_sin_ansi(const char *texto)
{
    const unsigned char *p = (const unsigned char *)texto;

    while (*p != '\0') {
        if (*p == 0x1B && p[1] == '[') {
            p += 2;
            while (*p != '\0' && !(*p >= 0x40 && *p <= 0x7E)) {
                p++;
            }
            if (*p != '\0') {
                p++;
            }
        } else {
            fputc((int)*p, stdout);
            p++;
        }
    }
}

/**
 * @brief Imprime texto convirtiendo las reglas de "=" o "-" en lineas continuas.
 *
 * Solo se aplica cuando el terminal admite UTF-8. Asi todos los mensajes que
 * traian "=====" o "-----" se ven con el mismo trazo continuo del menu sin
 * tener que reescribir cada cadena del juego.
 */
static void imprimir_con_reglas(const char *texto)
{
    const char *p = texto;
    char caracter;
    size_t cantidad;
    size_t i;

    while (*p != '\0') {
        if (*p == '=' || *p == '-') {
            caracter = *p;
            cantidad = 0;

            while (p[cantidad] == caracter) {
                cantidad++;
            }

            if (cantidad >= 3) {
                for (i = 0; i < cantidad; i++) {
                    fputs("\xE2\x94\x80", stdout);
                }
            } else {
                for (i = 0; i < cantidad; i++) {
                    fputc(caracter, stdout);
                }
            }
            p += cantidad;
        } else {
            fputc(*p, stdout);
            p++;
        }
    }
}

void consola_printf(const char *formato, ...)
{
    char buffer[4096];
    va_list argumentos;

    if (formato != NULL) {
        buffer[0] = '\0';
        va_start(argumentos, formato);
        vsnprintf(buffer, sizeof(buffer), formato, argumentos);
        va_end(argumentos);

        buffer[sizeof(buffer) - 1] = '\0';

        if (colores_ansi) {
            if (consola_usa_utf8()) {
                imprimir_con_reglas(buffer);
            } else {
                fputs(buffer, stdout);
            }
        } else {
            imprimir_sin_ansi(buffer);
        }

        /* Necesario para que los prompts sin salto de linea aparezcan antes de leer. */
        fflush(stdout);
    }
}

void consola_limpiar(void)
{
#if !defined(_WIN32) && !defined(_WIN64)
    int i;
#endif

    if (detectar_terminal()) {
        if (colores_ansi) {
            fputs("\033[2J\033[H", stdout);
            fflush(stdout);
        } else {
#if defined(_WIN32) || defined(_WIN64)
            system("cls");
#else
            if (system("clear") != 0) {
                for (i = 0; i < 30; i++) {
                    fputc('\n', stdout);
                }
            }
#endif
            fflush(stdout);
        }
    }
}

bool consola_esperar_enter(const char *mensaje)
{
    int caracter;
    bool leido = false;

    if (mensaje != NULL && mensaje[0] != '\0') {
        consola_printf("%s", mensaje);
    }
    fflush(stdout);

    caracter = getchar();
    if (caracter != EOF) {
        while (caracter != '\n' && caracter != EOF) {
            caracter = getchar();
        }

        leido = (caracter == '\n');
    }

    return leido;
}
