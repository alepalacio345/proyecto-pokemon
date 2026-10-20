#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funciones.h"

int main(){
    //semill
    srand(time(NULL));

    int opcion;
    int cantidad_entrenadores = 0;
    int cantidad_pokedex = 0;
    especies_pokemons vector_universo_pokemon[MAX_POKEDEX];
    Entrenador vector_entrenadores_torneo[MAX_ENTRENADORES];
    Partido vector_partidos[CANTIDAD_PARTIDOS];
    char vector_clasificados[MAX_CLASIFICADOS][8];
    TablaGrupo tabla_torneo[CANTIDAD_GRUPOS][ENTRENADORES_POR_GRUPO];

    llenar_vector_pokemon(vector_universo_pokemon);
    cantidad_pokedex = contar_especies_pokemon(vector_universo_pokemon, MAX_POKEDEX);
    llenar_vector_entrenador(vector_entrenadores_torneo,&cantidad_entrenadores);
    cargar_equipos_pokemon(vector_entrenadores_torneo, cantidad_entrenadores, vector_universo_pokemon);

    // El torneo arranca con sus 64 combates definidos y con los resultados que ya
    // estuvieran guardados en resultados_combates.txt (se pueden cargar de a poco).
    inicializar_partidos(vector_partidos, CANTIDAD_PARTIDOS,
                         vector_entrenadores_torneo, cantidad_entrenadores);
    cargar_partidos(vector_partidos, CANTIDAD_PARTIDOS);

    do {
        limpiar_consola();

        // Banner con arte ASCII (solo la primera vez) y encabezado compacto despues
        imprimir_banner("MENU PRINCIPAL");

        imprimir_opcion_menu("1", "Consultar Pokedex");
        imprimir_opcion_menu("2", "Registrar entrenador");
        imprimir_opcion_menu("3", "Crear equipo");
        imprimir_opcion_menu("4", "Consultar entrenadores");
        imprimir_opcion_menu("5", "Consultar equipos");
        imprimir_opcion_menu("6", "Cargar resultados");
        imprimir_opcion_menu("7", "Consultar clasificacion");
        imprimir_opcion_menu("8", "Consultar enfrentamientos");
        imprimir_opcion_menu("9", "Consultar historial de combates");
        imprimir_opcion_menu("10", "Mostrar resultados del torneo");
        imprimir_opcion_menu("11", "Mostrar campeon");
        imprimir_opcion_menu("12", "Recargar base de datos");
        imprimir_opcion_menu("13", "Disputar fase de grupos");
        imprimir_opcion_menu("14", "Disputar fase eliminatoria");
        imprimir_opcion_menu("15", "Salir");
        imprimir_separador();

        if (cantidad_entrenadores < 1) {
            printf("  \033[1;33m[!]\033[0m No hay entrenadores cargados: registre participantes\n");
            printf("      antes de usar las opciones del torneo.\n");
            imprimir_separador();
        }

        printf(" -> Seleccione una opcion: ");
        // Lectura segura: una opcion no numerica no deja el buffer trabado
        opcion = leer_entero(-1);

        switch(opcion) {
            case 1:
                menu_consultar_pokedex(vector_universo_pokemon, cantidad_pokedex);
                break;
            case 2:
                registrar_entrenador(vector_entrenadores_torneo, &cantidad_entrenadores);
                break;
            case 3:
                menu_crear_equipos(vector_universo_pokemon, vector_entrenadores_torneo, cantidad_entrenadores);
                break;
            case 4:
                Consultar_entrenadores(vector_entrenadores_torneo,cantidad_entrenadores);
                break;
            case 5:
                consultar_equipos_creasdos(vector_entrenadores_torneo,cantidad_entrenadores);
                break;
            case 6:
                menu_cargar_resultados(vector_partidos, CANTIDAD_PARTIDOS, vector_clasificados,
                                       vector_entrenadores_torneo, cantidad_entrenadores);
                break;
            case 7:
                // La clasificacion se recalcula siempre desde los resultados cargados,
                // asi el sistema funciona igual con resultados completos o parciales.
                calcular_clasificacion(vector_partidos, CANTIDAD_PARTIDOS,
                                       vector_entrenadores_torneo, cantidad_entrenadores,
                                       tabla_torneo);
                obtener_clasificados(tabla_torneo, vector_clasificados);
                mostrar_clasificacion(tabla_torneo);
                break;
            case 8:
                mostrar_enfrentamientos(vector_partidos, CANTIDAD_PARTIDOS,vector_entrenadores_torneo, cantidad_entrenadores);
                break;
            case 9:
                mostrar_historial_combates();
                break;
            case 10:
                mostrar_resultados_torneo(vector_partidos, CANTIDAD_PARTIDOS,vector_entrenadores_torneo, cantidad_entrenadores);
                break;
            case 11:
                mostrar_campeon(vector_partidos, CANTIDAD_PARTIDOS,vector_entrenadores_torneo, cantidad_entrenadores);
                break;
            case 12:
                cantidad_entrenadores = 0;
                llenar_vector_entrenador(vector_entrenadores_torneo,&cantidad_entrenadores);
                cargar_equipos_pokemon(vector_entrenadores_torneo, cantidad_entrenadores, vector_universo_pokemon);
                inicializar_partidos(vector_partidos, CANTIDAD_PARTIDOS,
                                     vector_entrenadores_torneo, cantidad_entrenadores);
                cargar_partidos(vector_partidos, CANTIDAD_PARTIDOS);
                limpiar_consola();
                imprimir_separador();
                printf("  \033[1;32m[+] BASE DE DATOS RECARGADA EXITOSAMENTE\033[0m\n");
                printf("      Entrenadores: %d | Especies: %d\n", cantidad_entrenadores, cantidad_pokedex);
                imprimir_separador();
                printf("\n");
                break;
            case 13:
                // Fase de grupos: 8 grupos de 4 entrenadores, 48 combates
                fase_de_grupos(vector_entrenadores_torneo, cantidad_entrenadores, vector_partidos);

                if (calcular_clasificacion(vector_partidos, CANTIDAD_PARTIDOS,vector_entrenadores_torneo, cantidad_entrenadores,tabla_torneo)) {
                    obtener_clasificados(tabla_torneo, vector_clasificados);
                }
                break;
            case 14:
                // Fase eliminatoria: octavos, cuartos, semifinal, tercer lugar y final
                if (calcular_clasificacion(vector_partidos, CANTIDAD_PARTIDOS,vector_entrenadores_torneo, cantidad_entrenadores,tabla_torneo)) {
                    obtener_clasificados(tabla_torneo, vector_clasificados);
                    fase_eliminatoria(vector_entrenadores_torneo, cantidad_entrenadores,vector_partidos, vector_clasificados);
                } else {
                    printf("\n\033[1;31m[!] ERROR:\033[0m aun faltan combates de la fase de grupos.\n");
                    printf(" Dispute o cargue todos los combates de grupos (1 a %d) antes de\n", COMBATES_GRUPOS);
                    printf(" avanzar a la fase eliminatoria.\n\n");
                }
                break;
            case 15:
                printf("\n\033[1;32mSaliendo del sistema del Gran Torneo. Hasta luego!\033[0m\n");
                break;
            default:
                printf("\n\033[1;31m[!] Opcion no valida.\033[0m Intente de nuevo.\n");
                break;
        }

        // Sistema de pausa: se consume el resto de la linea (incluido el ENTER de la
        // opcion) y luego se espera un ENTER del usuario. Si la entrada se agota
        // (fin de archivo) el ciclo termina en lugar de quedarse esperando.
        if (opcion != 15) {
            int caracter;

            printf("\nPresione ENTER para continuar...");
            while (((caracter = getchar()) != '\n') && (caracter != EOF));
        }

    } while (opcion != 15);



    return 0;
}
