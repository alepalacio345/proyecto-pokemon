/**
 * @file pixel_art.c
 * @brief Pantalla de bienvenida dibujada pixel a pixel y renderizada en consola.
 */
#include "pixel_art.h"
#include "consola.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* Fuente de 4x5 pixeles. Cada fila es una cadena de 4 caracteres. */
static const char FUENTE_ALFABETO[26][5][5] = {
    {"0110", "1001", "1111", "1001", "1001"}, /* A */
    {"1110", "1001", "1110", "1001", "1110"}, /* B */
    {"0111", "1000", "1000", "1000", "0111"}, /* C */
    {"1110", "1001", "1001", "1001", "1110"}, /* D */
    {"1111", "1000", "1110", "1000", "1111"}, /* E */
    {"1111", "1000", "1110", "1000", "1000"}, /* F */
    {"0111", "1000", "1011", "1001", "0111"}, /* G */
    {"1001", "1001", "1111", "1001", "1001"}, /* H */
    {"1111", "0110", "0110", "0110", "1111"}, /* I */
    {"0011", "0001", "0001", "1001", "0110"}, /* J */
    {"1001", "1010", "1100", "1010", "1001"}, /* K */
    {"1000", "1000", "1000", "1000", "1111"}, /* L */
    {"1001", "1111", "1111", "1001", "1001"}, /* M */
    {"1001", "1101", "1111", "1011", "1001"}, /* N */
    {"0110", "1001", "1001", "1001", "0110"}, /* O */
    {"1110", "1001", "1110", "1000", "1000"}, /* P */
    {"0110", "1001", "1001", "1011", "0111"}, /* Q */
    {"1110", "1001", "1110", "1010", "1001"}, /* R */
    {"0111", "1000", "0110", "0001", "1110"}, /* S */
    {"1111", "0110", "0110", "0110", "0110"}, /* T */
    {"1001", "1001", "1001", "1001", "0110"}, /* U */
    {"1001", "1001", "1001", "0110", "0110"}, /* V */
    {"1001", "1001", "1111", "1111", "1001"}, /* W */
    {"1001", "1001", "0110", "1001", "1001"}, /* X */
    {"1001", "1001", "0110", "0110", "0110"}, /* Y */
    {"1111", "0001", "0110", "1000", "1111"}  /* Z */
};

static const char FUENTE_DIGITOS[10][5][5] = {
    {"0110", "1001", "1001", "1001", "0110"}, /* 0 */
    {"0100", "1100", "0100", "0100", "1110"}, /* 1 */
    {"1110", "0001", "0110", "1000", "1111"}, /* 2 */
    {"1110", "0001", "0110", "0001", "1110"}, /* 3 */
    {"1001", "1001", "1111", "0001", "0001"}, /* 4 */
    {"1111", "1000", "1110", "0001", "1110"}, /* 5 */
    {"0110", "1000", "1110", "1001", "0110"}, /* 6 */
    {"1111", "0001", "0010", "0100", "0100"}, /* 7 */
    {"0110", "1001", "0110", "1001", "0110"}, /* 8 */
    {"0110", "1001", "0111", "0001", "0110"}  /* 9 */
};

static const char POKEBOLA[11][12] = {
    "    KKKKK  ",
    "  KKRRRRRKK",
    " KRRRRRRRRK",
    " KRRRRRRRRK",
    " KRRRRRRRRK",
    " KKKKKKKKKK",
    " KWWWKKKWWK",
    " KWWWKKKWWK",
    " KWWWKKKWWK",
    "  KWWWWWWK ",
    "    KKKKK  "
};

/* Indices 256 colores ANSI para la paleta del lienzo. */
static const int PALETA[PIXEL_NARANJA + 1] = {
    0,   /* PIXEL_VACIO     */
    16,  /* PIXEL_NEGRO     */
    231, /* PIXEL_BLANCO    */
    196, /* PIXEL_ROJO      */
    226, /* PIXEL_AMARILLO  */
    21,  /* PIXEL_AZUL      */
    51,  /* PIXEL_CYAN      */
    244, /* PIXEL_GRIS      */
    208  /* PIXEL_NARANJA   */
};

static const char (*obtener_glifo(char caracter))[5]
{
    const char (*glifo)[5] = NULL;
    char c;

    c = (char)toupper((unsigned char)caracter);

    if (c >= 'A' && c <= 'Z') {
        glifo = FUENTE_ALFABETO[c - 'A'];
    } else if (c >= '0' && c <= '9') {
        glifo = FUENTE_DIGITOS[c - '0'];
    }

    return glifo;
}

