#include <stdio.h>
#include <math.h>
#include <gmp.h>

int main(void)
{
	mpf_t pi, aAnt, aPos, yAnt, yPos; // Variáveis principais
	mpf_t aAntAux, aAntAux2, yAntAux, yAntAux2;
	int i, x;

	mpf_init2(pi, 1000);
	mpf_init2(aAnt, 1000);
	mpf_init2(aPos, 1000);
	mpf_init2(yAnt, 1000);
	mpf_init2(yPos, 1000);

	mpf_init2(aAntAux, 1000);
	mpf_init2(aAntAux2, 1000);
	mpf_init2(yAntAux, 1000);
	mpf_init2(yAntAux2, 1000);
	
	mpf_sqrt_ui(aAnt, 2);
	mpf_mul_ui(aAnt, aAnt, 4);
	mpf_ui_sub(aAnt, 6, aAnt);

	mpf_sqrt_ui(yAnt, 2);
	mpf_sub_ui(yAnt, yAnt, 1);

/*	gmp_printf("%.100Ff", aAnt);
	printf("\n");
	gmp_printf("%.100Ff", yAnt);
	printf("\n");
*/
	mpf_set_ui(pi, 0);
	i = 0;
	x = 0;

	while(i < 1000){
	mpf_pow_ui(yAntAux, yAnt, 4);
	mpf_ui_sub(yAntAux, 1, yAntAux);


	mpf_pow_ui(yAntAux, yAntAux, 0.25);
	
	mpf_add_ui(yAntAux2, yAntAux, 1);
	mpf_ui_sub(yAntAux, 1, yAntAux);


//	gmp_printf("Aux1: %.100Ff \n", yAntAux);

//	gmp_printf("Aux2: %.100Ff \n", yAntAux2);

	mpf_div(yPos, yAntAux, yAntAux2);

//        yPos = (1 - pow(1-pow(yAnt,4),0.25)) / (1 + pow(1-pow(yAnt,4),0.25));
//	printf("%d ", i);

//	gmp_printf("yPos: %.100Ff \n", yPos);



	mpf_mul(aAntAux, yPos, yPos);


//	gmp_printf("aAntAux: %.100Ff \n", aAntAux);

	mpf_add(aAntAux, aAntAux, yPos);
	mpf_add_ui(aAntAux, aAntAux, 1);

	mpf_mul(aAntAux, yPos, aAntAux);
	
	mpf_set_ui(aAntAux2, 2);

	x = (2*i) + 3;
	mpf_pow_ui(aAntAux2, aAntAux2, x);
	
//	gmp_printf("aAntAux2:  %.100Ff \n", aAntAux2);
	mpf_mul(aAntAux, aAntAux, aAntAux2);

	mpf_add_ui(yPos, yPos, 1);
	mpf_set(aAntAux2, yPos);
	mpf_sub_ui(yPos, yPos, 1);

	mpf_pow_ui(aAntAux2, aAntAux2, 4);
	mpf_mul(aAntAux2, aAnt, aAntAux2);

	mpf_sub(aPos, aAntAux2, aAntAux);

 
	mpf_set(aAnt, aPos);
	mpf_set(yAnt, yPos);

//	gmp_printf("%.100Ff \n", yAntAux);
      //aPos = (aAnt*pow((1+yPos),4)) - pow(2,(2*i+3))*yPos*(1 + yPos + pow(yPos,2));
       // aAnt = aPos;
        //yAnt = yPos;

	i++;
    }

	mpf_ui_div(pi, 1, aAnt);
    //pi = 1 / a

	gmp_printf("PI: %.100Ff\n", pi);

}
