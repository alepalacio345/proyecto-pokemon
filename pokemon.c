#include "funciones.h"

int main(){

    especies_pokemons vector[151];
    int cantidad = 0;

    llenar_vector_pokemon(vector,&cantidad);
    //imprimir_vector_pokemon(vector,i);
    buscar_pokemon_por_id(vector,0001,cantidad);

    buscar_pokemon_nombre(vector,"Mew",cantidad);

    return 0;
}