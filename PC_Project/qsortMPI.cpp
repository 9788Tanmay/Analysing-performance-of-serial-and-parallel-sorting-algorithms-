#include <iostream>
#include <mpi.h>
#include <math.h>
#include <time.h>
#include "chimtu.h"

void merge(int* array1, int* array2, int* merged_array, int size) {
	int i = 0, j = 0, k = 0;
	while (i < size/2 && j < size/2) {
		merged_array[k++] = array1[i] < array2[j] ? array1[i++] : array2[j++];
	}
	while (i < size/2) {
		merged_array[k++] = array1[i++];
	}
	while (j < size/2) {
		merged_array[k++] = array2[j++];
	}
}

int partition(int* array, int start, int end) {
	int pivot = array[end];
	int smallerCount = start;
	for (int j = start; j < end; j++) {
		if (array[j] <= pivot) {
			int temp = array[j];
			array[j] = array[smallerCount];
			array[smallerCount] = temp;
			smallerCount += 1;
		}
	}
	array[end] = array[smallerCount];
	array[smallerCount] = pivot;
	return smallerCount;
}
void quickSort(int* array, int start, int end) {
	if (start < end) {
		int pivotLoc = partition(array, start, end);
		quickSort(array, start, pivotLoc - 1);
		quickSort(array, pivotLoc + 1, end);
	}
}

void quickSort(int* array, int size) {
	quickSort(array, 0, size - 1);
}

void printArray(int* A, int size) {
	for (int i = 0; i < size; i++) {
		std::cout << "A[" << i << "]" << " is " << A[i] << "\n";
	}
	std::cout << "\n";
}


int main(int argc, char** argv){
	int my_rank, comm_sz;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	int size = atoi(argv[1]);
	int comm_size = atoi(argv[0]);
	int local_size = size / comm_sz;
	int *temp1=new int[size];
	int* A = NULL;
	int* local_A = (int*)malloc(local_size * sizeof(int));
	if (my_rank == 0) {

		A = (int*)malloc(size * sizeof(int));
		A=CreateArray(temp1,size);
		//printArray(A, size);
	}

	double start, finish, global_elapsed_time;
	MPI_Barrier(MPI_COMM_WORLD);
	start = MPI_Wtime();

	//distribute A to local_A for each process
	MPI_Scatter(A, local_size, MPI_INT, local_A, local_size, MPI_INT, 0, MPI_COMM_WORLD);

	quickSort(local_A, local_size);//perform local quicksort
	
	int merge_tree_depth = (int)log2(comm_sz);
	int my_index = my_rank;// each process's behavior in merge part
	int step = 0;
	int* current_array = local_A;
	while (step < merge_tree_depth) {
		int merged_size = local_size * (int)pow(2, step + 1);
		if (my_index % 2 == 0) {

			int* merged_array = (int*)malloc(merged_size * sizeof(int));
			int* recv_array = (int*)malloc((merged_size / 2) * sizeof(int));

			int src_rank = my_rank + (int)pow(2, step);
			MPI_Recv(recv_array, merged_size / 2, MPI_INT, src_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			merge(current_array, recv_array, merged_array, merged_size);
			current_array = merged_array;
			//printArray(current_array, merged_size);
		}
		else {
			int des_rank = my_rank - (int)pow(2, step);
			MPI_Send(current_array, merged_size / 2, MPI_INT, des_rank, 0, MPI_COMM_WORLD);
			break;
		}
		step++;
		my_index = my_index / 2;
	}
	finish = MPI_Wtime();
	double local_elapsed_time = finish - start;
	MPI_Reduce(&local_elapsed_time, &global_elapsed_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

	if (my_rank == 0) {
		
		std::cout << "With comm_sz = " << comm_sz << " and input array size = " << argv[1]
			<< ", elapsed time is " << global_elapsed_time << " seconds\n";
	}
	MPI_Finalize();
	return 0;
}