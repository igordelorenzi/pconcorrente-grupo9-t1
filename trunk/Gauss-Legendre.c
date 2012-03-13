#include <stdio.h>
#include <math.h>

int main(void)
{
    double pi = 0.0;
    double aAnt = 1, aPos;
    double bAnt = (1/sqrt(2)), bPos;
    double tAnt = 0.25, tPos;
    double pAnt = 1, pPos;
    long double i;

    for(i = 0; i < 500; i++)
    {
        aPos = (aAnt + bAnt)/2;
        bPos = sqrt(aAnt*bAnt);
        tPos = tAnt - pAnt*pow((aAnt - aPos),2);
        pPos = 2*pAnt;
        aAnt = aPos;
        bAnt = bPos;
        tAnt = tPos;
        pAnt = pPos;
    }
    pi = (pow((aAnt+bAnt),2))/(4*tAnt);

    printf("%.50f",pi);

}
