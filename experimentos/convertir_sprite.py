"""
Convierte un PNG (por ejemplo un sprite de PokeAPI) en un arreglo de C
listo para imprimirse en la consola con bloques de color.

Uso:
    python convertir_sprite.py pikachu.png sprite_pikachu.h 32 pikachu

Genera un arreglo RGBA de 32x32 (o el tamano indicado) y lo guarda como
static const unsigned char sprite_pikachu[32*32*4].
"""

import sys
from PIL import Image


def main():
    if len(sys.argv) < 5:
        print("Uso: python convertir_sprite.py entrada.png salida.h lado nombre")
        return 1

    entrada, salida = sys.argv[1], sys.argv[2]
    lado = int(sys.argv[3])
    nombre = sys.argv[4]

    imagen = Image.open(entrada).convert("RGBA")
    # Recorta el margen transparente para que el Pokemon ocupe todo el marco
    caja = imagen.getbbox()
    if caja:
        imagen = imagen.crop(caja)
    # NEAREST conserva el look pixelado en vez de suavizar los bordes
    imagen = imagen.resize((lado, lado), Image.NEAREST)

    pixeles = list(imagen.getdata())
    filas = []
    for y in range(lado):
        fila = []
        for x in range(lado):
            r, g, b, a = pixeles[y * lado + x]
            # Fondo transparente -> negro (el fondo normal de la consola)
            if a < 128:
                r = g = b = 0
            fila.append("0x%02x,0x%02x,0x%02x,0x%02x" % (r, g, b, a))
        filas.append("    " + ",".join(fila))

    with open(salida, "w", encoding="utf-8") as f:
        f.write("/* Generado con convertir_sprite.py a partir de %s */\n" % entrada)
        f.write("#ifndef SPRITE_%s_H\n#define SPRITE_%s_H\n\n" % (nombre.upper(), nombre.upper()))
        f.write("#define SPRITE_%s_LADO %d\n" % (nombre.upper(), lado))
        f.write("static const unsigned char sprite_%s[%d] = {\n" % (nombre, lado * lado * 4))
        f.write(",\n".join(filas))
        f.write("\n};\n\n#endif\n")

    print("Listo: %s (%dx%d, %d bytes)" % (salida, lado, lado, lado * lado * 4))
    return 0


if __name__ == "__main__":
    sys.exit(main())
