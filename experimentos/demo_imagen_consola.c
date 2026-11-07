/*
 * demo_imagen_consola.c
 * ---------------------------------------------------------------------------
 * Demo: como mostrar una imagen (un sprite de Pokemon) dentro de una consola
 * de texto en C, sin ventanas ni librerias graficas.
 *
 * Tecnica: "half blocks" + color ANSI de 24 bits.
 *   - Cada caracter de la consola se divide en dos pixeles verticales usando
 *     el caracter Unicode ▀ (U+2580, media celda superior).
 *   - El color del texto pinta el pixel de arriba y el color de fondo pinta
 *     el de abajo.
 *   - Resultado: una imagen cuadrada de N columnas ocupa N x N/2 caracteres.
 *
 * Compilar (MinGW):
 *     gcc -Wall -Wextra -o demo_imagen_consola.exe demo_imagen_consola.c
 *
 * Ejecutar:
 *     demo_imagen_consola.exe          -> version a color (Windows Terminal)
 *     demo_imagen_consola.exe ascii    -> version ASCII de respaldo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
/* MinGW viejo no define esta constante aunque Windows si la soporte */
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

#include "sprite_pikachu.h"
#include "sprite_bulbasaur.h"
#include "sprite_charmander.h"
#include "sprite_squirtle.h"

#define LADO_SPRITE SPRITE_PIKACHU_LADO

/* ------------------------------------------------------------------------- */
/* Preparacion de la consola                                                 */
/* ------------------------------------------------------------------------- */

/* Activa UTF-8 y el procesamiento de secuencias ANSI (VT) en Windows.
 * En Windows 10/11 la mayoria de terminales ya lo traen activo, pero
 * activarlo explicitamente evita que se vean los codigos como texto. */
