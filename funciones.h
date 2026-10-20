/**
 * @file funciones.h
 * @brief Estructuras, constantes y prototipos para la administracion de El Gran Torneo Pokemon.
 *
 * Organiza el sistema en modulos:
 *  - Modulo 1: Utilidades y sistema
 *  - Modulo 2: Gestion de la Pokedex
 *  - Modulo 3: Gestion de entrenadores
 *  - Modulo 4: Gestion de equipos y backtracking
 *  - Modulo 5: Motor de combate y efectividad de tipos
 *  - Modulo 6: Torneo (calendario, fases, clasificacion y resultados)
 */
#ifndef funciones
#define funciones
#include <stdbool.h>

// =========================================================================
// CONSTANTES GENERALES
// =========================================================================

#define MAX_POKEMONES_EN_EQ 6
#define MAX_ENTRENADORES 32
#define ARCHIVO_ERROR printf("Error al abrir el archivo\n")
#define MAX_POKEDEX 150
#define CANTIDAD_TIPOS 18
#define PODER_ATAQUE 50

/** @brief Maximo de rondas de un combate. Si se agota sin K.O. el combate es empate. */
#define MAX_RONDAS_COMBATE 100

/** @brief Combates de la fase de grupos (8 grupos x 6 combates). */
#define COMBATES_GRUPOS 48

/** @brief Combates totales del torneo (48 de grupos + 16 de eliminatoria). */
#define CANTIDAD_PARTIDOS 64

/** @brief Entrenadores que clasifican a la fase eliminatoria. */
#define MAX_CLASIFICADOS 16

/** @brief Entrenadores por grupo en la fase de grupos. */
#define ENTRENADORES_POR_GRUPO 4

/** @brief Cantidad de grupos de la fase de grupos. */
#define CANTIDAD_GRUPOS 8

// =========================================================================
// DEFINICIONES DE TIPOS PARA COMBATES
// =========================================================================
// Definimos los nombres de los 18 tipos de pokemon
extern const char* NOMBRES_TIPOS[CANTIDAD_TIPOS];

// Declaramos la Matriz 2D de Efectividad (Filas: Atacante, Columnas: Defensor)
extern const float TABLA_EFECTIVIDAD[CANTIDAD_TIPOS][CANTIDAD_TIPOS];

// Nombres de las etapas del torneo, indexados por el tipo EtapaTorneo
extern const char* NOMBRES_ETAPAS[];


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
    int pokemon_derrotados;
    char asignaciom_equipo[6];
} Entrenador;

// =========================================================================
// ESTRUCTURAS DEL TORNEO
// =========================================================================

/**
 * @enum EtapaTorneo
 * @brief Etapas o rondas en las que puede estar un combate del torneo.
 */
typedef enum {
    ETAPA_GRUPOS = 0,       /**< Fase de grupos (combates 1 a 48). */
    ETAPA_OCTAVOS,          /**< Octavos de final (combates 49 a 56). */
    ETAPA_CUARTOS,          /**< Cuartos de final (combates 57 a 60). */
    ETAPA_SEMIFINAL,        /**< Semifinales (combates 61 y 62). */
    ETAPA_TERCER_LUGAR,     /**< Combate por el tercer lugar (combate 63). */
    ETAPA_FINAL             /**< Final del torneo (combate 64). */
} EtapaTorneo;

/**
 * @struct Partido
 * @brief Representa un combate del torneo y su resultado.
 *
 * Es la unidad central del torneo: el calendario de grupos, los cruces de la
 * fase eliminatoria, la clasificacion y el campeon se derivan de este arreglo.
 * Cada partido se respalda en el archivo resultados_combates.txt.
 */
typedef struct {
    int numero;                     /**< Numero de combate (1 a 64). */
    EtapaTorneo etapa;              /**< Ronda a la que pertenece el combate. */
    int grupo;                      /**< Grupo (0 a 7) o -1 si es de eliminatoria. */
    char id_jugador1[8];            /**< Identificador del primer entrenador. */
    char id_jugador2[8];            /**< Identificador del segundo entrenador. */
    char id_ganador[8];             /**< Identificador del ganador (vacio si no se ha jugado). */
    char id_perdedor[8];            /**< Identificador del perdedor (vacio si no se ha jugado). */
    bool finalizado;                /**< true si el combate ya tiene resultado oficial. */
    bool empate;                    /**< true si el combate termino empatado (solo grupos). */
} Partido;

