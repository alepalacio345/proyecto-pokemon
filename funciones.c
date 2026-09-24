/**
 * @file funciones.c
 * @brief Implementación lógica de la Pokédex y utilidades del sistema.
 */
#include <stdio.h>
#include <string.h>
#include "funciones.h"
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// ======================================
// FUNCIONES AUXILIARES PARA LA POKEDEX =
// ======================================
void llenar_vector_pokemon(especies_pokemons vector[], int i){
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
                strcpy(vector[i].tipo_segundario, "Ninguno");
            }else{
                // si no es un pokemon que tiene tipo primario y segundario
                sscanf(linea,"%s %s %s %s %i %i %i %i",id,nombre,tipo_p,tipo_s,&hp,&ataque,&defensa,&velocidad);
                strcpy(vector[i].tipo_segundario,tipo_s);
            }

            strcpy(vector[i].ID,id);
            strcpy(vector[i].Nombre,nombre);
            strcpy(vector[i].tipo_primario,tipo_p);
            
            vector[i].HP_b = hp;
            vector[i].ataque_b = ataque;
            vector[i].defensa_b = defensa;
            vector[i].velocidad_b = velocidad;

            i++;
        };
    }
    fclose(archivo_entrada);
};

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

void buscar_vector_pokemon(especies_pokemons vector[], char string[], bool campo, int cantidad, bool *bandera, int *indice){

    *bandera = true;
    int i = 0;
    
    // buscamos hasta que se encuentre el nombre o hasta que lleguemos al final del vector
    while(*bandera && (i < cantidad) ){

        // si el campo es True osea que busca ID y encontramos el pokemon nos detenemos 
        if( campo && (strcmp(vector[i].ID, string) == 0) ){
            *bandera = false;
            *indice = i;
            
        }else if(strcmp(vector[i].Nombre, string) == 0){
            *bandera = false;
            *indice = i;
        }
        i++;
    }
}

void consultar_pokedex_id(especies_pokemons vector[], int cantidad) {

    bool bandera;
    int indice;

    char id_buscar[8];
    printf("\n-> Ingrese el ID (Numero) del Pokemon a buscar: ");
    scanf("%s",id_buscar);

    buscar_vector_pokemon(vector,id_buscar,true,cantidad,&bandera,&indice);
    
    limpiar_consola();

    if(!bandera){
        mostrar_estadisticas_pokemon(vector, atoi(vector[indice].ID) - 1);
    }else{

        printf("\n=======================================================\n");
        printf("            [!] ERROR: POKEMON NO ENCONTRADO           \n");
        printf("=======================================================\n");
        printf(" El numero ingresado no esta registrado en la Pokedex.\n");
        printf("=======================================================\n\n");
    }
}

void consultar_pokedex_nombre(especies_pokemons vector[], int cantidad) {

    bool bandera;
    int indice;

    char nombre_buscar[20];
    printf("\n-> Ingrese el nombre exacto del Pokemon: ");
    scanf("%19s", nombre_buscar);

    buscar_vector_pokemon(vector,nombre_buscar,false,MAX_POKEDEX,&bandera,&indice);
    
    limpiar_consola();

    if(!bandera){
        mostrar_estadisticas_pokemon(vector, atoi(vector[indice].ID) - 1);
    }else{

        printf("\n=======================================================\n");
        printf("            [!] ERROR: POKEMON NO ENCONTRADO           \n");
        printf("=======================================================\n");
        printf(" No existe ninguna especie registrada con ese nombre.\n");
        printf("=======================================================\n\n");
    }
    
}

void menu_consultar_pokedex(especies_pokemons vector_universo_pokemon[]) {
    //var
    int opcion_pokedex;
 
    limpiar_consola();
    printf("\n=======================================================\n");
    printf("                  CONSULTAR POKEDEX                    \n");
    printf("=======================================================\n");
    printf(" 1. Mostrar la Pokedex completa\n");
    printf(" 2. Buscar Pokemon por numero (ID)\n");
    printf(" 3. Buscar Pokemon por nombre\n");
    printf(" 4. Regresar al menu principal\n");
    printf("=======================================================\n");
    printf(" -> Seleccione una opcion: ");
    scanf("%d", &opcion_pokedex);

    switch(opcion_pokedex) {
        case 1:
            imprimir_vector_pokemon(vector_universo_pokemon, MAX_POKEDEX);
            break;
        case 2:
            consultar_pokedex_id(vector_universo_pokemon, MAX_POKEDEX);
            break;
        case 3:
            consultar_pokedex_nombre(vector_universo_pokemon, MAX_POKEDEX);
            break;
        case 4:
            // No hace nada, el flujo vuelve naturalmente al menú principal
            break;
        default:
            printf("\n[!] Opcion no valida.\n");
    }
}
// =========================================
// FUNCIONES PARA REGISTRO DE ENTRENADORES =
// =========================================

