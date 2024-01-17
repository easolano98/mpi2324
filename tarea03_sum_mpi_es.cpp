
#include <mpi.h>
#include <stdio.h>  // Incluimos la biblioteca para printf
#include <string>

#define  NUM_ITERACIONES 50

int sumar(int* tmp, int n){
    int suma = 0;
    for(int i = 0; i < n; i++){
        suma = suma +tmp[i];
    }
    return suma;
}

int * generar_vector(){
    for (int i = 0; i<NUM_ITERACIONES; i++){
        data[i] = i;
    }
}


int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);

    int rank,nprocs;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if(rank == 0){

        std::vector<int> vct= generar_vector();
       // std::vector<int> data= ;

        std::printf("total ranks:%d\n ", nprocs);

        int count=NUM_ITERACIONES/nprocs;
        int val_adicional=NUM_ITERACIONES%nprocs;

        int * data= vct.data();


        for(int rank_id = 1; rank_id < nprocs; rank_id++){
            std::printf("RANK_0 enviando datos a RANK_%d\n ", rank_id);
            int start = rank_id*count+val_adicional;
            MPI_Send(&count, 1, MPI_INT, rank_id, 1, MPI_COMM_WORLD);
            MPI_Send(&data[start], count, MPI_INT, rank_id, 0, MPI_COMM_WORLD);
        }


        int suma_ranks[nprocs];
        suma_ranks[0] = sumar(data, count+val_adicional);

        for(int rank_id = 1; rank_id < nprocs; rank_id++){
            MPI_Recv(&suma_ranks[rank_id], 1, MPI_INT, rank_id, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }


        int sumaTotal = sumar(suma_ranks,nprocs);

        std::printf("sumas Total: %d", sumaTotal);

    }else{
        int contador[1];
//        MPI_Status status;
        MPI_Recv(contador, 1, MPI_INT , 0  , 1  , MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int * data=new int[contador[0]];

        MPI_Recv(data, contador[0], MPI_INT , 0  , 0  , MPI_COMM_WORLD, MPI_STATUS_IGNORE);



//        MPI_Get_count(&status, MPI_INT, &contador);

        int suma_parcial = sumar(data, contador[0]);

        MPI_Send(&suma_parcial, 1, MPI_INT, 0,0,MPI_COMM_WORLD);

    }

    MPI_Finalize();

    return 0;



}