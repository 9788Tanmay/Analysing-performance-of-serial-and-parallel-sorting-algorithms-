#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
void merge(int *a, int *b, int low, int mid, int high) {

	int h, i, j, k;
	h = low;
	i = low;
	j = mid + 1;
	while ((h <= mid) && (j <= high)) {

		if (a[h] <= a[j]) {
			b[i] = a[h];
			h++;
		}
		else {
			b[i] = a[j];
			j++;
		}
		i++;
	}

	if (mid < h) {
		for (k = j; k <= high; k++) {
			b[i] = a[k];
			i++;
		}
	}

	else {
		for (k = h; k <= mid; k++) {
			b[i] = a[k];
			i++;
		}
	}

	for (k = low; k <= high; k++) {
		a[k] = b[k];
	}

}

void mergeSort(int *a, int *b, int low, int high) {
	if (low < high) {

		int mid = (low + high) / 2;
		mergeSort(a, b, low, mid);
		mergeSort(a, b, (mid + 1), high);
		merge(a, b, low, mid, high);

	}
}

int main(int argc, char** argv) {
	
	int globalArraySize =1000000;
	int *globalArray = (int*)malloc(globalArraySize * sizeof(int));

	for (int i = 0; i < globalArraySize; i++) {
		globalArray[i] = rand();
	}

	
	int world_rank;
	int world_size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int localArraySize = globalArraySize / world_size;
	int *localArray = (int*)malloc(localArraySize * sizeof(int));

	double starttime, endtime;
	starttime = MPI_Wtime();


	MPI_Scatter(globalArray, localArraySize, MPI_INT, localArray, localArraySize, MPI_INT, 0, MPI_COMM_WORLD);

	// Calling merge sort to perform sorting operation
	int *tempArray = (int*)malloc(localArraySize * sizeof(int));
	mergeSort(localArray, tempArray, 0, (localArraySize - 1));

	int *sortedArray = NULL;

	if (world_rank == 0) {
		sortedArray = (int*)malloc(globalArraySize * sizeof(int));
	}

	MPI_Gather(localArray, localArraySize, MPI_INT, sortedArray, localArraySize, MPI_INT, 0, MPI_COMM_WORLD);

	// Perform the final merge sort on the gathered arrays, in the host node which is "NODE 0"
	if (world_rank == 0) {

		int *tempArray_1 = (int*)malloc(globalArraySize * sizeof(int));
		mergeSort(sortedArray, tempArray_1, 0, (globalArraySize - 1));

		/*printf("This is the sorted array: \n");
		for (int j = 0; j < globalArraySize; j++) {
			printf("%d\n", sortedArray[j]);
		}
		*/

		free(sortedArray);
		free(tempArray_1);

		printf("\n\n\n");
		endtime = MPI_Wtime();
		printf("The total time taken to perfrom the merge sort using MPI is %f seconds\n", endtime - starttime);
	}
	free(globalArray);
	free(localArray);
	free(tempArray);

	
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
}