/**
 * @struct TablaGrupo
 * @brief Fila de la tabla de posiciones de un entrenador dentro de su grupo.
 */
typedef struct {
    Entrenador *entrenador;         /**< Puntero al entrenador en RAM. */
    int grupo;                      /**< Grupo al que pertenece (0 a 7). */
    int posicion;                   /**< Posicion final dentro del grupo (1 a 4). */
    int puntos;                     /**< Puntos acumulados (victoria 3, empate 1). */
    int ganados;                    /**< Combates ganados. */
    int empatados;                  /**< Combates empatados. */
    int perdidos;                   /**< Combates perdidos. */
} TablaGrupo;


// =========================================================================
// MODULO 1: UTILIDADES Y SISTEMA
// =========================================================================

/**
 * @brief Lee un numero entero desde el teclado de forma segura.
 *
 * Lee la linea completa y la convierte a entero. Si la entrada no es numerica
 * descarta el buffer y devuelve el valor por defecto, evitando los ciclos
 * infinitos de lecturas fallidas tipicos de scanf con entradas invalidas.
 *
 * @param valor_por_defecto Valor devuelto cuando la entrada no es valida.
 * @return int Numero leido, o el valor por defecto.
 */
int leer_entero(int valor_por_defecto);

/**
 * @brief Lee una palabra desde el teclado consumiendo la linea completa.
 *
 * Lee con fgets y toma el primer token, de modo que nunca queda un salto de
 * linea pendiente en el buffer (evita que la pausa del menu se salte sola).
 *
 * @param destino Cadena donde se escribe lo leido.
 * @param tamano Capacidad total de la cadena destino.
 */
void leer_texto(char destino[], int tamano);

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
 * @param encontrado Puntero que retorna 'true' si hubo exito.
 * @param indice Puntero que retorna la posicion encontrada.
 */
void buscar_vector_pokemon(especies_pokemons vector[], char string[], bool campo, int cantidad, bool *encontrado, int *indice);

/**
 * @brief Cuenta cuantas especies hay realmente cargadas en el arreglo de la Pokedex.
 * @param vector Arreglo global de la Pokedex.
 * @param maximo Capacidad maxima del arreglo (MAX_POKEDEX).
 * @return int Cantidad de especies distintas de cero encontradas.
 */
int contar_especies_pokemon(especies_pokemons vector[], int maximo);

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
 * @param cantidad Numero total de especies registradas.
 */
void menu_consultar_pokedex(especies_pokemons vector_universo_pokemon[], int cantidad);

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

/**
 * @brief Carga los equipos desde el disco duro y los asigna a la memoria RAM de cada entrenador.
 * @param vector_entrenadores Arreglo global de Entrenadores.
 * @param cantidad_entrenadores Total de entrenadores en RAM.
 * @param pokedex Arreglo maestro de la Pokedex para reconectar los punteros de especie.
 */
void cargar_equipos_pokemon(Entrenador vector_entrenadores[], int cantidad_entrenadores, especies_pokemons pokedex[]);

/**
 * @brief Recibe el nombre de un tipo en texto y devuelve su indice numerico (0-17).
 * @param nombre_tipo Cadena de caracteres con el nombre del tipo (ej. "Fuego").
 * @return int El indice de la matriz. Devuelve -1 si hay un error o tipo vacio.
 */
int obtener_indice_tipo(char nombre_tipo[]);

/**
 * @brief Calcula el multiplicador de daño entre un ataque y un pokemon defensor.
 * @param tipo_atacante El tipo primario del atacante.
 * @param tipo_defensor_1 El tipo primario del defensor.
 * @param tipo_defensor_2 El tipo secundario del defensor (puede ser "Ninguno").
 * @return float El multiplicador final (ej. 1.0, 2.0, 0.25).
 */
float calcular_multiplicador_tipos(char tipo_atacante[], char tipo_defensor_1[], char tipo_defensor_2[]);

/**
 * @brief Calcula el daño que produce un ejemplar atacante sobre un ejemplar defensor.
 * @param Atacante Ejemplar que ejecuta el ataque.
 * @param Defensor Ejemplar que recibe el ataque.
 * @return int Daño final (minimo 1 si el ataque tiene algun efecto).
 */
