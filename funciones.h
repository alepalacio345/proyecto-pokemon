/**
 * @file funciones.h
 * @brief Estructuras y prototipos para la administracion de El Gran Torneo Pokemon.
 */
#ifndef funciones
#define funciones
#include <stdbool.h>

#define MAX_POKEMONES_EN_EQ 6
#define MAX_ENTRENADORES 32
#define ARCHIVO_ERROR printf("Error al abrir el archivo\n")
#define MAX_POKEDEX 151

// =========================================================================
// ESTRUCTURAS DE DATOS
// =========================================================================

/**
 * @struct especies_pokemons
 * @brief Plantilla base que representa los datos inmutables de una especie en la Pokedex.
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
} especies_pokemons;

/**
 * @struct EjemplarPokemon
 * @brief Representa un Pokemon fisico y unico capturado por un entrenador.
 */
typedef struct {
    char id_ejemplar[6];              
    especies_pokemons* datos_especie; 
    char nombre[20];                 
    int nivel;                        
    int hp_actual;                    
    int hp_maximo;                    
    int ataque;                      
    int defensa;                      
    int velocidad;                    
    int experiencia;                  
    bool estado;                     
} EjemplarPokemon;

/**
 * @struct Entrenador
 * @brief Almacena la informacion de un participante del torneo y su equipo.
 */
typedef struct {
    char id_entrenador[8];
    char nombre[30];
    EjemplarPokemon equipo[MAX_POKEMONES_EN_EQ]; 
    int cantidad_pokemon; 
    int victorias;
    int empates;
    int derrotas;
    int puntuacion;
    char asignaciom_equipo[6];
} Entrenador;

// =========================================================================
// MODULO 1: UTILIDADES Y SISTEMA
// =========================================================================

/**
 * @brief Limpia la pantalla de la consola o terminal asegurando compatibilidad multiplataforma.
 */
void limpiar_consola();

// =========================================================================
// MODULO 2: GESTION DE LA POKEDEX
// =========================================================================

/**
 * @brief Lee el archivo pokedex.txt y transfiere las especies al arreglo en memoria RAM.
 * @param vector Arreglo global de tipo especies_pokemons.
 */
void llenar_vector_pokemon(especies_pokemons vector[]);

/**
 * @brief Imprime en pantalla una tabla tabulada con todos los Pokemon registrados.
 * @param vector Arreglo global de la Pokedex.
 * @param cantidad Numero entero que indica el total de especies cargadas.
 */
void imprimir_vector_pokemon(especies_pokemons vector[], int cantidad);

/**
 * @brief Imprime una ficha visual detallada con las estadisticas base de un Pokemon.
 * @param vector Arreglo global de la Pokedex.
 * @param indice Posicion (indice) del Pokemon en el arreglo.
 */
void mostrar_estadisticas_pokemon(especies_pokemons vector[], int indice);

/**
 * @brief Algoritmo universal para buscar en la Pokedex por ID o por Nombre.
 * @param vector Arreglo global de la Pokedex.
 * @param string Texto o numero ingresado por el usuario.
 * @param campo true = buscar por ID, false = buscar por Nombre.
 * @param cantidad Total de elementos a recorrer.
 * @param bandera Puntero que retorna 'true' si hubo exito.
 * @param indice Puntero que retorna la posicion encontrada.
 */
void buscar_vector_pokemon(especies_pokemons vector[], char string[], bool campo, int cantidad, bool *bandera, int *indice);

/**
 * @brief Solicita un ID por teclado y ejecuta la busqueda en la Pokedex.
 * @param vector Arreglo global de la Pokedex.
 * @param cantidad Numero total de especies registradas.
 */
void consultar_pokedex_id(especies_pokemons vector[], int cantidad);

/**
 * @brief Solicita un nombre exacto por teclado y ejecuta la busqueda en la Pokedex.
 * @param vector Arreglo global de la Pokedex.
 * @param cantidad Numero total de especies registradas.
 */
void consultar_pokedex_nombre(especies_pokemons vector[], int cantidad);

/**
 * @brief Despliega el menu interactivo para gestionar las opciones de consulta de la Pokedex.
 * @param vector_universo_pokemon Arreglo global de la Pokedex.
 */
void menu_consultar_pokedex(especies_pokemons vector_universo_pokemon[]);

// =========================================================================
// MODULO 3: GESTION DE ENTRENADORES
// =========================================================================

/**
 * @brief Lee el archivo entrenadores.txt y transfiere los datos a la memoria RAM.
 * @param vector Arreglo global de entrenadores.
 * @param i Puntero al contador total de entrenadores cargados.
 */
void llenar_vector_entrenador(Entrenador vector[], int *i);

/**
 * @brief Sobrescribe el archivo entrenadores.txt con los datos actualizados de la RAM.
 * @param vector Arreglo global de entrenadores.
 * @param cantidad Numero total de entrenadores en el sistema.
 */
void guardar_vector_entrenador(Entrenador vector[], int cantidad);

/**
 * @brief Inscribe a un nuevo entrenador, verifica que no exista duplicado y actualiza RAM y disco.
 * @param vector Arreglo global de entrenadores.
 * @param cantidad Puntero a la cantidad actual de entrenadores.
 */
