#include <stdio.h>

// funciones externas
void instanciaDeUI();
void instanciaDeLogica();

int main() {

    int opcion;

    while(1){
        printf("·························CRUSIGRAMA·····························\n");
        printf(":░█▀▀░█▀█░█▀▀░█▀█░░░█▀▄░█▀▀░░░█░░░█▀█░█▀▀░░░█░█░█▀█░▀▀█░█▀█░█▀▀:\n");
        printf(":░█░░░█▀█░▀▀█░█▀█░░░█░█░█▀▀░░░█░░░█▀█░▀▀█░░░█▀█░█░█░░░█░█▀█░▀▀█:\n");
        printf(":░▀▀▀░▀░▀░▀▀▀░▀░▀░░░▀▀░░▀▀▀░░░▀▀▀░▀░▀░▀▀▀░░░▀░▀░▀▀▀░▀▀░░▀░▀░▀▀▀:\n");
        printf("································································\n\n");
        printf("Presiona 1 para comenzar a jugar\n\n");
        printf("Presiona 2 para salir\n");

        scanf("%d", &opcion);

        switch(opcion){
            case 1:
                instanciaDeUI();
                instanciaDeLogica();
                break;
            case 2:
                return 0;

            default:
                printf("Opción inválida\n");
        }
    }

    return 0;
}