#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

/* 
 * Método de Monte Carlo para aproximação de Pi
 * Implementação Sequencial
 * 
 * Para compilar: gcc montecarlo.c -o montecarlo -lm
 *
 */

#define ITERACOES 5000000

double drand(double, double);

int main(void)
{	
	int i, pontosCirculo = 0;
	double px, py;

	srand(time(NULL));

	/* Para cada amostra, criar um ponto em (px, py) dentro do quadrado e 
	 * verificar se está dentro do círculo.*/
	for (i = 0; i < ITERACOES; i++) {
		px = drand(0.0, 2.0);
		py = drand(0.0, 2.0);

		/* Verificar se a distância do ponto ao centro do círculo 
		 * é menor ou igual a 1. Em caso afirmativo, o ponto pertence 
		 * ao círculo.*/
		if (sqrt( pow(px - 1, 2) + pow(py - 1, 2) ) <= 1)
			pontosCirculo++;
	}

	/* PI = 4 * Área_círculo/Área_quadrado => Área_círculo/Área_quadrado = 
	 * Pontos_no_círculo/Total_de_pontos. */
	printf("PI = %lf\n", 4.0 * (double) pontosCirculo / ITERACOES);

	return 0;
}

inline double drand(double li, double ls)
{
	return (li + ((double)rand() / (double)RAND_MAX) * (ls - li));
}

