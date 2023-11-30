#include <stdio.h>
#include <math.h>

// point struct
struct point {
	int x;
	int y;
};

double distance(struct point a, struct point b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

int main()
{
	struct point a, b;
	a.x = 1;
	a.y = 1;
	b.x = 2;
	b.y = 2;
	double c = distance(a, b);
	printf("%f\n", c);
	return 0;
}