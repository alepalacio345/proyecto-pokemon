/**
 * @file funciones.c
 * @brief Implementación lógica de la Pokédex, los equipos y el torneo.
 *
 * NOTA SOBRE EL ORDEN DE LAS FUNCIONES: en C toda función debe estar declarada
 * antes de usarse. Los prototipos viven en funciones.h, así que el orden de las
 * definiciones dentro de este archivo es libre.
 */
#include <stdio.h>
#include <string.h>
#include "funciones.h"
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>
#include <time.h>

// ===========================================================================
// MÓDULO 1: UTILIDADES Y SISTEMA
// ===========================================================================

/**
 * @brief Lee un numero entero desde el teclado de forma segura.
 *
 * Lee la linea completa con fgets y la convierte con sscanf. Si el usuario
 * escribe algo que no es un numero, el buffer se descarta por completo y se
 * devuelve el valor por defecto, evitando que el programa entre en un ciclo
 * infinito de lecturas fallidas.
 *
 * @param valor_por_defecto Valor que se devuelve si la entrada no es valida.
 * @return int Numero leido, o el valor por defecto.
 */
int leer_entero(int valor_por_defecto){
    char linea[50];
    int valor = valor_por_defecto;

    if (fgets(linea, sizeof(linea), stdin) != NULL) {
        if (sscanf(linea, "%d", &valor) != 1) {
            valor = valor_por_defecto;
        }
    }

    return valor;
}

void leer_texto(char destino[], int tamano){
    char linea[200];

    destino[0] = '\0';

    if (fgets(linea, sizeof(linea), stdin) != NULL) {
        if (sscanf(linea, "%199s", destino) != 1) {
            destino[0] = '\0';
        }

        // Proteccion adicional por si el token leido supera el destino
        destino[tamano - 1] = '\0';
    }
}

void imprimir_banner(char subtitulo[]){
    // Banner con titulo en letras simples y un subtitulo en una cajita centrada.
    if (subtitulo == NULL) {
        subtitulo = "";
    }

    // Marco superior
    printf("\033[1;34m");
    printf(" ============================================================================\n");
    printf("\033[0m");

    // Titulo centrado en letras simples
    printf("\n\033[1;33m");
    printf("                            EL GRAN TORNEO POKEMON\n");
    printf("\033[1;36m");
    printf("                              PRIMERA GENERACION\n");
    printf("\033[0m");

    // Subtitulo de la pantalla actual, en una cajita centrada dentro del ancho
    // interior (74 columnas) del banner.
    if (subtitulo[0] != '\0') {
        int largo = (int) strlen(subtitulo);
        int caja = largo + 2;               // contenido + un espacio a cada lado
        int relleno = (74 - caja) / 2;      // centrado dentro del interior
        int i;

        printf("\n");
        printf("\033[1;37m");
        printf(" %*s+", relleno, "");
        for (i = 0; i < caja; i++) {
            printf("-");
        }
        printf("+\n");

        printf(" %*s| \033[1;33m%s\033[1;37m |\n", relleno, "", subtitulo);

        printf(" %*s+", relleno, "");
        for (i = 0; i < caja; i++) {
            printf("-");
        }
        printf("+\n");
        printf("\033[0m");
    }

    // Marco inferior
    printf("\033[1;34m");
    printf(" ============================================================================\n");
    printf("\033[0m");
}

void imprimir_separador(void){
    // Linea separadora: un espacio + 74 guiones, igual que el interior del marco.
    printf("\033[1;34m --------------------------------------------------------------------------\n\033[0m");
}

void imprimir_opcion_menu(char tecla[], char texto[]){
    // Las opciones empiezan dos espacios adentro del marco, con el numero en
    // amarillo y el texto en blanco.
    printf("  \033[1;33m%-3s\033[0m \033[1;37m%s\033[0m\n", tecla, texto);
}

/**
 * @brief Devuelve el codigo de color ANSI asociado a un tipo elemental.
 *
 * Se usa para pintar la etiqueta del tipo en las pantallas de la Pokedex y en
 * las fichas de los Pokemon, de forma que cada tipo se identifique de un vistazo.
 *
 * @param nombre_tipo Nombre del tipo (ej. "Fuego").
 * @return const char* Secuencia ANSI del color, o blanco si el tipo no se reconoce.
 */
const char* color_tipo(char nombre_tipo[]){
    // Un solo punto de salida: se elige el color y se devuelve al final.
    const char *color = "\033[1;37m";

    if (strcasecmp(nombre_tipo, "Normal") == 0) {
        color = "\033[1;37m";
    } else if (strcasecmp(nombre_tipo, "Fuego") == 0) {
        color = "\033[1;31m";
    } else if (strcasecmp(nombre_tipo, "Agua") == 0) {
        color = "\033[1;34m";
    } else if (strcasecmp(nombre_tipo, "Planta") == 0) {
        color = "\033[1;32m";
    } else if (strcasecmp(nombre_tipo, "Electrico") == 0) {
        color = "\033[1;33m";
    } else if (strcasecmp(nombre_tipo, "Hielo") == 0) {
        color = "\033[1;96m";
    } else if (strcasecmp(nombre_tipo, "Lucha") == 0) {
        color = "\033[1;91m";
    } else if (strcasecmp(nombre_tipo, "Veneno") == 0) {
        color = "\033[1;35m";
    } else if (strcasecmp(nombre_tipo, "Tierra") == 0) {
        color = "\033[1;93m";
    } else if (strcasecmp(nombre_tipo, "Volador") == 0) {
        color = "\033[1;94m";
    } else if (strcasecmp(nombre_tipo, "Psiquico") == 0) {
        color = "\033[1;95m";
    } else if (strcasecmp(nombre_tipo, "Bicho") == 0) {
        color = "\033[1;92m";
    } else if (strcasecmp(nombre_tipo, "Roca") == 0) {
        color = "\033[1;33m";
    } else if (strcasecmp(nombre_tipo, "Fantasma") == 0) {
        color = "\033[1;35m";
    } else if (strcasecmp(nombre_tipo, "Dragon") == 0) {
        color = "\033[1;94m";
    } else if (strcasecmp(nombre_tipo, "Siniestro") == 0) {
        color = "\033[1;90m";
    } else if (strcasecmp(nombre_tipo, "Acero") == 0) {
        color = "\033[1;37m";
    } else if (strcasecmp(nombre_tipo, "Hada") == 0) {
        color = "\033[1;95m";
    }

    return color;
}

void imprimir_mensaje(char icono[], char titulo[], char detalle[], char color[]){
    // Mensaje corto con marco, icono y color. Se usa para errores, avisos y exitos.
    if (icono == NULL) { icono = ""; }
    if (titulo == NULL) { titulo = ""; }
    if (detalle == NULL) { detalle = ""; }
    if (color == NULL) { color = "\033[1;37m"; }

    printf("\n%s ============================================================================\n", color);
    printf("%s  %s %s\n", color, icono, titulo);
    printf("%s ============================================================================\n", color);

    if (detalle[0] != '\0') {
        printf("\033[1;37m  %s\n", detalle);
    }

    printf("%s ============================================================================\033[0m\n\n", color);
}

void imprimir_tipo_coloreado(char nombre_tipo[]){
    // Imprime el tipo con su color. \033[0m al final vuelve al color normal.
    printf("%s%s\033[0m", color_tipo(nombre_tipo), nombre_tipo);
}

// =========================================================================
// MÓDULO 5: MOTOR DE COMBATE Y EFECTIVIDAD DE TIPOS
// =========================================================================

// Arreglo de apoyo para el Traductor
const char* NOMBRES_TIPOS[CANTIDAD_TIPOS] = {
    "Normal", "Fuego", "Agua", "Planta", "Electrico", "Hielo", 
    "Lucha", "Veneno", "Tierra", "Volador", "Psiquico", "Bicho", 
    "Roca", "Fantasma", "Dragon", "Siniestro", "Acero", "Hada"
};

// Nombres legibles de las etapas del torneo (indexados por EtapaTorneo).
// IMPORTANTE: se escriben sin espacios porque este texto se guarda en
// resultados_combates.txt como un campo separado por espacios.
const char* NOMBRES_ETAPAS[] = {
    "Fase_de_Grupos", "Octavos_de_Final", "Cuartos_de_Final",
    "Semifinal", "Tercer_Lugar", "Final"
};

// La Gran Matriz de Multiplicadores Oficial de Nintendo (Generación 6+)
const float TABLA_EFECTIVIDAD[CANTIDAD_TIPOS][CANTIDAD_TIPOS] = {
    /*            Normal Fuego  Agua   Planta Electr Hielo  Lucha  Veneno Tierra Volad  Psiqui Bicho  Roca   Fantas Dragon Sinies Acero  Hada */
    /* Normal   */ { 1.0,  1.0,  1.0,  1.0,   1.0,   1.0,   1.0,   1.0,   1.0,   1.0,   1.0,   1.0,   0.5,   0.0,   1.0,   1.0,   0.5,   1.0 },
    /* Fuego    */ { 1.0,  0.5,  0.5,  2.0,   1.0,   2.0,   1.0,   1.0,   1.0,   1.0,   1.0,   2.0,   0.5,   1.0,   0.5,   1.0,   2.0,   1.0 },
    /* Agua     */ { 1.0,  2.0,  0.5,  0.5,   1.0,   1.0,   1.0,   1.0,   2.0,   1.0,   1.0,   1.0,   2.0,   1.0,   0.5,   1.0,   1.0,   1.0 },
    /* Planta   */ { 1.0,  0.5,  2.0,  0.5,   1.0,   1.0,   1.0,   0.5,   2.0,   0.5,   1.0,   0.5,   2.0,   1.0,   0.5,   1.0,   0.5,   1.0 },
    /* Electric */ { 1.0,  1.0,  2.0,  0.5,   0.5,   1.0,   1.0,   1.0,   0.0,   2.0,   1.0,   1.0,   1.0,   1.0,   0.5,   1.0,   1.0,   1.0 },
    /* Hielo    */ { 1.0,  0.5,  0.5,  2.0,   1.0,   0.5,   1.0,   1.0,   2.0,   2.0,   1.0,   1.0,   1.0,   1.0,   2.0,   1.0,   0.5,   1.0 },
    /* Lucha    */ { 2.0,  1.0,  1.0,  1.0,   1.0,   2.0,   1.0,   0.5,   1.0,   0.5,   0.5,   0.5,   2.0,   0.0,   1.0,   2.0,   2.0,   0.5 },
    /* Veneno   */ { 1.0,  1.0,  1.0,  2.0,   1.0,   1.0,   1.0,   0.5,   0.5,   1.0,   1.0,   1.0,   0.5,   0.5,   1.0,   1.0,   0.0,   2.0 },
    /* Tierra   */ { 1.0,  2.0,  1.0,  0.5,   2.0,   1.0,   1.0,   2.0,   1.0,   0.0,   1.0,   0.5,   2.0,   1.0,   1.0,   1.0,   2.0,   1.0 },
    /* Volador  */ { 1.0,  1.0,  1.0,  2.0,   0.5,   1.0,   2.0,   1.0,   1.0,   1.0,   1.0,   2.0,   0.5,   1.0,   1.0,   1.0,   0.5,   1.0 },
    /* Psiquico */ { 1.0,  1.0,  1.0,  1.0,   1.0,   1.0,   2.0,   2.0,   1.0,   1.0,   0.5,   1.0,   1.0,   1.0,   1.0,   0.0,   0.5,   1.0 },
    /* Bicho    */ { 1.0,  0.5,  1.0,  2.0,   1.0,   1.0,   0.5,   0.5,   1.0,   0.5,   2.0,   1.0,   1.0,   0.5,   1.0,   2.0,   0.5,   0.5 },
    /* Roca     */ { 1.0,  2.0,  1.0,  1.0,   1.0,   2.0,   0.5,   1.0,   0.5,   2.0,   1.0,   2.0,   1.0,   1.0,   1.0,   1.0,   0.5,   1.0 },
    /* Fantasma */ { 0.0,  1.0,  1.0,  1.0,   1.0,   1.0,   1.0,   1.0,   1.0,   1.0,   2.0,   1.0,   1.0,   2.0,   1.0,   0.5,   1.0,   1.0 },
    /* Dragon   */ { 1.0,  1.0,  1.0,  1.0,   1.0,   1.0,   1.0,   1.0,   1.0,   1.0,   1.0,   1.0,   1.0,   1.0,   2.0,   1.0,   0.5,   0.0 },
    /* Siniestr */ { 1.0,  1.0,  1.0,  1.0,   1.0,   1.0,   0.5,   1.0,   1.0,   1.0,   2.0,   1.0,   1.0,   2.0,   1.0,   0.5,   1.0,   0.5 },
    /* Acero    */ { 1.0,  0.5,  0.5,  1.0,   0.5,   2.0,   1.0,   1.0,   1.0,   1.0,   1.0,   1.0,   2.0,   1.0,   1.0,   1.0,   0.5,   2.0 },
    /* Hada     */ { 1.0,  0.5,  1.0,  1.0,   1.0,   1.0,   2.0,   0.5,   1.0,   1.0,   1.0,   1.0,   1.0,   1.0,   2.0,   2.0,   0.5,   1.0 }
};
// =========================================================================
// MÓDULO 1: UTILIDADES Y SISTEMA
// =========================================================================

void limpiar_consola() {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#elif defined(__APPLE__) || defined(__MACH__)
    system("clear");
#elif defined(__linux__) || defined(__unix__) || defined(__unix)
    system("clear");
#else
    printf("\033[H\033[2J"); 
#endif
}

// =========================================================================
// MÓDULO 2: GESTIÓN DE LA POKÉDEX
// =========================================================================

