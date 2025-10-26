#include <stdio.h>
#include <stdlib.h>

void bin(int num, int bits) 
{
    for(int i=bits-1;i>=0;i--)
        printf("%d",(num>>i)&1);
}
int main(){
    int a,b,rem=0;
    printf("Enter dividend\n");
    scanf("%d",&a);
    printf("Enter divisor\n");
    scanf("%d",&b);
    for (int i = 0; i < 32; i++) 
    {
        rem=(rem << 1)|((a>>(32-1))&1);
        a<<=1;
        rem-=b;
        if ((int)rem<0) 
        {
            a = a & ~1;
            rem+=b;
        } else 
        {
            a = a | 1; 
        }
    }
    printf("Quotient:");
    bin(a,32);
    printf("\nRemainder:");
    bin(rem,32);
}