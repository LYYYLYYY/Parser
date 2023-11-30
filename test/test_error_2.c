#include <stdio.h>

int sum(int a, int b)
{
	return a - b;
}

int main()
{
	int a, b;
	a = 1, b = 2;//123
	double c = sum(a, b);
	printf("%f\n", c);
	return 0 //Óï·¨´íÎó£¬Î´¼Ó·ÖºÅ
}