void llenar_vector_pokemon(especies_pokemons vector[]){
    FILE *archivo_entrada;
    char linea[150];
    char id[6];
    char nombre[15];
    char tipo_p[15];
    char tipo_s[15];
    int hp, ataque, defensa, velocidad;
    int i = 0;

    archivo_entrada = fopen("pokedex.txt", "r");
    
    if(archivo_entrada == NULL){
        ARCHIVO_ERROR;
    }else{
        while( (i < MAX_POKEDEX)&& (fgets(linea, sizeof(linea), archivo_entrada) != NULL)){
            if(sscanf(linea,"%s %s %s %i %i %i %i",id,nombre,tipo_p,&hp,&ataque,&defensa,&velocidad) == 7){
                strcpy(vector[i].tipo_segundario, "Ninguno");
            }else{
                sscanf(linea,"%s %s %s %s %i %i %i %i",id,nombre,tipo_p,tipo_s,&hp,&ataque,&defensa,&velocidad);
                strcpy(vector[i].tipo_segundario,tipo_s);
            }

            strcpy(vector[i].ID,id);
            strcpy(vector[i].Nombre,nombre);
            strcpy(vector[i].tipo_primario,tipo_p);
            vector[i].HP_b = hp;
            vector[i].ataque_b = ataque;
            vector[i].defensa_b = defensa;
            vector[i].velocidad_b = velocidad;
            i++;
        }
        fclose(archivo_entrada);
    }
    
}

int contar_especies_pokemon(especies_pokemons vector[], int maximo){
    int cantidad = 0;

    while ((cantidad < maximo) && (vector[cantidad].ID[0] != '\0')) {
        cantidad++;
    }

    return cantidad;
}

void imprimir_vector_pokemon(especies_pokemons vector[], int cantidad) {
    // Tabla de la Pokedex: encabezado con color, filas alternas y tipos coloreados.
    limpiar_consola();
    printf("\n\033[1;32m");
    printf(" ============================================================================\n");
    printf("                       POKEDEX - PRIMERA GENERACION\n");
    printf(" ============================================================================\n");
    printf("\033[0m");
    printf(" \033[1;36m%-5s| %-14s| %-12s| %-12s| %-4s| %-4s| %-4s| %-4s\033[0m\n",
           "ID", "NOMBRE", "TIPO 1", "TIPO 2", "HP", "ATQ", "DEF", "VEL");
    printf("\033[1;34m");
    printf(" ----------------------------------------------------------------------------\n");
    printf("\033[0m");

    for(int j = 0; j < cantidad; j++) {
        // Se pinta el nombre: los legendarios en amarillo y el resto en blanco
        printf(" \033[1;33m%-5s\033[0m| \033[1;37m%-14s\033[0m| ", vector[j].ID, vector[j].Nombre);
        printf("%s%-12s\033[0m| ", color_tipo(vector[j].tipo_primario), vector[j].tipo_primario);
        printf("%s%-12s\033[0m| ", color_tipo(vector[j].tipo_segundario), vector[j].tipo_segundario);
        printf("\033[1;32m%-4i\033[0m| \033[1;31m%-4i\033[0m| \033[1;34m%-4i\033[0m| \033[1;36m%-4i\033[0m\n",
               vector[j].HP_b, vector[j].ataque_b, vector[j].defensa_b, vector[j].velocidad_b);
    }

    printf("\033[1;34m");
    printf(" ============================================================================\n");
    printf("\033[0m");
    printf("  Total de especies registradas: \033[1;33m%d\033[0m\n", cantidad);
    printf("  HP = vida base | ATQ = ataque base | DEF = defensa base | VEL = velocidad base\n\n");
}

void mostrar_estadisticas_pokemon(especies_pokemons vector[], int indice){
    // Ficha detallada de una especie: tipos coloreados y estadisticas base destacadas.
    limpiar_consola();
    printf("\n\033[1;32m");
    printf(" ============================================================================\n");
    printf("                        FICHA DE LA POKEDEX\n");
    printf(" ============================================================================\n");
    printf("\033[0m\n");

    printf("\n\033[1;33m");
    printf("                     %s\n", vector[indice].Nombre);
    printf("\033[0m");
    printf("\033[1;37m");
    printf("                     Numero #%s\n", vector[indice].ID);
    printf("\033[0m\n");
    printf("\033[1;34m  ----------------------------------------------------------------------------\n\033[0m");

    printf("  \033[1;36mTIPOS\033[0m\n");
    printf("    ");
    imprimir_tipo_coloreado(vector[indice].tipo_primario);
    printf("  ");
    imprimir_tipo_coloreado(vector[indice].tipo_segundario);
    printf("\n\n");

    printf("\033[1;36m  ESTADISTICAS BASE\033[0m\n");
    printf("\033[1;34m  ----------------------------------------------------------------------------\n\033[0m");
    printf("   \033[1;32m%-14s\033[0m: \033[1;37m%3d\033[0m     \033[1;31m%-14s\033[0m: \033[1;37m%3d\033[0m\n",
           "HP", vector[indice].HP_b, "Ataque", vector[indice].ataque_b);
    printf("   \033[1;34m%-14s\033[0m: \033[1;37m%3d\033[0m     \033[1;36m%-14s\033[0m: \033[1;37m%3d\033[0m\n",
           "Defensa", vector[indice].defensa_b, "Velocidad", vector[indice].velocidad_b);
    printf("\033[1;34m  ----------------------------------------------------------------------------\n\033[0m");
    printf("   %-14s: \033[1;33m%d\033[0m\n", "Total base",
           vector[indice].HP_b + vector[indice].ataque_b + vector[indice].defensa_b + vector[indice].velocidad_b);

    printf("\n\033[1;32m ============================================================================\n\033[0m\n");
}

void buscar_vector_pokemon(especies_pokemons vector[], char string[], bool campo, int cantidad, bool *encontrado, int *indice){
    *encontrado = false;
    *indice = -1;
    int i = 0;

    while((i < cantidad) && !(*encontrado)){
        // Si la Pokedex no tiene cargada esa posicion no seguimos buscando
        if (vector[i].ID[0] == '\0') {
            break;
        }

        if (campo && (strcmp(vector[i].ID, string) == 0)) {
            *encontrado = true;
            *indice = i;
        }else if (!campo && (strcmp(vector[i].Nombre, string) == 0)) {
            *encontrado = true;
            *indice = i;
        }
        i++;
    }
}

void consultar_pokedex_id(especies_pokemons vector[], int cantidad) {
    bool encontrado;
    int indice;
    char id_buscar[8];

    limpiar_consola();
    imprimir_banner("BUSCAR POR NUMERO");
    printf("\n  Escriba el numero de la Pokedex (001 a 150).\n");
    printf("  Ejemplo: \033[1;33m025\033[0m para Pikachu\n\n");
    printf(" -> Numero del Pokemon: ");
    leer_texto(id_buscar, sizeof(id_buscar));

    buscar_vector_pokemon(vector,id_buscar,true,cantidad,&encontrado,&indice);

    if(encontrado){
        mostrar_estadisticas_pokemon(vector, indice);
    }else{
        imprimir_mensaje("[!]", "POKEMON NO ENCONTRADO",
                         "El numero ingresado no esta registrado en la Pokedex.", "\033[1;31m");
    }
}

void consultar_pokedex_nombre(especies_pokemons vector[], int cantidad) {
    bool encontrado;
    int indice;
    char nombre_buscar[20];

    limpiar_consola();
    imprimir_banner("BUSCAR POR NOMBRE");
    printf("\n  Escriba el nombre exacto, con la primera letra en mayuscula.\n");
    printf("  Ejemplo: \033[1;33mPikachu\033[0m\n\n");
    printf(" -> Nombre del Pokemon: ");
    leer_texto(nombre_buscar, sizeof(nombre_buscar));

    buscar_vector_pokemon(vector,nombre_buscar,false,cantidad,&encontrado,&indice);

    if(encontrado){
        mostrar_estadisticas_pokemon(vector, indice);
    }else{
        imprimir_mensaje("[!]", "POKEMON NO ENCONTRADO",
                         "No existe ninguna especie registrada con ese nombre.", "\033[1;31m");
    }
}

void menu_consultar_pokedex(especies_pokemons vector_universo_pokemon[], int cantidad) {
    int opcion_pokedex;

    limpiar_consola();
    imprimir_banner("CONSULTAR POKEDEX");

    printf("\n  \033[1;37mEspecies cargadas en el sistema\033[0m: \033[1;33m%d\033[0m\n", cantidad);
    printf("  \033[1;37mCatalogo oficial\033[0m: Primera Generacion (#001 a #150)\n\n");
    imprimir_separador();
    imprimir_opcion_menu("1", "Mostrar la Pokedex completa");
    imprimir_opcion_menu("2", "Buscar un Pokemon por numero (ID)");
    imprimir_opcion_menu("3", "Buscar un Pokemon por nombre");
    imprimir_opcion_menu("4", "Regresar al menu principal");
    imprimir_separador();
    printf(" -> Seleccione una opcion: ");
    // Lectura segura: una opcion no numerica no deja el buffer trabado
    opcion_pokedex = leer_entero(-1);

    switch(opcion_pokedex) {
        case 1:
            imprimir_vector_pokemon(vector_universo_pokemon, cantidad);
            break;
        case 2:
            consultar_pokedex_id(vector_universo_pokemon, cantidad);
            break;
        case 3:
            consultar_pokedex_nombre(vector_universo_pokemon, cantidad);
            break;
        case 4:
            break;
        default:
            imprimir_mensaje("[!]", "OPCION NO VALIDA",
                             "Elija una opcion del 1 al 4.", "\033[1;31m");
    }
}

// =========================================================================
// MÓDULO 3: GESTIÓN DE ENTRENADORES
// =========================================================================

void llenar_vector_entrenador(Entrenador vector[], int *i){
    FILE *entrada_archivo;
    entrada_archivo = fopen("entrenadores.txt", "a+");
    if(entrada_archivo == NULL){
        ARCHIVO_ERROR;
    }else{
        // El campo pokemon_derrotados es nuevo. Para no romper los archivos que aun no
        // lo tienen, cada linea se lee completa con fgets y se interpreta con sscanf:
        //   archivo nuevo (9 campos): id nombre cant V E D PTS PKD asignacion
        //   archivo viejo (8 campos): id nombre cant V E D PTS asignacion
        char linea[200];

        while((*i < MAX_ENTRENADORES) && (fgets(linea, sizeof(linea), entrada_archivo) != NULL)){
            int cantidad = 0;
            int victorias = 0;
            int empates = 0;
            int derrotas = 0;
            int puntuacion = 0;
            int pokemon_derrotados = 0;
            int leidos = sscanf(linea, "%7s %29s %d %d %d %d %d %d %5s",
                    vector[*i].id_entrenador, vector[*i].nombre, &cantidad,
                    &victorias, &empates, &derrotas,
                    &puntuacion, &pokemon_derrotados,
                    vector[*i].asignaciom_equipo);

            if (leidos == 8) {
                // Archivo sin migrar: en el formato viejo el ultimo valor leido era la
                // asignacion del equipo, asi que hay que releerlo y dejar el contador
                // nuevo en cero.
                leidos = sscanf(linea, "%7s %29s %d %d %d %d %d %5s",
                        vector[*i].id_entrenador, vector[*i].nombre, &cantidad,
                        &victorias, &empates, &derrotas,
                        &puntuacion, vector[*i].asignaciom_equipo);
                pokemon_derrotados = 0;
            }

            if (leidos >= 8) {
                vector[*i].victorias = victorias;
                vector[*i].empates = empates;
                vector[*i].derrotas = derrotas;
                vector[*i].puntuacion = (puntuacion < 0) ? 0 : puntuacion;
                vector[*i].pokemon_derrotados = (pokemon_derrotados < 0) ? 0 : pokemon_derrotados;

                // El campo cantidad_pokemon del archivo es solo informativo: el numero
                // real de integrantes lo determina el archivo de equipos. Si se confiara
                // en el, un entrenador sin equipo podria "aparentar" tener 6 Pokemon.
                vector[*i].cantidad_pokemon = 0;

                (*i)++;
            }
        }
        fclose(entrada_archivo);
    }
}

void guardar_vector_entrenador(Entrenador vector[], int cantidad){
    FILE *entrada_archivo;
    entrada_archivo = fopen("entrenadores.txt", "w");
    if(entrada_archivo == NULL){
        ARCHIVO_ERROR;
    }else{
        for(int j = 0; j < cantidad; j++){
                fprintf(entrada_archivo,"%s %s %d %d %d %d %d %d %s\n", vector[j].id_entrenador, vector[j].nombre,
                        vector[j].cantidad_pokemon, vector[j].victorias, vector[j].empates,
                        vector[j].derrotas, vector[j].puntuacion, vector[j].pokemon_derrotados,
                        vector[j].asignaciom_equipo);
        }
        fclose(entrada_archivo);
    }
}

void registrar_entrenador(Entrenador vector[], int *cantidad){

    if (*cantidad >= MAX_ENTRENADORES) {
        limpiar_consola();
        printf("\n=======================================================\n");
        printf("        \033[1;31m[!] ERROR: CUPO DE ENTRENADORES LLENO\033[0m          \n");
        printf("=======================================================\n");
        printf(" Ya se han registrado los %d participantes maximos.\n", MAX_ENTRENADORES);
        printf(" El torneo no admite mas inscripciones.\n");
        printf("=======================================================\n\n");
    }else{
        
        Entrenador info_entrenador;
        bool repetido;
        char opcion;

        limpiar_consola();
        printf("\n=======================================================\n");
        printf("             INSCRIPCION DE NUEVO ENTRENADOR           \n");
        printf("=======================================================\n");

        info_entrenador.cantidad_pokemon = 0;
        info_entrenador.victorias = 0;
        info_entrenador.empates = 0;
        info_entrenador.derrotas = 0;
        info_entrenador.puntuacion = 0;
        info_entrenador.pokemon_derrotados = 0;
        strcpy(info_entrenador.asignaciom_equipo, "false"); 

        printf("-> Ingrese el nombre del entrenador: ");
        leer_texto(info_entrenador.nombre, sizeof(info_entrenador.nombre));

        do {
            repetido = false;
            printf("-> Ingrese el ID identificador: ");
            leer_texto(info_entrenador.id_entrenador, sizeof(info_entrenador.id_entrenador));

            for(int i = 0; i < *cantidad; i++){
                if(strcmp(vector[i].id_entrenador, info_entrenador.id_entrenador) == 0){
                    repetido = true;
                }
            }

            if(repetido){
                char respuesta[10];

                printf("\n\033[1;31m[!] ERROR: ID DUPLICADO.\033[0m Ya existe un entrenador con ese ID.\n");
                printf(" Presione 'C' para intentar con otro o 'S' para salir.\n -> Opcion: ");
                leer_texto(respuesta, sizeof(respuesta));
                opcion = respuesta[0];
                if(toupper(opcion) == 'S') return; 
            } else {
                vector[*cantidad] = info_entrenador;
                (*cantidad)++;

                FILE *archivo_entrenadores = fopen("entrenadores.txt", "a+");
                if(archivo_entrenadores != NULL){
                    fprintf(archivo_entrenadores,"%s %s %d %d %d %d %d %d %s\n", 
                            info_entrenador.id_entrenador, info_entrenador.nombre, 
                            info_entrenador.cantidad_pokemon, info_entrenador.victorias, 
                            info_entrenador.empates, info_entrenador.derrotas, 
                            info_entrenador.puntuacion, info_entrenador.pokemon_derrotados,
                            info_entrenador.asignaciom_equipo);
                    fclose(archivo_entrenadores);
                }

                printf("\n=======================================================\n");
                printf(" Entrenador '%s' \033[1;32mregistrado exitosamente.\033[0m \n", info_entrenador.nombre);
                printf("=======================================================\n\n");
            }
        } while(repetido);
    }
}