void registrar_entrenador(){
    
    //var
    FILE *archivo_entrenadores;
    bool resultado;
    Entrenador info_entrenador;
    char opcion;

    limpiar_consola();

    // Encabezado visual de la planilla
    printf("\n=======================================================\n");
    printf("             INSCRIPCION DE NUEVO ENTRENADOR           \n");
    printf("=======================================================\n");

    // Inicializamos en cero para evitar basura en la memoria
    info_entrenador.cantidad_pokemon = 0;
    info_entrenador.victorias = 0;
    info_entrenador.empates = 0;
    info_entrenador.derrotas = 0;
    info_entrenador.puntuacion = 0;

    //Solicitud del Nombre
    printf("-> Ingrese el nombre del entrenador: ");
    scanf("%s", info_entrenador.nombre);


    do{
        //Solicitud del ID
        printf("-> Ingrese el ID identificador: ");
        scanf("%s",info_entrenador.id_entrenador);

        resultado = validad_entrenador_repetido(info_entrenador.id_entrenador);

        //validamos que la info no este repetida
        if(resultado){
            printf("\n=======================================================\n");
            printf("             \033[1;31m[!] ERROR: ID DUPLICADO\033[0m                   \n");
            printf("=======================================================\n");
            printf(" Ya existe un participante registrado con el ID: %s\n", info_entrenador.id_entrenador);
            printf("=======================================================\n");
            printf(" Presione 'C' para intentar con otro ID o 'S' para salir.\n");
            printf(" -> Opcion: ");
            scanf(" %c", &opcion);
            //convertimos a masyusculas
            opcion = toupper(opcion);
            //usamos el operador ternario
            resultado = (opcion == 'C') ? true : false;
            //limpiamos consola
            limpiar_consola();

        }else{
            
            //abrimos el archivo entrenadores
            archivo_entrenadores = fopen("entrenadores.txt", "a+");

            if(archivo_entrenadores == NULL){
                ARCHIVO_ERROR;
            }else{
                fprintf(archivo_entrenadores,"%s %s %d %d %d %d %d %s\n",info_entrenador.id_entrenador, info_entrenador.nombre,info_entrenador.cantidad_pokemon,info_entrenador.victorias,info_entrenador.empates,info_entrenador.derrotas,info_entrenador.puntuacion,"false");

                printf("=======================================================\n");
                printf(" Entrenador '%s' \033[1;32m registrado exitosamente.\033[0m \n", info_entrenador.nombre);
                printf("=======================================================\n\n");\
                //cerrar archivo
                fclose(archivo_entrenadores);
            }

           
        }
    
    }while(resultado);

}

bool validad_entrenador_repetido(char id_entrenador[]){
    //var
    FILE *archivo_entrenadores;
    bool bandera = true;
    char id_archivo[8];
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

            resultado_op = fscanf(archivo_entrenadores,"%s %*s",&id_archivo) == EOF;
            
            // si se llega al final se detiene el ciclo
            if(resultado_op){
                bandera = false;
            // o si se encuentra el repetido se detiene
            }else if(strcmp(id_archivo,id_entrenador) == 0){
                salida = true;
                bandera = false;
            }

        }

        return salida;
    }

    fclose(archivo_entrenadores);

}

