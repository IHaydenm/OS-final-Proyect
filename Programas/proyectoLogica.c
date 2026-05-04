#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

/*define*/
#define MAX_PALABRAS 100
#define MAX_TAM 100
#define TABLERO 20
#define PALABRAS_ACTIVAS 10
/*DEFINE*/

/*Struct*/
typedef struct{
    char palabra[MAX_TAM];
    char pista[MAX_TAM];
    int encontrada;
    int fila;
    int columna;
    int direccion; // 0 para horizontal, 1 para vertical
} Entrada;

/*variables*/
Entrada bancoPalabras[MAX_PALABRAS];
Entrada palabras[PALABRAS_ACTIVAS];

char tablero[TABLERO][TABLERO];
char visible[TABLERO][TABLERO];

int totalBanco = 0;
int totalPalabras = 0;
int finJuego = 0;

pthread_mutex_t mutex;
/*VARIABLES*/

void quitarSalto(char texto[]){
    int i = 0;
    while(texto[i] != '\0'){
        if(texto[i] == '\n'){
            texto[i] = '\0';
            break;
        }
        i++;
    }
}

void limpiarTablero(){
    int i, j;
    for(i = 0; i < TABLERO; i++){
        for(j = 0; j < TABLERO; j++){
            tablero[i][j] = ' ';
            visible[i][j] = ' ';
        }
    }
}

void limpiarLinea(char *s){
    int i, j = 0;
    char temp[MAX_TAM];

    for(i = 0; s[i]; i++){
        if(isprint((unsigned char)s[i]) && s[i] != '\r'){
            temp[j++] = s[i];
        }
    }
    temp[j] = '\0';
    strcpy(s, temp);
}

int lineaValida(char *s){
    int i;
    for(i = 0; s[i]; i++){
        if(!isspace((unsigned char)s[i]) && s[i] != '.'){
            return 1;
        }
    }
    return 0;
}

/*lectura de archivos*/
void leerArchivos(){
    FILE *fpalabras = fopen("Palabras.txt", "r");
    FILE *fpistas   = fopen("Pistas_de_articulos.txt", "r");

    if(!fpalabras || !fpistas){
        printf("Error abriendo archivos\n");
        exit(1);
    }
    char lineaPalabra[MAX_TAM];
    char lineaPista[MAX_TAM];

    totalBanco = 0;

    while (1){
        do {
            if (!fgets(lineaPalabra, MAX_TAM, fpalabras)) return;
            quitarSalto(lineaPalabra);
            limpiarLinea(lineaPalabra);
        } while (!lineaValida(lineaPalabra));

        do {
            if (!fgets(lineaPista, MAX_TAM, fpistas)) return;
            quitarSalto(lineaPista);
            limpiarLinea(lineaPista);
        } while (!lineaValida(lineaPista));
        strcpy(bancoPalabras[totalBanco].palabra, lineaPalabra);
        strcpy(bancoPalabras[totalBanco].pista, lineaPista);
        totalBanco++;
    }
}

void cargar10Palabras(){
    int usados[MAX_PALABRAS] = {0};
    int i;
    for(i = 0; i < PALABRAS_ACTIVAS; i++){
        int r;
        do{
            r = rand() % totalBanco;
        }while(usados[r]);
        usados[r] = 1;
        palabras[i] = bancoPalabras[r];
        palabras[i].encontrada = 0;
    }
    totalPalabras = PALABRAS_ACTIVAS;
}

void reemplazarNoEncontradas(){
    int usados[MAX_PALABRAS] = {0};
    int i;
    for(i = 0; i < totalPalabras; i++){
        if(palabras[i].encontrada){
            int j;
            for(j = 0; j < totalBanco; j++){
                if(strcmp(palabras[i].palabra, bancoPalabras[j].palabra) == 0){
                    usados[j] = 1;
                    break;
                }
            }
        }
    }
    for(i = 0; i < totalPalabras; i++){
        if(!palabras[i].encontrada){
            int r;
            do{
                r = rand() % totalBanco;
            }while(usados[r]);

            usados[r] = 1;

            strcpy(palabras[i].palabra, bancoPalabras[r].palabra);
            strcpy(palabras[i].pista, bancoPalabras[r].pista);

            palabras[i].encontrada = 0;
        }
    }
}

