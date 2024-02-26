#include<vector>
#include<iostream>
#include<mpi.h>
#include<cmath>
#include<fstream>
#include<string>

int max (int * vector, int n){
    int max= vector[0];
    for(int i=0; i<n; i++){
        if(vector[i]>max){
            max=vector[i];
        }
    }
    return max;
}

 
std::vector<int> read_file() { 
    std::fstream fs("./datos.txt", std::ios::in ); 
    std::string line; 
 
    std::vector<int> ret; 
 
    while( std::getline(fs, line) ){ 
        ret.push_back( std::stoi(line) ); 
    } 
    fs.close(); 
 
    return ret; 
}


int main (int argc, char** argv  ){

    MPI_Init( &argc , &argv);

    int rank, nprocs;

    MPI_Comm_rank( MPI_COMM_WORLD , &rank);
    MPI_Comm_size( MPI_COMM_WORLD , &nprocs);

    
    int max_local=0;
    int real_size;
    int block_size;
    int padding;
    if(rank==0){
         
        std::vector<int> elementos=read_file();

        int * data= elementos.data();
        
        real_size = std::ceil((double)elementos.size() / nprocs) * nprocs;
        block_size = real_size / nprocs;
        padding = real_size - elementos.size();
        
       /* for(int i=1;i<nprocs;i++){
            int start= i*block_size+diferencia;
           
          //  MPI_Send( &block_size , 1 , MPI_INT , i , 0 , MPI_COMM_WORLD);
           // MPI_Send( &data[start] , block_size , MPI_INT , i , 1 , MPI_COMM_WORLD);
           

        }*/
         MPI_Bcast( &block_size , 1 , MPI_INT , 0 , MPI_COMM_WORLD);
         MPI_Scatter( data , block_size , MPI_INT , MPI_IN_PLACE , 0 , MPI_INT , 0 , MPI_COMM_WORLD);
        
        int maximos[nprocs];
        maximos[0]= max(data, block_size);

        for(int i=1;  i<nprocs; i++){
            MPI_Recv( &maximos[i] , 1 , MPI_INT , i , 1 , MPI_COMM_WORLD , MPI_STATUS_IGNORE);
        }
        max_local=max(maximos, nprocs);

        std::printf("max %d \n ", max_local);

    }
    else{
        MPI_Bcast( &block_size , 1 , MPI_INT , 0 , MPI_COMM_WORLD); 
       // MPI_Recv( &block_size_r , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD , MPI_STATUS_IGNORE);
        int * maximos= new int [block_size];
        MPI_Scatter( nullptr , 0 , MPI_INT , maximos , block_size , MPI_INT , 0 , MPI_COMM_WORLD);
       // MPI_Recv( maximos , block_size_r , MPI_INT , 0 , 1 , MPI_COMM_WORLD , MPI_STATUS_IGNORE);
        max_local=max(maximos, block_size);
        MPI_Send( &max_local , 1 , MPI_INT , 0 , 1 , MPI_COMM_WORLD );

    }

    MPI_Finalize();

return 0;
}