void registrar_entrenador(Entrenador vector[], int *cantidad);

/**
 * @brief Imprime un listado simple de todos los entrenadores y sus IDs.
 * @param vector Arreglo global de entrenadores.
 * @param cantidad Numero total de entrenadores.
 */
void imprimir_vector_entrenador(Entrenador vector[], int cantidad);

/**
 * @brief Imprime un listado de entrenadores destacando con colores si poseen equipo asignado o no.
 * @param vector Arreglo global de entrenadores.
 * @param cantidad Numero total de entrenadores.
 */
void mostrar_seleccion_entrenadores(Entrenador vector[], int cantidad);

/**
 * @brief Busca a un entrenador especifico mediante su ID.
 * @param vector Arreglo global de entrenadores.
 * @param id Cadena de texto con el identificador del entrenador.
 * @param cantidad Numero total de entrenadores registrados.
 * @param encontrado Puntero que devuelve 'true' si el entrenador existe.
 * @param indice Puntero que devuelve la posicion del entrenador en la RAM.
 */
void buscar_entrenador(Entrenador vector[], char id[], int cantidad, bool *encontrado, int *indice);

// =========================================================================
// MODULO 4: GESTION DE EQUIPOS Y BACKTRACKING
// =========================================================================

/**
 * @brief Inicializa el arreglo de indices con valores del 0 al 150.
 * @param vector Arreglo de enteros a inicializar.
 */
void inicializar_barajas(int vector[]);

/**
 * @brief Mezcla de forma aleatoria un arreglo de enteros (Fisher-Yates) para el Backtracking.
 * @param arreglo Puntero al arreglo que se desea desordenar.
 * @param cantidad_elementos El tamaño del arreglo.
 */
void mezclar_baraja(int arreglo[], int cantidad_elementos);

/**
 * @brief Verifica si un candidato Pokemon ya se encuentra dentro del equipo en formacion.
 * @param equipo Arreglo temporal del equipo en ensamblaje.
 * @param cantidad Numero actual de integrantes asignados.
 * @param nombre Nombre del Pokemon candidato.
 * @return true si ya esta repetido, false si esta libre.
 */
bool pokemon_ya_esta_en_equipo(EjemplarPokemon equipo[], int cantidad, char nombre[]);

/**
 * @brief Evalua si el equipo ensamblado posee al menos 4 tipos elementales distintos.
 * @param equipo Arreglo del equipo ya completado con 6 integrantes.
 * @param cantidad Deberia ser siempre 6.
 * @return true si cumple con la variedad, false si hay demasiados tipos repetidos.
 */
bool validar_variedad_tipos(EjemplarPokemon equipo[], int cantidad);

/**
 * @brief Algoritmo principal de Backtracking para ensamblar un equipo valido y balanceado.
 * @param integrantes_equipo Nivel de profundidad de la recursion (0 a 6).
 * @param nivel_actual_eq Suma total de los niveles de los integrantes actuales.
 * @param baraja Arreglo mezclado con los indices de la Pokedex a explorar.
 * @param equipo_pokemon Arreglo temporal donde se estructura el equipo.
 * @param vector_universo_pokemon Arreglo maestro de la Pokedex.
 * @return true si encontro una ruta valida y termino el equipo, false en caso de callejon sin salida.
 */
bool crear_equipo_pokemon(int integrantes_equipo, int nivel_actual_eq, int baraja[], EjemplarPokemon equipo_pokemon[], especies_pokemons vector_universo_pokemon[]);

/**
 * @brief Genera y calcula matematicamente los stats (Ataque, HP, Velocidad) de un ejemplar fisico.
 * @param pokemon Arreglo con los 6 integrantes del equipo recien creado.
 * @param cantidad Total de integrantes (6).
 */
void crear_estadisticas_ejemplares(EjemplarPokemon pokemon[], int cantidad);

/**
 * @brief Guarda definitivamente el equipo en el archivo txt y vincula los stats a la estructura del entrenador.
 * @param entrenador Arreglo global de entrenadores en RAM.
 * @param indice Indice del entrenador al que pertenece el equipo.
 * @param equipo_pokemon Equipo recien generado.
 */
void guardar_equipo_pokemon(Entrenador entrenador[], int indice, EjemplarPokemon equipo_pokemon[]);

/**
 * @brief Imprime en consola la ficha tabular con las caracteristicas de los 6 integrantes.
 * @param equipo Arreglo con el equipo del entrenador.
 * @param cantidad Total de integrantes.
 */
void imprimir_equipo_pokemon(EjemplarPokemon equipo[], int cantidad);

/**
 * @brief Procedimiento general que agrupa la logica visual de seleccion, generacion y guardado del equipo.
 * @param vector_universo_pokemon Arreglo de la Pokedex.
 * @param vector_entrenadores Arreglo global de Entrenadores.
 * @param cantidad_entrenadores Total actual de entrenadores.
 */
void consulta_crear_equipo_pokemon(especies_pokemons vector_universo_pokemon[], Entrenador vector_entrenadores[], int cantidad_entrenadores);

#endif