int puedeColocarHorizontal(char palabra[], int fila, int col){
    int i, len = strlen(palabra);
    if(col + len >= TABLERO) return 0;
    for(i = 0; i < len; i++){
        int f = fila;
        int c = col + i;
        if(tablero[f][c] != ' ' &&
           tablero[f][c] != toupper(palabra[i]))
            return 0;
        if(tablero[f][c] == ' '){
            if(f > 0 && tablero[f-1][c] != ' ') return 0;
            if(f < TABLERO-1 && tablero[f+1][c] != ' ') return 0;
        }
    }
    if(col > 0 && tablero[fila][col-1] != ' ') return 0;
    if(col + len < TABLERO && tablero[fila][col+len] != ' ') return 0;

    return 1;
}

int puedeColocarVertical(char palabra[], int fila, int col){
    int i, len = strlen(palabra);
    if(fila + len >= TABLERO) return 0;
    for(i = 0; i < len; i++){
        int f = fila + i;
        int c = col;
        if(tablero[f][c] != ' ' &&
           tablero[f][c] != toupper(palabra[i]))
            return 0;
        if(tablero[f][c] == ' '){
            if(c > 0 && tablero[f][c-1] != ' ') return 0;
            if(c < TABLERO-1 && tablero[f][c+1] != ' ') return 0;
        }
    }
    if(fila > 0 && tablero[fila-1][col] != ' ') return 0;
    if(fila + len < TABLERO && tablero[fila+len][col] != ' ') return 0;
    return 1;
}

void colocarHorizontal(char palabra[], int fila, int col){
    int i;
    for(i = 0; palabra[i] != '\0'; i++){
        tablero[fila][col+i] = toupper(palabra[i]);
        visible[fila][col+i] = '-';
    }
}

void colocarVertical(char palabra[], int fila, int col){
    int i;
    for(i = 0; palabra[i] != '\0'; i++){
        tablero[fila+i][col] = toupper(palabra[i]);
        visible[fila+i][col] = '-';
    }
}

