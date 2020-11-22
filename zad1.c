
#include <stdio.h>
#include "mpi.h"
#include <math.h>

int main(int argc, char **argv){
int p; // numer procesu
int n; // ilosc procesow
int silnia = 1;
int tag = 5;
double wynik = 0;

MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &p);
MPI_Comm_size(MPI_COMM_WORLD, &n);
MPI_Status status;

if(*argv[1] == 'p'){
    if(p==0){
    // z zerowewgo procesu do pierwszego
    wynik = 4;
    MPI_Send(&wynik, 1, MPI_DOUBLE ,p+1 ,tag, MPI_COMM_WORLD);
    }

    if((p>0)&&(p<n)){
        MPI_Recv(&wynik, 1, MPI_DOUBLE, p-1, tag, MPI_COMM_WORLD, &status);
        wynik += (4*(pow(-1, p)/(2*(p+1)-1)));

        if(p!=n-1){
            MPI_Send(&wynik, 1, MPI_DOUBLE ,p+1 ,tag, MPI_COMM_WORLD);
        }
        else{
            printf("Obliczone na %d procesach przyblizenie liczby PI to: %f\n", p+1, wynik);
        }
    }
}
else if(*argv[1] == 'e'){
    if(p==0){
    // z zerowewgo procesu do pierwszego
    wynik = 1;
    MPI_Send(&wynik, 1, MPI_DOUBLE ,p+1 ,tag, MPI_COMM_WORLD);
    MPI_Send(&silnia, 1, MPI_INT ,p+1 ,tag, MPI_COMM_WORLD);
    }

    if((p>0)&&(p<n)){
    MPI_Recv(&wynik, 1, MPI_DOUBLE, p-1, tag, MPI_COMM_WORLD, &status);
    MPI_Recv(&silnia, 1, MPI_INT, p-1, tag, MPI_COMM_WORLD, &status);
    silnia = silnia*p;
    wynik += 1.0/silnia;
        if(p!=n-1){
        MPI_Send(&wynik, 1, MPI_DOUBLE ,p+1 ,tag, MPI_COMM_WORLD);
        MPI_Send(&silnia, 1, MPI_INT ,p+1 ,tag, MPI_COMM_WORLD);
        }
        else{
        printf("Obliczone na %d procesach przyblizenie liczby e to: %f\n", p+1, wynik);
        }
    }
}

MPI_Finalize();
return 0;
}
