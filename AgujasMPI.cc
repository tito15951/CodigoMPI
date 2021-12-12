#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <omp.h>
#include <math.h>
    
class Floor{
	public:
		float l;//Distancia entre las lineas verticales
};

double valorAbsoluto(double num)
{
	if (num<0)
		return -1*num;
	else
		return num;
}

void writeTime(float pi, double tiempo,double error,int N){
    FILE *f = fopen("AgujasMPI.txt","a+");
    fprintf(f,"%i;%f;%.6lf;%.2lf\n", N,pi, tiempo, error);
    fclose(f);
}
class Needle{
	public:
		float x;//Distancia entre el origen y el centro de la aguja
		float t;//Angulo entre la linea de la x y la pos de la aguja
		float L;//Longitud de la aguja
		void Imprimir(int cod){
			printf("Soy %u, x=%.6f t=%.6f\n",cod,x,t);
			//cout<< "x="<< x<< " t="<<t<<endl;
		}
};
Needle TirarAguja(float Longitud)
{
	double limite=1.0/RAND_MAX;
	Needle Aguja;
	srand((int)clock());
	Aguja.x=(double)rand()*limite;
	Aguja.t=(double)rand()*limite*180;
	Aguja.L=Longitud;
	return Aguja;
}
bool Cruzar_Linea(Needle Aguja, float Piso)
{
	float x_right_tip = Aguja.x + Aguja.L / 2.0 * (double)sin((double)(Aguja.t*M_PI/180.0));
    float x_left_tip  = Aguja.x - Aguja.L / 2.0 * (double)sin((double)(Aguja.t*M_PI/180.0));
    return x_right_tip >= Piso || x_left_tip <= 0.0;
}

int main(int argc, char *argv[])
{
	
	/*float NAgujas=(float)atoi(argv[1]);
	int Intentos=atoi(argv[2]);
	Hilos=atoi(argv[3]);*/
	//int N=10;
	int N=atoi(argv[1]);
	//int CantAgujas;
	//float NAgujas=1000000;
	//int Intentos=2;
	//int Hilos=1;
	double startTime;
    double endTime;
    double tiempo;
	int numranks, rank;

	MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int *nbcrosses = (int*)malloc(numranks*sizeof(int));
	int nbcrosseslocal =0;
	int Limite=N/numranks;
	int *nbcrossesoriginal=nbcrosses;

	if(rank==0)
	{
		for(int i=0;i<numranks;i++)
			nbcrosses[i]=0;
		startTime = MPI_Wtime();
		}
	MPI_Barrier(MPI_COMM_WORLD);	
	nbcrosseslocal=0;
	int t;
    for (t=0;t<Limite;t++)
        {Needle Aguja=TirarAguja(1);
		//Aguja.Imprimir(rank);
		if (Cruzar_Linea(Aguja, 1))
			{nbcrosseslocal+=1;}//Modifica solo la posicion que es el	
	}		
	//printf("Soy el proceso %u, tengo %u\n",rank,nbcrosseslocal);
    
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Gather(&nbcrosseslocal, 1, MPI_INT,nbcrosses, 1, MPI_INT,  0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);

	if(rank==0){
		endTime = MPI_Wtime();
		tiempo = endTime - startTime;
		int suma=0;
		for(int i=0;i<numranks;i++)
		{suma+=nbcrossesoriginal[i];}
		//printMat(hitoriginal,numranks,rank);
		double pi=2.0*(double)N/(double)suma;
		double error=((M_PI-pi)/pi)*100;
		printf("pi: %.8f\n", pi);
		printf("tiempo: %.5f\n", tiempo);
		printf("error: %.2f\n", valorAbsoluto(error));
		writeTime(pi, tiempo,valorAbsoluto(error),N);
		MPI_Finalize();
	}
		   
	

}
