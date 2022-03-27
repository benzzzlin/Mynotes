#include <stdio.h>

void exchage(int *a, int *b)
{
    int temp = *b;
    *b = *a;
    *a = temp;
}
int main()
{
    int a = 1;
    int b = 2;
    int temp = 0;
    exchage(&a, &b);
    printf("a = %d b = %d \n", a, b);
    printf("hello world!\n");
    return 0; 
}