void imprimir_vector_entrenador(Entrenador vector[], int cantidad) {
    limpiar_consola();
    printf("\n=======================================================\n");
    printf("          \033[1;36mLISTA DE ENTRENADORES REGISTRADOS\033[0m            \n");
    printf("=======================================================\n");
    printf(" \033[1;33m%-10s | %-30s\033[0m\n", "ID", "NOMBRE DEL ENTRENADOR");
    printf("-------------------------------------------------------\n");
    
    for(int i = 0; i < cantidad; i++){
        printf(" %-10s | %-30s\n", vector[i].id_entrenador, vector[i].nombre);
    }
    printf("=======================================================\n\n");
}

void mostrar_seleccion_entrenadores(Entrenador vector[], int cantidad) {
    limpiar_consola();
    printf("\n=================================================================\n");
    printf("           \033[1;36mDISPONIBILIDAD DE ENTRENADORES Y EQUIPOS\033[0m            \n");
    printf("=================================================================\n");
    printf(" \033[1;33m%-10s | %-25s | %-15s\033[0m\n", "ID", "NOMBRE", "ESTADO DEL EQUIPO");
    printf("-----------------------------------------------------------------\n");

    for (int i = 0; i < cantidad; i++) {
        if (strcmp(vector[i].asignaciom_equipo, "true") == 0) {
            printf(" %-10s | %-25s | \033[1;32m[+] Con Equipo\033[0m\n",vector[i].id_entrenador, vector[i].nombre);
        } else {
            printf(" %-10s | %-25s | \033[1;31m[-] Sin Equipo\033[0m\n",vector[i].id_entrenador, vector[i].nombre);
        }
    }
    printf("=================================================================\n");
}

void buscar_entrenador(Entrenador vector[], char id[], int cantidad, bool *encontrado, int *indice){
    *encontrado = false; 
    *indice = -1;
    int i = 0;
    while(!(*encontrado) && (i < cantidad)){
        if(strcmp(vector[i].id_entrenador, id) == 0){
            *encontrado = true; 
            *indice = i;
        }
        i++;
    }
}

void Consultar_entrenadores(Entrenador vector_entrenadores[], int cantidad){

    char id_entrenador[8];
    bool encontrar;
    int indice_entrenador_encontrado;

    imprimir_vector_entrenador(vector_entrenadores,cantidad);
    printf("-> Ingrese el ID del entrenador para ver su informacion: ");
    leer_texto(id_entrenador, sizeof(id_entrenador));

    buscar_entrenador(vector_entrenadores,id_entrenador,cantidad,&encontrar,&indice_entrenador_encontrado);

    if(encontrar){
        imprimir_perfil_entrenador(vector_entrenadores[indice_entrenador_encontrado]);
    }else{

        // Si el ID no existe
        printf("\n=======================================================\n");
        printf("          \033[1;31m[!] ERROR: ENTRENADOR NO ENCONTRADO\033[0m          \n");
        printf("=======================================================\n");
        printf(" El ID ingresado no coincide con ningun participante\n");
        printf(" registrado en la base de datos del torneo.\n");
        printf("=======================================================\n\n");
    }


}

void imprimir_perfil_entrenador(Entrenador entrenador) {
    limpiar_consola();
    
    printf("\n\033[1;36m========================================================================\033[0m\n");
    printf("                       \033[1;33mTARJETA DE ENTRENADOR POKEMON\033[0m                      \n");
    printf("\033[1;36m========================================================================\033[0m\n\n");
    
    // 1. Datos Personales
    printf("  \033[1;37m[ DATOS PERSONALES ]\033[0m\n");
    printf("  --------------------------------------------------\n");
    printf("  ID del Entrenador : \033[1;32m%s\033[0m\n", entrenador.id_entrenador);
    printf("  Nombre Oficial    : \033[1;32m%s\033[0m\n", entrenador.nombre);
    
    // 2. Estado del Equipo
    if (strcmp(entrenador.asignaciom_equipo, "true") == 0) {
        printf("  Estado del Equipo : \033[1;32m[+] Registrado (6 Pokemon)\033[0m\n\n");
    } else {
        printf("  Estado del Equipo : \033[1;31m[-] Sin Equipo Asignado\033[0m\n\n");
    }

    // 3. Estadísticas del Torneo
    printf("  \033[1;37m[ ESTADISTICAS DEL TORNEO ]\033[0m\n");
    printf("  --------------------------------------------------\n");
    printf("  Puntuacion Total  : \033[1;33m%d Pts\033[0m\n", entrenador.puntuacion);
    printf("  Victorias (W)     : \033[1;32m%d\033[0m\n", entrenador.victorias);
    printf("  Empates   (T)     : \033[1;36m%d\033[0m\n", entrenador.empates);
    printf("  Derrotas  (L)     : \033[1;31m%d\033[0m\n", entrenador.derrotas);
    printf("  Pkm Derrotados    : \033[1;35m%d\033[0m\n", entrenador.pokemon_derrotados);
    
    printf("\n\033[1;36m========================================================================\033[0m\n");

    // 4. Si tiene equipo, mostramos un pequeño resumen abajo
    if (strcmp(entrenador.asignaciom_equipo, "true") == 0) {
        printf("  \033[1;33m>> POKEMON REGISTRADOS EN SU ALINEACION:\033[0m\n");
        printf("  ");
        for (int i = 0; i < MAX_POKEMONES_EN_EQ; i++) {
            printf("[%s] ", entrenador.equipo[i].nombre);
            // Salto de línea a la mitad para que no se salga de la pantalla
            if (i == 2) printf("\n  "); 
        }
        printf("\n\033[1;36m========================================================================\033[0m\n\n");
    }
}

// =========================================================================
// MÓDULO 4: GESTIÓN DE EQUIPOS Y BACKTRACKING
// =========================================================================

void inicializar_barajas(int vector[]){
    for(int i = 0; i < MAX_POKEDEX; i++){
        vector[i] = i;
    }
}

void mezclar_baraja(int arreglo[], int cantidad_elementos) {
    for (int i = cantidad_elementos - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temporal = arreglo[i];
        arreglo[i] = arreglo[j];
        arreglo[j] = temporal;
    }
}

bool pokemon_ya_esta_en_equipo(EjemplarPokemon equipo[], int cantidad, char nombre[]) {
    bool encontrado = false;
    for (int i = 0; i < cantidad && !encontrado; i++) {
        if (strcmp(equipo[i].nombre, nombre) == 0) {
            encontrado = true;
        }
    }
    return encontrado;
}

bool validar_variedad_tipos(EjemplarPokemon equipo[], int cantidad) {
    char tipos_unicos[MAX_POKEMONES_EN_EQ][15];
    int contador_tipos = 0;
    bool resultado;

    for (int i = 0; i < cantidad; i++) {
        bool tipo_repetido = false;
        
        for (int j = 0; j < contador_tipos && !tipo_repetido; j++) {
            if (strcmp(equipo[i].datos_especie->tipo_primario, tipos_unicos[j]) == 0) {
                tipo_repetido = true;
            }
        }
        if (!tipo_repetido) {
            strcpy(tipos_unicos[contador_tipos], equipo[i].datos_especie->tipo_primario);
            contador_tipos++;
        }
    }

    if(contador_tipos >= 4){
        resultado = true;
    } else {
        resultado = false;
    }

    return resultado;
}

bool crear_equipo_pokemon(int integrantes_equipo, int nivel_actual_eq, int baraja[], EjemplarPokemon equipo_pokemon[], especies_pokemons vector_universo_pokemon[]) {
    bool salida = false; 

    if (integrantes_equipo == 6) {
        if (validar_variedad_tipos(equipo_pokemon, 6)) {
            salida = true;  
        } else {
            salida = false; 
        }
    } 
    else {
        for (int i = 0; i < MAX_POKEDEX && !salida; i++) {
            int indice = baraja[i];
            int nivel_simulado = 50; 
            
            if (!pokemon_ya_esta_en_equipo(equipo_pokemon, integrantes_equipo, vector_universo_pokemon[indice].Nombre)) {
                if ((nivel_actual_eq + nivel_simulado) <= 300) {
                    
                    equipo_pokemon[integrantes_equipo].datos_especie = &vector_universo_pokemon[indice];
                    strcpy(equipo_pokemon[integrantes_equipo].nombre, vector_universo_pokemon[indice].Nombre);
                    equipo_pokemon[integrantes_equipo].nivel = nivel_simulado;
                    
                    salida = crear_equipo_pokemon(
                        integrantes_equipo + 1, 
                        nivel_actual_eq + nivel_simulado, 
                        baraja, 
                        equipo_pokemon, 
                        vector_universo_pokemon
                    );
                }
            }
        }
    }
    return salida;
}

void crear_estadisticas_ejemplares(EjemplarPokemon pokemon[], int cantidad){
    int resultado_operacion;
    for(int i = 0; i < cantidad; i++){
        resultado_operacion = ((pokemon[i].datos_especie->HP_b * pokemon[i].nivel) / 50 ) + pokemon[i].nivel + 10;
        pokemon[i].hp_actual = resultado_operacion;
        pokemon[i].hp_maximo = resultado_operacion;
        pokemon[i].ataque = ( (pokemon[i].datos_especie->ataque_b * pokemon[i].nivel) / 50) + 5;
        pokemon[i].defensa = ( (pokemon[i].datos_especie->defensa_b * pokemon[i].nivel) / 50) + 5;
        pokemon[i].velocidad = ( (pokemon[i].datos_especie->velocidad_b * pokemon[i].nivel) / 50) + 5;
    }
}

void guardar_equipo_pokemon(Entrenador entrenador[],int indice, EjemplarPokemon equipo_pokemon[]){
    FILE *archivo_entreda;
    archivo_entreda = fopen("equipos_pokemon.txt","a+");

    if(archivo_entreda == NULL){
        ARCHIVO_ERROR;
    }else{
        fprintf(archivo_entreda,"%s %s\n",entrenador[indice].id_entrenador, entrenador[indice].nombre);
        entrenador[indice].cantidad_pokemon = 6;

        for(int i = 0; i < MAX_POKEMONES_EN_EQ; i++){
            sprintf(equipo_pokemon[i].id_ejemplar, "PKM%d", i + 1); 
            equipo_pokemon[i].experiencia = 0;                      
            equipo_pokemon[i].estado = 1;                           

            strcpy(entrenador[indice].equipo[i].id_ejemplar, equipo_pokemon[i].id_ejemplar);
            strcpy(entrenador[indice].equipo[i].nombre, equipo_pokemon[i].nombre);
            entrenador[indice].equipo[i].ataque = equipo_pokemon[i].ataque;
            entrenador[indice].equipo[i].defensa = equipo_pokemon[i].defensa;
            entrenador[indice].equipo[i].velocidad = equipo_pokemon[i].velocidad;
            entrenador[indice].equipo[i].estado = equipo_pokemon[i].estado;
            entrenador[indice].equipo[i].experiencia = equipo_pokemon[i].experiencia;
            entrenador[indice].equipo[i].hp_actual = equipo_pokemon[i].hp_actual;
            entrenador[indice].equipo[i].hp_maximo = equipo_pokemon[i].hp_maximo;
            entrenador[indice].equipo[i].nivel = equipo_pokemon[i].nivel;
            entrenador[indice].equipo[i].datos_especie = equipo_pokemon[i].datos_especie;

            fprintf(archivo_entreda, "%s %s %d %d %d %d %d %d %d %d\n", 
                    equipo_pokemon[i].id_ejemplar, equipo_pokemon[i].nombre, equipo_pokemon[i].nivel,
                    equipo_pokemon[i].hp_actual, equipo_pokemon[i].hp_maximo, equipo_pokemon[i].ataque,
                    equipo_pokemon[i].defensa, equipo_pokemon[i].velocidad, equipo_pokemon[i].experiencia,
                    equipo_pokemon[i].estado);
        }
        fclose(archivo_entreda);
    }
}

void imprimir_equipo_pokemon(EjemplarPokemon equipo[], int cantidad) {
    printf("\n========================================================================================\n");
    printf("                             \033[1;36mALINEACION DEL EQUIPO POKEMON\033[0m                              \n");
    printf("========================================================================================\n");
    printf(" \033[1;33m%-3s | %-12s | %-5s | %-10s | %-10s | %-4s | %-4s | %-4s\033[0m\n", 
           "N.", "NOMBRE", "NIVEL", "TIPO 1", "TIPO 2", "HP", "ATQ", "DEF");
    printf("----------------------------------------------------------------------------------------\n");

    for (int i = 0; i < cantidad; i++) {
        printf(" %-3d | \033[1;32m%-12s\033[0m | %-5d | %-10s | %-10s | %-4d | %-4d | %-4d\n",
               i + 1, equipo[i].nombre, equipo[i].nivel,                             
               equipo[i].datos_especie->tipo_primario, equipo[i].datos_especie->tipo_segundario,    
               equipo[i].hp_actual, equipo[i].ataque, equipo[i].defensa);         
    }
    printf("========================================================================================\n\n");
}

/**
 * @brief Genera, calcula y guarda el equipo de un entrenador usando backtracking.
 *
 * Es la pieza compartida entre la creacion manual y la creacion masiva: baraja la
 * Pokedex, ejecuta el backtracking (crear_equipo_pokemon), calcula las
 * estadisticas de los 6 ejemplares, los guarda en disco y marca al entrenador
 * como que ya tiene equipo.
 *
 * @param vector_universo_pokemon Arreglo global de la Pokedex.
 * @param vector_entrenadores Arreglo global de Entrenadores.
 * @param cantidad_entrenadores Numero real de entrenadores en RAM.
 * @param indice Entrenador al que se le crea el equipo.
 * @return true si el equipo se armo y guardo correctamente.
 */
