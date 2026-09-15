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


void llenar_vector_pokemon(especies_pokemons vector[], int *i){
    //variales
    FILE *archivo_entrada;
    char linea[150];
    char id[6];
    char nombre[15];
    char tipo_p[15];
    char tipo_s[15];
    int hp;
    int ataque;
    int defensa;
    int velocidad;

    // abrimos archivo de entrada
    archivo_entrada = fopen("pokedex.txt", "r");
    
    // validamos la apertura del archivo
    if(archivo_entrada == NULL){
        ARCHIVO_ERROR;
    }else{

        //leamos linea a linea con el ciclo
        while(fgets(linea, sizeof(linea), archivo_entrada) != NULL){
            
            // si es un pokemon que no tiene tipo segundario entra al bloque if 
            if(sscanf(linea,"%s %s %s %i %i %i %i",id,nombre,tipo_p,&hp,&ataque,&defensa,&velocidad) == 7){
                // llenamos el campo de tipo segundario con Ninguno para indicar que no tiene
                strcpy(vector[*i].tipo_segundario, "Ninguno");
            }else{
                // si no es un pokemon que tiene tipo primario y segundario
                sscanf(linea,"%s %s %s %s %i %i %i %i",id,nombre,tipo_p,tipo_s,&hp,&ataque,&defensa,&velocidad);
                strcpy(vector[*i].tipo_segundario,tipo_s);
            }

            strcpy(vector[*i].ID,id);
            strcpy(vector[*i].Nombre,nombre);
            strcpy(vector[*i].tipo_primario,tipo_p);
            
            vector[*i].HP_b = hp;
            vector[*i].ataque_b = ataque;
            vector[*i].defensa_b = defensa;
            vector[*i].velocidad_b = velocidad;

            (*i)++;
        };
    }
    fclose(archivo_entrada);
};


void imprimir_vector_pokemon(especies_pokemons vector[], int cantidad) {
    // imprimimos el encabezado de la tabla
    printf("\n================================ POKEDEX ================================\n");
    printf("%-5s | %-12s | %-10s | %-10s | %-4s | %-4s | %-4s | %-4s\n", 
           "ID", "Nombre", "Tipo 1", "Tipo 2", "HP", "ATQ", "DEF", "VEL");
    printf("-------------------------------------------------------------------------\n");

    // leemos el vector elemento por elemento con el ciclo
    for(int j = 0; j < cantidad; j++) {
        
        // imprimimos cada campo respetando el ancho de las columnas
        printf("%-5s | %-12s | %-10s | %-10s | %-4i | %-4i | %-4i | %-4i\n",
               vector[j].ID,
               vector[j].Nombre,
               vector[j].tipo_primario,
               vector[j].tipo_segundario,
               vector[j].HP_b,
               vector[j].ataque_b,
               vector[j].defensa_b,
               vector[j].velocidad_b);
    }
    
    // cerramos la tabla
    printf("=========================================================================\n");
};


void mostrar_estadisticas_pokemon(especies_pokemons vector[], int indice){

    // imprimimos la informacion con formato profesional tipo ficha
    printf("\n=======================================================\n");
    printf("                  DATOS DEL POKEMON                    \n");
    printf("=======================================================\n");
    
    // Fila 1: ID y Nombre
    printf("%-15s %-10s | %-15s %-15s\n", "ID:", vector[indice].ID, "Nombre:", vector[indice].Nombre);
    
    // Fila 2: Tipos
    printf("%-15s %-10s | %-15s %-15s\n", "Tipo Primario:", vector[indice].tipo_primario, "Tipo Secundario:", vector[indice].tipo_segundario);
    
    printf("-------------------------------------------------------\n");
    printf("                  ESTADISTICAS BASE                    \n");
    printf("-------------------------------------------------------\n");
    
    // Fila 3: HP y Defensa
    printf("%-15s %-10i | %-15s %-10i\n", "HP:", vector[indice].HP_b, "Defensa:", vector[indice].defensa_b);
    
    // Fila 4: Ataque y Velocidad
    printf("%-15s %-10i | %-15s %-10i\n", "Ataque:", vector[indice].ataque_b, "Velocidad:", vector[indice].velocidad_b);
    
    printf("=======================================================\n\n");

};



void buscar_pokemon_por_id(especies_pokemons vector[], int id_usuario, int cantidad){
    
    //validamos que el id este en el rango
    if((id_usuario > 0) && (id_usuario <= cantidad)){
        mostrar_estadisticas_pokemon(vector,id_usuario - 1);
    }else{
        printf("\n=======================================================\n");
        printf("            [!] ERROR: POKEMON NO ENCONTRADO           \n");
        printf("=======================================================\n");
        printf(" El numero ingresado no esta registrado en la Pokedex.\n");
        printf("=======================================================\n\n");
    }
};


