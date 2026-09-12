/**
 * @file funciones.h
 * @brief Estructuras y prototipos para la administración de El Gran Torneo Pokémon.
 */
#ifndef funciones
#define funciones
#include <stdbool.h>
#define MAX_POKEMON 6

/**
 * @struct especies_pokemons
 * @brief Plantilla base que representa los datos inmutables de una especie en la Pokédex[cite: 17].
 */
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

/**
 * @struct EjemplarPokemon
 * @brief Representa un Pokémon físico y único capturado por un entrenador[cite: 17].
 */
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

/**
 * @struct Entrenador
 * @brief Almacena la información de un participante del torneo y las estadísticas de su participación[cite: 17].
 */
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

/**
 * @brief Abre el archivo pokedex.txt y transfiere las especies a un arreglo en memoria[cite: 17].
 * @param vector Arreglo global de tipo especies_pokemons.
 * @param i Puntero a un entero que controla la cantidad de especies leídas.
 */
void llenar_vector_pokemon(especies_pokemons vector[], int *i);

/**
 * @brief Imprime en pantalla una tabla con todos los Pokémon registrados[cite: 17].
 * @param vector Arreglo global de tipo especies_pokemons.
 * @param cantidad Número entero que indica el total de especies en el arreglo.
 */
void imprimir_vector_pokemon(especies_pokemons vector[], int cantidad);

/**
 * @brief Busca en la Pokédex a través de un número de identificación[cite: 17].
 * @param vector Arreglo global de tipo especies_pokemons.
 * @param id_usuario Entero con el ID solicitado.
 * @param cantidad Número total de especies registradas en el sistema.
 */
void buscar_pokemon_por_id(especies_pokemons vector[], int id_usuario, int cantidad);

/**
 * @brief Busca en la Pokédex comparando cadenas de texto[cite: 17].
 * @param vector Arreglo global de tipo especies_pokemons.
 * @param nombre_usuario Cadena de caracteres que contiene el nombre exacto a buscar.
 * @param cantidad Número total de especies registradas en el sistema.
 */
void buscar_pokemon_nombre(especies_pokemons vector[], char nombre_usuario[], int cantidad);

#endif