int calcular_dano(EjemplarPokemon Atacante, EjemplarPokemon Defensor);

/**
 * @brief Muestra la lista de entrenadores y el equipo del que el usuario indique por ID.
 * @param vector_entrenadores Arreglo global de Entrenadores.
 * @param cantidad_entrenadores Total actual de entrenadores.
 */
void consultar_equipos_creasdos(Entrenador vector_entrenadores[], int cantidad_entrenadores);

/**
 * @brief Imprime la tarjeta completa con los datos y estadisticas de un entrenador.
 * @param entrenador Entrenador a mostrar.
 */
void imprimir_perfil_entrenador(Entrenador entrenador);

/**
 * @brief Muestra la lista de entrenadores y el perfil del que el usuario indique por ID.
 * @param vector_entrenadores Arreglo global de Entrenadores.
 * @param cantidad Numero total de entrenadores.
 */
void Consultar_entrenadores(Entrenador vector_entrenadores[], int cantidad);

// =========================================================================
// MODULO 5: MOTOR DE COMBATE
// =========================================================================

/**
 * @brief Restaura la vida y el estado de todos los integrantes de un equipo.
 *
 * Los daños de un combate son temporales: al iniciar cada combate ambos
 * entrenadores deben tener sus Pokemon disponibles y con la vida al maximo.
 *
 * @param entrenador Puntero al entrenador cuyo equipo se desea curar.
 */
void curar_equipo(Entrenador *entrenador);

/**
 * @brief Busca el primer Pokemon disponible (no derrotado y con vida) del equipo.
 *
 * La seleccion es automatica: el sistema siempre elige el primer ejemplar
 * apto segun el orden de la alineacion del entrenador.
 *
 * @param equipo Arreglo con los ejemplares del entrenador.
 * @param cantidad Total de integrantes del equipo.
 * @return int Indice del Pokemon disponible, o -1 si no le queda ninguno.
 */
int seleccionar_pokemon(EjemplarPokemon equipo[], int cantidad);

/**
 * @brief Suma la vida que le queda a todo el equipo de un entrenador.
 *
 * Se usa como criterio objetivo para desempatar un combate de la fase
 * eliminatoria que haya agotado las rondas sin K.O.
 *
 * @param entrenador Puntero al entrenador cuyo equipo se desea evaluar.
 * @return int Suma de los hp_actual de todos sus Pokemon.
 */
int calcular_hp_total(Entrenador *entrenador);

/**
 * @brief Disputa un combate completo entre dos entrenadores y registra su resultado.
 *
 * Funcion principal del motor de combate. Al comenzar cura ambos equipos,
 * selecciona automaticamente a los Pokemon iniciales y ejecuta el combate por
 * rondas: en cada ronda ataca primero el Pokemon mas veloz (moneda si empatan
 * en velocidad) y, si el defensor sobrevive, contraataca. Cuando un Pokemon
 * llega a 0 de vida entra el siguiente disponible del mismo entrenador. Si se
 * agotan las MAX_RONDAS_COMBATE sin K.O. el combate termina en empate; en la
 * fase eliminatoria (permitir_empate = false) ese caso se resuelve por la vida
 * total restante de cada equipo, porque la fase no admite tablas.
 *
 * @param numero Numero de combate del torneo (1 a 64).
 * @param player01 Puntero al primer entrenador.
 * @param player02 Puntero al segundo entrenador.
 * @param id_player01 ID del primer entrenador (el que se guarda en el torneo).
 * @param id_player02 ID del segundo entrenador (el que se guarda en el torneo).
 * @param permitir_empate true en fase de grupos, false en fase eliminatoria.
 * @param partidos Arreglo de partidos del torneo donde se registra el resultado.
 * @param cantidad_partidos Total de partidos del arreglo.
 */
void disputar_combate(int numero, Entrenador *player01, Entrenador *player02, char id_player01[], char id_player02[], bool permitir_empate, Partido partidos[], int cantidad_partidos);

// =========================================================================
// MODULO 6: TORNEO (CALENDARIO, FASES, CLASIFICACION Y RESULTADOS)
// =========================================================================

