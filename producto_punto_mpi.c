#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char** argv){
	int rank,size;
	int *vectorA, *vectorB, *sub_vectorA, *sub_vectorB;
	int local_sum = 0, global_sum = 0;
	int n = 12;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	if(rank == 0){
		vectorA = (int*)malloc(n*sizeof(int));
		vectorB = (int*)malloc(n*sizeof(int));
		for(int i=0;i<n; i++){
			vectorA[i] = i+1;
			vectorB[i] = i+2;
		}
	}

	int sub_n = n/size;
	sub_vectorA = (int*)malloc(sub_n*sizeof(int));
	sub_vectorB = (int*)malloc(sub_n*sizeof(int));

	MPI_Scatter(vectorA,sub_n,MPI_INT,sub_vectorA,sub_n,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Scatter(vectorB,sub_n,MPI_INT,sub_vectorB,sub_n,MPI_INT,0,MPI_COMM_WORLD);

	for(int i = 0; i<sub_n; i++){
		local_sum +=sub_vectorA[i]*sub_vectorB[i];
	}

	MPI_Reduce(&local_sum,&global_sum,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

	if(rank==0){
		printf("La suma global es: %d\n", global_sum);
	}

	free(sub_vectorA);
	free(sub_vectorB);
	if(rank==0){
		free(vectorA);
		free(vectorB);
	}

	MPI_Finalize();
	return 0;
}
