"# mpi2324" 

## Compilacion
sudo apt-get update
sudo apt-get install libopenmpi-dev

## Ejecuciones
mpicxx tarea03_sum_mpi_es.cpp -o tarea03_sum_mpi_es
mpiexec -n 4 ./tarea03_sum_mpi_es

