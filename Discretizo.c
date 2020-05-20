#include <stdio.h>
#include <time.h>


int main() {

    //Se inicializa el vector de edades con un tamaño de 30
    int vectorEdades[30];

    for (int i=0; i<sizeof(vectorEdades); i++) {
        vectorEdades[i] = rand()%95;
    }

    printf("Valores aleatorios del vector de edades: \n")
    //Se generan los valores aleatorios y se insertan en el vector de edades
    //Estudiar los tiempos al generar los valores aleatorios y ver si lo mejor es paralelizar el bucle
    for (int i=0; i<sizeof(vectorEdades); i++) {
        printf("%d " , vectorEdades[i])
    } 

}