/**
 * @brief Deja el arreglo de partidos en estado inicial (64 combates sin jugar).
 *
 * Los 48 combates de la fase de grupos quedan con sus participantes reales ya
 * resueltos (grupo + calendario + ID del entrenador); los de la fase eliminatoria
 * se completan cuando existan los resultados previos.
 *
 * @param partidos Arreglo de partidos del torneo.
 * @param cantidad Total de partidos (CANTIDAD_PARTIDOS).
 * @param entrenadores Arreglo global de Entrenadores (para resolver los IDs).
 * @param cantidad_entrenadores Numero real de entrenadores cargados en RAM.
 */
void inicializar_partidos(Partido partidos[], int cantidad, Entrenador entrenadores[], int cantidad_entrenadores);

/**
 * @brief Carga desde resultados_combates.txt los resultados ya disputados.
 *
 * Permite trabajar con resultados parciales: los combates sin linea en el
 * archivo quedan pendientes y el torneo puede continuar mas adelante.
 *
 * @param partidos Arreglo de partidos del torneo.
 * @param cantidad Total de partidos (CANTIDAD_PARTIDOS).
 */
void cargar_partidos(Partido partidos[], int cantidad);

/**
 * @brief Guarda en resultados_combates.txt todos los resultados oficiales.
 *
 * Reescribe el archivo completo desde el arreglo en RAM (no anexa), de modo
 * que nunca queden registros duplicados ni obsoletos.
 *
 * @param partidos Arreglo de partidos del torneo.
 * @param cantidad Total de partidos (CANTIDAD_PARTIDOS).
 */
void guardar_partidos(Partido partidos[], int cantidad);

/**
 * @brief Busca un combate por su numero dentro del arreglo de partidos.
 * @param partidos Arreglo de partidos del torneo.
 * @param cantidad Total de partidos (CANTIDAD_PARTIDOS).
 * @param numero Numero de combate buscado (1 a 64).
 * @return int Indice del partido en el arreglo, o -1 si no existe.
 */
int buscar_partido(Partido partidos[], int cantidad, int numero);

/**
 * @brief Determina los dos entrenadores que deben disputar un combate.
 *
 * Para los combates 1 a 48 los participantes salen del grupo y del calendario;
 * para los 49 a 56 salen de los clasificados de cada grupo; y para los 57 a 64
 * se derivan del ganador o del perdedor de combates anteriores segun el cuadro
 * oficial del torneo.
 *
 * @param numero Numero de combate (1 a 64).
 * @param partidos Arreglo de partidos del torneo.
 * @param cantidad Total de partidos (CANTIDAD_PARTIDOS).
 * @param clasificados Arreglo con los IDs de los 16 clasificados (posiciones 1A,2A,1B,2B...).
 * @param id_jugador1 Cadena donde se devuelve el ID del primer entrenador.
 * @param id_jugador2 Cadena donde se devuelve el ID del segundo entrenador.
 * @return true si ambos participantes estan definidos, false si dependen de resultados pendientes.
 */
bool obtener_participantes(int numero, Partido partidos[], int cantidad, char clasificados[][8], char id_jugador1[], char id_jugador2[]);

/**
 * @brief Registra el resultado oficial de un combate en el arreglo de partidos.
 * @param partidos Arreglo de partidos del torneo.
 * @param cantidad Total de partidos (CANTIDAD_PARTIDOS).
 * @param numero Numero de combate (1 a 64).
 * @param id_jugador1 ID del primer entrenador.
 * @param id_jugador2 ID del segundo entrenador.
 * @param id_ganador ID del ganador, o cadena vacia si el combate fue empate.
 * @param empate true si el combate termino empatado.
 */
void registrar_partido(Partido partidos[], int cantidad, int numero, char id_jugador1[], char id_jugador2[], char id_ganador[], bool empate);

/**
 * @brief Consulta si un combate ya tiene resultado oficial.
 * @param partidos Arreglo de partidos del torneo.
 * @param cantidad Total de partidos (CANTIDAD_PARTIDOS).
 * @param numero Numero de combate (1 a 64).
 * @return true si el combate esta finalizado.
 */
bool partido_finalizado(Partido partidos[], int cantidad, int numero);

