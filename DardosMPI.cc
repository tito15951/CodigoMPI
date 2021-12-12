#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <omp.h>
#include <math.h>

int N;
int Hilos;
const double factor=1.0/RAND_MAX;

double valorAbsoluto(double num)
{
	if (num<0)
		return -1*num;
	else
		return num;
}
void writeTime(double pi, double tiempo,double error,int N){
    FILE *f = fopen("DardosMPI.txt","a+");
    fprintf(f,"%i;%f;%.6lf;%.2lf\n", N,pi, tiempo, error);
    fclose(f);
}
void printMat(int *vect,int max,int id)
{
	printf("Soy el proceso: %i\n",id);
	for(int i=0;i<max;i++)
	{printf("%i   ", vect[i]);}
	printf("\n");
}
    
int main(int argc, char *argv[])
{	N=atoi(argv[1]);
	//int Intentos=atoi(argv[2]);
	//Hilos=atoi(argv[3]);
	//N=100000;
    double startTime;
    double endTime;
    double tiempo;
	int numranks, rank;

	MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int *hits = (int*)malloc(numranks*sizeof(int));
	int hitlocal =0;
	int Limite=N/numranks;
	int *hitoriginal=hits;
	
	if(rank==0)
	{
		for(int i=0;i<numranks;i++)
			hits[i]=0;
		startTime = MPI_Wtime();
		}
	MPI_Barrier(MPI_COMM_WORLD);	
	long k;
	double x,y;
	srand((int)clock());
		for(k=0;k<Limite;++k)
		{
			x = rand()*factor;
			y = rand()*factor;
			if(x*x+y*y<1)
				{hitlocal+=1;}
		}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Gather(&hitlocal, 1, MPI_INT,hits, 1, MPI_INT,  0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank==0){
		endTime = MPI_Wtime();
		tiempo = endTime - startTime;
		int suma=0;
		for(int i=0;i<numranks;i++)
		{suma+=hitoriginal[i];}
		//printMat(hitoriginal,numranks,rank);
		double pi=4.0*(double)suma/(double)N;
		double error=((M_PI-pi)/pi)*100;
		printf("pi: %.8f\n", pi);
		printf("tiempo: %.5f\n", tiempo);
		printf("error: %.2f\n", valorAbsoluto(error));
		writeTime(pi, tiempo,valorAbsoluto(error),N);
		MPI_Finalize();
	}
return 0;
}
