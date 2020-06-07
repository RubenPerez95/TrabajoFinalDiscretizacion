#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>


// CONSTANTES

#define TAM 100000
#define INFANTES 73 //Código ASCII para la letra I para referirse a personas menores de 15 años
#define JOVENES 74 //Código ASCII para la letra J para referirse a personas mayores de 14 años y menores de 25
#define ADULTOS 65 //Código ASCII para la letra A para referirse a personas mayores de 24 años y menores de 65
#define MAYORES 77 //Código ASCII para la letra M para referirse a personas mayores de 64 años

void imprimirVector(int vector[], int n);
void imprimirRangos(int vector[]);
void compararVectores(int vector1[], int vector2[], int n);
pid_t getpid(void);

int main(int argc, char**argv) {

    int vectorEdades[TAM]; //Se inicializa el vector de edades con un tamaño de 30
    int vectorSerialSalidaIntervalo[TAM]; //vector destinado a la disposición de los intervalos a los que pertenece cada edad para la parte serial
    int vectorParaleloSalidaIntervalo[TAM]; //vector destinado a la disposición de los intervalos a los que pertenece cada edad para la parte paralelizada
    int totalRangosSerial[4]; //Guarda el numero de personas que pertenecen a cada rango de edad para el código secuencial
    int totalRangosParalelo[4]; //Guarda el numero de personas que pertenecen a cada rango de edad para el código paralelo
    int* V = 0;
    int i = 0;
    int n;

    float tiempoInicioPar, tiempoInicioParSuma, tiempoInicioSerial, tiempoInicioSerialSuma, tiempoPar, tiempoParSuma, tiempoSerial, tiempoSerialSuma;


    if (argc != 2) {
        fprintf(stderr, "Como argumento, se debe introducir el tamaño deseado del vector\n");
        return 1;
    }
    else {
        n = atoi(argv[1]); 
        V = malloc(n * sizeof(int));
    }

    srand(getpid()); //Necesario para que cada vez que se ejecute, se obtengan valores aleatorios diferentes

    //Se generan los valores aleatorios y se insertan en el vector de edades
    for (i=0; i<n; i++) {
        vectorEdades[i] = rand()% 95 + 0;
    }

    printf("Valores generados para el vector de edades: \n");
    for (i = 0; i<n; i++) {
        printf("Posición %d(%d) ", i, vectorEdades[i]);
    }
    printf("\n\n");

    tiempoInicioSerial = omp_get_wtime();
    //Bucle serial
    for (i = 0; i<n; i++) {
        if(vectorEdades[i] >= 0 && vectorEdades[i] <= 14) {
            vectorSerialSalidaIntervalo[i] = INFANTES;
        }
        else if(vectorEdades[i] >= 15 && vectorEdades[i] <= 24) {
            vectorSerialSalidaIntervalo[i] = JOVENES;
        }
        else if(vectorEdades[i] >= 25 && vectorEdades[i] <= 64) {
            vectorSerialSalidaIntervalo[i] = ADULTOS;
        }
        else {
            vectorSerialSalidaIntervalo[i] = MAYORES;
        }
    }
    tiempoSerial = (omp_get_wtime() - tiempoInicioSerial) * 1000000;

    tiempoInicioSerialSuma = omp_get_wtime();
    //Bucle serial
    for (i = 0; i<n; i++) {
        if(vectorEdades[i] >= 0 && vectorEdades[i] <= 14) {
            totalRangosSerial[0]++;
        }
        else if(vectorEdades[i] >= 15 && vectorEdades[i] <= 24) {
            totalRangosSerial[1]++;
        }
        else if(vectorEdades[i] >= 25 && vectorEdades[i] <= 64) {
            totalRangosSerial[2]++;
        }
        else {
            totalRangosSerial[3]++;
        }
    }
    tiempoSerialSuma = (omp_get_wtime() - tiempoInicioSerialSuma) * 1000000;


    omp_set_num_threads(4);

    tiempoInicioPar = omp_get_wtime();
    //Bucle paralelizado
	//#pragma omp for ordered //Prueba 1
    //#pragma omp parallel for shared(edades, vectorParaleloSalidaIntervalo, vectorEdades) private(i) //Prueba 2
	#pragma omp for // Prueba 3
	for (i = 0; i<n; i++) {
		if(vectorEdades[i] >= 0 && vectorEdades[i] <= 14) {
			vectorParaleloSalidaIntervalo[i] = INFANTES;
		}
		else if(vectorEdades[i] >= 15 && vectorEdades[i] <= 24) {
			vectorParaleloSalidaIntervalo[i] = JOVENES;
		}
		else if(vectorEdades[i] >= 25 && vectorEdades[i] <= 64) {
			vectorParaleloSalidaIntervalo[i] = ADULTOS;
		}
		else {
			vectorParaleloSalidaIntervalo[i] = MAYORES;
		}

	}
    tiempoPar = (omp_get_wtime() - tiempoInicioPar) * 1000000;

    tiempoInicioParSuma = omp_get_wtime();
    //Bucle paralelizado
	//#pragma omp for ordered //Prueba 1
	//#pragma omp parallel for default(none) shared(edades, vectorEdades, totalRangosParalelo) schedule(static) //Prueba2
    //#pragma omp parallel for shared(edades, vectorEdades) private(i) reduction(+:totalRangosParalelo) //Prueba 3
	#pragma omp for //Prueba 4
    for (i = 0; i<n; i++) {
        if(vectorEdades[i] >= 0 && vectorEdades[i] <= 14) {
            totalRangosParalelo[0]++;
        }
        else if(vectorEdades[i] >= 15 && vectorEdades[i] <= 24) {
            totalRangosParalelo[1]++;
        }
        else if(vectorEdades[i] >= 25 && vectorEdades[i] <= 64) {
            totalRangosParalelo[2]++;
        }
        else {
            totalRangosParalelo[3]++;
        }

    }
    tiempoParSuma = (omp_get_wtime() - tiempoInicioParSuma) * 1000000;

    printf("Resultado de la ejecución secuencial del vector:\n");
    imprimirVector(vectorSerialSalidaIntervalo, n);
    printf("Resultado de la ejecución en paralelo del vector:\n");
    imprimirVector(vectorParaleloSalidaIntervalo, n);
    printf("Tiempo ejecución secuencial: %0.5f microsegundos \n", fabs(tiempoSerial));
    printf("Tiempo ejecución paralelizado: %0.5f microsegundos \n\n", fabs(tiempoPar));
    printf("Tiempo ejecución secuencial suma: %0.5f microsegundos \n", fabs(tiempoSerialSuma));
    printf("Tiempo ejecución paralelizado suma: %0.5f microsegundos \n\n", fabs(tiempoParSuma));
    printf("Resultados totales del vector secuencial:\n");
    imprimirRangos(totalRangosSerial);
    printf("Resultados totales del vector paralelo:\n");
    imprimirRangos(totalRangosParalelo);
    compararVectores(vectorSerialSalidaIntervalo, vectorParaleloSalidaIntervalo, n);
}

void imprimirVector(int *vector, int n) {
    for (int i=0; i<n; i++) {
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

void compararVectores(int *vector1, int *vector2, int n) {
    int orden = 0;
    for (int i=0; i<n; i++)
        if (vector1[i] != vector2[i])
            orden = 1;
    if (orden == 1)
        printf("¡Los vectores NO están ordenados de la misma forma!");
    else
        printf("¡Los vectores SI están ordenados de la misma forma!");

    printf("\n\n");
}
