#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include "timer.hh"


using namespace std;

string Convert (float number){
    ostringstream buff;
    buff<<number;
    return buff.str();}
    
int main(int argc, char const *argv[])
{	int N=atoi(argv[1]);
	int Intentos=atoi(argv[2]);
	long k,n,hits;
	clock_t tiempoInicio, tiempoFinal;
	ofstream file;
	file.open("ResultadosDardosSecuencial.txt",ios::app);
	float segundos;
	const double factor=1.0/RAND_MAX;
	file << "Resultados para "<<N<<" dardos:\n";
	file << "PI;Segundos;Error\n";
	for(int inte=0;inte<Intentos;inte++)
		{
		hits=0;
		Timer t;
		//tiempoInicio=clock();
		srand((int)clock());
		for(k=hits=0;k<N;++k)
		{
			double x = rand()*factor;
			double y = rand()*factor;
			if(x*x+y*y<1)
				++hits;
		}
		float pi_approx=4.0*hits/N;
		//tiempoFinal = clock();
		long long ms=t.elapsed();
		segundos=ms/1000000.0;
		//segundos = (double) (tiempoFinal - tiempoInicio) / CLOCKS_PER_SEC;
	    float error=((M_PI-pi_approx)/pi_approx)*100;
	    cout << "PI=: " <<pi_approx<<endl;
	    cout << "Segundos= "<<segundos<<endl;
	    cout << "Error relativo=" << error <<"%\n"<< endl; 
	    file << Convert(pi_approx)<<";"<< Convert(segundos)<<";"<< Convert(error)<<";\n";
	}
return 0;
}

