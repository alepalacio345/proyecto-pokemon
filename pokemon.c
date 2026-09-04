#include "funciones.h"


int main(){

    especies_pokemons vector[151];
    int i = 0;

    llenar_vector_pokemon(vector,&i);
    imprimir_vector_pokemon(vector,i);

    

    return 0;
}