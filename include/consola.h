/**
 * @file consola.h
 * @brief Capa de salida portable para Windows, Linux y macOS.
 *
 * Concentra todo lo que depende del sistema operativo: activar los colores ANSI
 * en la consola de Windows, configurar UTF-8, limpiar la pantalla y filtrar las
 * secuencias de color cuando el terminal no las soporta. Asi el resto del
 * programa puede seguir usando printf con colores sin romperse en otro sistema.
 */
#ifndef CONSOLA_H
#define CONSOLA_H

#include <stdbool.h>

#if defined(__GNUC__) || defined(__clang__)
#define CONSOLA_ATRIBUTO_FORMATO(argumento_formato, primer_argumento) \
    __attribute__((format(printf, argumento_formato, primer_argumento)))
#else
#define CONSOLA_ATRIBUTO_FORMATO(argumento_formato, primer_argumento)
#endif

/**
 * @brief Prepara la consola una sola vez.
 *
 * En Windows activa ENABLE_VIRTUAL_TERMINAL_PROCESSING y la pagina de codigos
 * UTF-8. En Linux/macOS comprueba si la salida es un terminal y si el idioma
 * usa UTF-8. Si los colores no se pueden activar, el programa sigue funcionando
 * en blanco y negro.
 *
 * @return true si los colores ANSI quedaron activos.
 */
bool consola_iniciar(void);

/** @brief Indica si se pueden emitir secuencias ANSI. */
bool consola_usa_ansi(void);

/** @brief Indica si la salida admite caracteres UTF-8 (bloques del arte). */
bool consola_usa_utf8(void);

/** @brief Indica si stdout es un terminal interactivo. */
bool consola_es_terminal(void);

/** @brief Limpia la pantalla sin depender de 'cls' ni 'clear' cuando es posible. */
void consola_limpiar(void);

/**
 * @brief printf portable que elimina los codigos ANSI si el terminal no los soporta.
 *
 * Tambien evita que las secuencias de color ensucien los archivos cuando la
 * salida se redirige (por ejemplo, al generar un reporte con '>').
 */
void consola_printf(const char *formato, ...) CONSOLA_ATRIBUTO_FORMATO(1, 2);

/**
 * @brief Espera a que el usuario pulse ENTER.
 * @param mensaje Texto opcional que se imprime antes de esperar (puede ser NULL).
 * @return true si se leyo un ENTER; false si se acabo la entrada (EOF).
 */
bool consola_esperar_enter(const char *mensaje);

#endif /* CONSOLA_H */
