#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <omp.h>

int N;
int Hilos;
const double factor=1.0/RAND_MAX;
void Calcular(int* hits, int nHilos, int Limite)
{
	omp_set_num_threads(nHilos);
	//int Limite=N/Hilos;
	long k;
	double x,y;
	#pragma omp parallel shared(hits,Limite) private(k,x,y)
	{
	srand((int)clock());
		for(k=0;k<Limite;++k)
		{
			x = rand()*factor;
			y = rand()*factor;
			if(x*x+y*y<1)
				{hits[omp_get_thread_num()]+=1;}
		}
	}
}

void writeTime(int wnodos, int nhilos, int tam, double tiempo){
    FILE *f = fopen("timesOpenMPI.txt","a+");
    fprintf(f,"%i;%i;%i;%.6lf\n", wnodos, nhilos, tam, tiempo);
    fclose(f);
}
void printMat(int *vect)
{
	for(int i=0;i<sizeof(vect)/8;i++)
	{printf("%i   ", vect[i]);}
	printf("\n");
}
    
int main(int argc, char *argv[])
{	//N=atoi(argv[1]);
	//int Intentos=atoi(argv[2]);
	//Hilos=atoi(argv[3]);
	Hilos=1;
	N=100000;
	int Intentos=1;
	int NPC=1;
	
	//variables to measure time
    double startTime;
    double endTime;
    double tiempo;
	int numranks, rank, len;
	MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int Limite=N/1;
	printf("En total son: %u equipos\n",numranks);
	int *hits = (int *)malloc(numranks);
	//ofstream file;
	//file.open("ResultadosDardosOpen.txt",ios::app);
	//file << "Resultados para "<<N<<" dardos con "<<Hilos<< " hilos\n";
	//file << "PI;Segundos;Error\n";
	int *hitoriginal=hits;
	for(int inte=0;inte<Intentos;inte++)
		{
		for(int i=0;i<numranks;i++)
			hits[i]=0;
		printf("Inicialmente, el vector de resultados esta en:\n");
		printMat(hits);
		
		//inicia la medicion del tiempo
		startTime = MPI_Wtime();
		//Enviar la info necesaria a todos los nodos
		MPI_Bcast(&Limite, 1, MPI_INT, 0, MPI_COMM_WORLD);
		//MPI_Barrier(MPI_COMM_WORLD);
		Calcular(hits, Hilos,Limite);
		//Recibe todos los datos obtenidos en los nodos
		MPI_Gather(&hits[rank], 1, MPI_INT,&hits, numranks, MPI_INT,  0, MPI_COMM_WORLD);
    	MPI_Barrier(MPI_COMM_WORLD);
		//Termina de medir el tiempo
		endTime = MPI_Wtime();
		
		//printf("Direccion del puntero %p, la copia tiene %p\n",hits,hitoriginal);
		printf("Finalmente, el vector de resultados esta en:\n");
		printMat(hitoriginal);
	  	if(rank==0){
			tiempo = endTime - startTime;
			//writeTime(numranks, Hilos, NPC, tiempo);
		}
		printf("tiempo: %3f\n", tiempo);
		
		/*float Suma=0;
		for(int i=0;i<Hilos;i++)
			Suma+=hits[i];
		float pi_approx=4.0*Suma/N;
	    float error=((M_PI-pi_approx)/pi_approx)*100;
	    cout << "PI=: " <<pi_approx<<endl;
	    cout << "Segundos= "<<milisecs/1000<<endl;
	    cout << "Error relativo=" << error <<"%\n"<< endl; 
	    file << Convert(pi_approx)<<";"<< Convert(milisecs/1000)<<";"<< Convert(error)<<";\n";*/
	}
return 0;
}
