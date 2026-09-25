/**
 * @file funciones.c
 * @brief Implementación lógica de la Pokédex y utilidades del sistema.
 */
#include <stdio.h>
#include <string.h>
#include "funciones.h"
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

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
        };
    }
    fclose(archivo_entrada);
}

void imprimir_vector_pokemon(especies_pokemons vector[], int cantidad) {
    printf("\n================================ POKEDEX ================================\n");
    printf("%-5s | %-12s | %-10s | %-10s | %-4s | %-4s | %-4s | %-4s\n", 
           "ID", "Nombre", "Tipo 1", "Tipo 2", "HP", "ATQ", "DEF", "VEL");
    printf("-------------------------------------------------------------------------\n");

    for(int j = 0; j < cantidad; j++) {
        printf("%-5s | %-12s | %-10s | %-10s | %-4i | %-4i | %-4i | %-4i\n",
               vector[j].ID, vector[j].Nombre, vector[j].tipo_primario, vector[j].tipo_segundario,
               vector[j].HP_b, vector[j].ataque_b, vector[j].defensa_b, vector[j].velocidad_b);
    }
    printf("=========================================================================\n");
}

void mostrar_estadisticas_pokemon(especies_pokemons vector[], int indice){
    printf("\n=======================================================\n");
    printf("                  DATOS DEL POKEMON                    \n");
    printf("=======================================================\n");
    printf("%-15s %-10s | %-15s %-15s\n", "ID:", vector[indice].ID, "Nombre:", vector[indice].Nombre);
    printf("%-15s %-10s | %-15s %-15s\n", "Tipo Primario:", vector[indice].tipo_primario, "Tipo Secundario:", vector[indice].tipo_segundario);
    printf("-------------------------------------------------------\n");
    printf("                  ESTADISTICAS BASE                    \n");
    printf("-------------------------------------------------------\n");
    printf("%-15s %-10i | %-15s %-10i\n", "HP:", vector[indice].HP_b, "Defensa:", vector[indice].defensa_b);
    printf("%-15s %-10i | %-15s %-10i\n", "Ataque:", vector[indice].ataque_b, "Velocidad:", vector[indice].velocidad_b);
    printf("=======================================================\n\n");
}

void buscar_vector_pokemon(especies_pokemons vector[], char string[], bool campo, int cantidad, bool *bandera, int *indice){
    *bandera = true;
    int i = 0;
    while(*bandera && (i < cantidad) ){
        if( campo && (strcmp(vector[i].ID, string) == 0) ){
            *bandera = false;
            *indice = i;
        }else if(strcmp(vector[i].Nombre, string) == 0){
            *bandera = false;
            *indice = i;
        }
        i++;
    }
}

void consultar_pokedex_id(especies_pokemons vector[], int cantidad) {
    bool bandera;
    int indice;
    char id_buscar[8];
    printf("\n-> Ingrese el ID (Numero) del Pokemon a buscar: ");
    scanf("%s",id_buscar);

    buscar_vector_pokemon(vector,id_buscar,true,cantidad,&bandera,&indice);
    limpiar_consola();

    if(!bandera){
        mostrar_estadisticas_pokemon(vector, atoi(vector[indice].ID) - 1);
    }else{
        printf("\n=======================================================\n");
        printf("            [!] ERROR: POKEMON NO ENCONTRADO           \n");
        printf("=======================================================\n");
        printf(" El numero ingresado no esta registrado en la Pokedex.\n");
        printf("=======================================================\n\n");
    }
}

void consultar_pokedex_nombre(especies_pokemons vector[], int cantidad) {
    bool bandera;
    int indice;
    char nombre_buscar[20];
    printf("\n-> Ingrese el nombre exacto del Pokemon: ");
    scanf("%19s", nombre_buscar);

    buscar_vector_pokemon(vector,nombre_buscar,false,MAX_POKEDEX,&bandera,&indice);
    limpiar_consola();

    if(!bandera){
        mostrar_estadisticas_pokemon(vector, atoi(vector[indice].ID) - 1);
    }else{
        printf("\n=======================================================\n");
        printf("            [!] ERROR: POKEMON NO ENCONTRADO           \n");
        printf("=======================================================\n");
        printf(" No existe ninguna especie registrada con ese nombre.\n");
        printf("=======================================================\n\n");
    }
}