void buscar_pokemon_nombre(especies_pokemons vector[], char nombre_usuario[], int cantidad){

    bool bandera = true;
    int i = 0;
    
    // buscamos hasta que se encuentre el nombre 
    // o hasta que lleguemos al final del vector
    while(bandera && (i < cantidad) ){
        // si encontramos el pokemon nos detenemos y 
        //llamamos al procedimiento que muestra las estadisticas
        if(strcmp(vector[i].Nombre, nombre_usuario) == 0){
            bandera = false;
            mostrar_estadisticas_pokemon(vector, atoi(vector[i].ID) - 1);
        }
        i++;
    }

    if(bandera){
        printf("\n=======================================================\n");
        printf("            [!] ERROR: POKEMON NO ENCONTRADO           \n");
        printf("=======================================================\n");
        printf(" No existe ninguna especie registrada con ese nombre.\n");
        printf("=======================================================\n\n");
    }
};



void registrar_entrenador(){
    
    //var
    FILE *archivo_entrenadores;
    bool resultado;
    Entrenador info_entrenador;\
    char opcion;

    // Encabezado visual de la planilla
    printf("\n=======================================================\n");
    printf("             INSCRIPCION DE NUEVO ENTRENADOR           \n");
    printf("=======================================================\n");

    // Inicializamos en cero para evitar basura en la memoria
    info_entrenador.cantidad_pokemon = 0;
    info_entrenador.victorias = 0;
    info_entrenador.empates = 0;
    info_entrenador.derrotas = 0;
    info_entrenador.puntuacion = 0;

    //Solicitud del Nombre
    printf("-> Ingrese el nombre del entrenador: ");
    scanf("%s", info_entrenador.nombre);


    do{
        //Solicitud del ID
        printf("-> Ingrese el ID identificador: ");
        scanf("%s",info_entrenador.id_entrenador);

        resultado = validad_entrenador_repetido(info_entrenador.id_entrenador);

        //validamos que la info no este repetida
        if(resultado){
            printf("\n=======================================================\n");
            printf("             [!] ERROR: ID DUPLICADO                   \n");
            printf("=======================================================\n");
            printf(" Ya existe un participante registrado con el ID: %s\n", info_entrenador.id_entrenador);
            printf("=======================================================\n");
            printf(" Presione 'C' para intentar con otro ID o 'S' para salir.\n");
            printf(" -> Opcion: ");
            scanf(" %c", &opcion);
            //convertimos a masyusculas
            opcion = toupper(opcion);
            //usamos el operador ternario
            resultado = (opcion == 'C') ? true : false;
            //limpiamos consola
            limpiar_consola();

        }else{
            
            //abrimos el archivo entrenadores
            archivo_entrenadores = fopen("entrenadores.txt", "a+");

            if(archivo_entrenadores == NULL){
                ARCHIVO_ERROR;
            }else{
                fprintf(archivo_entrenadores,"%s %s\n",info_entrenador.id_entrenador, info_entrenador.nombre);

                printf("=======================================================\n");
                printf(" Entrenador '%s' registrado exitosamente.\n", info_entrenador.nombre);
                printf("=======================================================\n\n");\
                //cerrar archivo
                fclose(archivo_entrenadores);
            }

           
        }
    
    }while(resultado);

}


bool validad_entrenador_repetido(char id_entrenador[]){
    //var
    FILE *archivo_entrenadores;
    bool bandera = true;
    char id_archivo[8];
    char nombre_archivo[20];
    bool resultado_op;
    bool salida = false;


    //abrimos el archivo entrenadores
    archivo_entrenadores = fopen("entrenadores.txt", "a+");

    // validamos apertura
    if(archivo_entrenadores == NULL){
        ARCHIVO_ERROR;
    }else{

        //leamos esta llegar al final del archivoS
        while(bandera){

            resultado_op = fscanf(archivo_entrenadores,"%s %*s",&id_archivo) == EOF;
            
            // si se llega al final se detiene el ciclo
            if(resultado_op){
                bandera = false;
            // o si se encuentra el repetido se detiene
            }else if(strcmp(id_archivo,id_entrenador) == 0){
                salida = true;
                bandera = false;
            }

        }

        return salida;
    }

    fclose(archivo_entrenadores);

}


void consultar_pokedex_id(especies_pokemons vector[], int cantidad) {
    int id_buscar;
    printf("\n-> Ingrese el ID (Numero) del Pokemon a buscar: ");
    scanf("%d", &id_buscar);
    buscar_pokemon_por_id(vector, id_buscar, cantidad);
}

