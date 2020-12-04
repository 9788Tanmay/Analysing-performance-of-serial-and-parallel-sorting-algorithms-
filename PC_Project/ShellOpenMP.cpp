#include <omp.h>
#include <iostream>
#include <time.h>
#include <ctime>
#include <cstdlib>
#include "chimtu.h"
#include <fstream>
using namespace std;


int* CreateArray( int*);
void shellSortParallel( int*, int);
void shellSortSequential(int*, int);
void InsertSort( int*, int, int, int);


int main()
{	
	int num = 0;
	cout << "Select 1 for sequential shell sort or 2 for parallel shell sort." << endl;
	cin >> num;
	double start, end, instanceTotal, total = 0, ntotal = 0, newtotal = 0;
	int* A;
	int n = 1000000;
	int temp[1000000];
	
	double endTime = 0.000, startTime = 0.000, totalTime = 0.000;
	switch(num)
	{
	case 1:
				startTime = time(NULL);
				
					
						A = CreateArray(temp,n);
						start = omp_get_wtime();// Start performance timer 1 run
						shellSortSequential(A, n);//Run the algorithm
						end = omp_get_wtime();// End performance timer 1 run
						instanceTotal = end - start;// Get performance time 1 run
						total += instanceTotal;
					
				
				endTime = time(NULL);
		   break;
	case 2:
			startTime = time(NULL);
			    //#pragma omp single
					
						A = CreateArray(temp,n);
						start = omp_get_wtime();// Start performance timer 1 run
						shellSortParallel(A, n);//Run the algorithm
						end = omp_get_wtime();// End performance timer 1 run
						instanceTotal = end - start;
						total += instanceTotal;
				
			endTime = time(NULL);
			break;
	default:
			cout <<"invalid number" << endl;
	}
	
	totalTime = endTime - startTime;
	cout <<"This is the total time for the algorithm performance  " << total << "." << endl;
	
	
	return 0;

	
}
void shellSortParallel(int array[], int n)
{
	int h;
	int j = 0;
	int temp = 0;
	int i = 0;
	#pragma omp parallel shared( array, n) private(h,i) default(none) 
	for(h =n/2.5; h > 0; h = h/2.5)
	{
		#pragma omp for //schedule(runtime) 
		for( i = 0; i < h; i++)
		{
			//int ID = omp_get_thread_num();
			InsertSort(array, i, n, h);
		}
	}
}
void InsertSort(int arr[], int i, int n,  int half){
	//cout << ID << "   ";
	int temp = 0;
	int j = 0;
    //#pragma omp parallel schedule(guided,half) 
	for (int f = half + i; f < n; f = f + half)
	{
		j = f;
		//#pragma omp sections
		//{
		//#pragma omp section	
		while(j > i && arr[j-half] > arr[j])
		{
			temp = arr[j];
			arr[j] = arr[j-half];
			arr[j-half] = temp;
			j = j -half;
		}
	//}
}
}
/*void Printarray( vector<int> n)
{ int num = 0;
	for(int r = 0; r < 100000; r++)
	{
		cout << n << " ";
	}
	cout << endl;*/
	
void shellSortSequential(int array[], int n)
{
	int count = 0, j = 0, temp = 0, h=0;
	
	for(int h =n/2.5; h > 0; h = h/2.5)
	{	
		for(int i = 0; i < h; i++)
		{
			for (int f = h + i; f < n; f = f + h)
			{
			 j = f;
				while(j > i && array[j-h] > array[j])
				{
					temp = array[j];
					array[j] = array[j-h];
					array[j-h] = temp;
					j = j -h;
				}
			}
		}
	}
}