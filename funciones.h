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

void llenar_vector_pokemon(FILE *archivo_entrada, especies_pokemons vector[]);
void imprimir_vector_pokemon();




#endif