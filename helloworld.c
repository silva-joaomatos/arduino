#include <stdio.h>
#include "hello.h"
int square(int a);
int main(void)
{
	int c=0,b=0;
	printf("Insert integer to calculate square\n");
	scanf("%d",&c);
	b= square(c);
	printf("%d² = %d", c,b);
	return 0;
}
int square(int a) {
	return a*a;
}

