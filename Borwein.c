#include <stdio.h>
#include <math.h>

int main(void)
{
    double pi = 0.0;
    double aAnt = 6 - (4*sqrt(2)), aPos;
    double yAnt = sqrt(2) - 1, yPos;
    long double i;

    for(i = 0; i < 500; i++)
    {
        yPos = (1 - pow(1-pow(yAnt,4),0.25)) / (1 + pow(1-pow(yAnt,4),0.25));
        aPos = (aAnt*pow((1+yPos),4)) - pow(2,(2*i+3))*yPos*(1 + yPos + pow(yPos,2));
        aAnt = aPos;
        yAnt = yPos;
    }
    pi = 1 / aAnt;

    printf("%.50f",pi);

}