static int ancho_texto(const char *texto, int escala)
{
    int largo = (int)strlen(texto);
    int ancho = 0;

    if (largo > 0) {
        ancho = (largo * 5 * escala) - escala;
    }

    return ancho;
}

void lienzo_limpiar(Lienzo *lienzo)
{
    if (lienzo == NULL) {
        return;
    }

    memset(lienzo->pixeles, PIXEL_VACIO, sizeof(lienzo->pixeles));
    lienzo->ancho = LIENZO_ANCHO;
    lienzo->alto = LIENZO_ALTO;
}

void lienzo_pixel(Lienzo *lienzo, int x, int y, int color)
{
    if (lienzo == NULL || x < 0 || y < 0 ||
        x >= lienzo->ancho || y >= lienzo->alto) {
        return;
    }

    lienzo->pixeles[y][x] = (unsigned char)color;
}

void lienzo_rectangulo(Lienzo *lienzo, int x0, int y0, int x1, int y1, int color)
{
    int x;
    int y;

    if (lienzo == NULL) {
        return;
    }

    for (x = x0; x <= x1; x++) {
        lienzo_pixel(lienzo, x, y0, color);
        lienzo_pixel(lienzo, x, y1, color);
    }
    for (y = y0; y <= y1; y++) {
        lienzo_pixel(lienzo, x0, y, color);
        lienzo_pixel(lienzo, x1, y, color);
    }
}

void lienzo_texto(Lienzo *lienzo, int x, int y, const char *texto, int color, int escala)
{
    int i;
    const char (*glifo)[5];
    int fila;
    int columna;
    int dx;
    int dy;

    if (lienzo == NULL || texto == NULL || escala < 1) {
        return;
    }

    for (i = 0; texto[i] != '\0'; i++) {
        glifo = obtener_glifo(texto[i]);

        if (glifo != NULL) {
            for (fila = 0; fila < 5; fila++) {
                for (columna = 0; columna < 4; columna++) {
                    if (glifo[fila][columna] == '1') {
                        for (dy = 0; dy < escala; dy++) {
                            for (dx = 0; dx < escala; dx++) {
                                lienzo_pixel(lienzo,
                                             x + (columna * escala) + dx,
                                             y + (fila * escala) + dy,
                                             color);
                            }
                        }
                    }
                }
            }
        }

        x += 5 * escala;
    }
}

void lienzo_texto_centrado(Lienzo *lienzo, int y, const char *texto,
                           int color, int escala, int color_sombra)
{
    int ancho;
    int x;

    if (lienzo == NULL || texto == NULL) {
        return;
    }

    ancho = ancho_texto(texto, escala);
    x = (lienzo->ancho - ancho) / 2;
    if (x < 0) {
        x = 0;
    }

    if (color_sombra != PIXEL_VACIO) {
        lienzo_texto(lienzo, x + 1, y + 1, texto, color_sombra, escala);
    }
    lienzo_texto(lienzo, x, y, texto, color, escala);
}

void lienzo_pokeball(Lienzo *lienzo, int centro_x, int centro_y)
{
    int fila;
    int columna;
    int color;
    char pixel;

    if (lienzo == NULL) {
        return;
    }

    for (fila = 0; fila < 11; fila++) {
        for (columna = 0; columna < 11; columna++) {
            color = PIXEL_VACIO;
            pixel = POKEBOLA[fila][columna];

            if (pixel == 'K') {
                color = PIXEL_NEGRO;
            } else if (pixel == 'R') {
                color = PIXEL_ROJO;
            } else if (pixel == 'W') {
                color = PIXEL_BLANCO;
            }

            if (color != PIXEL_VACIO) {
                lienzo_pixel(lienzo,
                             centro_x - 5 + columna,
                             centro_y - 5 + fila,
                             color);
            }
        }
    }
}

static void agregar_texto(char *buffer, size_t *longitud, size_t capacidad,
                          const char *texto)
{
    size_t largo = strlen(texto);

    if (*longitud + largo >= capacidad) {
        largo = capacidad - *longitud - 1;
    }

    memcpy(buffer + *longitud, texto, largo);
    *longitud += largo;
    buffer[*longitud] = '\0';
}

static void agregar_color(char *buffer, size_t *longitud, size_t capacidad,
                          int color, bool fondo)
{
    char secuencia[32];

    snprintf(secuencia, sizeof(secuencia), "\033[%d;5;%dm",
             fondo ? 48 : 38, PALETA[color]);
    agregar_texto(buffer, longitud, capacidad, secuencia);
}