void imprimir_vector_entrenador(){
    
    //var
    FILE *archivo_entrenadores;
    char id_entrenador[8];
    char nombre_entrenador[30];

    //abrimos archivo
    archivo_entrenadores = fopen("entrenadores.txt", "r");

    //validar apertura del archivo
    if(archivo_entrenadores == NULL){
        ARCHIVO_ERROR;
        
    }else{

        limpiar_consola();
        
        // Encabezado de la tabla
        printf("\n=======================================================\n");
        printf("          \033[1;36mLISTA DE ENTRENADORES REGISTRADOS\033[0m            \n");
        printf("=======================================================\n");
        printf(" \033[1;33m%-10s | %-30s\033[0m\n", "ID", "NOMBRE DEL ENTRENADOR");
        printf("-------------------------------------------------------\n");

        // Lectura de datos
        while(fscanf(archivo_entrenadores,"%s %s",id_entrenador,nombre_entrenador) == 2){
            
            // Imprimimos los datos del entrenador alineados en columnas
            printf(" %-10s | %-30s\n", id_entrenador, nombre_entrenador);
        }

        // Cierre de la tabla
        printf("=======================================================\n\n");
        
        // El archivo SOLO se cierra si se abrio correctamente
        fclose(archivo_entrenadores);
    }
}

void mostrar_seleccion_entrenadores(Entrenador vector[], int cantidad) {
    
    limpiar_consola();

    // Encabezado de la tabla (Color Cyan)
    printf("\n=================================================================\n");
    printf("           \033[1;36mDISPONIBILIDAD DE ENTRENADORES Y EQUIPOS\033[0m            \n");
    printf("=================================================================\n");
    
    // Nombres de las columnas (Color Amarillo)
    printf(" \033[1;33m%-10s | %-25s | %-15s\033[0m\n", "ID", "NOMBRE", "ESTADO DEL EQUIPO");
    printf("-----------------------------------------------------------------\n");

    // Recorremos el vector que ya esta cargado en la memoria RAM
    for (int i = 0; i < cantidad; i++) {

        // Comparamos el campo de tu estructura para decidir el color
        if (strcmp(vector[i].asignaciom_equipo, "true") == 0) {
            // Entrenador que YA tiene equipo asignado (Verde)
            printf(" %-10s | %-25s | \033[1;32m[+] Con Equipo\033[0m\n",vector[i].id_entrenador, vector[i].nombre);
                   
        } else {
            // Entrenador que NO tiene equipo todavia (Rojo)
            printf(" %-10s | %-25s | \033[1;31m[-] Sin Equipo\033[0m\n",vector[i].id_entrenador, vector[i].nombre);
        }
    }
    
    printf("=================================================================\n");
}

void llenar_y_guardar_entrenadores(Entrenador vector[], int *i, char operacion[7]){
    
    FILE *entrada_archivo;
    char id[8];
    char nombre[30];
    char asignacion[6];

    // abrir archivo
    entrada_archivo = fopen("entrenadores.txt", "r+");

    //validar apertura
    if(entrada_archivo == NULL){
        ARCHIVO_ERROR;
    }else{

        if(strcmp(operacion,"llenar") == 0){

            while(fscanf(entrada_archivo,"%s %s %d %d %d %d %d %s\n",id,nombre,&vector[*i].cantidad_pokemon,&vector[*i].victorias,&vector[*i].empates,&vector[*i].derrotas,&vector[*i].puntuacion,asignacion) == 8){
                strcpy(vector[*i].id_entrenador, id);
                strcpy(vector[*i].nombre, nombre);
                strcpy(vector[*i].asignaciom_equipo, asignacion);
                (*i)++;
            }

        }else{
            
            for(int j = 0; j < *i; j++){
                fprintf(entrada_archivo,"%s %s %d %d %d %d %d %s\n",vector[j].id_entrenador,vector[j].nombre,vector[j].cantidad_pokemon,vector[j].victorias,vector[j].empates,vector[j].derrotas,vector[j].puntuacion,vector[j].asignaciom_equipo);
            }
        }



        fclose(entrada_archivo);
    }
}

void buscar_entrenador(Entrenador vector[], char id[], int cantidad, bool *bandera, int *indice){

    int i = 0;
    *bandera = true;
    // buscamos hasta que se encuentre el nombre o hasta que lleguemos al final del vector
    while(*bandera && (i < cantidad) ){

        // si el campo es True osea que busca ID y encontramos el pokemon nos detenemos 
        if((strcmp(vector[i].id_entrenador, id) == 0) ){
            *bandera = false;
            *indice = i;
        }

        i++;
    }

}
// =============================================================
// FUNCIONES PARA CREAR EQUIPO POKEMONES (EL BACKTRACKING)     =
// =============================================================