bool generar_equipo_entrenador(especies_pokemons vector_universo_pokemon[], Entrenador vector_entrenadores[], int cantidad_entrenadores, int indice){
    EjemplarPokemon equipo_pokemon[MAX_POKEMONES_EN_EQ];
    int vector_baraja[MAX_POKEDEX];
    bool creado;

    inicializar_barajas(vector_baraja);
    mezclar_baraja(vector_baraja, MAX_POKEDEX);

    creado = crear_equipo_pokemon(0, 0, vector_baraja, equipo_pokemon, vector_universo_pokemon);

    if (creado) {
        crear_estadisticas_ejemplares(equipo_pokemon, MAX_POKEMONES_EN_EQ);
        guardar_equipo_pokemon(vector_entrenadores, indice, equipo_pokemon);
        strcpy(vector_entrenadores[indice].asignaciom_equipo, "true");
        guardar_vector_entrenador(vector_entrenadores, cantidad_entrenadores);
    }

    return creado;
}

int crear_equipos_todos(especies_pokemons vector_universo_pokemon[], Entrenador vector_entrenadores[], int cantidad_entrenadores){
    // Crea el equipo de cada entrenador que todavia no tenga uno.
    int creados = 0;
    int fallidos = 0;
    int ya_tenian = 0;

    limpiar_consola();
    imprimir_banner("CREACION MASIVA DE EQUIPOS");

    if (cantidad_entrenadores < 1) {
        printf("\n\033[1;31m[!] ERROR:\033[0m no hay entrenadores cargados.\n\n");
        return 0;
    }

    printf("\n Se generara un equipo de %d Pokemon a cada entrenador que no lo tenga.\n", MAX_POKEMONES_EN_EQ);
    printf(" Esto puede tardar unos segundos...\n\n");
    imprimir_separador();

    for (int i = 0; i < cantidad_entrenadores; i++) {
        if (strcmp(vector_entrenadores[i].asignaciom_equipo, "true") == 0) {
            ya_tenian++;
            printf("  \033[1;30m[-] %-12s ya tenia equipo (se omite)\033[0m\n", vector_entrenadores[i].nombre);
        } else if (generar_equipo_entrenador(vector_universo_pokemon, vector_entrenadores, cantidad_entrenadores, i)) {
            creados++;
            printf("  \033[1;32m[+] %-12s equipo creado\033[0m\n", vector_entrenadores[i].nombre);
        } else {
            fallidos++;
            printf("  \033[1;31m[!] %-12s no se pudo armar el equipo\033[0m\n", vector_entrenadores[i].nombre);
        }
    }

    imprimir_separador();
    printf("\n  Equipos creados .......: \033[1;32m%d\033[0m\n", creados);
    printf("  Ya tenian equipo ......: \033[1;33m%d\033[0m\n", ya_tenian);
    printf("  No se pudieron crear ..: \033[1;31m%d\033[0m\n", fallidos);

    if (fallidos == 0) {
        printf("\n\033[1;32m[+] LISTO:\033[0m todos los entrenadores tienen su equipo completo.\n\n");
    } else {
        printf("\n\033[1;33m[!] AVISO:\033[0m algunos entrenadores quedaron sin equipo.\n\n");
    }

    return creados;
}

void menu_crear_equipos(especies_pokemons vector_universo_pokemon[], Entrenador vector_entrenadores[], int cantidad_entrenadores){
    // Submenu: crear todos los equipos de una vez o elegir un entrenador.
    int opcion;
    int sin_equipo = 0;

    for (int i = 0; i < cantidad_entrenadores; i++) {
        if (strcmp(vector_entrenadores[i].asignaciom_equipo, "true") != 0) {
            sin_equipo++;
        }
    }

    limpiar_consola();
    imprimir_banner("CREAR EQUIPO");
    printf("\n  Entrenadores registrados ......: \033[1;37m%d\033[0m\n", cantidad_entrenadores);
    printf("  Entrenadores sin equipo .......: \033[1;33m%d\033[0m\n", sin_equipo);
    printf("  Entrenadores con equipo .......: \033[1;32m%d\033[0m\n", cantidad_entrenadores - sin_equipo);
    printf("\n");
    imprimir_separador();
    printf("  \033[1;33m1.\033[0m Crear TODOS los equipos que falten (automatico)\n");
    printf("  \033[1;33m2.\033[0m Seleccionar un entrenador manualmente\n");
    printf("  \033[1;33m3.\033[0m Regresar al menu principal\n");
    imprimir_separador();
    printf(" -> Seleccione una opcion: ");
    opcion = leer_entero(-1);

    switch (opcion) {
        case 1:
            if (sin_equipo == 0) {
                printf("\n\033[1;33m[!] AVISO:\033[0m todos los entrenadores ya tienen equipo.\n\n");
            } else {
                crear_equipos_todos(vector_universo_pokemon, vector_entrenadores, cantidad_entrenadores);
            }
            break;
        case 2:
            consulta_crear_equipo_pokemon(vector_universo_pokemon, vector_entrenadores, cantidad_entrenadores);
            break;
        case 3:
            break;
        default:
            printf("\n\033[1;31m[!] Opcion no valida.\033[0m\n\n");
            break;
    }
}

void consulta_crear_equipo_pokemon(especies_pokemons vector_universo_pokemon[], Entrenador vector_entrenadores[], int cantidad_entrenadores){
    bool encontrado;
    char id_elegido[8];
    int indice;

    mostrar_seleccion_entrenadores(vector_entrenadores, cantidad_entrenadores);
    printf("\n -> Ingrese el ID del entrenador para crearle un equipo: ");
    leer_texto(id_elegido, sizeof(id_elegido));

    buscar_entrenador(vector_entrenadores, id_elegido, cantidad_entrenadores, &encontrado, &indice);
    
    if(encontrado){
        if(strcmp(vector_entrenadores[indice].asignaciom_equipo, "true") == 0){
            printf("\n\033[1;33m[!] ADVERTENCIA:\033[0m El entrenador '%s' ya tiene un equipo asignado.\n", vector_entrenadores[indice].nombre);
        }else{
            limpiar_consola();

            if (generar_equipo_entrenador(vector_universo_pokemon, vector_entrenadores, cantidad_entrenadores, indice)) {
                printf("\n");
                imprimir_separador();
                printf("  \033[1;32m[+] EQUIPO CREADO EXITOSAMENTE PARA %s\033[0m\n", vector_entrenadores[indice].nombre);
                imprimir_separador();
                imprimir_equipo_pokemon(vector_entrenadores[indice].equipo, MAX_POKEMONES_EN_EQ);
            } else {
                printf("\n=======================================================\n");
                printf("             \033[1;31m[-] ERROR AL ENSAMBLAR EQUIPO\033[0m             \n");
                printf("=======================================================\n\n");
            }

        }

    }else{
        printf("\n=======================================================\n");
        printf("          \033[1;31m[!] ERROR: ENTRENADOR NO ENCONTRADO\033[0m          \n");
        printf("=======================================================\n");
        printf(" El ID ingresado no coincide con ningun participante\n");
        printf(" registrado en la base de datos del torneo.\n");
        printf("=======================================================\n\n");
    }
}

void cargar_equipos_pokemon(Entrenador vector_entrenadores[], int cantidad_entrenadores, especies_pokemons pokedex[]) {
    
    // 1. Abrimos 
    FILE *archivo_equipos = fopen("equipos_pokemon.txt", "r");
    
    // Si el archivo no existe o esta vacio, simplemente no hacemos nada y salimos
    if (archivo_equipos == NULL) {
        ARCHIVO_ERROR; 
    }else{

        char id_entrenador[8];
        char nombre_entrenador[30];
        int total_pokedex = contar_especies_pokemon(pokedex, MAX_POKEDEX);
        
        //Leemos la cabecera de cada equipo (ej: "001 Saul")
        while (fscanf(archivo_equipos, "%7s %29s", id_entrenador, nombre_entrenador) == 2) {
            
            bool encontrado;
            int indice_ent;
            
            //Buscamos en que indice de la RAM esta ese entrenador
            buscar_entrenador(vector_entrenadores, id_entrenador, cantidad_entrenadores, &encontrado, &indice_ent);
            
            if (encontrado) {
                int integrantes_validos = 0;

                //Si existe, leemos exactamente 6 lineas con sus 6 Pokemon
                for (int i = 0; i < MAX_POKEMONES_EN_EQ; i++) {
                    
                    char nombre_pokemon[20];
                    int estado_temp;
                    int leidos;

                    leidos = fscanf(archivo_equipos, "%5s %19s %d %d %d %d %d %d %d %d",
                        vector_entrenadores[indice_ent].equipo[i].id_ejemplar,
                        nombre_pokemon,
                        &vector_entrenadores[indice_ent].equipo[i].nivel,
                        &vector_entrenadores[indice_ent].equipo[i].hp_actual,
                        &vector_entrenadores[indice_ent].equipo[i].hp_maximo,
                        &vector_entrenadores[indice_ent].equipo[i].ataque,
                        &vector_entrenadores[indice_ent].equipo[i].defensa,
                        &vector_entrenadores[indice_ent].equipo[i].velocidad,
                        &vector_entrenadores[indice_ent].equipo[i].experiencia,
                        &estado_temp);

                    if (leidos != 10) {
                        // Linea incompleta: el ejemplar no puede combatir
                        vector_entrenadores[indice_ent].equipo[i].estado = false;
                        continue;
                    }

                    // Guardamos el nombre y el estado
                    strcpy(vector_entrenadores[indice_ent].equipo[i].nombre, nombre_pokemon);
                    vector_entrenadores[indice_ent].equipo[i].estado = estado_temp;

                    // Reconectar el puntero a la Pokédex maestra
                    bool poke_encontrado;
                    int poke_indice;
                    
                    // Buscamos el nombre del Pokemon en la Pokedex general
                    buscar_vector_pokemon(pokedex, nombre_pokemon, false, total_pokedex, &poke_encontrado, &poke_indice);
                    
                    // Solo enlazamos la especie si realmente existe en la Pokedex
                    if (poke_encontrado) { 
                        vector_entrenadores[indice_ent].equipo[i].datos_especie = &pokedex[poke_indice];
                        integrantes_validos++;
                    } else {
                        // Sin especie valida el ejemplar no puede combatir: lo marcamos como no disponible
                        vector_entrenadores[indice_ent].equipo[i].estado = false;
                    }
                }

                // La cantidad real de integrantes sale de lo que se pudo cargar del archivo
                vector_entrenadores[indice_ent].cantidad_pokemon = integrantes_validos;
            }
        }
        
        // 5. Cerramos la nevera
        fclose(archivo_equipos);



    }

}

void consultar_equipos_creasdos(Entrenador vector_entrenadores[], int cantidad_entrenadores){

    bool encontrado;
    char id_elegido[8];
    int indice;

    imprimir_vector_entrenador(vector_entrenadores, cantidad_entrenadores);
    printf("\n -> Ingrese el ID del entrenador para buscar su equipo: ");
    leer_texto(id_elegido, sizeof(id_elegido));

    buscar_entrenador(vector_entrenadores, id_elegido, cantidad_entrenadores, &encontrado, &indice);
    limpiar_consola();

        
    if (encontrado) {
        
        // Verificamos si tiene equipo (true) o no (false)
        if (strcmp(vector_entrenadores[indice].asignaciom_equipo, "true") == 0) {
            
            // Si tiene equipo, llamamos a la funcion que dibuja la tabla
            imprimir_equipo_pokemon(vector_entrenadores[indice].equipo, MAX_POKEMONES_EN_EQ);
            
        } else {
        
            printf("\n=======================================================\n");
            printf("           \033[1;33m[!] AVISO: ENTRENADOR SIN EQUIPO\033[0m            \n");
            printf("=======================================================\n");
            printf(" El participante seleccionado actualmente no posee\n");
            printf(" una alineacion Pokemon registrada en el sistema.\n");
            printf("=======================================================\n\n");
        }

    } else {
        
        // Si el ID no existe
        printf("\n=======================================================\n");
        printf("          \033[1;31m[!] ERROR: ENTRENADOR NO ENCONTRADO\033[0m          \n");
        printf("=======================================================\n");
        printf(" El ID ingresado no coincide con ningun participante\n");
        printf(" registrado en la base de datos del torneo.\n");
        printf("=======================================================\n\n");
    }
}

// =========================================================================
// MÓDULO 5: COMBATE
// =========================================================================

float calcular_multiplicador_tipos(char tipo_atacante[], char tipo_defensor_1[], char tipo_defensor_2[]) {
    
    // Por defecto, asumimos que el ataque hace daño normal (1.0)
    float multiplicador_final = 1.0; 
    
    int ind_ataque = obtener_indice_tipo(tipo_atacante);
    int ind_def_1 = obtener_indice_tipo(tipo_defensor_1);
    int ind_def_2 = obtener_indice_tipo(tipo_defensor_2);

    // Solo hacemos calculos si tanto el ataque como la primera defensa son validos
    if (ind_ataque != -1 && ind_def_1 != -1) {
        
        // Buscamos el daño que le hace al Tipo 1
        multiplicador_final = TABLA_EFECTIVIDAD[ind_ataque][ind_def_1];

        // Si el pokemon tiene un segundo tipo válido, multiplicamos el efecto
        if (ind_def_2 != -1) {
            multiplicador_final *= TABLA_EFECTIVIDAD[ind_ataque][ind_def_2];
        }
    }

    // Único punto de salida
    return multiplicador_final; 
}

int obtener_indice_tipo(char nombre_tipo[]) {
    int indice = -1;
    bool encontrado = false;
    int i = 0; // Inicializamos el contador

    // Solo buscamos si el tipo NO viene vacio y NO es "Ninguno"
    if (nombre_tipo != NULL && strcmp(nombre_tipo, "Ninguno") != 0) {
        
        // El ciclo se detiene si llega al final o si 'encontrado' cambia a true
        while ((i < CANTIDAD_TIPOS) && !encontrado) {
            
            if (strcasecmp(nombre_tipo, NOMBRES_TIPOS[i]) == 0) {
                indice = i;
                encontrado = true;
            }
            
            i++; // Incrementamos el contador al final de cada vuelta
        }
    }
    
    // Unico punto de salida
    return indice; 
}