/**
 * @brief Valida que existan los 32 entrenadores con su equipo completo antes del torneo.
 * @param entrenadores Arreglo global de Entrenadores.
 * @param cantidad Numero real de entrenadores cargados en RAM.
 * @return true si el torneo puede iniciar la fase de grupos.
 */
bool validar_participantes(Entrenador entrenadores[], int cantidad);

/**
 * @brief Disputa la fase de grupos completa: 8 grupos de 4 entrenadores.
 *
 * Valida que existan los 32 entrenadores con equipo, asigna los grupos por
 * orden de carga y disputa las 3 rondas de cada grupo (6 combates por grupo,
 * 48 en total) usando el calendario oficial de round-robin.
 *
 * @param vector_entrenadores_torneo Arreglo global de Entrenadores participantes.
 * @param cantidad_entrenadores Numero real de entrenadores cargados en RAM.
 * @param partidos Arreglo de partidos del torneo.
 * @return true si la fase se disputo completa, false si faltan datos.
 */
bool fase_de_grupos(Entrenador vector_entrenadores_torneo[], int cantidad_entrenadores, Partido partidos[]);

/**
 * @brief Calcula la tabla de posiciones de los 8 grupos con los criterios de desempate.
 *
 * Acumula puntos (victoria 3, empate 1), victorias, empates, derrotas y Pokemon
 * derrotados recorriendo los combates 1 a 48 ya finalizados, y ordena cada grupo
 * con qsort aplicando: puntos, victorias y Pokemon derrotados.
 *
 * @param partidos Arreglo de partidos del torneo.
 * @param cantidad Total de partidos (CANTIDAD_PARTIDOS).
 * @param entrenadores Arreglo global de Entrenadores.
 * @param cantidad_entrenadores Numero real de entrenadores en RAM.
 * @param tabla Matriz de 8 grupos por 4 posiciones donde se devuelve la clasificacion.
 * @return true si todos los combates de grupos estaban finalizados, false si faltan resultados.
 */
bool calcular_clasificacion(Partido partidos[], int cantidad, Entrenador entrenadores[], int cantidad_entrenadores, TablaGrupo tabla[CANTIDAD_GRUPOS][ENTRENADORES_POR_GRUPO]);

/**
 * @brief Obtiene los 16 entrenadores clasificados a la fase eliminatoria.
 *
 * Almacena en el arreglo el primer y segundo lugar de cada grupo con la
 * nomenclatura oficial: 1A, 2A, 1B, 2B, ... 1H, 2H.
 *
 * @param tabla Matriz de clasificacion de los 8 grupos ya ordenada.
 * @param clasificados Arreglo donde se devuelven los IDs de los 16 clasificados.
 */
void obtener_clasificados(TablaGrupo tabla[CANTIDAD_GRUPOS][ENTRENADORES_POR_GRUPO], char clasificados[][8]);

/**
 * @brief Disputa la fase eliminatoria completa (octavos, cuartos, semifinal, 3.er lugar y final).
 *
 * Recorre los combates 49 a 64 pidiendo sus participantes al cuadro oficial. Si
 * los participantes dependen de un resultado pendiente, el combate se omite y
 * se continua con el siguiente. En esta fase no se permiten empates.
 *
 * @param entrenadores Arreglo global de Entrenadores.
 * @param cantidad_entrenadores Numero real de entrenadores en RAM.
 * @param partidos Arreglo de partidos del torneo.
 * @param clasificados Arreglo con los IDs de los 16 clasificados.
 * @return true si se disputaron todos los combates de la fase.
 */
bool fase_eliminatoria(Entrenador entrenadores[], int cantidad_entrenadores, Partido partidos[], char clasificados[][8]);

/**
 * @brief Muestra por pantalla los enfrentamientos de los combates de eliminatoria.
 * @param partidos Arreglo de partidos del torneo.
 * @param cantidad Total de partidos (CANTIDAD_PARTIDOS).
 * @param entrenadores Arreglo global de Entrenadores.
 * @param cantidad_entrenadores Numero real de entrenadores en RAM.
 */
void mostrar_enfrentamientos(Partido partidos[], int cantidad, Entrenador entrenadores[], int cantidad_entrenadores);

/**
 * @brief Pantalla de la clasificacion general de los 8 grupos.
 * @param tabla Matriz de clasificacion de los 8 grupos.
 */
