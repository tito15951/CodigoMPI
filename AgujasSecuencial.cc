#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <ctime>
#include <math.h>
#include <fstream>
#include <sstream>
#include "timer.hh"

using namespace std;

string Convert (float number){
    std::ostringstream buff;
    buff<<number;
    return buff.str();}
    
class Floor{
	public:
		float l;//Distancia entre las lineas verticales
};
class Needle{
	public:
		float x;//Distancia entre el origen y el centro de la aguja
		float t;//Angulo entre la linea de la x y la pos de la aguja
		float L;//Longitud de la aguja
		void Imprimir(){
			cout<< "x="<< x<< " t="<<t<<endl;
		}
};
Needle TirarAguja(float Longitud, Floor Piso)
{
	double limite=1.0/RAND_MAX;
	Needle Aguja;
	srand((int)clock());
	Aguja.x=(double)rand()*limite;
	Aguja.t=(double)rand()*limite*180;
	Aguja.L=Longitud;
	return Aguja;
}
bool Cruzar_Linea(Needle Aguja, Floor Piso)
{
	float x_right_tip = Aguja.x + Aguja.L / 2 * sin(Aguja.t*M_PI/180);
    float x_left_tip  = Aguja.x - Aguja.L / 2 * sin(Aguja.t*M_PI/180);
    return x_right_tip >= Piso.l || x_left_tip <= 0.0;
}
void Estimar_Probabilidad_Aguja_Cruzar_Linea(float NAgujas, Floor Piso, float L, float Resultados[4])
{
	clock_t tiempoInicio, tiempoFinal;
	float segundos;
	int nb_crosses=0;
	//tiempoInicio=clock();
	Timer t;
    for (int t=0;t<NAgujas;t++)
        {Needle Aguja=TirarAguja(L,Piso);
        //Aguja.Imprimir();
		if (Cruzar_Linea(Aguja, Piso))
            nb_crosses += 1;
    }
    //tiempoFinal = clock();
    long long ms=t.elapsed();
	segundos=ms/1000000.0;
	//segundos = (double) (tiempoFinal - tiempoInicio) / CLOCKS_PER_SEC;
    float pi_calculated= 2*  NAgujas/nb_crosses;
    float error=((M_PI-pi_calculated)/pi_calculated)*100;
    cout << "PI=: " <<pi_calculated<<endl;
    cout << "Segundos= "<<segundos<<endl;
    cout << "Numero de intentos= "<<NAgujas<< " Numero de aciertos= "<<nb_crosses<<endl;
    cout << "Error relativo=" << error <<"%\n"<< endl;
    Resultados[0]=pi_calculated;
    Resultados[1]=segundos;
    Resultados[2]=nb_crosses;
    Resultados[3]=error;
}

int main(int argc, char const *argv[])
{
	int NAgujas=atoi(argv[1]);
	int Intentos=atoi(argv[2]);
	//int NAgujas=20;
	//int Intentos=10;
	ofstream file;
	file.open("ResultadosAgujasSecuencial.txt",ios::app);
	Floor Piso;
	Piso.l=1;
	file << "Resultados para "<<NAgujas<<" agujas:\n";
	file << "PI;Segundos;Aciertos;Error\n";
	for(int inte=0;inte<Intentos;inte++)
		{
		float Resultados[4];
		Estimar_Probabilidad_Aguja_Cruzar_Linea(NAgujas,Piso,1,Resultados);
		file << Convert(Resultados[0])<<";"<< Convert(Resultados[1])<<";"<< Convert(Resultados[2])<<";"<< Convert(Resultados[3])<<";\n";
		}
	file.close();
}
