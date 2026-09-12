#ifndef funciones
#define funciones
#include <stdbool.h>
#define MAX_POKEMON 6

typedef struct{
     char ID[6];
     char Nombre[15];
     char tipo_primario[15];
     char tipo_segundario[15];
     int HP_b;
     int ataque_b;
     int defensa_b;
     int velocidad_b;
}especies_pokemons;


typedef struct {
    int id_ejemplar;                  // EXIGIDO: Para diferenciar dos Pokémon iguales
    especies_pokemons* datos_especie; // EXIGIDO: El puntero a la especie
    char nombre[20];                 
    int nivel;                        
    
    // Estadísticas generadas
    int hp_actual;                    
    int hp_maximo;                    // RECOMENDADO: Tope de vida para curarlo
    int ataque;                      
    int defensa;                      
    int velocidad;                    
    
    int experiencia;                  
    bool estado;                     
} EjemplarPokemon;


typedef struct {
    int id_entrenador;
    char nombre[30];
    
    // El arreglo que intuiste, guardando a los ejemplares del equipo
    EjemplarPokemon equipo[MAX_POKEMON]; 
    int cantidad_pokemon; // Útil para saber cuántos Pokémon tiene asignados realmente
    
    // Estadísticas para la tabla de posiciones de la Fase de Grupos
    int victorias;
    int empates;
    int derrotas;
    int puntuacion;
} Entrenador;

void llenar_vector_pokemon(especies_pokemons vector[], int *i);
void imprimir_vector_pokemon(especies_pokemons vector[], int cantidad);
void buscar_pokemon_por_id(especies_pokemons vector[], int id_usuario, int cantidad);
void buscar_pokemon_nombre(especies_pokemons vector[], char nombre_usuario[], int cantidad);


#endif