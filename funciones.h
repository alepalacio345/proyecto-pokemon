/**
 * @file funciones.h
 * @brief Estructuras y prototipos para la administración de El Gran Torneo Pokémon.
 */
#ifndef funciones
#define funciones
#include <stdbool.h>
#define MAX_POKEMONES_EN_EQ 6
#define MAX_ENTRENADORES 32
#define ARCHIVO_ERROR printf("Error al abrir el archivo\n")
#define MAX_POKEDEX 151

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
    char id_ejemplar[6];                  // EXIGIDO: Para diferenciar dos Pokémon iguales
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
    char id_entrenador[8];
    char nombre[30];
    
    // El arreglo que intuiste, guardando a los ejemplares del equipo
    EjemplarPokemon equipo[MAX_POKEMONES_EN_EQ]; 
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


/**
 * @brief Registra a los entrenadores del torneo.
 * 
 * Solicita los datos del entrenador (nombre e ID) mediante la entrada estandar.
 * Valida que el ID ingresado no este duplicado en el archivo del sistema.
 * Una vez validado, inicializa sus estadisticas y guarda el registro en el archivo de texto.
 * 
 * @return void No retorna ningun valor.
 */
void registrar_entrenador();


/**
 * @brief Verifica si un ID de entrenador ya existe en la base de datos.
 * 
 * Abre el archivo "entrenadores.txt" y lee los registros linea por linea para comparar
 * los identificadores existentes con el ID proporcionado por el usuario.
 * 
 * @param id_entrenador Cadena de caracteres que contiene el ID a verificar.
 * @return true Si el ID se encuentra repetido en el archivo.
 * @return false Si el ID es unico y no se encontro en el archivo.
 */
bool validad_entrenador_repetido(char id_entrenador[]);


/**
 * @brief Limpia la pantalla de la consola o terminal.
 * 
 * Ejecuta comandos del sistema especificos dependiendo de si la compilacion 
 * se realiza en un entorno de Windows o en un sistema basado en Unix.
 * 
 * @return void No retorna ningun valor.
 */
void limpiar_consola();


/**
 * @brief Submenú interactivo para gestionar las opciones de consulta de la Pokédex.
 */
void menu_consultar_pokedex();

/**
 * @brief Procedimiento envoltorio que solicita un ID por teclado y ejecuta la búsqueda.
 * @param vector Arreglo global de tipo especies_pokemons.
 * @param cantidad Número total de especies registradas en el sistema.
 */
void consultar_pokedex_id(especies_pokemons vector[], int cantidad);

/**
 * @brief Procedimiento envoltorio que solicita un nombre por teclado y ejecuta la búsqueda.
 * @param vector Arreglo global de tipo especies_pokemons.
 * @param cantidad Número total de especies registradas en el sistema.
 */
void consultar_pokedex_nombre(especies_pokemons vector[], int cantidad);

void consulta_crear_equipo_pokemon(especies_pokemons vector_universo_pokemon[]);

bool pokemon_ya_esta_en_equipo(EjemplarPokemon equipo[], int cantidad, char nombre[]);

bool validar_variedad_tipos(EjemplarPokemon equipo[], int cantidad);

bool crear_equipo_pokemon(int integrantes_equipo, int nivel_actual_eq, int baraja[], EjemplarPokemon equipo_pokemon[],especies_pokemons vector_universo_pokemon[]);

void crear_estadisticas_ejemplar(EjemplarPokemon pokemon);

void imprimir_vector_entrenador();

/**
 * @brief se inicializa un vector de numeros enteros
 * @param vector es el vector de enteros
 */
void inicializar_barajas(int vector[]);

/**
 * @brief Mezcla de forma aleatoria un arreglo de enteros usando Fisher-Yates.
 * 
 * @param arreglo Puntero al arreglo que se desea desordenar.
 * @param cantidad_elementos El tamaño del arreglo.
 */
void mezclar_baraja(int arreglo[], int cantidad_elementos);

#endif