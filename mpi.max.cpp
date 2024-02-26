#include <iostream> 
#include <fstream> 
#include <string> 
#include <vector> 
#include <cmath>
#include <mpi.h>
//MAX 

int max(int * tmp,  int n){
    int max=0;
    for(int i=0; i<n; i++){
        if(tmp[i]>max){
            max=tmp[i];
        }
    }
    return max;

}


int main(int argc, char** argv){

    MPI_Init( &argc , &argv);

    int rank,nprocs;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size( MPI_COMM_WORLD , &nprocs);

   
   
    int max_global;


    if(rank==0){
        std::vector<int>vector={1,5,8,0,9,10,15,11,50,800,40,90,100,2};
        int * data= vector.data();

        int count= vector.size()/nprocs;
        int resto= vector.size()%nprocs;

        for(int i=1; i<nprocs; i++){
            int start = i*count+resto;
          MPI_Send( &count , 1 , MPI_INT , i , 0 , MPI_COMM_WORLD);
          MPI_Send( &data[start] , count ,MPI_INT , i ,1 ,  MPI_COMM_WORLD);  
        }

        int datos_max[nprocs];
        datos_max[0]=max(data, count+resto);

        for(int i=1; i<nprocs; i++){
            MPI_Recv( &datos_max[i] , 1 , MPI_INT , i , 1 , MPI_COMM_WORLD , MPI_STATUS_IGNORE);
        }

        max_global=max(datos_max, nprocs);
        std::printf("max: %d ", max_global);

    }else{

        int contador[1];
        
        MPI_Recv( contador , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD , MPI_STATUS_IGNORE);

        int * datos= new int[contador[0]];

        MPI_Recv( datos , contador[0] ,  MPI_INT , 0 , 1 , MPI_COMM_WORLD , MPI_STATUS_IGNORE);
        
         int max_local=max(datos, contador[0]);

        MPI_Send( &max_local , 1 , MPI_INT , 0 , 1 , MPI_COMM_WORLD);


    }

    MPI_Finalize();
    return 0;

}