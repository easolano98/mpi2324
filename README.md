"# mpi2324" 

## Compilacion
sudo apt-get update
sudo apt-get install libopenmpi-dev

## Ejecucion
mpicxx ejemplo02.cpp -o ejemplo02
mpiexec -n 4 ./ejemplo02
