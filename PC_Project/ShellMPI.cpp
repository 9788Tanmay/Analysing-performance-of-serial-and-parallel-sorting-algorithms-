#include <iostream>
#include <cstdlib>
#include <time.h>
#include "mpi.h"
#include "chimtu.h"
using namespace std;

void shellSort(int* arr, int n2)
{
	int gap;
	for (gap = 1; gap < n2 / 3; gap = gap * 3 + 1);
	for (; gap > 0; gap = (gap - 1) / 3)
	{
		for (int i = gap; i < n2; i++)
		{
			int temp1 = arr[i];
			int j;
			for (j = i; j >= gap && arr[j - gap] > temp1; j -= gap)
				arr[j] = arr[j - gap];

			arr[j] = temp1;
		}
	}
}

int main(int argc, char **argv) {
	int size, rank, n = 1000000;
	int *array = new int[n];
	int *temp=new int[n];
	MPI_Status status;


	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	srand(time(NULL));

	if (rank == 0) {
		array= CreateArray(temp,n);
	}

	int *subArray;
	int n2 = size > 1 ? n / (size - 1) : n;
	double begin;
	double end;

	if (rank == 0) {
		begin = clock();

		for (int i = 1; i < size; i++) {
			MPI_Send(array + n2 * (i - 1), n2, MPI_INT, i, 0, MPI_COMM_WORLD);
		}

		int k = n - n2 * (size - 1);
		subArray = new int[k];

		for (int i = n2 * (size - 1); i < n; i++) {
			subArray[i - n2 * (size - 1)] = array[i];
		}

		shellSort(subArray, k);

		int *rArray = new int[n];

		for (int i = 0; i < k; i++) {
			rArray[i] = subArray[i];
		}

		for (int i = 1; i < size; i++) {
			MPI_Recv(rArray + n2 * (i - 1) + k, n2, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
		}

		shellSort(rArray, n);
		end = clock();

		//for (int i = 0; i < n; i++)
		//	cout << "sorted array[" << i << "] = " << rArray[i] << '\n';
         cout<<endl;        
		cout << "Time elapsed " << (end - begin) / CLOCKS_PER_SEC << "seconds"<<endl;
	}
	else
	{
		subArray = new int[n2];
		MPI_Recv(subArray, n2, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		shellSort(subArray, n2);
		MPI_Send(subArray, n2, MPI_INT, 0, 1, MPI_COMM_WORLD);
	}

	MPI_Finalize();

	return 0;
}
