#include <omp.h>
#include <iostream>
#include <time.h>
#include <ctime>
#include <cstdlib>
#include "chimtu.h"

using namespace std;


//int* CreateArray( int*, int);
void shellSortParallel( int*, int);
void shellSortSequential(int*, int);
void InsertSort( int*, int, int, int);


int main()
{	
	int num = 0;
	int n;
	cout<<"Enter the size of the array you want ";
	cin>>n;
	cout<<endl;
	double start, end, instanceTotal, total = 0, ntotal = 0, newtotal = 0;
	int* A;
	int length = n;
	int temp[n];
	
	double endTime, startTime, totalTime,endTime1, startTime1,arrTime,total1;
	
				startTime = time(NULL);
				
					 
						startTime1=time(NULL);
						A = CreateArray(temp,n);
						endTime1 = time(NULL);
						arrTime=endTime1 - startTime1;
						start = omp_get_wtime();
						shellSortSequential(A, length);
						end = omp_get_wtime();
						total = end - start;
						total1 += arrTime;
					
				endTime = time(NULL);

	cout <<"This is the total time for the algorithm performance  " << total << "." << endl;
	return 0;
}
/*int* CreateArray(int temp[],int n)
{
	int rnum = 0;
	int arr[n];
	srand(time(0));
	srand((unsigned) time(NULL));
	for (int i = 0; i < n; i++)
	{
		rnum = rand() % 1000000;
		arr[i] = rnum;
		temp[i] = arr[i];
	}
	return temp;
}
*/
void shellSortSequential(int array[], int length)
{
	int count = 0, j = 0, temp = 0, h=0;
	
	for(int h =length/2.5; h > 0; h = h/2.5)
	{	
		for(int i = 0; i < h; i++)
		{
			for (int f = h + i; f < length; f = f + h)
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