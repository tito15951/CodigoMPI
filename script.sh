#!/bin/bash
cd CodigoMPI/
mpicc -Wall AgujasMPI.cc -o AgujasMPI -lm
mpicc DardosMPI.cc -o DardosMPI
g++ AgujasSecuencial.cc -o AgujasSecuencial
g++ DardosSecuencial.cc -o DardosSecuencial
mv DardosMPI ../
mv AgujasMPI ../
mv AgujasSecuencial ../
mv DardosSecuencial ../
cd
scp DardosMPI cluser@wn1:/home/cluser
scp DardosMPI cluser@wn2:/home/cluser
scp DardosMPI cluser@wn3:/home/cluser
scp AgujasMPI cluser@wn1:/home/cluser
scp AgujasMPI cluser@wn2:/home/cluser
scp AgujasMPI cluser@wn3:/home/cluser

for j in 10 100 1000 1000 10000 100000
do
	for i in 1 ... 5
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