static void preparar_consola(void)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);

    HANDLE salida = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo = 0;

    if (GetConsoleMode(salida, &modo)) {
        SetConsoleMode(salida, modo | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif
}

/* ------------------------------------------------------------------------- */
/* Render a color con bloques                                                */
/* ------------------------------------------------------------------------- */

/* Devuelve el pixel (x, y) del sprite escalado a "ancho" columnas.
 * El muestreo es por vecino mas cercano para conservar el look pixelado. */
static void leer_pixel(const unsigned char *px, int lado, int ancho,
                       int x, int y,
                       int *r, int *g, int *b)
{
    int sx = x * lado / ancho;
    int sy = y * lado / ancho;
    const unsigned char *p = px + (sy * lado + sx) * 4;

    *r = p[0];
    *g = p[1];
    *b = p[2];
}

/* Imprime una sola fila de texto del sprite. "fila" va de 0 a ancho/2-1.
 * Se separa de imprimir_sprite_color para poder poner varios sprites uno al
 * lado del otro (ver imprimir_galeria). */
static void imprimir_fila_sprite(const unsigned char *px, int lado, int ancho, int fila)
{
    int x;

    for (x = 0; x < ancho; x++) {
        int r1, g1, b1, r2, g2, b2;

        leer_pixel(px, lado, ancho, x, fila * 2, &r1, &g1, &b1);
        leer_pixel(px, lado, ancho, x, fila * 2 + 1, &r2, &g2, &b2);

        if (r1 == 0 && g1 == 0 && b1 == 0 &&
            r2 == 0 && g2 == 0 && b2 == 0) {
            /* Ambos pixeles transparentes: fondo normal de la consola.
             * Se resetea el color para no arrastrar el fondo anterior. */
            printf("\033[0m ");
        } else if (r1 == r2 && g1 == g2 && b1 == b2) {
            /* Ambos pixeles iguales: un espacio con fondo de color */
            printf("\033[48;2;%d;%d;%dm ", r1, g1, b1);
        } else {
            printf("\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm\xE2\x96\x80",
                   r1, g1, b1, r2, g2, b2);
        }
    }
}

/* Imprime el sprite completo. "ancho" es el ancho en caracteres; la altura
 * resultante es ancho/2 lineas. */
static void imprimir_sprite_color(const unsigned char *px, int lado, int ancho)
{
    int filas = ancho / 2;
    int y;

    for (y = 0; y < filas; y++) {
        imprimir_fila_sprite(px, lado, ancho, y);
        printf("\033[0m\n");
    }
}

/* ------------------------------------------------------------------------- */
/* Respaldo ASCII (sin color, para consolas viejas o salida a archivo)       */
/* ------------------------------------------------------------------------- */

static void imprimir_sprite_ascii(const unsigned char *px, int lado, int ancho)
{
    /* De mas claro a mas oscuro. El fondo transparente se deja en blanco. */
    static const char densidad[] = " .:-=+*#%@";
    int filas = ancho;   /* en ASCII cada pixel ocupa una celda completa */
    int x, y;

    for (y = 0; y < filas; y++) {
        for (x = 0; x < ancho; x++) {
            int sx = x * lado / ancho;
            int sy = y * lado / ancho;
            const unsigned char *p = px + (sy * lado + sx) * 4;
            int brillo;

            if (p[3] < 128) {
                putchar(' ');
                continue;
            }

            brillo = (p[0] * 30 + p[1] * 59 + p[2] * 11) / 100;
            putchar(densidad[brillo * 9 / 255]);
        }
        putchar('\n');
    }
}

/* ------------------------------------------------------------------------- */
/* Ejemplo de uso: una "ficha" de la Pokedex con imagen                      */
/* ------------------------------------------------------------------------- */

static void imprimir_ficha_con_imagen(void)
{
    printf("\n\033[1;32m");
    printf(" ============================================================================\n");
    printf("                        FICHA DE LA POKEDEX\n");
    printf(" ============================================================================\n");
    printf("\033[0m\n");

    /* Zona izquierda: datos. Zona derecha: sprite de 32 columnas. */
    printf("   \033[1;33mPikachu\033[0m                       \033[1;37m#025\033[0m\n");
    printf("   \033[1;36mTipo:\033[0m Electrico\n");
    printf("   \033[1;36mHP:\033[0m 35   \033[1;36mAtaque:\033[0m 55\n");
    printf("   \033[1;36mDefensa:\033[0m 40   \033[1;36mVelocidad:\033[0m 90\n\n");

    imprimir_sprite_color(sprite_pikachu, LADO_SPRITE, 32);

    //imprimir_galeria(void);

    printf("\n\033[1;34m ============================================================================\n");
    printf("\033[0m");
}

/* ------------------------------------------------------------------------- */
/* Ejemplo de uso: "galeria" con varios Pokemon, como una lista con imagen   */
/* ------------------------------------------------------------------------- */

static void imprimir_galeria(void)
{
    const unsigned char *sprites[3] = { sprite_bulbasaur, sprite_charmander,
                                        sprite_squirtle };
    const char *nombres[3] = { "Bulbasaur", "Charmander", "Squirtle" };
    const char *ids[3] = { "#001", "#004", "#007" };
    const int ancho = 24;   /* 24 columnas -> 12 lineas por sprite */
    const int lado = SPRITE_BULBASAUR_LADO;
    int fila, i;

    printf("\n\033[1;32m");
    printf(" ============================================================================\n");
    printf("                   POKEDEX - VISTA DE GALERIA (3 por fila)\n");
    printf(" ============================================================================\n");
    printf("\033[0m\n");

    for (fila = 0; fila < ancho / 2; fila++) {
        printf("  ");
        for (i = 0; i < 3; i++) {
            imprimir_fila_sprite(sprites[i], lado, ancho, fila);
            printf("\033[0m   ");
        }
        printf("\n");
    }

    for (i = 0; i < 3; i++) {
        printf("   \033[1;33m%-14s\033[0m\033[1;37m%-8s\033[0m  ",
               nombres[i], ids[i]);
    }
    printf("\n\n   Total mostrado: 3 de 150 especies\n\n");
}

int main(int argc, char *argv[])
{
    int modo_ascii = (argc > 1 && strcmp(argv[1], "ascii") == 0);
    int modo_galeria = (argc > 1 && strcmp(argv[1], "galeria") == 0);

    preparar_consola();

    if (modo_ascii) {
        printf("\n Pikachu en ASCII (%dx%d):\n\n", LADO_SPRITE, LADO_SPRITE);
        imprimir_sprite_ascii(sprite_pikachu, LADO_SPRITE, 32);
    } else if (modo_galeria) {
        imprimir_galeria();
        printf(" Demo: tres sprites de 24x24 uno al lado del otro.\n");
        printf(" Prueba tambien: demo_imagen_consola.exe ascii\n\n");
    } else {
        imprimir_ficha_con_imagen();
        printf(" Demo: sprite %dx%d dibujado con bloques de color ANSI.\n",
               LADO_SPRITE, LADO_SPRITE);
        printf(" Prueba tambien: demo_imagen_consola.exe galeria | ascii\n\n");
    }

    return 0;
}
