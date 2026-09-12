/**
 * @file funciones.c
 * @brief Implementación lógica de la Pokédex y utilidades del sistema.
 */
#include <stdio.h>
#include <string.h>
#include "funciones.h"
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Extrae información del archivo de texto y rellena el vector global[cite: 16].
 * @param vector Arreglo de estructuras especies_pokemons.
 * @param i Puntero al entero que funcionará como índice total de registros.
 */
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

/**
 * @brief Imprime todo el catálogo actual de Pokémon con un diseño tabulado[cite: 16].
 * @param vector Arreglo de estructuras especies_pokemons a mostrar.
 * @param cantidad El total de elementos a recorrer en el arreglo.
 */
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

/**
 * @brief Despliega una ficha técnica detallada basada en la posición de un elemento[cite: 16].
 * @param vector Arreglo que contiene las características de la Pokédex.
 * @param indice Índice entero desde donde se extraerá la información a mostrar.
 */
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


/**
 * @brief Verifica la existencia del Pokémon a través del identificador y solicita la ficha[cite: 16].
 * @param vector Arreglo que contiene los datos base.
 * @param id_usuario ID a validar y buscar proporcionado por teclado.
 * @param cantidad El total actual de especies cargadas para evitar desbordamientos.
 */
void buscar_pokemon_por_id(especies_pokemons vector[], int id_usuario, int cantidad){
    
    //validamos que el id este en el rango
    if((id_usuario > 0) && (id_usuario <= cantidad)){
        mostrar_estadisticas_pokemon(vector,id_usuario - 1);
    }else{
        printf("!!Este Numero de pokemon no existe!!\n");
    }
};

/**
 * @brief Busca una coincidencia exacta de nombre de Pokémon y solicita su ficha[cite: 16].
 * @param vector Arreglo que contiene los datos base.
 * @param nombre_usuario Cadena de texto objetivo a evaluar contra el arreglo.
 * @param cantidad El total actual de especies cargadas.
 */
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
        printf("!!Este nombre de pokemon no existe!!\n");
    }
};



/**
 * @brief Registra a los entrenadores del torneo
*/
void registrar_entrenador(){
    
    //var
    FILE *archivo_entrenadores;
    bool resultado;
    Entrenador info_entrenador;

    // Encabezado visual de la planilla
    printf("\n=======================================================\n");
    printf("             INSCRIPCION DE NUEVO ENTRENADOR           \n");
    printf("=======================================================\n");
    
    // 1. Solicitud del ID
    printf("-> Ingrese el ID identificador (Numero entero): ");
    scanf("%i", &info_entrenador.id_entrenador);
    
    // 2. Solicitud del Nombre
    printf("-> Ingrese el nombre del entrenador: ");
    scanf("%s", info_entrenador.nombre);
    
    // Inicializamos en cero para evitar basura en la memoria
    info_entrenador.cantidad_pokemon = 0;
    info_entrenador.victorias = 0;
    info_entrenador.empates = 0;
    info_entrenador.derrotas = 0;
    info_entrenador.puntuacion = 0;

    resultado = validad_entrenador_repetido(info_entrenador);

    //validamos que la info no este repetida
    if(resultado){
        printf("=======================================================\n");
        printf(" Entrenador '%s' ya esta registrado.\n", info_entrenador.nombre);
        printf("=======================================================\n\n");
    }else{
        
        //abrimos el archivo entrenadores
        archivo_entrenadores = fopen("entrenadores.txt", "a+");

        if(archivo_entrenadores == NULL){
            ARCHIVO_ERROR;
        }else{
            fprintf(archivo_entrenadores,"%d %s",info_entrenador.id_entrenador, info_entrenador.nombre);

            printf("=======================================================\n");
            printf(" Entrenador '%s' registrado exitosamente.\n", info_entrenador.nombre);
            printf("=======================================================\n\n");
        }

        fclose(archivo_entrenadores);
    }
}



bool validad_entrenador_repetido(Entrenador info_entrenador){
    //var
    FILE *archivo_entrenadores;
    bool bandera = true;
    int id_archivo;
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

            resultado_op = fscanf(archivo_entrenadores,"%d %s",&id_archivo, nombre_archivo) == EOF;
            
            // si se llega al final se detiene el ciclo
            if(resultado_op){
                bandera = false;
            // o si se encuentra el repetido se detiene
            }else if( (id_archivo == info_entrenador.id_entrenador) || (nombre_archivo == info_entrenador.nombre)){
                salida = true;
                bandera = false;
            }

        }

        return salida;
    }

    fclose(archivo_entrenadores);

}