static void renderizar_utf8(const Lienzo *lienzo)
{
    int y;
    char linea[16384];
    size_t longitud;
    int x;
    int arriba;
    int abajo;

    for (y = 0; y < lienzo->alto; y += 2) {
        longitud = 0;
        linea[0] = '\0';

        for (x = 0; x < lienzo->ancho; x++) {
            arriba = lienzo->pixeles[y][x];
            abajo = (y + 1 < lienzo->alto) ? lienzo->pixeles[y + 1][x]
                                           : PIXEL_VACIO;

            if (consola_usa_ansi()) {
                agregar_texto(linea, &longitud, sizeof(linea), "\033[0m");
            }

            if (arriba == PIXEL_VACIO && abajo == PIXEL_VACIO) {
                agregar_texto(linea, &longitud, sizeof(linea), " ");
            } else if (arriba == abajo) {
                agregar_color(linea, &longitud, sizeof(linea), arriba, false);
                agregar_texto(linea, &longitud, sizeof(linea), "\xE2\x96\x88");
            } else if (arriba == PIXEL_VACIO) {
                agregar_color(linea, &longitud, sizeof(linea), abajo, false);
                agregar_texto(linea, &longitud, sizeof(linea), "\xE2\x96\x84");
            } else if (abajo == PIXEL_VACIO) {
                agregar_color(linea, &longitud, sizeof(linea), arriba, false);
                agregar_texto(linea, &longitud, sizeof(linea), "\xE2\x96\x80");
            } else {
                agregar_color(linea, &longitud, sizeof(linea), arriba, false);
                agregar_color(linea, &longitud, sizeof(linea), abajo, true);
                agregar_texto(linea, &longitud, sizeof(linea), "\xE2\x96\x80");
            }
        }

        if (consola_usa_ansi()) {
            agregar_texto(linea, &longitud, sizeof(linea), "\033[0m");
        }
        consola_printf("%s\n", linea);
    }
}

static void renderizar_ascii(const Lienzo *lienzo)
{
    int y;
    char linea[16384];
    size_t longitud;
    int x;
    int arriba;
    int abajo;
    int color;

    for (y = 0; y < lienzo->alto; y += 2) {
        longitud = 0;
        linea[0] = '\0';

        for (x = 0; x < lienzo->ancho; x++) {
            arriba = lienzo->pixeles[y][x];
            abajo = (y + 1 < lienzo->alto) ? lienzo->pixeles[y + 1][x]
                                           : PIXEL_VACIO;
            color = (arriba != PIXEL_VACIO) ? arriba : abajo;

            if (color == PIXEL_VACIO) {
                agregar_texto(linea, &longitud, sizeof(linea), " ");
            } else {
                if (consola_usa_ansi()) {
                    agregar_texto(linea, &longitud, sizeof(linea), "\033[0m");
                    agregar_color(linea, &longitud, sizeof(linea), color, false);
                }
                agregar_texto(linea, &longitud, sizeof(linea), "#");
                if (consola_usa_ansi()) {
                    agregar_texto(linea, &longitud, sizeof(linea), "\033[0m");
                }
            }
        }

        consola_printf("%s\n", linea);
    }
}

void lienzo_renderizar(const Lienzo *lienzo)
{
    if (lienzo == NULL) {
        return;
    }

    if (consola_usa_utf8()) {
        renderizar_utf8(lienzo);
    } else {
        renderizar_ascii(lienzo);
    }
}

void pantalla_bienvenida(void)
{
    Lienzo lienzo;

    consola_iniciar();
    lienzo_limpiar(&lienzo);

    lienzo_rectangulo(&lienzo, 1, 0, LIENZO_ANCHO - 2, LIENZO_ALTO - 1,
                      PIXEL_AZUL);
    lienzo_pokeball(&lienzo, LIENZO_ANCHO / 2, 7);

    /* Sin sombra: a escala 2 un desplazamiento de 1 pixel se mete dentro de
     * los trazos y hacia que el titulo se viera sucio. */
    lienzo_texto_centrado(&lienzo, 14, "TORNEO", PIXEL_BLANCO, 2, PIXEL_VACIO);
    lienzo_texto_centrado(&lienzo, 26, "POKEMON", PIXEL_AMARILLO, 2, PIXEL_VACIO);
    lienzo_texto_centrado(&lienzo, 37, "PRESIONA ENTER", PIXEL_CYAN, 1,
                          PIXEL_VACIO);

    consola_limpiar();
    lienzo_renderizar(&lienzo);
    consola_esperar_enter("");

    /* Al pulsar ENTER la imagen desaparece antes de cargar el menu. */
    consola_limpiar();
}