void entregar_y_guardar_equipo_pokemon(Entrenador entrenador[],int indice, EjemplarPokemon equipo_pokemon[]){

    //var
    FILE *archivo_entreda;

    //abrir archivo
    archivo_entreda = fopen("equipos_pokemon.txt","a+");

    //validamos apertura del archivo
    if(archivo_entreda == NULL){
        ARCHIVO_ERROR;
    }else{

        fprintf(archivo_entreda,"%s %s\n",entrenador[indice].id_entrenador, entrenador[indice].nombre);
        //cantidad de pokemones actuales
        entrenador[indice].cantidad_pokemon = 6;

        for(int i = 0; i < MAX_POKEMONES_EN_EQ; i++){
            strcpy(entrenador[indice].equipo[i].id_ejemplar,equipo_pokemon[i].id_ejemplar);
            strcpy(entrenador[indice].equipo[i].nombre,equipo_pokemon[i].nombre);
            entrenador[indice].equipo[i].ataque = equipo_pokemon[i].ataque;
            entrenador[indice].equipo[i].defensa = equipo_pokemon[i].defensa;
            entrenador[indice].equipo[i].velocidad = equipo_pokemon[i].velocidad;
            entrenador[indice].equipo[i].estado = equipo_pokemon[i].estado;
            entrenador[indice].equipo[i].experiencia = equipo_pokemon[i].experiencia;
            entrenador[indice].equipo[i].hp_actual = equipo_pokemon[i].hp_actual;
            entrenador[indice].equipo[i].hp_maximo = equipo_pokemon[i].hp_maximo;
            entrenador[indice].equipo[i].nivel = equipo_pokemon[i].nivel;
            entrenador[indice].equipo[i].datos_especie = equipo_pokemon[i].datos_especie;

            fprintf(archivo_entreda, "%s %s %d %d %d %d %d %d %d %d\n", 
                    equipo_pokemon[i].id_ejemplar, 
                    equipo_pokemon[i].nombre,
                    equipo_pokemon[i].nivel,
                    equipo_pokemon[i].hp_actual,
                    equipo_pokemon[i].hp_maximo,
                    equipo_pokemon[i].ataque,
                    equipo_pokemon[i].defensa,
                    equipo_pokemon[i].velocidad,
                    equipo_pokemon[i].experiencia,
                    equipo_pokemon[i].estado);
        }

        fclose(archivo_entreda);
    }

    
}

void imprimir_equipo_pokemon(EjemplarPokemon equipo[], int cantidad) {
    
    // Encabezado de la tabla (Cyan)
    printf("\n========================================================================================\n");
    printf("                             \033[1;36mALINEACION DEL EQUIPO POKEMON\033[0m                              \n");
    printf("========================================================================================\n");
    
    // Nombres de las columnas (Amarillo)
    printf(" \033[1;33m%-3s | %-12s | %-5s | %-10s | %-10s | %-4s | %-4s | %-4s\033[0m\n", 
           "N.", "NOMBRE", "NIVEL", "TIPO 1", "TIPO 2", "HP", "ATQ", "DEF");
    printf("----------------------------------------------------------------------------------------\n");

    // Ciclo para recorrer a todos los integrantes del equipo
    for (int i = 0; i < cantidad; i++) {
        
        // Imprimimos los datos. Notarás que extraemos los tipos y estadisticas 
        // viajando a traves del puntero "datos_especie->" que vinculamos en el backtracking.
        printf(" %-3d | \033[1;32m%-12s\033[0m | %-5d | %-10s | %-10s | %-4d | %-4d | %-4d\n",
               i + 1,                                       
               equipo[i].nombre,                            // Nombre del Ejemplar (En color verde)
               equipo[i].nivel,                             
               equipo[i].datos_especie->tipo_primario,     
               equipo[i].datos_especie->tipo_segundario,    
               equipo[i].hp_actual,                          
               equipo[i].ataque,           
               equipo[i].defensa);         
    }
    
    // Cierre de la tabla
    printf("========================================================================================\n\n");
}

