#ifndef funciones
#define funciones

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

void llenar_vector_pokemon(especies_pokemons vector[], int *i);
void imprimir_vector_pokemon(especies_pokemons vector[], int cantidad);
void buscar_pokemon_por_id(especies_pokemons vector[], int id_usuario, int cantidad);
void buscar_pokemon_nombre(especies_pokemons vector[], char nombre_usuario[], int cantidad);


#endif