void mostrar_clasificacion(TablaGrupo tabla[CANTIDAD_GRUPOS][ENTRENADORES_POR_GRUPO]);

/**
 * @brief Pantalla con todos los resultados oficiales del torneo agrupados por etapa.
 * @param partidos Arreglo de partidos del torneo.
 * @param cantidad Total de partidos (CANTIDAD_PARTIDOS).
 * @param entrenadores Arreglo global de Entrenadores.
 * @param cantidad_entrenadores Numero real de entrenadores en RAM.
 */
void mostrar_resultados_torneo(Partido partidos[], int cantidad, Entrenador entrenadores[], int cantidad_entrenadores);

/**
 * @brief Pantalla con el campeon, subcampeon, tercer y cuarto lugar del torneo.
 * @param partidos Arreglo de partidos del torneo.
 * @param cantidad Total de partidos (CANTIDAD_PARTIDOS).
 * @param entrenadores Arreglo global de Entrenadores.
 * @param cantidad_entrenadores Numero real de entrenadores en RAM.
 */
void mostrar_campeon(Partido partidos[], int cantidad, Entrenador entrenadores[], int cantidad_entrenadores);

/**
 * @brief Carga por teclado el resultado de uno o varios combates faltantes.
 *
 * Pide el numero de combate, verifica mediante obtener_participantes() quienes
 * deben jugarlo y valida que el ganador sea uno de los dos participantes.
 *
 * @param partidos Arreglo de partidos del torneo.
 * @param cantidad Total de partidos (CANTIDAD_PARTIDOS).
 * @param clasificados Arreglo con los IDs de los 16 clasificados.
 */
void cargar_resultados_teclado(Partido partidos[], int cantidad, char clasificados[][8]);

/**
 * @brief Muestra el historial detallado de combates guardado en disco.
 */
void mostrar_historial_combates(void);

/**
 * @brief Devuelve el nombre legible de la etapa de un combate.
 * @param etapa Etapa del torneo.
 * @return const char* Nombre de la etapa tal como se guarda (ej. "Octavos_de_Final").
 */
const char* nombre_etapa(EtapaTorneo etapa);

/**
 * @brief Copia el nombre de una etapa reemplazando los guiones bajos por espacios.
 * @param etapa Etapa del torneo.
 * @param destino Cadena donde se escribe el nombre legible (ej. "Octavos de Final").
 */
void nombre_etapa_legible(EtapaTorneo etapa, char destino[]);

/**
 * @brief Crea de una sola vez los equipos de todos los entrenadores que no lo tengan.
 *
 * Recorre el arreglo de entrenadores y, por cada uno sin equipo asignado, genera
 * su alineacion con el algoritmo de backtracking (crear_equipo_pokemon), calcula
 * las estadisticas de los ejemplares y los guarda en disco.
 *
 * @param vector_universo_pokemon Arreglo global de la Pokedex.
 * @param vector_entrenadores Arreglo global de Entrenadores.
 * @param cantidad_entrenadores Numero real de entrenadores cargados en RAM.
 * @return int Cantidad de equipos creados en esta ejecucion.
 */
int crear_equipos_todos(especies_pokemons vector_universo_pokemon[], Entrenador vector_entrenadores[], int cantidad_entrenadores);

/**
 * @brief Submenu de "Crear equipo": crear el equipo de uno o crear todos los que falten.
 * @param vector_universo_pokemon Arreglo global de la Pokedex.
 * @param vector_entrenadores Arreglo global de Entrenadores.
 * @param cantidad_entrenadores Numero real de entrenadores cargados en RAM.
 */
void menu_crear_equipos(especies_pokemons vector_universo_pokemon[], Entrenador vector_entrenadores[], int cantidad_entrenadores);

/**
 * @brief Submenu de "Cargar resultados": por teclado o desde un archivo de texto.
 * @param partidos Arreglo de partidos del torneo.
 * @param cantidad Total de partidos (CANTIDAD_PARTIDOS).
 * @param clasificados Arreglo con los IDs de los 16 clasificados.
 * @param entrenadores Arreglo global de Entrenadores.
 * @param cantidad_entrenadores Numero real de entrenadores en RAM.
 */
void menu_cargar_resultados(Partido partidos[], int cantidad, char clasificados[][8], Entrenador entrenadores[], int cantidad_entrenadores);

