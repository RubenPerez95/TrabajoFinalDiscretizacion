#include <stdio.h>
#include <time.h>


void imprimirVectorDiscretizado(int vector[], char tipoEdad);

int main() {
    
    int vectorEdades[30]; //Se inicializa el vector de edades con un tamaño de 30

    char *vectorSalidaIntervalo[30]; //vector destinado a la disposición de los intervalos a los que pertenece cada edad
    int vectorSalidaCuentaEdades[4]; //vector destinado a la cuenta de edades en un mismo intervalo
    int vectorSalidaPosicion[4]; 


    double tiempoInicio, tiempoFin;

    for (int i=0; i<sizeof(vectorEdades); i++) {
        vectorEdades[i] = rand()%95;
    }

    printf("Valores aleatorios del vector de edades: \n")
    //Se generan los valores aleatorios y se insertan en el vector de edades
    //Estudiar los tiempos al generar los valores aleatorios y ver si lo mejor es paralelizar el bucle
    for (int i=0; i<sizeof(vectorEdades); i++) {
        printf("%d ", vectorEdades[i])
    } 

    //Por defecto debería coger 4 siempre. Por si acaso.
    omp_set_num_threads(4);

    int inf = "Infantil";
    int jov = "Joven";
    int adu = "Adulto";
    int may = "Mayor";

    tiempoInicio = omp_get_wtime;
    //Bucle serial
    for (int i=0; i<sizeof(vectorEdades); i++) {
            if(vectorEdades[i] >= 0 && vectorEdades[i] <= 14) {
                vectorSalidaIntervalo[i] = inf;
            }
            else if(vectorEdades[i] >= 15 && vectorEdades[i] <= 24) {
                vectorSalidaIntervalo[i] = jov;
            }
            else if(vectorEdades[i] >= 25 && vectorEdades[i] <= 64) {
                vectorSalidaIntervalo[i] = adu;
            }
            else {
                vectorSalidaIntervalo[i] = may;
            }
    }
    tiempoFin = omp_get_wtime;
    printf("Tiempo ejecución serial:  %d", tiempoFin - tiempoInicio);


    tiempoInicio = omp_get_wtime;
    //Bucle paralelizado
    #pragma omp paralel for shared(vectorEdades, i)
    for (int i=0; i<sizeof(vectorEdades); i++) {
        #pragma omp sections
            #pragma omp section
            if(vectorEdades[i] >= 0 && vectorEdades[i] <= 14) {
                vectorSalidaIntervalo[i] = inf;
            }
            #pragma omp section
            else if(vectorEdades[i] >= 15 && vectorEdades[i] <= 24) {
                vectorSalidaIntervalo[i] = jov;
            }
            #pragma omp section
            else if(vectorEdades[i] >= 25 && vectorEdades[i] <= 64) {
                vectorSalidaIntervalo[i] = adu;
            }
            #pragma omp section
            else {
                vectorSalidaIntervalo[i] = may;
            }
    }
    tiempoFin = omp_get_wtime;
    printf("Tiempo ejecución paralelizado:  %d", tiempoFin - tiempoInicio);

    imprimirVectorDiscretizado(vectorSalidaIntervalo);
    /*imprimirVectorDiscretizado(vectorSalidaCuentaEdades);*/
}

void imprimirVectorDiscretizado(int vector[]) {
    printf("Vector: \n");
    for (int i=0; i<sizeof(vector); i++) {
        printf("%c", vector[i]);
    }
}