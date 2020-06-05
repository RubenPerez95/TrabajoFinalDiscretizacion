#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

// CONSTANTES

#define TAM 100000
#define EDADES 90000
#define VALORES 50
#define INFANTES 73 //Código ASCII para la letra I para referirse a personas menores de 15 años
#define JOVENES 74 //Código ASCII para la letra J para referirse a personas mayores de 14 años y menores de 25
#define ADULTOS 65 //Código ASCII para la letra A para referirse a personas mayores de 24 años y menores de 65
#define MAYORES 77 //Código ASCII para la letra M para referirse a personas mayores de 64 años

void imprimirVector(int vector[]);
void imprimirRangos(int vector[]);
void compararVectores(int vector1[], int vector2[]);
pid_t getpid(void);
//pid_t getppid(void);

int main(int argc, char**argv) {
    
    int vectorEdades[TAM]; //Se inicializa el vector de edades con un tamaño de 30
    int vectorSerialSalidaIntervalo[TAM]; //vector destinado a la disposición de los intervalos a los que pertenece cada edad para la parte serial
    int vectorParaleloSalidaIntervalo[TAM]; //vector destinado a la disposición de los intervalos a los que pertenece cada edad para la parte paralelizada
    int totalRangosSerial[4]; //Guarda el numero de personas que pertenecen a cada rango de edad para el código secuencial
    int totalRangosParalelo[4]; //Guarda el numero de personas que pertenecen a cada rango de edad para el código paralelo

    float tiempoInicioPar, tiempoInicioSerial, tiempoPar, tiempoSerial;

    srand(getpid()); //Necesario para que cada vez que se ejecute, se obtengan valores aleatorios diferentes

    for (int i=0; i<EDADES; i++) {
        vectorEdades[i] = rand()% 95 + 1;
    }

    printf("Primeros 50 valores aleatorios del vector de edades: \n");
    //Se generan los valores aleatorios y se insertan en el vector de edades
    //Estudiar los tiempos al generar los valores aleatorios y ver si lo mejor es paralelizar el bucle
    for (int i = 0; i<VALORES; i++) {
        printf("Posición %d(%d) ", i, vectorEdades[i]);
    }
    printf("\n\n");

    //Por defecto debería coger 4 siempre. Por si acaso.
    omp_set_num_threads(4);

    tiempoInicioSerial = omp_get_wtime();
    //Bucle serial
    for (int i = 0; i<EDADES; i++) {
            if(vectorEdades[i] >= 0 && vectorEdades[i] <= 14) {
                vectorSerialSalidaIntervalo[i] = INFANTES;
                totalRangosSerial[0] = totalRangosSerial[0] + 1;
            }
            else if(vectorEdades[i] >= 15 && vectorEdades[i] <= 24) {
                vectorSerialSalidaIntervalo[i] = JOVENES;
                totalRangosSerial[1] = totalRangosSerial[1] + 1;
            }
            else if(vectorEdades[i] >= 25 && vectorEdades[i] <= 64) {
                vectorSerialSalidaIntervalo[i] = ADULTOS;
                totalRangosSerial[2] = totalRangosSerial[2] + 1;
            }
            else {
                vectorSerialSalidaIntervalo[i] = MAYORES;
                totalRangosSerial[3] = totalRangosSerial[3] + 1;
            }
    }
    tiempoSerial = (omp_get_wtime() - tiempoInicioSerial) * 1000000;


    tiempoInicioPar = omp_get_wtime();
    //Bucle paralelizado
    #pragma omp paralel for shared(vectorEdades, EDADES, vectorParaleloSalidaIntervalo) private(i) schedule(dynamic)
    for (int i = 0; i<EDADES; i++) {
            if(vectorEdades[i] >= 0 && vectorEdades[i] <= 14) {
                vectorParaleloSalidaIntervalo[i] = INFANTES;
                totalRangosParalelo[0] = totalRangosParalelo[0] + 1;
            }
            else if(vectorEdades[i] >= 15 && vectorEdades[i] <= 24) {
                vectorParaleloSalidaIntervalo[i] = JOVENES;
                totalRangosParalelo[1] = totalRangosParalelo[1] + 1;
            }
            else if(vectorEdades[i] >= 25 && vectorEdades[i] <= 64) {
                vectorParaleloSalidaIntervalo[i] = ADULTOS;
                totalRangosParalelo[2] = totalRangosParalelo[2] + 1;
            }
            else {
                vectorParaleloSalidaIntervalo[i] = MAYORES;
                totalRangosParalelo[3] = totalRangosParalelo[3] + 1;
            }

    }
    tiempoPar = (omp_get_wtime() - tiempoInicioPar) * 1000000;

    printf("Resultado de los 50 primeros valores la ejecución secuencial del vector:\n");
    imprimirVector(vectorSerialSalidaIntervalo);
    printf("Resultado de los 50 primeros valores la ejecución en paralelo del vector:\n");
    imprimirVector(vectorParaleloSalidaIntervalo);
    printf("Tiempo ejecución secuencial: %0.5f microsegundos \n", tiempoSerial);
    printf("Tiempo ejecución paralelizado: %0.5f microsegundos \n\n", tiempoPar);
    printf("Resultados totales del vector secuencial:\n");
    imprimirRangos(totalRangosSerial);
    printf("Resultados totales del vector paralelo:\n");
    imprimirRangos(totalRangosParalelo);
    compararVectores(vectorSerialSalidaIntervalo, vectorParaleloSalidaIntervalo);
}

void imprimirVector(int *vector) {
    for (int i=0; i<VALORES; i++) {
        printf("Posición %d(%c) ", i, vector[i]);
    }
    printf("\n\n");
}

void imprimirRangos(int *vector) {
    printf("Hay un total de %d infantes \n", vector[0]);
    printf("Hay un total de %d jóvenes \n", vector[1]);
    printf("Hay un total de %d adultos \n", vector[2]);
    printf("Hay un total de %d mayores \n\n", vector[3]);
}

void compararVectores(int *vector1, int *vector2) {
    int orden = 0;
    for (int i=0; i<EDADES; i++)
        if (vector1[i] != vector2[i])
            orden = 1;
    if (orden == 1)
        printf("¡Los vectores NO están ordenados de la misma forma!");
    else
        printf("¡Los vectores SI están ordenados de la misma forma!");

    printf("\n\n");
}