int calcular_dano(EjemplarPokemon Atacante, EjemplarPokemon Defensor){

    float modificador;
    float nivel_f;
    float ataque_f;
    float defensa_f;
    int danio_final;
    float danio_base;

    // Obtenemos el multiplicador de tipos
    modificador = calcular_multiplicador_tipos(Atacante.datos_especie->tipo_primario,Defensor.datos_especie->tipo_primario,Defensor.datos_especie->tipo_segundario);

    // Calculamos la parte interna de la fórmula (Daño Base)
    // Usamos 2.0 y 5.0 (en vez de 2 y 5) y (float) para asegurar precisión decimal.
    nivel_f = (float)Atacante.nivel;
    ataque_f = (float)Atacante.ataque;
    defensa_f = (float)Defensor.defensa;

    danio_base = ((((2.0 * nivel_f / 5.0) + 2.0) * PODER_ATAQUE * (ataque_f / defensa_f)) / 50.0) + 2.0;

    // Multiplicamos por la efectividad de tipos y lo convertimos de vuelta a Entero (int)
    danio_final = (int)(danio_base * modificador);

    // REGLA OFICIAL: Cualquier ataque que no sea "Sin Efecto" debe hacer mínimo 1 de daño
    if (danio_final == 0 && modificador != 0.0) {
        danio_final = 1;
    }

    return danio_final;
}

void curar_equipo(Entrenador *entrenador){
    // Restaura la vida al maximo y reactiva a todos los ejemplares del equipo.
    for (int k = 0; k < entrenador->cantidad_pokemon; k++) {
        entrenador->equipo[k].hp_actual = entrenador->equipo[k].hp_maximo;
        entrenador->equipo[k].estado = true;
    }
}

int seleccionar_pokemon(EjemplarPokemon equipo[], int cantidad){
    // Recorremos todo el equipo en orden y nos quedamos con el indice del primer
    // Pokemon apto. Un solo punto de salida.
    int indice = -1;
    bool encontrado = false;

    for (int i = 0; i < cantidad && !encontrado; i++) {
        if (equipo[i].estado && equipo[i].hp_actual > 0) {
            indice = i;
            encontrado = true;
        }
    }

    return indice;
}

int seleccionar_siguiente(EjemplarPokemon equipo[], int cantidad, int actual){
    // Busca el proximo Pokemon disponible empezando por el que sigue al actual y,
    // si no hay mas adelante, vuelve a revisar desde el comienzo del equipo.
    // Un solo punto de salida.
    int indice = -1;
    bool encontrado = false;

    for (int desplazamiento = 0; desplazamiento < cantidad && !encontrado; desplazamiento++) {
        int posicion = (actual + 1 + desplazamiento) % cantidad;

        if (equipo[posicion].estado && equipo[posicion].hp_actual > 0) {
            indice = posicion;
            encontrado = true;   // corta el ciclo
        }
    }

    return indice;
}

void ejecutar_turno_ataque(Entrenador *atacante, int indice_atacante, Entrenador *defensor, int indice_defensor, bool permitir_empate, int *derrotados){
    // Un turno de ataque: el atacante golpea al defensor y, si este ultimo cae,
    // se contabiliza en el contador de Pokemon derrotados del torneo.
    int dano = calcular_dano(atacante->equipo[indice_atacante], defensor->equipo[indice_defensor]);

    defensor->equipo[indice_defensor].hp_actual = defensor->equipo[indice_defensor].hp_actual - dano;
    if (defensor->equipo[indice_defensor].hp_actual < 0) {
        defensor->equipo[indice_defensor].hp_actual = 0;
    }

    if (defensor->equipo[indice_defensor].hp_actual == 0) {
        defensor->equipo[indice_defensor].estado = false;

        // En fase de grupos el empate es valido y el criterio de desempate no se usa
        if (permitir_empate && derrotados != NULL) {
            (*derrotados)++;
        }
    }
}

int calcular_hp_total(Entrenador *entrenador){
    // Suma la vida que le queda a todo el equipo. Se usa como criterio objetivo
    // para desempatar un combate de eliminatoria que agoto las rondas.
    int total = 0;

    for (int k = 0; k < entrenador->cantidad_pokemon; k++) {
        total = total + entrenador->equipo[k].hp_actual;
    }

    return total;
}

void disputar_combate(int numero, Entrenador *player01, Entrenador *player02, char id_player01[], char id_player02[], bool permitir_empate, Partido partidos[], int cantidad_partidos){

    bool empate = false;
    int  pokemon_derrotados = 0;
    int  ronda = 0;
    int  i;
    int  j;
    Entrenador *ganador = NULL;

    if (partidos != NULL && cantidad_partidos > 0) {
        FILE *bitacora = fopen("historial_detallado.txt", "a+");

        if (bitacora == NULL) {
            ARCHIVO_ERROR;
        } else {
            // 1) Cada combate arranca con ambos equipos sanos: los daños son temporales
            curar_equipo(player01);
            curar_equipo(player02);

            // 2) El sistema elige automaticamente al primer Pokemon disponible de cada equipo
            i = seleccionar_pokemon(player01->equipo, MAX_POKEMONES_EN_EQ);
            j = seleccionar_pokemon(player02->equipo, MAX_POKEMONES_EN_EQ);

            fprintf(bitacora, "COMBATE %d: %s vs %s\n", numero, player01->nombre, player02->nombre);

            // 3) El combate se disputa ronda por ronda hasta que alguien se quede sin
            //    Pokemon disponibles o hasta agotar MAX_RONDAS_COMBATE (empate).
            while ((i != -1) && (j != -1) && (ronda < MAX_RONDAS_COMBATE)) {

                if (player01->equipo[i].velocidad > player02->equipo[j].velocidad) {
                    // Ataca primero el Pokemon de player01
                    ejecutar_turno_ataque(player01, i, player02, j, permitir_empate, &pokemon_derrotados);

                    if (player02->equipo[j].hp_actual > 0) {
                        // El defensor sobrevivio: contraataca
                        ejecutar_turno_ataque(player02, j, player01, i, permitir_empate, &pokemon_derrotados);
                    }

                } else if (player02->equipo[j].velocidad > player01->equipo[i].velocidad) {
                    // Ataca primero el Pokemon de player02
                    ejecutar_turno_ataque(player02, j, player01, i, permitir_empate, &pokemon_derrotados);

                    if (player01->equipo[i].hp_actual > 0) {
                        // El defensor sobrevivio: contraataca
                        ejecutar_turno_ataque(player01, i, player02, j, permitir_empate, &pokemon_derrotados);
                    }

                } else {
                    // Empate de velocidad: la moneda decide quien ataca primero
                    int moneda = rand() % 2;

                    if (moneda == 0) {
                        fprintf(bitacora, "[*] Empate de velocidad. La moneda favorece a %s.\n", player01->nombre);
                        ejecutar_turno_ataque(player01, i, player02, j, permitir_empate, &pokemon_derrotados);

                        if (player02->equipo[j].hp_actual > 0) {
                            ejecutar_turno_ataque(player02, j, player01, i, permitir_empate, &pokemon_derrotados);
                        }
                    } else {
                        fprintf(bitacora, "[*] Empate de velocidad. La moneda favorece a %s.\n", player02->nombre);
                        ejecutar_turno_ataque(player02, j, player01, i, permitir_empate, &pokemon_derrotados);

                        if (player01->equipo[i].hp_actual > 0) {
                            ejecutar_turno_ataque(player01, i, player02, j, permitir_empate, &pokemon_derrotados);
                        }
                    }
                }

                // Registro de los Pokemon que cayeron en esta ronda
                if (player01->equipo[i].hp_actual == 0) {
                    fprintf(bitacora, "%s vs %s GANO: %s (DERROTADO: %s)\n",
                            player01->equipo[i].nombre, player02->equipo[j].nombre,
                            player02->equipo[j].nombre, player01->equipo[i].nombre);
                }
                if (player02->equipo[j].hp_actual == 0) {
                    fprintf(bitacora, "%s vs %s GANO: %s (DERROTADO: %s)\n",
                            player01->equipo[i].nombre, player02->equipo[j].nombre,
                            player01->equipo[i].nombre, player02->equipo[j].nombre);
                }

                // Sustituciones: en la siguiente ronda entra el Pokemon que sigue
                if (player01->equipo[i].hp_actual == 0) {
                    i = seleccionar_siguiente(player01->equipo, MAX_POKEMONES_EN_EQ, i);
                }
                if (player02->equipo[j].hp_actual == 0) {
                    j = seleccionar_siguiente(player02->equipo, MAX_POKEMONES_EN_EQ, j);
                }

                ronda++;
            }

            // 4) Cierre del combate: se decide al ganador o se declara el empate.
            //    En eliminatoria (permitir_empate = false) un combate sin K.O. se
            //    resuelve por la vida total restante, porque la fase no admite tablas.
            if ((i != -1) && (j != -1)) {
                int hp_player01 = calcular_hp_total(player01);
                int hp_player02 = calcular_hp_total(player02);

                fprintf(bitacora, "SIN K.O. tras %d rondas: %s (%d HP) vs %s (%d HP)\n",
                        MAX_RONDAS_COMBATE, player01->nombre, hp_player01,
                        player02->nombre, hp_player02);

                if (permitir_empate) {
                    empate = true;
                    fprintf(bitacora, "EMPATE: %s y %s agotaron las %d rondas sin K.O.\n",
                            player01->nombre, player02->nombre, MAX_RONDAS_COMBATE);
                } else if (hp_player01 >= hp_player02) {
                    ganador = player01;
                    fprintf(bitacora, "GANADOR DEL COMBATE (desempate por HP): %s\n", player01->nombre);
                } else {
                    ganador = player02;
                    fprintf(bitacora, "GANADOR DEL COMBATE (desempate por HP): %s\n", player02->nombre);
                }

            } else if (j == -1) {
                // player02 se quedo sin Pokemon: gana player01
                ganador = player01;
                fprintf(bitacora, "GANADOR DEL COMBATE: %s\n", player01->nombre);
            } else {
                // player01 se quedo sin Pokemon: gana player02
                ganador = player02;
                fprintf(bitacora, "GANADOR DEL COMBATE: %s\n", player02->nombre);
            }

            fclose(bitacora);

            // 5) Registro oficial del resultado y de las estadisticas del combate
            if (empate) {
                player01->empates = player01->empates + 1;
                player02->empates = player02->empates + 1;
                player01->puntuacion = player01->puntuacion + 1;
                player02->puntuacion = player02->puntuacion + 1;

                registrar_partido(partidos, cantidad_partidos, numero,
                                  id_player01, id_player02, "", true);
            } else if (ganador != NULL) {
                Entrenador *perdedor = (ganador == player01) ? player02 : player01;

                // REGLAMENTO: Victoria = 3 puntos, Derrota = 0 puntos
                ganador->victorias = ganador->victorias + 1;
                ganador->puntuacion = ganador->puntuacion + 3;
                perdedor->derrotas = perdedor->derrotas + 1;

                // El 3.er criterio de desempate solo cuenta en la fase de grupos
                if (permitir_empate) {
                    ganador->pokemon_derrotados = ganador->pokemon_derrotados + pokemon_derrotados;
                }

                registrar_partido(partidos, cantidad_partidos, numero,
                                  id_player01, id_player02, ganador->id_entrenador, false);
            }
        }
    }
}

// =========================================================================
// MÓDULO 6: TORNEO (CALENDARIO, FASES, CLASIFICACION Y RESULTADOS)
// =========================================================================

// Calendario oficial de la fase de grupos: las 3 rondas de un grupo de 4
// entrenadores. Cada fila son los 2 combates de una ronda, expresados como
// desplazamientos (0 a 3) dentro del grupo.
static const int CALENDARIO_GRUPOS[3][2][2] = {
    { {0, 3}, {1, 2} },   // Ronda 1: 1-4 y 2-3
    { {0, 2}, {3, 1} },   // Ronda 2: 1-3 y 4-2
    { {0, 1}, {2, 3} }    // Ronda 3: 1-2 y 3-4
};

// Mapa de un combate de eliminatoria (49 a 64): de donde sale cada participante.
// La posicion 0 del arreglo corresponde al combate 49 y la 15 al combate 64.
//   tipo 0 = CLASIFICADO (indice dentro del arreglo de clasificados 1A,2A,1B,2B,...)
//   tipo 1 = GANADOR de otro combate
//   tipo 2 = PERDEDOR de otro combate
static const int CUADRO_ELIMINATORIA[16][2][2] = {
    { {0,  0}, {0,  3} },   // 49: 1A - 2B
    { {0,  4}, {0,  7} },   // 50: 1C - 2D
    { {0,  8}, {0, 11} },   // 51: 1E - 2F
    { {0, 12}, {0, 15} },   // 52: 1G - 2H
    { {0,  2}, {0,  1} },   // 53: 1B - 2A
    { {0,  6}, {0,  5} },   // 54: 1D - 2C
    { {0, 10}, {0,  9} },   // 55: 1F - 2E
    { {0, 14}, {0, 13} },   // 56: 1H - 2G
    { {1, 49}, {1, 50} },   // 57: G49 - G50
    { {1, 53}, {1, 54} },   // 58: G53 - G54
    { {1, 51}, {1, 52} },   // 59: G51 - G52
    { {1, 55}, {1, 56} },   // 60: G55 - G56
    { {1, 57}, {1, 58} },   // 61: G57 - G58 (semifinal)
    { {1, 59}, {1, 60} },   // 62: G59 - G60 (semifinal)
    { {2, 61}, {2, 62} },   // 63: P61 - P62 (tercer lugar)
    { {1, 61}, {1, 62} }    // 64: G61 - G62 (final)
};

const char* nombre_etapa(EtapaTorneo etapa){
    // Devuelve el nombre legible de la etapa evitando salir del arreglo.
    // Un solo punto de salida.
    const char *nombre = "Desconocida";

    if ((etapa >= ETAPA_GRUPOS) && (etapa <= ETAPA_FINAL)) {
        nombre = NOMBRES_ETAPAS[etapa];
    }

    return nombre;
}

