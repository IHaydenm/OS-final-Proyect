#include <stdio.h>
#include <stdlib.h>

void limpiarPantalla() {
    system("clear");
}

void titulo(){
    printf("·························CRUSIGRAMA·····························\n");
        printf(":░█▀▀░█▀█░█▀▀░█▀█░░░█▀▄░█▀▀░░░█░░░█▀█░█▀▀░░░█░█░█▀█░▀▀█░█▀█░█▀▀:\n");
        printf(":░█░░░█▀█░▀▀█░█▀█░░░█░█░█▀▀░░░█░░░█▀█░▀▀█░░░█▀█░█░█░░░█░█▀█░▀▀█:\n");
        printf(":░▀▀▀░▀░▀░▀▀▀░▀░▀░░░▀▀░░▀▀▀░░░▀▀▀░▀░▀░▀▀▀░░░▀░▀░▀▀▀░▀▀░░▀░▀░▀▀▀:\n");
        printf("································································\n\n");
}

int main() {
    int opcion;

    while (1) {
        limpiarPantalla();

        titulo();

        printf("Seleccione una opción:\n");
        printf("\033[33m[1] \033[0mCómo jugar\n");
        printf("\033[33m[2] \033[0mNueva partida\n");
        printf("\033[91m[3] Salir\033[0m\n\n");
        printf(">> ");

        scanf("%d", &opcion);
        getchar();

        limpiarPantalla();

        if (opcion == 1) {
            titulo();
            printf("=== ¿Cómo jugar? ===\n");
            printf("🧩 OBJETIVO:\n");
            printf("Completa el crucigrama encontrando las palabras ocultas.\n\n");
            printf("⌛ MECÁNICA PRINCIPAL:\n");
            printf("- Se te mostrará una palabra para adivinar.\n");
            printf("- Tendrás 20 segundos para escribirla correctamente.\n");
            printf("- Si no la aciertas a tiempo la palabra cambiará automáticamente.\n");

        } 
        else if (opcion == 2) {
            titulo();
            
        } 
        else if (opcion == 3) {
            titulo();
            printf("Saliendo...\n");
            break;
        } 
        else {
            printf("Opción no válida\n");
        }

        printf("\nPresiona Enter para continuar...");
        getchar();
    }

    return 0;
}