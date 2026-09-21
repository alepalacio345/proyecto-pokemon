#include <stdio.h>
#include "funciones.h"

int main(){

    int opcion;
    especies_pokemons vector_universo_pokemon[MAX_POKEDEX];

    do {
        limpiar_consola();
        
        // Título con colores ANSI (Amarillo brillante) y reseteo al final
        printf("\033[1;33m"); 
        printf(" ==================================================================== \n");
        printf("                        EL GRAN TORNEO POKEMON                        \n");
        printf(" ==================================================================== \n");
        printf("\033[0m"); 

        printf("  1. Consultar Pokedex\n");
        printf("  2. Registrar entrenador\n");
        printf("  3. Crear equipo\n");
        printf("  4. Consultar entrenadores\n");
        printf("  5. Consultar equipos\n");
        printf("  6. Cargar resultados\n");
        printf("  7. Consultar clasificacion\n");
        printf("  8. Consultar enfrentamientos\n");
        printf("  9. Consultar historial de combates\n");
        printf(" 10. Mostrar resultados del torneo\n");
        printf(" 11. Mostrar campeon\n");
        printf(" 12. Salir\n");
        printf(" ==================================================================== \n");
        printf(" -> Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                menu_consultar_pokedex(vector_universo_pokemon);
                break;
            case 2:
                registrar_entrenador();
                break;
            case 3:
                consulta_crear_equipo_pokemon(vector_universo_pokemon);
                break;
            case 4:
                imprimir_vector_entrenador();
                break;
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
                // Bloque para las opciones que aún no has programado
                printf("\n[!] Este modulo se encuentra en desarrollo...\n");
                break;
            case 12:
                printf("\nSaliendo del sistema del Gran Torneo. Hasta luego!\n");
                break;
            default:
                printf("\n[!] Opcion no valida. Intente de nuevo.\n");
                break;
        }

        // Sistema de pausa automática para que el usuario pueda leer los resultados
        // antes de que el ciclo do-while borre la pantalla.
        if (opcion != 12) {
            printf("\nPresione ENTER para continuar...");
            while(getchar() != '\n'); // Limpia cualquier residuo (como otros enters) en el buffer
            getchar();                // Espera a que el usuario presione la tecla
        }

    } while (opcion != 12);



    return 0;
}