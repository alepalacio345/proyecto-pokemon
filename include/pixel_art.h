/**
 * @file pixel_art.h
 * @brief Mini motor de dibujo por pixeles para la pantalla de bienvenida.
 *
 * La imagen se construye en una matriz Lienzo[alto][ancho]. Cada celda guarda
 * un indice de la paleta. Al final se recorre la matriz y se convierte en
 * bloques de color ANSI (dos pixeles verticales por caracter con el bloque
 * medio superior) para que los pixeles se vean casi cuadrados en la consola.
 */
#ifndef PIXEL_ART_H
#define PIXEL_ART_H

#define LIENZO_ANCHO 78
#define LIENZO_ALTO 44

/** @brief Colores disponibles en la paleta del lienzo. */
typedef enum {
    PIXEL_VACIO = 0,
    PIXEL_NEGRO,
    PIXEL_BLANCO,
    PIXEL_ROJO,
    PIXEL_AMARILLO,
    PIXEL_AZUL,
    PIXEL_CYAN,
    PIXEL_GRIS,
    PIXEL_NARANJA
} ColorPixel;

typedef struct {
    unsigned char pixeles[LIENZO_ALTO][LIENZO_ANCHO];
    int ancho;
    int alto;
} Lienzo;

/** @brief Llena el lienzo de pixeles transparentes. */
void lienzo_limpiar(Lienzo *lienzo);

/** @brief Pinta un pixel si las coordenadas estan dentro del lienzo. */
void lienzo_pixel(Lienzo *lienzo, int x, int y, int color);

/** @brief Dibuja el marco rectangular de la imagen. */
void lienzo_rectangulo(Lienzo *lienzo, int x0, int y0, int x1, int y1, int color);

/** @brief Dibuja un texto con la fuente de 3x5 pixeles. */
void lienzo_texto(Lienzo *lienzo, int x, int y, const char *texto, int color, int escala);

/** @brief Dibuja un texto centrado horizontalmente, con sombra opcional. */
void lienzo_texto_centrado(Lienzo *lienzo, int y, const char *texto,
                           int color, int escala, int color_sombra);

/** @brief Dibuja la Pokebola de la cabecera. */
void lienzo_pokeball(Lienzo *lienzo, int centro_x, int centro_y);

/** @brief Convierte la matriz de pixeles en caracteres de consola. */
void lienzo_renderizar(const Lienzo *lienzo);

/** @brief Muestra la pantalla TORNEO POKEMON y espera ENTER. */
void pantalla_bienvenida(void);

#endif /* PIXEL_ART_H */
