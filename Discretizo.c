#include <stdio.h>
#include <time.h>


void insertarEdad(int valor, int vector[]);
void imprimirVectorDiscretizado(int vector[], char tipoEdad);

int main() {

    //Se inicializa el vector de edades con un tamaño de 30
    int vectorEdades[30];
    double tiempoInicio, tiempoFin;

    int vectorInfantil[30];
    int vectorJuvenil[30];
    int vectorAdulto[30];
    int vectorAnciano[30];


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

    int contador1 = 0;
    int contador2 = 0;
    int contador3 = 0;
    int contador4 = 0;

    tiempoInicio = omp_get_wtime;
    printf("Tiempo antes del bucle de discretización: %d", tiempoInicio);
    //Bucle serial
    for (int i=0; i<sizeof(vectorEdades); i++) {
            if(vectorEdades[i] >= 0 && vectorEdades[i] <= 14) {
                insertarEdad(vectorEdades[i], vectorInfantil, contador1)
                /**vectorInfantil[contador1] = vectorEdades[i];
                contador1++;**/
            }
            else if(vectorEdades[i] >= 15 && vectorEdades[i] <= 24) {
                insertarEdad(vectorEdades[i], vectorJuvenil, contador2)
                /**vectorJuvenil[contador2] = vectorEdades[i];
                contador2++;**/
            }
            else if(vectorEdades[i] >= 25 && vectorEdades[i] <= 64) {
                insertarEdad(vectorEdades[i], vectorAdulto, contador3)
                /**vectorAdulto[contador3] = vectorEdades[i];
                contador3++;**/
            }
            else {
                insertarEdad(vectorEdades[i], vectorAnciano, contador4)
                /**vectorAnciano[contador4] = vectorEdades[i];
                contador4++;**/
            }
    }
    tiempoFin = omp_get_wtime;
    printf("Tiempo después del bucle de discretización %d", tiempoInicio);


    int vectorInfantil[30];
    int vectorJuvenil[30];
    int vectorAdulto[30];
    int vectorAnciano[30];

    tiempoInicio = omp_get_wtime;
    printf("Tiempo antes del bucle de discretización: %d", tiempoInicio);
    //Bucle paralelizado
    int contador1 = 0;
    int contador2 = 0;
    int contador3 = 0;
    int contador4 = 0;
    #pragma omp paralel for shared(vectorEdades, i)
    for (int i=0; i<sizeof(vectorEdades); i++) {
        #pragma omp sections
            #pragma omp section
            if(vectorEdades[i] >= 0 && vectorEdades[i] <= 14) {
                insertarEdad(vectorEdades[i], vectorInfantil, contador1)
                /**vectorInfantil[contador1] = vectorEdades[i];
                contador1++;**/
            }
            #pragma omp section
            else if(vectorEdades[i] >= 15 && vectorEdades[i] <= 24) {
                insertarEdad(vectorEdades[i], vectorJuvenil, contador2)
                /**vectorJuvenil[contador2] = vectorEdades[i];
                contador2++;**/
            }

            #pragma omp section
            else if(vectorEdades[i] >= 25 && vectorEdades[i] <= 64) {
                insertarEdad(vectorEdades[i], vectorAdulto, contador3)
                /**vectorAdulto[contador3] = vectorEdades[i];
                contador3++;**/
            }

            #pragma omp section
            else {
                insertarEdad(vectorEdades[i], vectorAnciano, contador4)
                /**vectorAnciano[contador4] = vectorEdades[i];
                contador4++;**/
            }
    }
    tiempoFin = omp_get_wtime;
    printf("Tiempo después del bucle de discretización %d", tiempoInicio);

    imprimirVectorDiscretizado(vectorInfantil, "Infantes");
    imprimirVectorDiscretizado(vectorJuvenil, "Jóvenes");
    imprimirVectorDiscretizado(vectorAdulto, "Adultos");
    imprimirVectorDiscretizado(vectorAnciano, "Ancianos")
}

void insertarEdad(int valor, int vector[], int contador) {
    vector[contador] = valor;
    contador++;
}

void imprimirVectorDiscretizado(int vector[], char tipoEdad) {
    printf("Vector de %c:", tipoEdad);
    for (int i=0; i<sizeof(vector); i++) {
        printf("%d", vector[i]);
    }
}