void crear_estadisticas_ejemplares(EjemplarPokemon pokemon[], int cantidad){

    int resultado_operacion;

    for(int i = 0; i < cantidad; i++){

        // calculamos la hp-actual y el hp-maximo
        resultado_operacion = ((pokemon[i].datos_especie->HP_b * pokemon[i].nivel) / 50 ) + pokemon[i].nivel + 10;
        pokemon[i].hp_actual = resultado_operacion;
        pokemon[i].hp_maximo = resultado_operacion;

        // calculamos Ataque, Defensa y Velocidad
        pokemon[i].ataque = ( (pokemon[i].datos_especie->ataque_b * pokemon[i].nivel) / 50) + 5;
        pokemon[i].defensa = ( (pokemon[i].datos_especie->defensa_b * pokemon[i].nivel) / 50) + 5;
        pokemon[i].velocidad = ( (pokemon[i].datos_especie->velocidad_b * pokemon[i].nivel) / 50) + 5;

    }



};
// Verifica si un Pokemon ya fue metido en el arreglo del equipo
bool pokemon_ya_esta_en_equipo(EjemplarPokemon equipo[], int cantidad, char nombre[]) {
    bool encontrado = false;
    
    // El ciclo se detiene solo si llega al final o si 'encontrado' se vuelve true
    for (int i = 0; i < cantidad && !encontrado; i++) {
        if (strcmp(equipo[i].nombre, nombre) == 0) {
            encontrado = true;
        }
    }
    return encontrado;
}
// Cuenta cuantos tipos elementales distintos hay en el equipo ya formado
bool validar_variedad_tipos(EjemplarPokemon equipo[], int cantidad) {
    char tipos_unicos[MAX_POKEMONES_EN_EQ][15];
    int contador_tipos = 0;

    for (int i = 0; i < cantidad; i++) {
        bool tipo_repetido = false;
        
        // Revisamos si el tipo del pokemon actual ya lo habiamos anotado
        for (int j = 0; j < contador_tipos && !tipo_repetido; j++) {
            // Entramos a los datos de la especie original para ver su tipo primario
            if (strcmp(equipo[i].datos_especie->tipo_primario, tipos_unicos[j]) == 0) {
                tipo_repetido = true;
            }
        }
        
        // Si no estaba repetido, lo agregamos a nuestra lista
        if (!tipo_repetido) {
            strcpy(tipos_unicos[contador_tipos], equipo[i].datos_especie->tipo_primario);
            contador_tipos++;
        }
    }

    // Retorna verdadero si logramos juntar 4 o mas tipos diferentes
    if(contador_tipos >= 4){
        return true;
    } else {
        return false;
    }
}

void mezclar_baraja(int arreglo[], int cantidad_elementos) {
    // Recorremos el arreglo desde la ultima posicion hasta la posicion 1
    for (int i = cantidad_elementos - 1; i > 0; i--) {
        
        // Generamos un indice aleatorio 'j' que va desde 0 hasta 'i'
        int j = rand() % (i + 1);

        // Hacemos el intercambio (swap) de los valores
        int temporal = arreglo[i];
        arreglo[i] = arreglo[j];
        arreglo[j] = temporal;
    }
}

void inicializar_barajas(int vector[]){

    for(int i = 0; i < MAX_POKEDEX; i++){
        vector[i] = i;
    }

}

bool crear_equipo_pokemon(int integrantes_equipo, int nivel_actual_eq, int baraja[], EjemplarPokemon equipo_pokemon[], especies_pokemons vector_universo_pokemon[]) {
    
    // Variable de control (Único punto de salida)
    bool salida = false; 

    // ==========================================================
    // 1. CASO BASE (Si ya llenamos los 6 espacios)
    // ==========================================================
    if (integrantes_equipo == 6) {
        
        // Llamamos a la función auxiliar para contar los tipos
        if (validar_variedad_tipos(equipo_pokemon, 6)) {
            salida = true;  // Éxito: Suman menos de 300 (por la Poda 2) y hay buena variedad
        } else {
            salida = false; // Fracaso: Faltó variedad de tipos
        }
        
    } 
    // ==========================================================
    // 2. EXPLORACIÓN (Si aún nos faltan integrantes)
    // ==========================================================
    else {
        
        // El ciclo revisa la baraja. Se detiene si llega al final (MAX_POKEDEX)
        // O se detiene si 'salida' se vuelve true (lo que reemplaza al 'break')
        for (int i = 0; i < MAX_POKEDEX && !salida; i++) {

            int indice = baraja[i];
            int nivel_simulado = 50; 
            
            // PODA 1: Solo avanza si NO está repetido (Reemplaza al 'continue')
            if (!pokemon_ya_esta_en_equipo(equipo_pokemon, integrantes_equipo, vector_universo_pokemon[indice].Nombre)) {
                
                // PODA 2: Solo avanza si el nivel no excede 300 (Reemplaza al 'continue')
                if ((nivel_actual_eq + nivel_simulado) <= 300) {
                    
                    // PASO A: HACER
                    equipo_pokemon[integrantes_equipo].datos_especie = &vector_universo_pokemon[indice];
                    strcpy(equipo_pokemon[integrantes_equipo].nombre, vector_universo_pokemon[indice].Nombre);
                    equipo_pokemon[integrantes_equipo].nivel = nivel_simulado;
                    
                    // PASO B: EXPLORAR
                    salida = crear_equipo_pokemon(
                        integrantes_equipo + 1, 
                        nivel_actual_eq + nivel_simulado, 
                        baraja, 
                        equipo_pokemon, 
                        vector_universo_pokemon
                    );

                    // PASO C: DESHACER
                    // Como no hay variables de control extra que limpiar, si 'salida' regresa 
                    // valiendo 'false', el bloque if termina, el ciclo for da otra vuelta 
                    // y el PASO A simplemente aplasta los datos del Pokémon que falló.
                }
            }
        }
    }

    // ÚNICO RETURN DE LA FUNCIÓN
    return salida;
}