void nombre_etapa_legible(EtapaTorneo etapa, char destino[]){
    // Copia el nombre de la etapa cambiando los guiones bajos por espacios, que es
    // como se muestra al usuario (en el archivo van sin espacios).
    const char *origen = nombre_etapa(etapa);
    int i = 0;

    while (origen[i] != '\0') {
        destino[i] = (origen[i] == '_') ? ' ' : origen[i];
        i++;
    }

    destino[i] = '\0';
}

void inicializar_partidos(Partido partidos[], int cantidad, Entrenador entrenadores[], int cantidad_entrenadores){
    // Deja los 64 combates definidos. Los 48 de la fase de grupos quedan con sus
    // participantes reales ya resueltos (grupo + calendario + ID del entrenador);
    // los de eliminatoria se completan cuando existan los resultados previos.
    for (int k = 0; k < cantidad; k++) {
        int numero = k + 1;

        partidos[k].numero = numero;
        partidos[k].finalizado = false;
        partidos[k].empate = false;
        partidos[k].grupo = -1;
        partidos[k].id_jugador1[0] = '\0';
        partidos[k].id_jugador2[0] = '\0';
        partidos[k].id_ganador[0] = '\0';
        partidos[k].id_perdedor[0] = '\0';

        if (numero <= COMBATES_GRUPOS) {
            int grupo = (numero - 1) / 6;               // 0 a 7
            int posicion = (numero - 1) % 6;            // 0 a 5 dentro del grupo
            int ronda = posicion / 2;                   // 0 a 2
            int choque = posicion % 2;                  // 0 o 1
            int base = grupo * ENTRENADORES_POR_GRUPO;
            int indice1 = base + CALENDARIO_GRUPOS[ronda][choque][0];
            int indice2 = base + CALENDARIO_GRUPOS[ronda][choque][1];

            partidos[k].etapa = ETAPA_GRUPOS;
            partidos[k].grupo = grupo;

            // Se guarda el ID real del entrenador (el mismo formato del archivo de
            // resultados), no un indice interno: asi el calendario sobrevive a
            // cargar_partidos() y a guardar_partidos().
            if (indice1 < cantidad_entrenadores) {
                strcpy(partidos[k].id_jugador1, entrenadores[indice1].id_entrenador);
            }
            if (indice2 < cantidad_entrenadores) {
                strcpy(partidos[k].id_jugador2, entrenadores[indice2].id_entrenador);
            }
        } else {
            if (numero <= 56) {
                partidos[k].etapa = ETAPA_OCTAVOS;
            } else if (numero <= 60) {
                partidos[k].etapa = ETAPA_CUARTOS;
            } else if (numero <= 62) {
                partidos[k].etapa = ETAPA_SEMIFINAL;
            } else if (numero == 63) {
                partidos[k].etapa = ETAPA_TERCER_LUGAR;
            } else {
                partidos[k].etapa = ETAPA_FINAL;
            }
        }
    }
}

int buscar_partido(Partido partidos[], int cantidad, int numero){
    // Busqueda lineal por numero de combate. Un solo punto de salida.
    int indice = -1;
    bool encontrado = false;

    for (int k = 0; k < cantidad && !encontrado; k++) {
        if (partidos[k].numero == numero) {
            indice = k;
            encontrado = true;
        }
    }

    return indice;
}

bool partido_finalizado(Partido partidos[], int cantidad, int numero){
    // Indica si un combate ya tiene resultado oficial
    bool finalizado = false;
    int indice = buscar_partido(partidos, cantidad, numero);

    if ((indice != -1) && partidos[indice].finalizado) {
        finalizado = true;
    }

    return finalizado;
}

void registrar_partido(Partido partidos[], int cantidad, int numero, char id_jugador1[], char id_jugador2[], char id_ganador[], bool empate){
    // Guarda el resultado oficial de un combate en el arreglo en RAM
    int indice = buscar_partido(partidos, cantidad, numero);

    if (indice != -1) {
        strcpy(partidos[indice].id_jugador1, id_jugador1);
        strcpy(partidos[indice].id_jugador2, id_jugador2);
        partidos[indice].empate = empate;
        partidos[indice].finalizado = true;

        if (empate) {
            partidos[indice].id_ganador[0] = '\0';
            partidos[indice].id_perdedor[0] = '\0';
        } else {
            strcpy(partidos[indice].id_ganador, id_ganador);

            if (strcmp(id_ganador, id_jugador1) == 0) {
                strcpy(partidos[indice].id_perdedor, id_jugador2);
            } else {
                strcpy(partidos[indice].id_perdedor, id_jugador1);
            }
        }
    }
}

bool obtener_participantes(int numero, Partido partidos[], int cantidad, char clasificados[][8], char id_jugador1[], char id_jugador2[]){
    // Resuelve los dos entrenadores de un combate. Un solo punto de salida.
    bool definido = false;

    id_jugador1[0] = '\0';
    id_jugador2[0] = '\0';

    if ((numero >= 1) && (numero <= COMBATES_GRUPOS)) {
        // Combates de grupos: los participantes ya vienen resueltos en el calendario
        int indice = buscar_partido(partidos, cantidad, numero);

        if (indice != -1) {
            strcpy(id_jugador1, partidos[indice].id_jugador1);
            strcpy(id_jugador2, partidos[indice].id_jugador2);

            definido = (id_jugador1[0] != '\0') && (id_jugador2[0] != '\0');
        }

    } else if ((numero > COMBATES_GRUPOS) && (numero <= CANTIDAD_PARTIDOS)) {
        // Combates de eliminatoria: se resuelven con el cuadro oficial
        int posicion = numero - COMBATES_GRUPOS - 1;

        for (int lado = 0; lado < 2; lado++) {
            int tipo = CUADRO_ELIMINATORIA[posicion][lado][0];
            int referencia = CUADRO_ELIMINATORIA[posicion][lado][1];

            if (tipo == 0) {
                // Viene de los clasificados de la fase de grupos
                if ((referencia >= 0) && (referencia < MAX_CLASIFICADOS)) {
                    strcpy((lado == 0) ? id_jugador1 : id_jugador2, clasificados[referencia]);
                }
            } else {
                // Viene del ganador o del perdedor de un combate anterior
                int indice_partido = buscar_partido(partidos, cantidad, referencia);

                if ((indice_partido != -1) && partidos[indice_partido].finalizado) {
                    if (tipo == 1) {
                        strcpy((lado == 0) ? id_jugador1 : id_jugador2, partidos[indice_partido].id_ganador);
                    } else {
                        strcpy((lado == 0) ? id_jugador1 : id_jugador2, partidos[indice_partido].id_perdedor);
                    }
                }
            }
        }

        definido = (id_jugador1[0] != '\0') && (id_jugador2[0] != '\0');

        // Se guardan tambien en el partido para que queden disponibles al mostrarlo
        if (definido) {
            int indice = buscar_partido(partidos, cantidad, numero);

            if (indice != -1) {
                strcpy(partidos[indice].id_jugador1, id_jugador1);
                strcpy(partidos[indice].id_jugador2, id_jugador2);
            }
        }
    }

    return definido;
}

bool es_id_entrenador(char texto[]){
    // Comprueba que la cadena tenga contenido: los participantes de un combate ya
    // definido siempre son IDs reales de entrenador.
    bool es_id = true;

    if (texto[0] == '\0') {
        es_id = false;
    }

    return es_id;
}

void cargar_partidos(Partido partidos[], int cantidad){
    // Lee los resultados ya disputados desde resultados_combates.txt
    FILE *archivo = fopen("resultados_combates.txt", "r");

    if (archivo != NULL) {
        char linea[200];

        while (fgets(linea, sizeof(linea), archivo) != NULL) {
            int numero;
            char etapa_texto[20];
            char resultado[15];
            char id1[8];
            char id2[8];
            char ganador[8];

            int leidos = sscanf(linea, "%d %19s %7s %7s %14s %7s",
                                &numero, etapa_texto, id1, id2, resultado, ganador);

            if ((leidos >= 5) && (numero >= 1) && (numero <= cantidad)) {
                bool empate = (strcmp(resultado, "Empate") == 0);

                if (empate) {
                    ganador[0] = '\0';
                }

                registrar_partido(partidos, cantidad, numero, id1, id2, ganador, empate);
            }
        }

        fclose(archivo);
    }
}

void guardar_partidos(Partido partidos[], int cantidad){
    // Reescribe el archivo completo: no se anexa, por lo que nunca quedan
    // registros duplicados ni resultados obsoletos.
    FILE *archivo = fopen("resultados_combates.txt", "w");

    if (archivo == NULL) {
        ARCHIVO_ERROR;
    } else {
        fprintf(archivo, "# Numero Etapa Entrenador1 Entrenador2 Resultado Ganador\n");

        for (int k = 0; k < cantidad; k++) {
            if (partidos[k].finalizado) {
                if (partidos[k].empate) {
                    fprintf(archivo, "%d %s %s %s Empate -\n",
                            partidos[k].numero, nombre_etapa(partidos[k].etapa),
                            partidos[k].id_jugador1, partidos[k].id_jugador2);
                } else {
                    fprintf(archivo, "%d %s %s %s Victoria %s\n",
                            partidos[k].numero, nombre_etapa(partidos[k].etapa),
                            partidos[k].id_jugador1, partidos[k].id_jugador2,
                            partidos[k].id_ganador);
                }
            }
        }

        fclose(archivo);
    }
}

// =========================================================================
// FASE DE GRUPOS
// =========================================================================

bool validar_participantes(Entrenador entrenadores[], int cantidad){
    // La fase de grupos exige 32 entrenadores, cada uno con su equipo completo.
    bool valido = true;

    if (cantidad != MAX_ENTRENADORES) {
        valido = false;
        printf("\n=======================================================\n");
        printf("        \033[1;31m[!] NO SE PUEDE INICIAR LA FASE DE GRUPOS\033[0m       \n");
        printf("=======================================================\n");
        printf(" Se requieren exactamente %d entrenadores y hay %d.\n", MAX_ENTRENADORES, cantidad);
        printf("=======================================================\n\n");
    } else {
        int sin_equipo = 0;
        int ids_invalidos = 0;

        // 1) Se revisan primero los IDs para no seguir validando datos invalidos
        for (int i = 0; i < cantidad; i++) {
            if (!es_id_entrenador(entrenadores[i].id_entrenador) ||
                (entrenadores[i].id_entrenador[0] == '\0')) {
                ids_invalidos++;
            }
        }

        if (ids_invalidos > 0) {
            valido = false;
            printf("\n=======================================================\n");
            printf("        \033[1;31m[!] NO SE PUEDE INICIAR LA FASE DE GRUPOS\033[0m       \n");
            printf("=======================================================\n");
            printf(" Hay %d entrenadores con un ID invalido.\n", ids_invalidos);
            printf(" Corrija el archivo entrenadores.txt antes de continuar.\n");
            printf("=======================================================\n\n");
        } else {
            // 2) Ahora se revisa que todos tengan su equipo completo y utilizable
            for (int i = 0; i < cantidad; i++) {
                int disponibles = 0;
                bool equipo_completo = (strcmp(entrenadores[i].asignaciom_equipo, "true") == 0) &&
                                       (entrenadores[i].cantidad_pokemon == MAX_POKEMONES_EN_EQ);

                // Se confirma que cada ejemplar tenga su especie y pueda combatir
                for (int p = 0; p < entrenadores[i].cantidad_pokemon; p++) {
                    if ((entrenadores[i].equipo[p].datos_especie != NULL) &&
                        (entrenadores[i].equipo[p].hp_maximo > 0)) {
                        disponibles++;
                    }
                }

                if (!equipo_completo || (disponibles != MAX_POKEMONES_EN_EQ)) {
                    sin_equipo++;
                    printf(" [!] %s (%s) no tiene un equipo valido de %d Pokemon.\n",
                           entrenadores[i].nombre, entrenadores[i].id_entrenador, MAX_POKEMONES_EN_EQ);
                }
            }

            if (sin_equipo > 0) {
                valido = false;
                printf("\n=======================================================\n");
                printf("        \033[1;31m[!] NO SE PUEDE INICIAR LA FASE DE GRUPOS\033[0m       \n");
                printf("=======================================================\n");
                printf(" Hay %d entrenadores sin equipo completo.\n", sin_equipo);
                printf(" Cree los equipos antes de disputar el torneo.\n");
                printf("=======================================================\n\n");
            }
        }
    }

    return valido;
}

bool fase_de_grupos(Entrenador vector_entrenadores_torneo[], int cantidad_entrenadores, Partido partidos[]){
    // Disputa las 3 rondas de cada uno de los 8 grupos (6 combates por grupo).
    bool jugada = false;

    if (partidos == NULL) {
        printf("\n[!] Error interno: el torneo no esta inicializado.\n");
    } else if (!validar_participantes(vector_entrenadores_torneo, cantidad_entrenadores)) {
        printf(" No se disputo ningun combate de la fase de grupos.\n\n");
    } else {
        printf("\n\033[1;33m>>> DISPUTANDO LA FASE DE GRUPOS (48 combates)...\033[0m\n");

        for (int grupo = 0; grupo < CANTIDAD_GRUPOS; grupo++) {
            int base = grupo * ENTRENADORES_POR_GRUPO;

            for (int ronda = 0; ronda < 3; ronda++) {
                for (int choque = 0; choque < 2; choque++) {
                    int numero = (grupo * 6) + (ronda * 2) + choque + 1;
                    int indice_p1 = base + CALENDARIO_GRUPOS[ronda][choque][0];
                    int indice_p2 = base + CALENDARIO_GRUPOS[ronda][choque][1];

                    disputar_combate(numero,
                                     &vector_entrenadores_torneo[indice_p1],
                                     &vector_entrenadores_torneo[indice_p2],
                                     vector_entrenadores_torneo[indice_p1].id_entrenador,
                                     vector_entrenadores_torneo[indice_p2].id_entrenador,
                                     true, partidos, CANTIDAD_PARTIDOS);
                }
            }

            printf("   Grupo %c completado (combates %d a %d).\n",
                   'A' + grupo, (grupo * 6) + 1, (grupo * 6) + 6);
        }

        guardar_partidos(partidos, CANTIDAD_PARTIDOS);
        guardar_vector_entrenador(vector_entrenadores_torneo, cantidad_entrenadores);
        jugada = true;

        printf("\n\033[1;32m[+] FASE DE GRUPOS FINALIZADA\033[0m\n\n");
    }

    return jugada;
}

