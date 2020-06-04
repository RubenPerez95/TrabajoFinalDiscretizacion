#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


void imprimirVectorDiscretizado(int vector[]);

int main(int argc, char**argv) {
    
    int vectorEdades[30]; //Se inicializa el vector de edades con un tamaño de 30

    int vectorSerialSalidaIntervalo[30]; //vector destinado a la disposición de los intervalos a los que pertenece cada edad para la parte serial
    int vectorParaleloSalidaIntervalo[30]; //vector destinado a la disposición de los intervalos a los que pertenece cada edad para la parte paralelizada
    int vectorSalidaCuentaEdades[4]; //vector destinado a la cuenta de edades en un mismo intervalo
    int vectorSalidaPosicion[4]; 


    float tiempoInicio, tiempoFin, tiempoTotal;

    for (int i=0; i<sizeof(vectorEdades); i++) {
        vectorEdades[i] = rand()%95;
    }

    printf("Valores aleatorios del vector de edades: \n");
    //Se generan los valores aleatorios y se insertan en el vector de edades
    //Estudiar los tiempos al generar los valores aleatorios y ver si lo mejor es paralelizar el bucle
    for (int i = 0; i<sizeof(vectorEdades); i++) {
        printf("%d ", vectorEdades[i]);
    }
    printf("\n");

    //Por defecto debería coger 4 siempre. Por si acaso.
    omp_set_num_threads(4);

    int inf = 1;
    int jov = 2;
    int adu = 3;
    int may = 4;

    tiempoInicio = omp_get_wtime();
    //Bucle serial
    for (int i = 0; i<30; i++) {
            if(vectorEdades[i] >= 0 && vectorEdades[i] <= 14) {
                vectorSerialSalidaIntervalo[i] = inf;
            }
            else if(vectorEdades[i] >= 15 && vectorEdades[i] <= 24) {
                vectorSerialSalidaIntervalo[i] = jov;
            }
            else if(vectorEdades[i] >= 25 && vectorEdades[i] <= 64) {
                vectorSerialSalidaIntervalo[i] = adu;
            }
            else {
                vectorSerialSalidaIntervalo[i] = may;
            }
    }
    tiempoFin = omp_get_wtime();
    tiempoTotal = tiempoFin - tiempoInicio;
    printf("Tiempo ejecución serial:  %0.10f \n", tiempoTotal);


    tiempoInicio = omp_get_wtime();
    //Bucle paralelizado
    #pragma omp paralel for shared(vectorEdades, i)
    for (int i = 0; i<30; i++) {
        #pragma omp sections
        {
            #pragma omp section
              if(vectorEdades[i] >= 0 && vectorEdades[i] <= 14) {
                  vectorParaleloSalidaIntervalo[i] = inf;
              }
            #pragma omp section
              if(vectorEdades[i] >= 15 && vectorEdades[i] <= 24) {
                  vectorParaleloSalidaIntervalo[i] = jov;
              }
            #pragma omp section
              if(vectorEdades[i] >= 25 && vectorEdades[i] <= 64) {
                  vectorParaleloSalidaIntervalo[i] = adu;
              }
            #pragma omp section
              if(vectorEdades[i] >= 65) {
                  vectorParaleloSalidaIntervalo[i] = may;
              }

        }
    }
    tiempoFin = omp_get_wtime();
    tiempoTotal = tiempoFin - tiempoInicio;
    printf("Tiempo ejecución paralelizado:  %0.10f \n", tiempoTotal);


    imprimirVectorDiscretizado(vectorParaleloSalidaIntervalo);
    //imprimirVectorDiscretizado(vectorSerialSalidaIntervalo);
}

void imprimirVectorDiscretizado(int *vector) {
    printf("Vector: \n");
    for (int i=0; i<30; i++) {
        printf("%d ", vector[i]);
    }
    printf("\n");
}
