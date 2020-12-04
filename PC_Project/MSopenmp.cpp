#include <bits/stdc++.h>
#include <omp.h>
#include "chimtu.h"
using namespace std;
int n=1000000;
vector<int> merge(const vector<int>& left, const vector<int>& right)
{
	vector<int> result;
	unsigned left_it = 0, right_it = 0;
    
			while (left_it < left.size() && right_it < right.size())
			{
				if (left[left_it] < right[right_it])
				{
					result.push_back(left[left_it]);
					left_it++;
				}
				else
				{
					result.push_back(right[right_it]);
					right_it++;
				}
			}
    
			while (left_it < left.size())
			{
				result.push_back(left[left_it]);
				left_it++;
			}
			
			while (right_it < right.size())
			{
				result.push_back(right[right_it]);
				right_it++;
			}
    

	return result;
}
vector<int> mergesort(vector<int>& vec, int threads)
{
	
	if (vec.size() == 1)
	{
		return vec;
	}
	vector<int>::iterator middle = vec.begin() + (vec.size() / 2);

	vector<int> left(vec.begin(), middle);
	vector<int> right(middle, vec.end());
	if (threads > 1)
	{
		#pragma omp parallel sections
		{
			#pragma omp section
			{
			  left = mergesort(left, threads / 2);
			}
			#pragma omp section
			{
			  right = mergesort(right, threads - threads / 2);
			}
		}
	}
	else
	{
		left = mergesort(left, 1);
		right = mergesort(right, 1);
	}

	return merge(left, right);
}

int main()
{
n/=4;
	vector<int> v;	
    int *temp=new int[n];
	int num_threads=4;
	double wall_timer1,wall_timer2,wall_timer3,wall_timer4;
    int *arr=new int[n];
	//for (num_threads = 1; num_threads <= 16; ++num_threads)
	//
	int i;
		arr=CreateArray(temp,n);
		   
			#pragma omp for schedule(runtime)
		  		for(i=0;i<n;i++)
					v.push_back(arr[i]);
        
		wall_timer1 = omp_get_wtime();
		v = mergesort(v, num_threads);
		wall_timer2 = omp_get_wtime();

		cout << "threads: " << num_threads << "\ttime on wall: " <<  (wall_timer2-wall_timer1) << endl;
	//}
}