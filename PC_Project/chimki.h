
void CreateArray(int *a,int *b,int elements)
{
	int rnum = 0;
	int arr[2*elements];
	srand(time(0));
	srand((unsigned) time(NULL));
	for (int i = 0; i < 2*elements; i++)
	{
		rnum = rand() % 1000000;
		arr[i] = rnum;
		a[i] = arr[i];
        b[i] = arr[i];
	}
}
