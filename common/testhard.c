#include <stdio.h>

struct hhh{
	int l,r;
};
struct hhh h[100];

int sum(struct hhh a,struct hhh b)
{
	return a.l+a.r+b.l+b.r;
}

int main()
{
	h[0].l=1,h[0].r=1;
	h[1].l=1,h[1].r=1;
	printf("%d\n",sum(h[0],h[1]));
	return 0;
}
