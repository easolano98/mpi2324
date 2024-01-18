
#include <mpi.h>
#include <stdio.h>  // Incluimos la biblioteca para printf
#include <string>
#include <fstream>
#include <functional>

#define  NUM_ITERACIONES 100

std::vector<int> read_file() {
    std::fstream fs("./datos.txt", std::ios::in);
    std::string line;

    std::vector<int> ret;

    while (std::getline(fs, line)) {
        ret.push_back(std::stoi(line));
    }
    fs.close();
    return ret;
}

int sumar(int* tmp, int n){
    int suma = 0;
    for(int i = 0; i < n; i++){
        suma = suma +tmp[i];
    }
    return suma;
}

int* generar_vector(){
    int * data=new int[NUM_ITERACIONES];
    for (int i = 0; i<NUM_ITERACIONES; i++){
        data[i] = i;
    }
    return data;
}


int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);

    int rank,nprocs;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if(rank == 0){

     //  int *data= generar_vector();

      std::vector<int>data_vect=read_file();
      int *data=data_vect.data();

        std::printf("total ranks:%d\n ", nprocs);

        int count=NUM_ITERACIONES/nprocs;

        std::printf("counts:%d\n ", count);

        int val_adicional=NUM_ITERACIONES%nprocs;

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

        std::string str = "";
        for (int i = 0; i < nprocs; i++) {
            str = str + std::to_string(suma_ranks[i]) + ", ";
        }
        std::printf("Sumas parciales: %s\n", str.c_str());

        std::printf("sumas Total: %d\n", sumaTotal);

    }else{
        int contador[1];

        MPI_Recv(contador, 1, MPI_INT , 0  , 1  , MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int * data=new int[contador[0]];

        MPI_Recv(data, contador[0], MPI_INT , 0  , 0  , MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        std::string str ="";
        for(int i = 0; i<contador[0]; i++){
            str=str+std::to_string(data[i])+",";

        }
        std::printf("RANK_%d datos recibinedo ==>%s\n", rank, str.c_str());


        int suma_parcial = sumar(data, contador[0]);

        MPI_Send(&suma_parcial, 1, MPI_INT, 0,0,MPI_COMM_WORLD);

    }

    MPI_Finalize();

    return 0;



}