void menu_consultar_pokedex(especies_pokemons vector_universo_pokemon[]) {
    int opcion_pokedex;
    limpiar_consola();
    printf("\n=======================================================\n");
    printf("                  CONSULTAR POKEDEX                    \n");
    printf("=======================================================\n");
    printf(" 1. Mostrar la Pokedex completa\n");
    printf(" 2. Buscar Pokemon por numero (ID)\n");
    printf(" 3. Buscar Pokemon por nombre\n");
    printf(" 4. Regresar al menu principal\n");
    printf("=======================================================\n");
    printf(" -> Seleccione una opcion: ");
    scanf("%d", &opcion_pokedex);

    switch(opcion_pokedex) {
        case 1:
            imprimir_vector_pokemon(vector_universo_pokemon, MAX_POKEDEX);
            break;
        case 2:
            consultar_pokedex_id(vector_universo_pokemon, MAX_POKEDEX);
            break;
        case 3:
            consultar_pokedex_nombre(vector_universo_pokemon, MAX_POKEDEX);
            break;
        case 4:
            break;
        default:
            printf("\n[!] Opcion no valida.\n");
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
        while(fscanf(entrada_archivo,"%s %s %d %d %d %d %d %s\n",vector[*i].id_entrenador,vector[*i].nombre,&vector[*i].cantidad_pokemon,&vector[*i].victorias,&vector[*i].empates,&vector[*i].derrotas,&vector[*i].puntuacion,vector[*i].asignaciom_equipo) == 8){
                (*i)++;
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
                fprintf(entrada_archivo,"%s %s %d %d %d %d %d %s\n", vector[j].id_entrenador, vector[j].nombre, vector[j].cantidad_pokemon, vector[j].victorias, vector[j].empates, vector[j].derrotas, vector[j].puntuacion, vector[j].asignaciom_equipo);
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
        strcpy(info_entrenador.asignaciom_equipo, "false"); 

        printf("-> Ingrese el nombre del entrenador: ");
        scanf("%s", info_entrenador.nombre);

        do {
            repetido = false;
            printf("-> Ingrese el ID identificador: ");
            scanf("%s", info_entrenador.id_entrenador);

            for(int i = 0; i < *cantidad; i++){
                if(strcmp(vector[i].id_entrenador, info_entrenador.id_entrenador) == 0){
                    repetido = true;
                }
            }

            if(repetido){
                printf("\n\033[1;31m[!] ERROR: ID DUPLICADO.\033[0m Ya existe un entrenador con ese ID.\n");
                printf(" Presione 'C' para intentar con otro o 'S' para salir.\n -> Opcion: ");
                scanf(" %c", &opcion);
                if(toupper(opcion) == 'S') return; 
            } else {
                vector[*cantidad] = info_entrenador;
                (*cantidad)++;

                FILE *archivo_entrenadores = fopen("entrenadores.txt", "a+");
                if(archivo_entrenadores != NULL){
                    fprintf(archivo_entrenadores,"%s %s %d %d %d %d %d %s\n", 
                            info_entrenador.id_entrenador, info_entrenador.nombre, 
                            info_entrenador.cantidad_pokemon, info_entrenador.victorias, 
                            info_entrenador.empates, info_entrenador.derrotas, 
                            info_entrenador.puntuacion, info_entrenador.asignaciom_equipo);
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
        return true;
    } else {
        return false;
    }
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

void consulta_crear_equipo_pokemon(especies_pokemons vector_universo_pokemon[], Entrenador vector_entrenadores[], int cantidad_entrenadores){
    EjemplarPokemon equipo_pokemon[MAX_POKEMONES_EN_EQ];
    int vector_baraja[MAX_POKEDEX];
    bool encontrado;
    char id_elegido[8];
    int indice;

    mostrar_seleccion_entrenadores(vector_entrenadores, cantidad_entrenadores);
    printf("\n -> Ingrese el ID del entrenador para crearle un equipo: ");
    scanf("%s", id_elegido);

    buscar_entrenador(vector_entrenadores, id_elegido, cantidad_entrenadores, &encontrado, &indice);
    
    if(encontrado){
        if(strcmp(vector_entrenadores[indice].asignaciom_equipo, "true") == 0){
            printf("\n\033[1;33m[!] ADVERTENCIA:\033[0m El entrenador '%s' ya tiene un equipo asignado.\n", vector_entrenadores[indice].nombre);
            return;
        }

        inicializar_barajas(vector_baraja);
        mezclar_baraja(vector_baraja, MAX_POKEDEX);
        limpiar_consola();

        if (crear_equipo_pokemon(0, 0, vector_baraja, equipo_pokemon, vector_universo_pokemon)) {
            crear_estadisticas_ejemplares(equipo_pokemon, MAX_POKEMONES_EN_EQ);
            guardar_equipo_pokemon(vector_entrenadores, indice, equipo_pokemon);
            strcpy(vector_entrenadores[indice].asignaciom_equipo, "true"); 
            guardar_vector_entrenador(vector_entrenadores, cantidad_entrenadores);
            
            printf("\n======================================================================================\n");
            printf("                            \033[1;32m[+] EQUIPO CREADO EXITOSAMENTE\033[0m                        \n");
            printf("=======================================================================================\n");
            imprimir_equipo_pokemon(vector_entrenadores[indice].equipo, MAX_POKEMONES_EN_EQ);
        } else {
            printf("\n=======================================================\n");
            printf("             \033[1;31m[-] ERROR AL ENSAMBLAR EQUIPO\033[0m             \n");
            printf("=======================================================\n\n");
        }
    } else {
        printf("\n=======================================================\n");
        printf("          \033[1;31m[!] ERROR: ENTRENADOR NO ENCONTRADO\033[0m          \n");
        printf("=======================================================\n");
        printf(" El ID ingresado no coincide con ningun participante\n");
        printf(" registrado en la base de datos del torneo.\n");
        printf("=======================================================\n\n");
    }
}