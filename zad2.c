#include <stdio.h>
#include "mpi.h"
#include <math.h>

double funkcja(double x){
return x*x;
}

int main(int argc, char **argv){
int p; // numer procesu
int n; // ilosc procesow
double a = 1.0;
double b = 3.0;
int tag = 5;
double punkt;
double punktT;
double wynik = 0;

MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &p);
MPI_Comm_size(MPI_COMM_WORLD, &n);
MPI_Status status;

if(*argv[1] == 't'){
    if(p==n-1){
    // z zerowewgo procesu do pierwszego
    wynik = funkcja(a)/2;
    wynik += funkcja(b)/2;
    wynik += funkcja(a+(1.0*p/n)*(b-a));
    MPI_Send(&wynik, 1, MPI_DOUBLE ,p-1 ,tag, MPI_COMM_WORLD);
    }

    if((p>0)&&(p<n-1)){
        MPI_Recv(&wynik, 1, MPI_DOUBLE, p+1, tag, MPI_COMM_WORLD, &status);
        wynik += funkcja(a+(1.0*p/n)*(b-a));
        MPI_Send(&wynik, 1, MPI_DOUBLE ,p-1 ,tag, MPI_COMM_WORLD);
    }
    if(p == 0){
        MPI_Recv(&wynik, 1, MPI_DOUBLE, p+1, tag, MPI_COMM_WORLD, &status);
        wynik += funkcja(a+(1.0*p/n)*(b-a));
        double h = (b-a)/n;
        printf("Obliczone na %d procesach przyblizenie calki x^2 metoda Trapezow to: %f\n", n, wynik*h);
    }
}

else if(*argv[1] == 's'){
    if(p==n-1){
    // z zerowewgo procesu do pierwszego
    wynik = funkcja(a);
    wynik += funkcja(b);
    punkt = a+(1.0*p/n)*(b-a);
    wynik += 2.0*funkcja(punkt);
    wynik += 4.0*funkcja((b+punkt)/2);
    MPI_Send(&wynik, 1, MPI_DOUBLE ,p-1 ,tag, MPI_COMM_WORLD);
    MPI_Send(&punkt, 1, MPI_DOUBLE ,p-1 ,tag, MPI_COMM_WORLD);
    }

    if((p>0)&&(p<n-1)){
        MPI_Recv(&wynik, 1, MPI_DOUBLE, p+1, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&punkt, 1, MPI_DOUBLE, p+1, tag, MPI_COMM_WORLD, &status);
        punktT = punkt;
        punkt = a+(1.0*p/n)*(b-a);
        wynik += 2.0*funkcja(punkt);
        wynik += 4.0*funkcja((punktT+punkt)/2);
        MPI_Send(&wynik, 1, MPI_DOUBLE ,p-1 ,tag, MPI_COMM_WORLD);
        MPI_Send(&punkt, 1, MPI_DOUBLE ,p-1 ,tag, MPI_COMM_WORLD);
    }
    if(p == 0){
        MPI_Recv(&wynik, 1, MPI_DOUBLE, p+1, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&punkt, 1, MPI_DOUBLE, p+1, tag, MPI_COMM_WORLD, &status);
        wynik += 4.0*funkcja((punkt+a)/2);
        double h = (punkt-a)/6;
        printf("Obliczone na %d procesach przyblizenie calki x^2 metoda Simpsona to: %f\n", n, wynik*h);
    }
}

MPI_Finalize();
return 0;
}
