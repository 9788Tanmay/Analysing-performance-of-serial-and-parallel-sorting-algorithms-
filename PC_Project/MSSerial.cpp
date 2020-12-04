#include <bits/stdc++.h>
#include<omp.h>
#include <time.h>
#include <stddef.h>
#include <ctime>
#include <cstdlib>
#include "chimki.h"

int elements = 1000000;
using namespace std;

void merge(int*, int*, int, int, int);

void mergesort(int *a, int*b, int start, int end) {
  int halfpoint;
  if (start < end) {
    halfpoint = (start + end) / 2;
   //#pragma omp task
    mergesort(a, b, start, halfpoint);
    mergesort(a, b, halfpoint + 1, end);
    //#pragma omp taskwait
    merge(a, b, start, halfpoint, end);
  }
}

void merge(int *a, int *b, int start, int halfpoint, int end) {
  int h, i, j, k;
  h = start;
  i = start;
  j = halfpoint + 1;

  while ((h <= halfpoint) && (j <= end)) {
    if (a[h] <= a[j]) {
      b[i] = a[h];
      h++;
    } else {
      b[i] = a[j];
      j++;
    }
    i++;
  }
  if (h > halfpoint) {
    for (k = j; k <= end; k++) {
      b[i] = a[k];
      i++;
    }
  } else {
    for (k = h; k <= halfpoint; k++) {
      b[i] = a[k];
      i++;
    }
  }

  // Write the final sorted array to our original one
  for (k = start; k <= end; k++) {
    a[k] = b[k];
  }
}
void printFoo(int *a)
{
  int i=0;
  while(a[i]!=NULL)
  {
    cout<<"a["<<i<<"]"<<" "<<a[i]<<" ";
    i++;
  }
}
int main() {

  //int elements;
  //cout<<"Enter the no of elements :";
  //cin>>elements; 
  int *a;
  int *b;
  a = (int*) malloc(2*elements*sizeof(int));
  b = (int*) malloc(2*elements*sizeof(int));
  double startTime=0,stopTime=0;
  srand(time(NULL));
  

  // Start merge sort
  CreateArray(a,b,elements);
  startTime = clock();
  mergesort(a, b, 0, 2*elements - 1);
  stopTime = clock();
  // Print the sorted array
  cout <<"After Sorting" << endl;
  if(elements<=100)
  {
    printFoo(a);
  }
  cout<<"Time taken for sorting of "<<elements<<" elements with serial mergesort is: "<<(stopTime-startTime)/CLOCKS_PER_SEC<<endl;
  free(a);
  free(b);
  
  return 0;
}