/**
 * @brief Carga resultados de combates desde un archivo de texto.
 *
 * El archivo debe tener una linea por combate con el formato:
 *   numero_combate id_ganador
 * Se valida cada linea: el combate debe existir, no estar jugado, tener sus
 * participantes definidos y el ganador debe ser uno de ellos. Las lineas
 * invalidas se informan y se omiten sin detener la carga.
 *
 * @param partidos Arreglo de partidos del torneo.
 * @param cantidad Total de partidos (CANTIDAD_PARTIDOS).
 * @param clasificados Arreglo con los IDs de los 16 clasificados.
 * @param nombre_archivo Nombre del archivo de texto a leer.
 * @return int Cantidad de combates registrados desde el archivo.
 */
int cargar_resultados_archivo(Partido partidos[], int cantidad, char clasificados[][8], char nombre_archivo[]);

/**
 * @brief Imprime el banner ASCII con colores que encabeza cada pantalla.
 * @param subtitulo Texto que se muestra debajo del titulo (puede ser "").
 */
void imprimir_banner(char subtitulo[]);

/**
 * @brief Imprime una linea separadora con color, del ancho estandar del sistema.
 */
void imprimir_separador(void);

/**
 * @brief Imprime una linea de menu alineada con el banner.
 * @param tecla Texto de la opcion (ej. "1", "10").
 * @param texto Descripcion de la opcion.
 */
void imprimir_opcion_menu(char tecla[], char texto[]);

/**
 * @brief Devuelve el codigo de color ANSI asociado a un tipo elemental.
 *
 * Permite pintar la etiqueta del tipo en las pantallas de la Pokedex y en las
 * fichas de los Pokemon, de forma que cada tipo se distinga de un vistazo.
 *
 * @param nombre_tipo Nombre del tipo (ej. "Fuego").
 * @return const char* Secuencia ANSI del color, o blanco si no se reconoce.
 */
const char* color_tipo(char nombre_tipo[]);

/**
 * @brief Imprime el tipo con su color y vuelve al color normal al terminar.
 * @param nombre_tipo Nombre del tipo a pintar.
 */
void imprimir_tipo_coloreado(char nombre_tipo[]);

/**
 * @brief Imprime un mensaje corto con marco, icono y color.
 *
 * Se usa en toda la aplicacion para errores, avisos y confirmaciones, de modo
 * que los mensajes tengan un formato uniforme y se distingan por color.
 *
 * @param icono Icono del mensaje (ej. "[!]").
 * @param titulo Titulo corto del mensaje.
 * @param detalle Explicacion del mensaje (puede ser "").
 * @param color Secuencia ANSI del color del marco (ej. "\033[1;31m").
 */
void imprimir_mensaje(char icono[], char titulo[], char detalle[], char color[]);

// =========================================================================
// MODULO 7: FUNCIONES AUXILIARES INTERNAS
// -------------------------------------------------------------------------
// Son de apoyo para las funciones grandes del torneo (seleccion de Pokemon,
// sustituciones, turnos de ataque, busquedas dentro de los grupos y armado de
// etiquetas). Se declaran aqui, en el header, igual que todas las demas.
// =========================================================================

/**
 * @brief Genera, calcula y guarda el equipo de un entrenador usando backtracking.
 *
 * Es la pieza compartida entre la creacion manual y la creacion masiva: baraja
 * la Pokedex, ejecuta el backtracking (crear_equipo_pokemon), calcula las
 * estadisticas de los 6 ejemplares, los guarda en disco y marca al entrenador
 * como que ya tiene equipo asignado.
 *
 * @param vector_universo_pokemon Arreglo global de la Pokedex.
 * @param vector_entrenadores Arreglo global de Entrenadores.
 * @param cantidad_entrenadores Numero real de entrenadores en RAM.
 * @param indice Entrenador al que se le crea el equipo.
 * @return true si el equipo se armo y guardo correctamente.
 */
bool generar_equipo_entrenador(especies_pokemons vector_universo_pokemon[], Entrenador vector_entrenadores[], int cantidad_entrenadores, int indice);

