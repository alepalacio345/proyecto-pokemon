#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funciones.h"
#include "consola.h"
#include "pixel_art.h"

int main(void){
    int opcion;
    int cantidad_entrenadores = 0;
    int cantidad_pokedex = 0;
    especies_pokemons vector_universo_pokemon[MAX_POKEDEX];
    Entrenador vector_entrenadores_torneo[MAX_ENTRENADORES];
    Partido vector_partidos[CANTIDAD_PARTIDOS];
    char vector_clasificados[MAX_CLASIFICADOS][8];
    TablaGrupo tabla_torneo[CANTIDAD_GRUPOS][ENTRENADORES_POR_GRUPO];

    // Semilla de las tiradas (monedas y barajado de equipos).
    srand((unsigned int)time(NULL));

    // Preparar la consola y mostrar la pantalla pixelada de bienvenida.
    consola_iniciar();
    pantalla_bienvenida();

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
        // El menu principal es una tabla tipo Excel con una division por modulo.
        imprimir_menu_principal();

        if (cantidad_entrenadores < 1) {
            consola_printf("  \033[1;33m[!]\033[0m No hay entrenadores cargados: registre participantes\n");
            consola_printf("      antes de usar las opciones del torneo.\n");
        }

        consola_printf(" -> Seleccione una opcion: ");
        // Lectura segura: una opcion no numerica no deja el buffer trabado
        opcion = leer_entero(-1);

        // Al entrar a cualquier modulo se limpia la pantalla.
        consola_limpiar();

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
                mostrar_historial_combates(vector_entrenadores_torneo, cantidad_entrenadores);
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
                imprimir_separador();
                consola_printf("  \033[1;32m[+] BASE DE DATOS RECARGADA EXITOSAMENTE\033[0m\n");
                consola_printf("      Entrenadores: %d | Especies: %d\n", cantidad_entrenadores, cantidad_pokedex);
                imprimir_separador();
                consola_printf("\n");
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
                    consola_printf("\n\033[1;31m[!] ERROR:\033[0m aun faltan combates de la fase de grupos.\n");
                    consola_printf(" Dispute o cargue todos los combates de grupos (1 a %d) antes de\n", COMBATES_GRUPOS);
                    consola_printf(" avanzar a la fase eliminatoria.\n\n");
                }
                break;
            case 15:
                consola_printf("\n\033[1;32mSaliendo del sistema del Gran Torneo. Hasta luego!\033[0m\n");
                break;
            default:
                consola_printf("\n\033[1;31m[!] Opcion no valida.\033[0m Intente de nuevo.\n");
                break;
        }

        // Pausa portable. Si la entrada se agota (por ejemplo, al ejecutar con
        // un archivo redirigido) el ciclo termina en lugar de quedarse esperando.
        if (opcion != 15) {
            if (!consola_esperar_enter("\nPresione ENTER para continuar...")) {
                break;
            }
            // Al volver al menu se limpia lo que dejo el modulo anterior.
            consola_limpiar();
        }

    } while (opcion != 15);



    return 0;
}
