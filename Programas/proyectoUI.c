#include <stdio.h>
#include <stdlib.h>


void instanciaDeLogica();

void titulo(){
    printf("·························CRUSIGRAMA·····························\n");
        printf(":░█▀▀░█▀█░█▀▀░█▀█░░░█▀▄░█▀▀░░░█░░░█▀█░█▀▀░░░█░█░█▀█░▀▀█░█▀█░█▀▀:\n");
        printf(":░█░░░█▀█░▀▀█░█▀█░░░█░█░█▀▀░░░█░░░█▀█░▀▀█░░░█▀█░█░█░░░█░█▀█░▀▀█:\n");
        printf(":░▀▀▀░▀░▀░▀▀▀░▀░▀░░░▀▀░░▀▀▀░░░▀▀▀░▀░▀░▀▀▀░░░▀░▀░▀▀▀░▀▀░░▀░▀░▀▀▀:\n");
        printf("································································\n\n");
}

void limpiarPantalla() {
    system("clear");
    titulo();
}

int es_igual(char a[], char b[]) {
    int i = 0;
    while (a[i] != '\0' || b[i] != '\0') {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return 1;
}

void instanciaDeUI() {
    int opcion;

    while (1) {
        limpiarPantalla();

        printf("Seleccione una opción:\n");
        printf("\033[33m[1] \033[0mCómo jugar\n");
        printf("\033[33m[2] \033[0mNueva partida\n");
        printf("\033[91m[3] Salir\033[0m\n\n");

        scanf("%d", &opcion);
        getchar();

        limpiarPantalla();

        if (opcion == 1) {
            printf("=== ¿Cómo jugar? ===\n");
            printf("OBJETIVO:\n");
            printf("Completa el crucigrama encontrando las palabras ocultas.\n\n");
            printf("MECÁNICA PRINCIPAL:\n");
            printf("- Se te mostrará una palabra para adivinar.\n");
            printf("- Tendrás 30 segundos para escribirla correctamente.\n");
            printf("- Si no la aciertas en el tiempo disponible, la palabra cambiará automáticamente.\n");

        } 
        else if (opcion == 2) {
            limpiarPantalla();
            printf("Iniciando nueva partida...\n\n");
            instanciaDeLogica();
        } 
        else if (opcion == 3) {
            printf("Saliendo...\n");
            exit(0);
        } 
        else {
            printf("Opción no válida\n");
        }

        printf("\nPresiona Enter para continuar...");
        getchar();
    }
}