int insertarCruzada(int indice){
    char *nueva = palabras[indice].palabra;
    for(int intento = 0; intento < 100; intento++){
        int base = rand() % indice; // pick random existing word
        char *actual = palabras[base].palabra;
        for(int j = 0; actual[j] != '\0'; j++){
            for(int k = 0; nueva[k] != '\0'; k++){
                if(toupper(actual[j]) == toupper(nueva[k])){
                    int fila, col;
                    if(palabras[base].direccion == 0){
                        // base horizontal → new vertical
                        fila = palabras[base].fila - k;
                        col  = palabras[base].columna + j;
                        if(fila >= 0 && puedeColocarVertical(nueva, fila, col)){
                            colocarVertical(nueva, fila, col);
                            palabras[indice].fila = fila;
                            palabras[indice].columna = col;
                            palabras[indice].direccion = 1;
                            return 1;
                        }
                    } 
                    else{
                        fila = palabras[base].fila + j;
                        col  = palabras[base].columna - k;
                        if(col >= 0 && puedeColocarHorizontal(nueva, fila, col)){
                            colocarHorizontal(nueva, fila, col);
                            palabras[indice].fila = fila;
                            palabras[indice].columna = col;
                            palabras[indice].direccion = 0;
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

void generarCrucigrama(){
    int centroFila = TABLERO / 2;
    int centroCol = TABLERO / 2;

    palabras[0].fila = centroFila;
    palabras[0].columna = centroCol;
    palabras[0].direccion = 0;

    colocarHorizontal(palabras[0].palabra, centroFila, centroCol);

    int i;
    for(i = 1; i < totalPalabras; i++){
        if(insertarCruzada(i) == 0){
            printf("No se pudo insertar: %s\n", palabras[i].palabra);
        }
    }
}

/*func declaration*/
void revelarPalabra(int indice);

void reiniciarJuego(){
    int i;
    limpiarTablero();
    cargar10Palabras();
    generarCrucigrama();
}


void reiniciarJuegoParcial(){
    int i;
    reemplazarNoEncontradas(); 
    limpiarTablero();
    generarCrucigrama();

    for(i = 0; i < totalPalabras; i++){
        if(palabras[i].encontrada){
            revelarPalabra(i);
        }
    }
}

void mostrarTablero(){
    int i, j;

    printf("\n========== CRUCIGRAMA ==========\n\n");

    for(i = 0; i < TABLERO; i++){
        for(j = 0; j < TABLERO; j++){
            if(tablero[i][j] == ' ')
                printf("  ");
            else
                printf("%c ", visible[i][j]);
        }
        printf("\n");
    }

    printf("\n================================\n");
}

void mostrarPistas(){
    int i;

    printf("\nPISTAS:\n\n");

    for(i = 0; i < totalPalabras; i++){
        if(!palabras[i].encontrada){
            printf("%d. %s (%d letras)\n",
                   i + 1,
                   palabras[i].pista,
                   (int)strlen(palabras[i].palabra));
        }
    }
}

void revelarPalabra(int indice){
    int i;
    int fila = palabras[indice].fila;
    int col = palabras[indice].columna;

    if(palabras[indice].direccion == 0){
        for(i = 0; palabras[indice].palabra[i] != '\0'; i++){
            visible[fila][col+i] = tablero[fila][col+i];
        }
    } else {
        for(i = 0; palabras[indice].palabra[i] != '\0'; i++){
            visible[fila+i][col] = tablero[fila+i][col];
        }
    }
}

int comparar(char a[], char b[]){
    int i = 0;
    while(a[i] && b[i]){
        if(toupper((unsigned char)a[i]) != toupper((unsigned char)b[i]))
            return 0;
        i++;
    }
    return a[i] == b[i];
}

int todasEncontradas(){
    int i;
    for(i = 0; i < totalPalabras; i++){
        if(!palabras[i].encontrada) return 0;
    }
    return 1;
}

void* cambiarPistas(void* arg){
    while(1){
        sleep(30);

        pthread_mutex_lock(&mutex);

        if(finJuego){
            pthread_mutex_unlock(&mutex);
            break;
        }
        system("clear");
        printf("\n\n======= CAMBIANDO PALABRAS =======\n");
        reiniciarJuegoParcial();
        mostrarTablero();
        mostrarPistas();

        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int instanciaDeLogica(){

    srand(time(NULL));

    pthread_t hilo;

    leerArchivos();

    if(totalBanco < PALABRAS_ACTIVAS){
        printf("Debe haber al menos %d palabras\n", PALABRAS_ACTIVAS);
        return 0;
    }

    reiniciarJuego();

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&hilo, NULL, cambiarPistas, NULL);

    char entrada[200];
    int numero;
    char intento[MAX_TAM];

    while(1){

        mostrarTablero();
        mostrarPistas();
        printf("\nEscribe numero y palabra: ");
        fgets(entrada, 200, stdin);

        pthread_mutex_lock(&mutex);
        if(sscanf(entrada, "%d %s", &numero, intento) != 2){
            printf("Formato incorrecto\n");
            pthread_mutex_unlock(&mutex);
            continue;
        }
        numero--;
        if(numero < 0 || numero >= totalPalabras){
            printf("Numero invalido\n");
        }
        else if(palabras[numero].encontrada){
            printf("Ya habias adivinado esta palabra\n");
        }
        else{
            if(comparar(intento, palabras[numero].palabra)){
                printf("Adivinaste la palabra! Continuemos...\n");
                palabras[numero].encontrada = 1;
                revelarPalabra(numero);

                if(todasEncontradas()){
                    mostrarTablero();
                    printf("\nFelicidades! GANASTE\n");
                    finJuego = 1;
                    pthread_mutex_unlock(&mutex);
                    break;
                }
            }
            else{
                printf("Tu respuesta fue incorrecta. Intenta otra vez!\n");
            }
        }
        pthread_mutex_unlock(&mutex);
    }

    pthread_join(hilo, NULL);

    pthread_mutex_destroy(&mutex);

    return 0;
}