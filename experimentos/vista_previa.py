"""
Convierte la salida ANSI de demo_imagen_consola.exe en una imagen PNG,
para poder verla fuera de la consola. No forma parte del programa en C:
es solo una ayuda para revisar el resultado.
"""

import re
import sys

from PIL import Image, ImageDraw, ImageFont

FUENTE = r"C:\Windows\Fonts\consola.ttf"
FUENTE_NEGRITA = r"C:\Windows\Fonts\consolab.ttf"
TAM = 16

ANSI = re.compile(r"\x1b\[([0-9;]*)m")


def main():
    entrada = sys.argv[1] if len(sys.argv) > 1 else "salida_color.txt"
    salida = sys.argv[2] if len(sys.argv) > 2 else "vista_previa.png"

    with open(entrada, "rb") as f:
        crudo = f.read()
    # PowerShell redirige en UTF-16; la consola real emite UTF-8
    if crudo.startswith(b"\xff\xfe"):
        texto = crudo.decode("utf-16")
    else:
        texto = crudo.decode("utf-8")

    normal = ImageFont.truetype(FUENTE, TAM)
    negrita = ImageFont.truetype(FUENTE_NEGRITA, TAM)
    ancho_celda = int(round(normal.getlength("M")))
    alto_celda = int(TAM * 1.25)

    lineas = texto.split("\n")
    ancho = max(len(ANSI.sub("", l)) for l in lineas) * ancho_celda + 20
    alto = len(lineas) * alto_celda + 20

    imagen = Image.new("RGB", (ancho, alto), (12, 12, 12))
    dibujo = ImageDraw.Draw(imagen)

    for fila, linea in enumerate(lineas):
        x = 10
        y = 10 + fila * alto_celda
        fg = (204, 204, 204)
        bg = None
        negrita_activa = False

        pos = 0
        for m in ANSI.finditer(linea):
            tramo = linea[pos:m.start()]
            x = dibujar_tramo(dibujo, tramo, x, y, fg, bg, negrita_activa,
                              normal, negrita, ancho_celda, alto_celda)
            fg, bg, negrita_activa = aplicar_sgr(m.group(1), fg, bg, negrita_activa)
            pos = m.end()

        dibujar_tramo(dibujo, linea[pos:], x, y, fg, bg, negrita_activa,
                      normal, negrita, ancho_celda, alto_celda)

    imagen.save(salida)
    print("Vista previa guardada en %s (%dx%d)" % (salida, ancho, alto))


def aplicar_sgr(codigos, fg, bg, negrita_activa):
    partes = [int(p) for p in codigos.split(";") if p != ""] or [0]
    i = 0
    while i < len(partes):
        c = partes[i]
        if c == 0:
            fg, bg, negrita_activa = (204, 204, 204), None, False
        elif c == 1:
            negrita_activa = True
        elif c == 22:
            negrita_activa = False
        elif c == 38 and i + 4 < len(partes) and partes[i + 1] == 2:
            fg = tuple(partes[i + 2:i + 5])
            i += 4
        elif c == 48 and i + 4 < len(partes) and partes[i + 1] == 2:
            bg = tuple(partes[i + 2:i + 5])
            i += 4
        elif 30 <= c <= 37:
            base = [(0, 0, 0), (205, 49, 49), (13, 188, 121), (229, 229, 16),
                    (36, 114, 200), (188, 63, 188), (17, 168, 205), (229, 229, 229)]
            fg = base[c - 30]
        elif 90 <= c <= 97:
            base = [(102, 102, 102), (241, 76, 76), (35, 209, 139), (245, 245, 67),
                    (59, 142, 234), (214, 112, 214), (41, 184, 219), (255, 255, 255)]
            fg = base[c - 90]
        i += 1
    return fg, bg, negrita_activa


def dibujar_tramo(dibujo, tramo, x, y, fg, bg, negrita_activa,
                  normal, negrita, ancho_celda, alto_celda):
    fuente = negrita if negrita_activa else normal
    for caracter in tramo:
        if bg is not None:
            dibujo.rectangle([x, y, x + ancho_celda - 1, y + alto_celda - 1], fill=bg)
        if caracter == "\u2580":  # media celda superior: se pinta a mano
            dibujo.rectangle([x, y, x + ancho_celda - 1, y + alto_celda // 2 - 1], fill=fg)
        elif caracter != " ":
            # +2 de margen superior para que el texto quede centrado en la celda
            dibujo.text((x, y + 2), caracter, font=fuente, fill=fg)
        x += ancho_celda
    return x


if __name__ == "__main__":
    main()
