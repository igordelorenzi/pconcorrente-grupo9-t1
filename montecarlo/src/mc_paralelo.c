/**
 * Método de Monte Carlo para aproximação de Pi
 * Implementação Pararela (Pthread)
 * 
 * Para compilar: gcc mc_paralelo.c -o mc_paralelo -pthread -lm
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <gmp.h>
#include <math.h>
#include <stdint.h>

/* 1 bilhão de iterações */
#define NUM_ITERACOES 1000000000
#define NUM_THREADS 70
#define ldouble long double

/**
 * http://en.wikipedia.org/wiki/Linear_congruential_generator
 * http://www.dreamincode.net/forums/topic/24225-random-number-generation-102/
 */
#define M 4294967296 // 2^32
#define A 22695477
#define C 1

/* Subrotina de cada thread */
void *subrotina_montecarlo(void *);
/* Retorna um número randômico */
inline double get_random(long int *, double, double);

/* Variáveis globais */
pthread_mutex_t mc_mutex; // Mutual exclusion
ldouble total_over = 0.0;
ldouble total_under = 0.0;

int main(int argc, char *argv[])
{
	ldouble pi;
	long int i;
	struct timeval tv1, tv2;
    double t1, t2;
	pthread_t threads[NUM_THREADS];
	int check;

	/* Registra a marca de tempo t1 antes de iniciar os cálculos */
	gettimeofday(&tv1, NULL);
    t1 = (double)(tv1.tv_sec) + (double)(tv1.tv_usec) / 1000000.00;

	/* Inicializa os mc_mutexes */
	pthread_mutex_init(&mc_mutex, NULL);

	/* Cria as threads para divisão de carga do método de aproximação */
	for (i = 0L; i < NUM_THREADS; i++) {
		check = pthread_create(&threads[i], NULL, subrotina_montecarlo, (void *)i);
		if (check) {
			printf("ERRO! pthread_create() retornou um erro de codigo: %d\n", check);
			exit(-1);
		}
	}

	/* Aguarda as threads terminarem */
	for (i = 0L; i < NUM_THREADS; i++)
		pthread_join(threads[i], NULL);

	/* Aproximação de Pi */
	pi = 4.0 * total_under / (total_over + total_under);

	/* Registra a marca de tempo t2 depois de terminar os cálculos */
	gettimeofday(&tv2, NULL);
	t2 = (double)(tv2.tv_sec) + (double)(tv2.tv_usec) / 1000000.00;

	//printf("total_under: %.10Lf\ntotal_over: %.10Lf\n\n",total_under,total_over);	
	printf("Pi aproximado: %.10Lf\nTempo: %lf\n", pi, (t2-t1));

	pthread_mutex_destroy(&mc_mutex);

	return 0;
}

void *subrotina_montecarlo(void *ptr)
{
	long int fator = (long int)ptr + 1L;
	ldouble over = 0.0;
	ldouble under = 0.0;
	double x, y;
	int i;
	long int seed = (long int)time(NULL);

	for (i = 0L; i < NUM_ITERACOES/NUM_THREADS; i++) {
		x = get_random(&seed, 0.0, 1.0);
		y = get_random(&seed, 0.0, 1.0);

		if (y > sqrt(1.0 - x*x))
			over++;
		else
			under++;
	}

	/* Adiciona as variáveis locais às variavéis globais */
	pthread_mutex_lock(&mc_mutex);
	total_over += over;
	total_under += under;
	pthread_mutex_unlock(&mc_mutex);

	pthread_exit(NULL);
}

inline double get_random(long int *x, double li, double ls)
{
	*x = (A * *x + C) % M;
	return (li + ((double)*x / (double)M) * (ls - li));
}

