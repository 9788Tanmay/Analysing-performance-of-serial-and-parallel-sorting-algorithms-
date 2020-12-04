
int* CreateArray(int temp[],int n)
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