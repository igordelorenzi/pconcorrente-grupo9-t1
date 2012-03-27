/**
 * Método de Monte Carlo para aproximação de Pi
 * Implementação Sequencial
 * 
 * Para compilar: gcc mc_seq.c -o mc_seq -lm
 *
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

/* 1 bilhão de iterações */
#define NUM_ITERACOES 1000000000
#define ldouble long double
#define llong long long int

/**
 * http://en.wikipedia.org/wiki/Linear_congruential_generator
 * http://www.dreamincode.net/forums/topic/24225-random-number-generation-102/
 */
#define M 4294967296 // 2^32
#define A 22695477
#define C 1

/* Retorna um número randômico */
inline double get_random(long int *, double, double);

int main(void)
{	
	llong i;
	ldouble pi, pontosDentro = 0.0, pontosFora = 0.0;
	struct timeval tv1, tv2;
    double x, y, t1, t2;
	long int seed = (long int)time(NULL);

	/* Registra a marca de tempo t1 antes de iniciar os cálculos */
	gettimeofday(&tv1, NULL);
    t1 = (double)(tv1.tv_sec) + (double)(tv1.tv_usec) / 1000000.00;

	/* Método de Monte Carlo */
	for (i = 0L; i < NUM_ITERACOES; i++) {
		x = get_random(&seed, 0.0, 1.0);
		y = get_random(&seed, 0.0, 1.0);

		if (y > sqrt(1.0 - x*x))
			pontosFora++;
		else
			pontosDentro++;
	}

	/* Pi = 4 * Área_círculo / Área_quadrado */
	pi = 4.0 * pontosDentro / (pontosFora + pontosDentro);

	/* Registra a marca de tempo t2 depois de terminar os cálculos */
	gettimeofday(&tv2, NULL);
    t2 = (double)(tv2.tv_sec) + (double)(tv2.tv_usec) / 1000000.00;

	printf("Pi aproximado: %.10Lf\nTempo: %lf\n", pi, (t2-t1));

	return 0;
}

/** 
 * Retorna um número randômico entre o li (limite inferior) e 
 * ls (limite superior)
 */
inline double get_random(long int *x, double li, double ls)
{
	*x = (A * *x + C) % M;
	return (li + ((double)*x / (double)M) * (ls - li));
}