// =========================================================================
// CLASIFICACION Y AVANCE A LA FASE ELIMINATORIA
// =========================================================================

int buscar_entrenador_en_grupo(Entrenador entrenadores[], int cantidad, int grupo, char id[]){
    // Devuelve la posicion (0 a 3) de un entrenador dentro de su grupo, o -1.
    // Un solo punto de salida.
    int posicion = -1;
    int base = grupo * ENTRENADORES_POR_GRUPO;

    for (int i = 0; i < ENTRENADORES_POR_GRUPO && posicion == -1; i++) {
        int indice = base + i;

        if ((indice < cantidad) && (strcmp(entrenadores[indice].id_entrenador, id) == 0)) {
            posicion = i;
        }
    }

    return posicion;
}

int comparar_tabla_grupo(const void *a, const void *b){
    // Orden oficial: 1) puntos, 2) victorias, 3) Pokemon derrotados.
    // Un solo punto de salida.
    const TablaGrupo *fila_a = (const TablaGrupo *) a;
    const TablaGrupo *fila_b = (const TablaGrupo *) b;
    int resultado = 0;

    if (fila_a->puntos != fila_b->puntos) {
        resultado = fila_b->puntos - fila_a->puntos;
    } else if (fila_a->ganados != fila_b->ganados) {
        resultado = fila_b->ganados - fila_a->ganados;
    } else if (fila_a->entrenador->pokemon_derrotados != fila_b->entrenador->pokemon_derrotados) {
        resultado = fila_b->entrenador->pokemon_derrotados - fila_a->entrenador->pokemon_derrotados;
    } else {
        // Criterio adicional de desempate: orden alfabetico por nombre
        resultado = strcmp(fila_a->entrenador->nombre, fila_b->entrenador->nombre);
    }

    return resultado;
}

void mostrar_etiqueta_clasificado(int indice, char destino[]){
    // Traduce la posicion del arreglo de clasificados a la etiqueta oficial:
    // 0 -> 1A, 1 -> 2A, 2 -> 1B, 3 -> 2B, ... 14 -> 1H, 15 -> 2H
    int grupo = indice / 2;
    int puesto = (indice % 2) + 1;

    sprintf(destino, "%d%c", puesto, 'A' + grupo);
}

bool calcular_clasificacion(Partido partidos[], int cantidad, Entrenador entrenadores[], int cantidad_entrenadores, TablaGrupo tabla[CANTIDAD_GRUPOS][ENTRENADORES_POR_GRUPO]){
    // Arma las 8 tablas de posiciones a partir de los combates ya finalizados.
    bool completa = true;
    int faltantes = 0;

    // Sin entrenadores cargados no hay nada que clasificar (evita recorrer grupos vacios)
    if ((entrenadores == NULL) || (cantidad_entrenadores < 1) || (partidos == NULL)) {
        return false;
    }

    // 1) Una fila por entrenador, ubicado en su grupo por orden de carga
    for (int i = 0; i < MAX_ENTRENADORES; i++) {
        int grupo = i / ENTRENADORES_POR_GRUPO;
        int posicion = i % ENTRENADORES_POR_GRUPO;

        tabla[grupo][posicion].entrenador = (i < cantidad_entrenadores) ? &entrenadores[i] : NULL;
        tabla[grupo][posicion].grupo = grupo;
        tabla[grupo][posicion].posicion = 0;
        tabla[grupo][posicion].puntos = 0;
        tabla[grupo][posicion].ganados = 0;
        tabla[grupo][posicion].empatados = 0;
        tabla[grupo][posicion].perdidos = 0;
    }

    // 2) Acumular los resultados de los combates de grupos
    for (int k = 0; k < cantidad; k++) {
        if ((partidos[k].numero <= COMBATES_GRUPOS) && partidos[k].finalizado) {
            int grupo = partidos[k].grupo;
            int pos1 = buscar_entrenador_en_grupo(entrenadores, cantidad_entrenadores, grupo, partidos[k].id_jugador1);
            int pos2 = buscar_entrenador_en_grupo(entrenadores, cantidad_entrenadores, grupo, partidos[k].id_jugador2);

            if (partidos[k].empate) {
                if (pos1 != -1) {
                    tabla[grupo][pos1].empatados++;
                    tabla[grupo][pos1].puntos += 1;
                }
                if (pos2 != -1) {
                    tabla[grupo][pos2].empatados++;
                    tabla[grupo][pos2].puntos += 1;
                }
            } else {
                int pos_ganador = (strcmp(partidos[k].id_ganador, partidos[k].id_jugador1) == 0) ? pos1 : pos2;
                int pos_perdedor = (pos_ganador == pos1) ? pos2 : pos1;

                if (pos_ganador != -1) {
                    tabla[grupo][pos_ganador].ganados++;
                    tabla[grupo][pos_ganador].puntos += 3;
                }
                if (pos_perdedor != -1) {
                    tabla[grupo][pos_perdedor].perdidos++;
                }
            }
        }
    }

    // 3) Ordenar cada grupo y asignar la posicion final
    for (int grupo = 0; grupo < CANTIDAD_GRUPOS; grupo++) {
        qsort(tabla[grupo], ENTRENADORES_POR_GRUPO, sizeof(TablaGrupo), comparar_tabla_grupo);

        for (int posicion = 0; posicion < ENTRENADORES_POR_GRUPO; posicion++) {
            tabla[grupo][posicion].posicion = posicion + 1;
        }
    }

    // 4) Informar si faltan combates por disputar (resultados parciales)
    for (int k = 0; k < cantidad; k++) {
        if ((partidos[k].numero <= COMBATES_GRUPOS) && !partidos[k].finalizado) {
            faltantes++;
        }
    }

    if (faltantes > 0) {
        completa = false;
        printf("\n\033[1;33m[!] AVISO:\033[0m faltan %d combates de la fase de grupos por disputar.\n", faltantes);
        printf(" La clasificacion mostrada es parcial.\n");
    }

    return completa;
}

void obtener_clasificados(TablaGrupo tabla[CANTIDAD_GRUPOS][ENTRENADORES_POR_GRUPO], char clasificados[][8]){
    // Guarda los 16 clasificados con la nomenclatura oficial: 1A, 2A, 1B, 2B, ...
    for (int grupo = 0; grupo < CANTIDAD_GRUPOS; grupo++) {
        for (int posicion = 0; posicion < 2; posicion++) {
            int indice = (grupo * 2) + posicion;

            if (tabla[grupo][posicion].entrenador != NULL) {
                strcpy(clasificados[indice], tabla[grupo][posicion].entrenador->id_entrenador);
            } else {
                clasificados[indice][0] = '\0';
            }
        }
    }
}

// =========================================================================
// FASE ELIMINATORIA
// =========================================================================

bool fase_eliminatoria(Entrenador entrenadores[], int cantidad_entrenadores, Partido partidos[], char clasificados[][8]){
    // Disputa los combates 49 a 64 siguiendo el cuadro oficial del torneo.
    bool completa = true;

    if (partidos == NULL) {
        printf("\n[!] Error interno: el torneo no esta inicializado.\n");
        completa = false;
    } else {
        printf("\n\033[1;33m>>> DISPUTANDO LA FASE ELIMINATORIA...\033[0m\n");

        for (int numero = COMBATES_GRUPOS + 1; numero <= CANTIDAD_PARTIDOS; numero++) {
            char id1[8];
            char id2[8];

            if (obtener_participantes(numero, partidos, CANTIDAD_PARTIDOS, clasificados, id1, id2)) {
                bool encontrado1;
                bool encontrado2;
                int indice1;
                int indice2;

                buscar_entrenador(entrenadores, id1, cantidad_entrenadores, &encontrado1, &indice1);
                buscar_entrenador(entrenadores, id2, cantidad_entrenadores, &encontrado2, &indice2);

                if (encontrado1 && encontrado2) {
                    int indice_partido = buscar_partido(partidos, CANTIDAD_PARTIDOS, numero);
                    char etapa_texto[30];

                    nombre_etapa_legible(partidos[indice_partido].etapa, etapa_texto);
                    printf("   %s (combate %d): %s vs %s\n", etapa_texto,
                           numero, entrenadores[indice1].nombre, entrenadores[indice2].nombre);

                    disputar_combate(numero, &entrenadores[indice1], &entrenadores[indice2],
                                     id1, id2, false, partidos, CANTIDAD_PARTIDOS);
                } else {
                    completa = false;
                    printf("   [!] El combate %d hace referencia a entrenadores inexistentes.\n", numero);
                }
            } else {
                // Los participantes aun dependen de resultados pendientes
                completa = false;
            }
        }

        guardar_partidos(partidos, CANTIDAD_PARTIDOS);
        guardar_vector_entrenador(entrenadores, cantidad_entrenadores);

        if (completa) {
            printf("\n\033[1;32m[+] FASE ELIMINATORIA FINALIZADA\033[0m\n\n");
        } else {
            printf("\n\033[1;33m[!] La fase eliminatoria quedo incompleta:\033[0m faltan resultados previos.\n\n");
        }
    }

    return completa;
}

// =========================================================================
// PANTALLAS DEL TORNEO
// =========================================================================

void buscar_nombre_entrenador(Entrenador entrenadores[], int cantidad, char id[], char destino[]){
    // Copia en 'destino' el nombre del entrenador con ese ID. Si no existe deja
    // el propio ID para que la pantalla nunca muestre un espacio vacio.
    bool encontrado;
    int indice;

    buscar_entrenador(entrenadores, id, cantidad, &encontrado, &indice);

    if (encontrado) {
        strcpy(destino, entrenadores[indice].nombre);
    } else if (!es_id_entrenador(id)) {
        strcpy(destino, "Por definir");
    } else {
        strcpy(destino, id);
    }
}

void mostrar_etiqueta_cruce(int posicion, char destino[]){
    // Etiqueta de un cruce de eliminatoria cuando todavia no hay clasificados:
    // 0..15 son los clasificados (1A, 2A, ...) y 49..64 son ganadores o perdedores.
    if ((posicion >= 0) && (posicion < MAX_CLASIFICADOS)) {
        mostrar_etiqueta_clasificado(posicion, destino);
    } else {
        sprintf(destino, "Ganador %d", posicion);
    }
}

void mostrar_enfrentamientos(Partido partidos[], int cantidad, Entrenador entrenadores[], int cantidad_entrenadores){
    // Muestra el cuadro de la fase eliminatoria y quien debe jugar cada combate.
    TablaGrupo tabla[CANTIDAD_GRUPOS][ENTRENADORES_POR_GRUPO];
    char clasificados[MAX_CLASIFICADOS][8];

    // Se recalcula la clasificacion para conocer los 16 clasificados actuales
    calcular_clasificacion(partidos, cantidad, entrenadores, cantidad_entrenadores, tabla);
    obtener_clasificados(tabla, clasificados);

    limpiar_consola();
    printf("\n\033[1;36m========================================================================================\033[0m\n");
    printf("                     \033[1;33mENFRENTAMIENTOS DE LA FASE ELIMINATORIA\033[0m\n");
    printf("\033[1;36m========================================================================================\033[0m\n");
    printf(" \033[1;33m%-5s | %-17s | %-22s | %-22s | %-10s\033[0m\n",
           "N.", "ETAPA", "ENTRENADOR 1", "ENTRENADOR 2", "ESTADO");
    printf("----------------------------------------------------------------------------------------\n");

    for (int numero = COMBATES_GRUPOS + 1; numero <= CANTIDAD_PARTIDOS; numero++) {
        int indice = buscar_partido(partidos, cantidad, numero);

        if (indice != -1) {
            char id1[8];
            char id2[8];
            char nombre1[30];
            char nombre2[30];
            char etapa_texto[30];
            int posicion_cuadro = numero - COMBATES_GRUPOS - 1;
            bool definido = obtener_participantes(numero, partidos, cantidad, clasificados, id1, id2);

            nombre_etapa_legible(partidos[indice].etapa, etapa_texto);

            if (definido && es_id_entrenador(id1) && es_id_entrenador(id2)) {
                buscar_nombre_entrenador(entrenadores, cantidad_entrenadores, id1, nombre1);
                buscar_nombre_entrenador(entrenadores, cantidad_entrenadores, id2, nombre2);
            } else {
                // Sin clasificados definidos se muestran las etiquetas del cuadro (1A, 2B, ...)
                mostrar_etiqueta_cruce(CUADRO_ELIMINATORIA[posicion_cuadro][0][1], nombre1);
                mostrar_etiqueta_cruce(CUADRO_ELIMINATORIA[posicion_cuadro][1][1], nombre2);
            }

            printf(" %-5d | %-17s | %-22s | %-22s | %-10s\n",
                   numero, etapa_texto, nombre1, nombre2,
                   partidos[indice].finalizado ? "Jugado" : "Pendiente");
        }
    }

    printf("\033[1;36m========================================================================================\033[0m\n\n");
}

void mostrar_clasificacion(TablaGrupo tabla[CANTIDAD_GRUPOS][ENTRENADORES_POR_GRUPO]){
    // Imprime las 8 tablas de posiciones con los criterios de desempate.
    limpiar_consola();
    printf("\n\033[1;36m========================================================================================\033[0m\n");
    printf("                              \033[1;33mCLASIFICACION DE LA FASE DE GRUPOS\033[0m\n");
    printf("\033[1;36m========================================================================================\033[0m\n");

    for (int grupo = 0; grupo < CANTIDAD_GRUPOS; grupo++) {
        printf("\n \033[1;33m--- GRUPO %c ---\033[0m\n", 'A' + grupo);
        printf(" %-4s | %-8s | %-20s | %-3s | %-3s | %-3s | %-4s | %-4s | %-9s\n",
               "POS", "ID", "NOMBRE", "W", "T", "L", "PTS", "PKD", "ESTADO");
        printf(" -----------------------------------------------------------------------------------\n");

        for (int posicion = 0; posicion < ENTRENADORES_POR_GRUPO; posicion++) {
            if (tabla[grupo][posicion].entrenador != NULL) {
                Entrenador *ent = tabla[grupo][posicion].entrenador;

                // Los acumulados se leen de la tabla, que se recalcula siempre a partir
                // de los combates: asi la pantalla es correcta incluso al reabrir el programa.
                printf(" %-4d | %-8s | %-20s | %-3d | %-3d | %-3d | %-4d | %-4d | %-9s\n",
                       tabla[grupo][posicion].posicion, ent->id_entrenador, ent->nombre,
                       tabla[grupo][posicion].ganados, tabla[grupo][posicion].empatados,
                       tabla[grupo][posicion].perdidos, tabla[grupo][posicion].puntos,
                       ent->pokemon_derrotados,
                       (posicion < 2) ? "\033[1;32mCLASIFICA\033[0m" : "");
            }
        }
    }

    printf("\n W = ganados, T = empatados, L = perdidos, PTS = puntos, PKD = Pokemon derrotados\n");
    printf("\033[1;36m========================================================================================\033[0m\n\n");
}

