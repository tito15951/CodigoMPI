#!/bin/bash
intentos=3
mpicc -Wall AgujasMPI.cc -o AgujasMPI -lm
mpicc DardosMPI.cc -o DardosMPI
g++ AgujasSecuencial.cc -o AgujasSecuencial
g++ DardosSecuencial.cc -o DardosSecuencial

for j in 10 100 1000 1000 10000 100000
do
	for i in 1 ... 3
	do
			echo "Agujas secuencial"
			./AgujasSecuencial $j 1
			echo "Dardos secuencial"
			./DardosSecuencial $j 1
			echo "Dardos MPI"
			mpiexec -n 3 ./DardosMPI $j 	
			echo "Agujas MPI"
			mpiexec -n 3 ./AgujasMPI $j 
	done
done