void consulta_crear_equipo_pokemon(especies_pokemons vector_universo_pokemon[], Entrenador vector_entrenadores[], int cantidad_entrenadores){


    //arreglos
    EjemplarPokemon equipo_pokemon[MAX_POKEMONES_EN_EQ];
    int vector_baraja[MAX_POKEDEX];
    bool existe_entrenador;
    char nombre_elegido[30];
    int indice;

    mostrar_seleccion_entrenadores(vector_entrenadores,cantidad_entrenadores);
    printf("\n -> Ingrese el nombre del entrenador para crearle un equipo: ");
    scanf("%s", nombre_elegido);

    buscar_entrenador(vector_entrenadores,nombre_elegido,cantidad_entrenadores,&existe_entrenador,&indice);
    // si existe el entrenador 
    if( (!existe_entrenador) ){

        //llenar vector baraja
        inicializar_barajas(vector_baraja);
        // se llama a funcion que mezcla el vector
        mezclar_baraja(vector_baraja,MAX_POKEDEX);

        limpiar_consola();

        if (crear_equipo_pokemon(0, 0, vector_baraja, equipo_pokemon, vector_universo_pokemon)) {

            crear_estadisticas_ejemplares(equipo_pokemon,MAX_POKEMONES_EN_EQ);
            //le vamos sus pokemones al entrenador
            
            // Bloque de ÉXITO (Letras Verdes)
            printf("\n=======================================================\n");
            printf("             \033[1;32m[+] EQUIPO CREADO EXITOSAMENTE\033[0m            \n");
            printf("=======================================================\n");
            printf(" El sistema ha ensamblado un equipo equilibrado que\n");
            printf(" cumple estrictamente con las reglas del torneo.\n");
            printf("=======================================================\n\n");

        } else {
            
            // Bloque de FRACASO (Letras Rojas)
            printf("\n=======================================================\n");
            printf("             \033[1;31m[-] ERROR AL ENSAMBLAR EQUIPO\033[0m             \n");
            printf("=======================================================\n");
            printf(" El algoritmo no encontro ninguna combinacion valida\n");
            printf(" que cumpla con los limites de nivel y tipos.\n");
            printf("=======================================================\n\n");
            
        }

    }
    
    

}

// ======================
// FUNCIONES DE CONSOLA =
// ======================
void limpiar_consola() {
#if defined(_WIN32) || defined(_WIN64)
    // Cubre Windows en versiones de 32 y 64 bits
    system("cls");
#elif defined(__APPLE__) || defined(__MACH__)
    // Cubre el ecosistema macOS de Apple
    system("clear");
#elif defined(__linux__) || defined(__unix__) || defined(__unix)
    // Cubre distribuciones Linux y sistemas basados en Unix
    system("clear");
#else
    // Respaldo de seguridad (Fallback): 
    // Si el sistema es un entorno raro o desconocido, usamos un Código de Escape ANSI
    // \033[H mueve el cursor al inicio, \033[2J limpia toda la pantalla
    printf("\033[H\033[2J"); 
#endif
}