void consultar_pokedex_nombre(especies_pokemons vector[], int cantidad) {
    char nombre_buscar[20];
    printf("\n-> Ingrese el nombre exacto del Pokemon: ");
    scanf("%19s", nombre_buscar);
    buscar_pokemon_nombre(vector, nombre_buscar, cantidad);
}

void menu_consultar_pokedex(especies_pokemons vector_universo_pokemon[]) {
    //var
    int opcion_pokedex;
    int indice = 0;
    
    //llenamos pokedex
    llenar_vector_pokemon(vector_universo_pokemon,&indice);
    
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
            // No hace nada, el flujo vuelve naturalmente al menú principal
            break;
        default:
            printf("\n[!] Opcion no valida.\n");
    }
}


void crear_estadisticas_ejemplar(EjemplarPokemon pokemon){

    // calculamos la hp-actual y el hp-maximo
    pokemon.hp_actual = ((pokemon.hp_actual * pokemon.nivel) / 50 ) + pokemon.nivel + 10;
    pokemon.hp_maximo = ((pokemon.hp_maximo * pokemon.nivel) / 50 ) + pokemon.nivel + 10;

    // calculamos Ataque, Defensa y Velocidad
    pokemon.ataque = ( (pokemon.ataque * pokemon.nivel) / 50) + 5;
    pokemon.defensa = ( (pokemon.defensa * pokemon.nivel) / 50) + 5;
    pokemon.velocidad = ( (pokemon.velocidad * pokemon.nivel) / 50) + 5;

};


// Funcion auxiliar cortita y eficiente
bool pokemon_ya_esta_en_equipo(EjemplarPokemon equipo[], int cantidad_actual, char nombre_candidato[]) {
    for(int i = 0; i < cantidad_actual; i++) {
        if(strcmp(equipo[i].nombre, nombre_candidato) == 0) {
            return true; // Ya lo elegimos antes
        }
    }
    return false; // Esta libre
}




void consulta_crear_equipo_pokemon(especies_pokemons vector_universo_pokemon[]){

    //arreglos
    EjemplarPokemon equipo_pokemon[MAX_POKEMONES_EN_EQ];
    int vector_baraja[MAX_POKEDEX];

    //llenar vector baraja
    inicializar_barajas(vector_baraja);

    // se llama a funcion que mezcla el vector
    mezclar_baraja(vector_baraja,MAX_POKEDEX);

    if(crear_equipo_pokemon(0,0,0,vector_baraja,equipo_pokemon, vector_universo_pokemon)){
        //se creo el equipo pokemon correctamente
    }else{
        // no se puedo crear el equipo
    }



}




bool crear_equipo_pokemon(int integrantes_equipo, int integrantes_diferentes, int nivel_actual_eq, int baraja[], EjemplarPokemon equipo_pokemon[],especies_pokemons vector_universo_pokemon[]){

    bool salida;

    //caso base (condicion de parada)
    if( (integrantes_equipo == 6) && (integrantes_diferentes == 4)){
        // la suma de los nivles de los pokemones no puede ser mayor que 300
        if(nivel_actual_eq < 300){
            salida = true;
        }else{
            salida = false;
        }
        
        return salida;
    }

    for(int i = 0; i < MAX_POKEDEX;i++){

        //creamos creamos el ejemplar
        


    
    }
}


void mezclar_baraja(int arreglo[], int cantidad_elementos) {
    // Recorremos el arreglo desde la ultima posicion hasta la posicion 1
    for (int i = cantidad_elementos - 1; i > 0; i--) {
        
        // Generamos un indice aleatorio 'j' que va desde 0 hasta 'i'
        int j = rand() % (i + 1);

        // Hacemos el intercambio (swap) de los valores
        int temporal = arreglo[i];
        arreglo[i] = arreglo[j];
        arreglo[j] = temporal;
    }
}


void inicializar_barajas(int vector[]){

    for(int i = 0; i < MAX_POKEDEX; i++){
        vector[i] = i;
    }

}


void limpiar_consola() {
#if defined(_WIN32) || defined(_WIN64)
    // Cubre Windows en versiones de 32 y 64 bits
    system("cls");
#elif defined(__APPLE__) || defined(__MACH__)
    // Cubre el ecosistema macOS de Apple
    system("clear");
#elif defined(__linux__) || defined(__unix__) || defined(__unix)
    // Cubre distribuciones Linux y sistemas basados en Unix
    system("clear");
#else
    // Respaldo de seguridad (Fallback): 
    // Si el sistema es un entorno raro o desconocido, usamos un Código de Escape ANSI
    // \033[H mueve el cursor al inicio, \033[2J limpia toda la pantalla
    printf("\033[H\033[2J"); 
#endif
}