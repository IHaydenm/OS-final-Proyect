#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>
#include <string.h>

#define MAX_PALABRAS 100
#define MAX_TAM 100
#define MAX_CAMBIOS 10

char palabras[MAX_PALABRAS][MAX_CAMBIOS][MAX_TAM];
char pistas[MAX_PALABRAS][MAX_CAMBIOS][MAX_TAM];

int contestadas[MAX_PALABRAS];
int cambioActual[MAX_PALABRAS];
int cantidadCambios[MAX_PALABRAS];

int totalPalabras = 0;
int finJuego = 0;

pthread_mutex_t mutex;

// Para quitar el salto de linea de fgets
void quitarSalto(char texto[]){
    int j = 0;

    while(texto[j] != '\0'){
        if(texto[j] == '\n'){
            texto[j] = '\0';
            break;
        }
        j++;
    }
}

// Leemos las palabras desde palabras.txt
void leerPalabras(){
    FILE *archivo;
    char linea[MAX_TAM];
    int cambio = 0;

    archivo = fopen("palabras.txt","r");

    if(archivo == NULL){
        printf("No se pudo abrir el archivo palabras.txt\n");
        exit(1);
    }

    totalPalabras = 0;
    cambio = 0;

    while(fgets(linea, MAX_TAM, archivo) != NULL){
        quitarSalto(linea);

        if(linea[0] == '\0'){
            if(cambio > 0){
                cantidadCambios[totalPalabras] = cambio;
                contestadas[totalPalabras] = 0;
                cambioActual[totalPalabras] = 0;
                totalPalabras++;
                cambio = 0;
            }
        }
        else{
            strcpy(palabras[totalPalabras][cambio], linea);
            cambio++;
        }
    }

    if(cambio > 0){
        cantidadCambios[totalPalabras] = cambio;
        contestadas[totalPalabras] = 0;
        cambioActual[totalPalabras] = 0;
        totalPalabras++;
    }

    fclose(archivo);
}

// Leer pistas desde pistas.txt
void leerPistas(){
    FILE *archivo;
    char linea[MAX_TAM];
    int palabra = 0;
    int cambio = 0;

    archivo = fopen("pistas.txt", "r");

    if(archivo == NULL){
        printf("No se pudo abrir pistas.txt\n");
        exit(1);
    }

    palabra = 0;
    cambio = 0;

    while(fgets(linea, MAX_TAM, archivo) != NULL && palabra < totalPalabras){
        quitarSalto(linea);

        if(linea[0] == '\0'){
            palabra++;
            cambio = 0;
        }
        else{
            strcpy(pistas[palabra][cambio], linea);
            cambio++;
        }
    }

    fclose(archivo);
}

// Revisa si todas ya fueron contestadas
int todasContestadas(){
    int j;

    for(j = 0; j < totalPalabras; j++){
        if(contestadas[j] == 0){
            return 0;
        }
    }

    return 1;
}

// Muestra todas las pistas actuales
void mostrarPistas(){
    int j;

    printf("\n----- PISTAS ACTUALES -----\n");

    for(j = 0; j < totalPalabras; j++){
        if(contestadas[j] == 0){
            printf("%d. %s", j + 1, pistas[j][cambioActual[j]]);
            printf("  (%d letras)\n", (int)strlen(palabras[j][cambioActual[j]]));
        }
        else{
            printf("%d. Ya contestada\n", j + 1);
        }
    }

    printf("---------------------------\n");
}

// Compara sin distinguir mayusculas y minusculas
int comparar(char intento[], char correcta[]){
    int j = 0;

    while(intento[j] != '\0' && correcta[j] != '\0'){
        if(tolower(intento[j]) != tolower(correcta[j])){
            return 0;
        }

        j++;
    }

    if(intento[j] != correcta[j]){
        return 0;
    }

    return 1;
}

// Hilo que cambia palabras cada 20 segundos
void* cambiarPalabras(void* arg){
    int j;

    while(1){
        sleep(20);

        pthread_mutex_lock(&mutex);

        if(finJuego == 1){
            pthread_mutex_unlock(&mutex);
            break;
        }

        for(j = 0; j < totalPalabras; j++){
            if(contestadas[j] == 0){
                cambioActual[j] = (cambioActual[j] + 1) % cantidadCambios[j];
            }
        }

        printf("\n\nLas palabras y pistas cambiaron\n");
        mostrarPistas();
        printf("\nEscribe numero y palabra: ");
        fflush(stdout);

        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main(){
    pthread_t hiloCambios;

    leerPalabras();
    leerPistas();

    if(totalPalabras == 0){
        printf("No hay palabras cargadas\n");
        return 0;
    }

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&hiloCambios, NULL, cambiarPalabras, NULL);

    pthread_mutex_lock(&mutex);
    mostrarPistas();
    pthread_mutex_unlock(&mutex);

    int numero;
    char intento[MAX_TAM];
    char entrada[200];

    while(1){
        printf("\nEscribe numero y palabra: ");

        if(fgets(entrada, 200, stdin) == NULL){
            printf("Error leyendo entrada\n");
            continue;
        }

        if(sscanf(entrada, "%d %s", &numero, intento) != 2){
            printf("Formato incorrecto. Escribe: numero palabra\n");
            continue;
        }

        pthread_mutex_lock(&mutex);

        numero = numero - 1;

        if(numero < 0 || numero >= totalPalabras){
            printf("Ese numero de palabra no existe\n");
        }
        else if(contestadas[numero] == 1){
            printf("Esa palabra ya fue contestada\n");
        }
        else{
            if(comparar(intento, palabras[numero][cambioActual[numero]]) == 1){
                printf("Correcto\n");
                contestadas[numero] = 1;

                if(todasContestadas() == 1){
                    printf("\nGanaste, adivinaste todas las palabras\n");
                    finJuego = 1;
                    pthread_mutex_unlock(&mutex);
                    break;
                }

                mostrarPistas();
            }
            else{
                printf("Incorrecto\n");
                printf("La pista actual de esa palabra es: %s\n", pistas[numero][cambioActual[numero]]);
            }
        }

        pthread_mutex_unlock(&mutex);
    }

    pthread_join(hiloCambios, NULL);

    pthread_mutex_destroy(&mutex);

    return 0;
}