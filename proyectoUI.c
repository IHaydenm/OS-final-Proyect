#include <stdio.h>
#include <stdlib.h>

void limpiarPantalla() {
    system("clear");
    titulo();
}

void titulo(){
    printf("·························CRUSIGRAMA·····························\n");
        printf(":░█▀▀░█▀█░█▀▀░█▀█░░░█▀▄░█▀▀░░░█░░░█▀█░█▀▀░░░█░█░█▀█░▀▀█░█▀█░█▀▀:\n");
        printf(":░█░░░█▀█░▀▀█░█▀█░░░█░█░█▀▀░░░█░░░█▀█░▀▀█░░░█▀█░█░█░░░█░█▀█░▀▀█:\n");
        printf(":░▀▀▀░▀░▀░▀▀▀░▀░▀░░░▀▀░░▀▀▀░░░▀▀▀░▀░▀░▀▀▀░░░▀░▀░▀▀▀░▀▀░░▀░▀░▀▀▀:\n");
        printf("································································\n\n");
}

char tablero[10][10];

int es_igual(char a[], char b[]) {
    int i = 0;
    while (a[i] != '\0' || b[i] != '\0') {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return 1;
}

void mostrar() {

    printf("\033[H\033[J\n      1 2 3 4 5 6 7 8 9\n");
    for (int i = 1; i <= 9; i++) {
        printf("  %d   ", i);
        for (int j = 1; j <= 9; j++) printf("%c ", tablero[i][j]);
        printf("\n");
    }
}

int main() {
    int opcion;

    while (1) {
        limpiarPantalla();

        printf("Seleccione una opción:\n");
        printf("\033[33m[1] \033[0mCómo jugar\n");
        printf("\033[33m[2] \033[0mNueva partida\n");
        printf("\033[91m[3] Salir\033[0m\n\n");
        printf(">> ");

        scanf("%d", &opcion);
        getchar();

        limpiarPantalla();

        if (opcion == 1) {
            printf("=== ¿Cómo jugar? ===\n");
            printf("🧩 OBJETIVO:\n");
            printf("Completa el crucigrama encontrando las palabras ocultas.\n\n");
            printf("⌛ MECÁNICA PRINCIPAL:\n");
            printf("- Se te mostrará una palabra para adivinar.\n");
            printf("- Tendrás 20 segundos para escribirla correctamente.\n");
            printf("- Si no la aciertas a tiempo la palabra cambiará automáticamente.\n");

        } 
        else if (opcion == 2) {
            
            char r[20]; 
            int a = 0, n;

            for (int i = 0; i < 10; i++) for (int j = 0; j < 10; j++) tablero[i][j] = '.';
            for (int i = 0; i < 6; i++) tablero[2][2+i] = '_'; // BOSQUE
            for (int i = 0; i < 5; i++) tablero[5][2+i] = '_'; // RATON
            for (int i = 0; i < 4; i++) tablero[8][4+i] = '_'; // AGUA
            
            for (int i = 0; i < 5; i++) tablero[2+i][2] = '_'; // BARCO
            for (int i = 0; i < 5; i++) tablero[2+i][5] = '_'; // QUESO
            for (int i = 0; i < 4; i++) tablero[5+i][6] = '_'; // NUBE

            while (a < 6) {

                mostrar();
                printf("\n--- PISTAS ---\n");
                printf("1.H: Lugar con muchos arboles                | 4.V: Medio de transporte por mar\n");
                printf("2.H: Clase donde aprendes tecnicas           | 5.V: Alimento derivado de la leche\n");
                printf("3.H: Se festeja en san valentin              | 6.V: shrek era un\n");
                
                printf("\nNumero de pista: "); scanf("%d", &n);
                printf("Tu respuesta: "); scanf("%s", r);
                for(int i = 0; r[i]; i++) r[i] = toupper(r[i]);

                if (n==1 && es_igual(r,"BOSQUE"))  { for(int i=0;i<6;i++) tablero[2][2+i]=r[i]; a++; }
                else if (n==2 && es_igual(r,"CURSO"))   { for(int i=0;i<5;i++) tablero[5][2+i]=r[i]; a++; }
                else if (n==3 && es_igual(r,"AMOR"))    { for(int i=0;i<4;i++) tablero[8][4+i]=r[i]; a++; }
                else if (n==4 && es_igual(r,"BARCO"))   { for(int i=0;i<5;i++) tablero[2+i][2]=r[i]; a++; }
                else if (n==5 && es_igual(r,"QUESO"))   { for(int i=0;i<5;i++) tablero[2+i][5]=r[i]; a++; }
                else if (n==6 && es_igual(r,"OGRO"))    { for(int i=0;i<4;i++) tablero[5+i][6]=r[i]; a++; }
                else { printf("\n¡Error! Revisa la palabra o el numero."); getchar(); getchar(); }
            }

            mostrar();
            printf("\n¡FELICIDADES! Haz completado el crusigrama!.\n");
            return 0;


            
        } 
        else if (opcion == 3) {
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