void mostrar_resultados_torneo(Partido partidos[], int cantidad, Entrenador entrenadores[], int cantidad_entrenadores){
    // Imprime todos los combates con resultado, agrupados por etapa.
    limpiar_consola();
    printf("\n\033[1;36m========================================================================================\033[0m\n");
    printf("                                \033[1;33mRESULTADOS DEL TORNEO\033[0m\n");
    printf("\033[1;36m========================================================================================\033[0m\n");

    for (int etapa = ETAPA_GRUPOS; etapa <= ETAPA_FINAL; etapa++) {
        int jugados = 0;
        char etapa_texto[30];

        nombre_etapa_legible((EtapaTorneo) etapa, etapa_texto);
        printf("\n \033[1;33m--- %s ---\033[0m\n", etapa_texto);

        for (int k = 0; k < cantidad; k++) {
            if ((partidos[k].etapa == (EtapaTorneo) etapa) && partidos[k].finalizado) {
                char nombre1[30];
                char nombre2[30];
                char nombre_ganador[30];

                buscar_nombre_entrenador(entrenadores, cantidad_entrenadores, partidos[k].id_jugador1, nombre1);
                buscar_nombre_entrenador(entrenadores, cantidad_entrenadores, partidos[k].id_jugador2, nombre2);

                if (partidos[k].empate) {
                    strcpy(nombre_ganador, "Empate");
                } else {
                    buscar_nombre_entrenador(entrenadores, cantidad_entrenadores, partidos[k].id_ganador, nombre_ganador);
                }

                printf("   %-3d | %-20s vs %-20s | \033[1;32m%s\033[0m\n",
                       partidos[k].numero, nombre1, nombre2, nombre_ganador);
                jugados++;
            }
        }

        if (jugados == 0) {
            printf("   (sin resultados registrados)\n");
        }
    }

    printf("\n\033[1;36m========================================================================================\033[0m\n\n");
}

void mostrar_campeon(Partido partidos[], int cantidad, Entrenador entrenadores[], int cantidad_entrenadores){
    // Muestra los cuatro primeros lugares del torneo.
    int indice_final = buscar_partido(partidos, cantidad, 64);
    int indice_tercero = buscar_partido(partidos, cantidad, 63);

    limpiar_consola();
    printf("\n\033[1;36m========================================================================================\033[0m\n");
    printf("                            \033[1;33mPODIO DEL GRAN TORNEO POKEMON\033[0m\n");
    printf("\033[1;36m========================================================================================\033[0m\n");

    if ((indice_final != -1) && partidos[indice_final].finalizado && !partidos[indice_final].empate) {
        char campeon[30];
        char subcampeon[30];

        buscar_nombre_entrenador(entrenadores, cantidad_entrenadores, partidos[indice_final].id_ganador, campeon);
        buscar_nombre_entrenador(entrenadores, cantidad_entrenadores, partidos[indice_final].id_perdedor, subcampeon);

        printf("  \033[1;33m1er lugar (CAMPEON)\033[0m    : \033[1;32m%s\033[0m\n", campeon);
        printf("  \033[1;37m2do lugar (SUBCAMPEON)\033[0m  : %s\n", subcampeon);
    } else {
        printf("  \033[1;33m[!]\033[0m La final (combate 64) todavia no se ha disputado.\n");
    }

    if ((indice_tercero != -1) && partidos[indice_tercero].finalizado && !partidos[indice_tercero].empate) {
        char tercero[30];
        char cuarto[30];

        buscar_nombre_entrenador(entrenadores, cantidad_entrenadores, partidos[indice_tercero].id_ganador, tercero);
        buscar_nombre_entrenador(entrenadores, cantidad_entrenadores, partidos[indice_tercero].id_perdedor, cuarto);

        printf("  \033[1;33m3er lugar\033[0m               : %s\n", tercero);
        printf("  \033[1;37m4to lugar\033[0m               : %s\n", cuarto);
    } else {
        printf("  \033[1;33m[!]\033[0m El combate por el tercer lugar (combate 63) todavia no se ha disputado.\n");
    }

    printf("\033[1;36m========================================================================================\033[0m\n\n");
}

void cargar_resultados_teclado(Partido partidos[], int cantidad, char clasificados[][8]){
    // Permite completar a mano los combates que falten, validando el resultado.
    int numero;

    limpiar_consola();
    imprimir_banner("CARGAR RESULTADO POR TECLADO");
    printf("  Indique el numero de combate (1 a %d) o 0 para volver.\n", CANTIDAD_PARTIDOS);
    printf("  Se validan los participantes antes de aceptar el ganador.\n\n");
    printf(" -> Combate: ");
    // Lectura segura: una entrada no numerica no deja el buffer trabado
    numero = leer_entero(-1);

    if (numero == 0) {
        printf("\n Regresando al menu principal...\n\n");
    } else if ((numero < 1) || (numero > cantidad)) {
        printf("\n\033[1;31m[!] ERROR:\033[0m el numero de combate debe estar entre 1 y %d.\n\n", cantidad);
    } else {
        int indice = buscar_partido(partidos, cantidad, numero);

        if (indice == -1) {
            printf("\n\033[1;31m[!] ERROR:\033[0m el combate %d no existe en el torneo.\n\n", numero);
        } else if (partidos[indice].finalizado) {
            printf("\n\033[1;33m[!] AVISO:\033[0m el combate %d ya tiene resultado registrado.\n\n", numero);
        } else {
            char id1[8];
            char id2[8];

            if (!obtener_participantes(numero, partidos, cantidad, clasificados, id1, id2)) {
                printf("\n\033[1;31m[!] ERROR:\033[0m el combate %d depende de resultados que aun no existen.\n", numero);
                printf(" Cargue primero los combates anteriores de esa ronda.\n\n");
            } else if (!es_id_entrenador(id1) || !es_id_entrenador(id2)) {
                printf("\n\033[1;31m[!] ERROR:\033[0m el combate %d todavia no tiene participantes definidos.\n\n", numero);
            } else {
                char leido[8];
                char etapa_texto[30];

                nombre_etapa_legible(partidos[indice].etapa, etapa_texto);
                printf("\n Combate %d (%s)\n", numero, etapa_texto);
                printf("   Entrenador 1: %s\n", id1);
                printf("   Entrenador 2: %s\n", id2);
                printf(" -> Ingrese el ID del GANADOR: ");
                leer_texto(leido, sizeof(leido));

                if (strcmp(leido, id1) != 0 && strcmp(leido, id2) != 0) {
                    printf("\n\033[1;31m[!] ERROR:\033[0m el ganador debe ser uno de los dos participantes.\n\n");
                } else {
                    registrar_partido(partidos, cantidad, numero, id1, id2, leido, false);
                    guardar_partidos(partidos, cantidad);

                    printf("\n\033[1;32m[+] Combate %d registrado. Ganador: %s\033[0m\n\n", numero, leido);
                }
            }
        }
    }
}

int cargar_resultados_archivo(Partido partidos[], int cantidad, char clasificados[][8], char nombre_archivo[]){
    // Lee resultados desde un archivo de texto con el formato: numero id_ganador
    FILE *archivo = fopen(nombre_archivo, "r");
    int registrados = 0;
    int rechazados = 0;
    int numero_linea = 0;
    char linea[200];

    limpiar_consola();
    imprimir_banner("CARGAR RESULTADOS DESDE ARCHIVO");
    printf("  Archivo: \033[1;37m%s\033[0m\n", nombre_archivo);
    printf("  Formato de cada linea: \033[1;33mnumero_combate id_ganador\033[0m\n\n");

    if (archivo == NULL) {
        printf("\033[1;31m[!] ERROR:\033[0m no se pudo abrir el archivo '%s'.\n", nombre_archivo);
        printf(" Cree el archivo en la misma carpeta del programa e intente de nuevo.\n\n");
        return 0;
    }

    imprimir_separador();

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        int numero = 0;
        char id_ganador[8];
        char id1[8];
        char id2[8];

        numero_linea++;

        // Se ignoran las lineas de comentario y las vacias
        if ((linea[0] == '#') || (linea[0] == '\n') || (linea[0] == '\r')) {
            continue;
        }

        if (sscanf(linea, "%d %7s", &numero, id_ganador) != 2) {
            printf("  \033[1;31m[x]\033[0m linea %d: formato invalido (se espera: numero id_ganador)\n", numero_linea);
            rechazados++;
            continue;
        }

        if ((numero < 1) || (numero > cantidad)) {
            printf("  \033[1;31m[x]\033[0m linea %d: el combate %d no existe (1 a %d)\n", numero_linea, numero, cantidad);
            rechazados++;
            continue;
        }

        if (partido_finalizado(partidos, cantidad, numero)) {
            printf("  \033[1;33m[-]\033[0m linea %d: el combate %d ya tenia resultado (se omite)\n", numero_linea, numero);
            rechazados++;
            continue;
        }

        if (!obtener_participantes(numero, partidos, cantidad, clasificados, id1, id2)) {
            printf("  \033[1;31m[x]\033[0m linea %d: el combate %d depende de resultados anteriores\n", numero_linea, numero);
            rechazados++;
            continue;
        }

        if ((!es_id_entrenador(id1) || !es_id_entrenador(id2))) {
            printf("  \033[1;31m[x]\033[0m linea %d: el combate %d no tiene participantes definidos\n", numero_linea, numero);
            rechazados++;
            continue;
        }

        if ((strcmp(id_ganador, id1) != 0) && (strcmp(id_ganador, id2) != 0)) {
            printf("  \033[1;31m[x]\033[0m linea %d: '%s' no juega el combate %d (%s vs %s)\n",
                   numero_linea, id_ganador, numero, id1, id2);
            rechazados++;
            continue;
        }

        registrar_partido(partidos, cantidad, numero, id1, id2, id_ganador, false);
        registrados++;
        printf("  \033[1;32m[+]\033[0m linea %d: combate %d -> gana %s\n", numero_linea, numero, id_ganador);
    }

    fclose(archivo);

    if (registrados > 0) {
        guardar_partidos(partidos, cantidad);
    }

    imprimir_separador();
    printf("\n  Combates registrados ...: \033[1;32m%d\033[0m\n", registrados);
    printf("  Lineas rechazadas ......: \033[1;31m%d\033[0m\n\n", rechazados);

    return registrados;
}

void menu_cargar_resultados(Partido partidos[], int cantidad, char clasificados[][8], Entrenador entrenadores[], int cantidad_entrenadores){
    // Submenu: cargar un resultado por teclado o varios desde un archivo.
    int opcion;
    int jugados = 0;
    char nombre_archivo[60];

    for (int k = 0; k < cantidad; k++) {
        if (partidos[k].finalizado) {
            jugados++;
        }
    }

    limpiar_consola();
    imprimir_banner("CARGAR RESULTADOS");
    printf("\n  Combates del torneo ...........: \033[1;37m%d\033[0m\n", cantidad);
    printf("  Combates con resultado ........: \033[1;32m%d\033[0m\n", jugados);
    printf("  Combates pendientes ...........: \033[1;33m%d\033[0m\n", cantidad - jugados);
    printf("\n");
    imprimir_separador();
    printf("  \033[1;33m1.\033[0m Cargar un combate por teclado\n");
    printf("  \033[1;33m2.\033[0m Cargar varios combates desde un archivo de texto\n");
    printf("  \033[1;33m3.\033[0m Ver el formato del archivo de resultados\n");
    printf("  \033[1;33m4.\033[0m Regresar al menu principal\n");
    imprimir_separador();
    printf(" -> Seleccione una opcion: ");
    opcion = leer_entero(-1);

    switch (opcion) {
        case 1:
            cargar_resultados_teclado(partidos, cantidad, clasificados);
            break;
        case 2:
            printf("\n -> Nombre del archivo (ej. resultados_entrada.txt): ");
            leer_texto(nombre_archivo, sizeof(nombre_archivo));
            cargar_resultados_archivo(partidos, cantidad, clasificados, nombre_archivo);
            break;
        case 3:
            limpiar_consola();
            imprimir_banner("FORMATO DEL ARCHIVO DE RESULTADOS");
            printf("\n  Escriba una linea por combate con el numero y el ID del ganador:\n\n");
            printf("     \033[1;33m1 001\033[0m        (en el combate 1 gana el entrenador 001)\n");
            printf("     \033[1;33m2 003\033[0m        (en el combate 2 gana el entrenador 003)\n");
            printf("     \033[1;33m49 005\033[0m       (en octavos, combate 49, gana el 005)\n\n");
            printf("  Reglas que valida el sistema:\n");
            printf("   - El combate debe existir (1 a %d).\n", cantidad);
            printf("   - El combate no debe tener ya un resultado.\n");
            printf("   - Sus participantes deben estar definidos.\n");
            printf("   - El ganador tiene que ser uno de los dos participantes.\n");
            printf("  Las lineas que empiezan con # se ignoran como comentarios.\n\n");
            break;
        case 4:
            break;
        default:
            printf("\n\033[1;31m[!] Opcion no valida.\033[0m\n\n");
            break;
    }

    (void) entrenadores;
    (void) cantidad_entrenadores;
}

void mostrar_historial_combates(void){
    // Muestra el contenido del historial detallado generado por el motor de combate.
    FILE *archivo = fopen("historial_detallado.txt", "r");
    char linea[300];

    limpiar_consola();
    imprimir_banner("HISTORIAL DETALLADO DE COMBATES");

    if (archivo == NULL) {
        printf("\n\033[1;33m[!]\033[0m Todavia no existe historial: no se ha disputado ningun combate.\n\n");
    } else {
        printf("\n");
        while (fgets(linea, sizeof(linea), archivo) != NULL) {
            printf(" %s", linea);
        }
        fclose(archivo);
        imprimir_separador();
        printf("\n");
    }
}