/**
 * @brief Ejecuta un turno de ataque de un Pokemon contra otro.
 *
 * Resta el daño calculado a la vida del defensor y, si este llega a 0, lo marca
 * como derrotado y suma la baja al contador de Pokemon derrotados del combate.
 *
 * @param atacante Entrenador dueno del Pokemon que ataca.
 * @param indice_atacante Indice del Pokemon atacante dentro de su equipo.
 * @param defensor Entrenador dueno del Pokemon que recibe el ataque.
 * @param indice_defensor Indice del Pokemon defensor dentro de su equipo.
 * @param permitir_empate true si el combate admite empate (fase de grupos).
 * @param derrotados Contador de Pokemon derrotados en el combate (puede ser NULL).
 */
void ejecutar_turno_ataque(Entrenador *atacante, int indice_atacante, Entrenador *defensor, int indice_defensor, bool permitir_empate, int *derrotados);

/**
 * @brief Elige el proximo Pokemon disponible despues del actual.
 *
 * Si no encuentra ninguno mas adelante, vuelve a revisar desde el comienzo del
 * equipo, de modo que la sustitucion siempre aprovecha a los Pokemon que quedan.
 *
 * @param equipo Arreglo con los ejemplares del entrenador.
 * @param cantidad Total de integrantes del equipo.
 * @param actual Indice del Pokemon que acaba de caer.
 * @return int Indice del siguiente Pokemon disponible, o -1 si no queda ninguno.
 */
int seleccionar_siguiente(EjemplarPokemon equipo[], int cantidad, int actual);

/**
 * @brief Valida que existan los 32 entrenadores con su equipo completo y utilizable.
 * @param entrenadores Arreglo global de Entrenadores.
 * @param cantidad Numero real de entrenadores cargados en RAM.
 * @return true si el torneo puede iniciar la fase de grupos.
 */
bool validar_participantes(Entrenador entrenadores[], int cantidad);

/**
 * @brief Busca la posicion (0 a 3) de un entrenador dentro de su grupo.
 * @param entrenadores Arreglo global de Entrenadores.
 * @param cantidad Numero real de entrenadores en RAM.
 * @param grupo Grupo en el que se busca (0 a 7).
 * @param id Identificador del entrenador buscado.
 * @return int Posicion dentro del grupo, o -1 si no pertenece a el.
 */
int buscar_entrenador_en_grupo(Entrenador entrenadores[], int cantidad, int grupo, char id[]);

/**
 * @brief Funcion de comparacion de qsort para ordenar la tabla de un grupo.
 *
 * Criterios en orden: puntos, victorias, Pokemon derrotados y, si persistiera el
 * empate, orden alfabetico por nombre.
 *
 * @param a Primera fila de la tabla a comparar.
 * @param b Segunda fila de la tabla a comparar.
 * @return int Negativo si 'a' va antes, positivo si va despues, 0 si empatan.
 */
int comparar_tabla_grupo(const void *a, const void *b);

/**
 * @brief Convierte un indice de clasificado en su etiqueta oficial (1A, 2B, ...).
 * @param indice Posicion dentro del arreglo de clasificados (0 a 15).
 * @param destino Cadena donde se escribe la etiqueta (al menos 4 bytes).
 */
void mostrar_etiqueta_clasificado(int indice, char destino[]);

/**
 * @brief Convierte la referencia de un cruce en una etiqueta legible.
 *
 * Si la referencia es un indice de clasificado (0 a 15) devuelve la etiqueta
 * oficial (1A, 2B, ...); si es un numero de combate devuelve "Ganador N".
 *
 * @param posicion Indice de clasificado o numero de combate de referencia.
 * @param destino Cadena donde se escribe la etiqueta (al menos 16 bytes).
 */
void mostrar_etiqueta_cruce(int posicion, char destino[]);

/**
 * @brief Indica si una cadena tiene contenido (es un ID de entrenador valido).
 * @param texto Cadena a evaluar.
 * @return true si la cadena no esta vacia.
 */
bool es_id_entrenador(char texto[]);

/**
 * @brief Copia el nombre del entrenador con ese ID, o una etiqueta si no existe.
 * @param entrenadores Arreglo global de Entrenadores.
 * @param cantidad Numero real de entrenadores en RAM.
 * @param id Identificador del entrenador buscado.
 * @param destino Cadena donde se escribe el nombre.
 */
void buscar_nombre_entrenador(Entrenador entrenadores[], int cantidad, char id[], char destino[]);

#endif
