/* 
 * Método de Monte Carlo para aproximação de Pi
 * Implementação Pararela (Pthread)
 * 
 * Para compilar: gcc montecarlo2.c -o montecarlo2 -pthread -lm
 *
 */

#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>

#define NUM_ITERACOES 50000000
#define NUM_THREADS 200
#define PI 3.14159265359

void *subrotina_montecarlo(void *);

// Variáveis globais
pthread_mutex_t mutex; // Mutual exclusion
struct drand48_data status[NUM_THREADS];
struct drand48_data *pts[NUM_THREADS];
double total_over = 0;
double total_under = 0;
long int seed;

int main(void)
{
	int check;
	double pi, piaux, erro;
	ssize_t bytes_read;
	size_t nbytes;
	long i;
	pthread_t threads[NUM_THREADS];
	FILE *fp;

	fp = fopen("/dev/random","r");
	fread(&seed, 1, 8, fp);
	fclose(fp);
	printf("Semente: %ld\n", seed);

	pthread_mutex_init(&mutex,NULL);

	for (i=0; i < NUM_THREADS; i++) {
		check = pthread_create(&threads[i], NULL, subrotina_montecarlo, (void *)i);
		if (check) {
			printf("ERRO! pthread_create() retornou um erro de codigo: %d\n", check);
			exit(-1);
		}
	}

	for (i=0; i < NUM_THREADS; i++)
		pthread_join(threads[i], NULL);

	pi = 4.0*total_under/(total_over + total_under);
	erro = sqrt((PI-pi)*(PI-pi));

	printf("Valor de Pi: %.20lf\n"
		"Erro: %.20lf\n"
		"Foram feitas %d iteracoes usando %d threads.\n",
		pi,erro,NUM_ITERACOES,NUM_THREADS);

	return 0;
}

void *subrotina_montecarlo(void *ptr)
{
	long worker = (long)ptr + 1;
	double over = 0;
	double under = 0;
	double x, y, *ptx, *pty;
	double count = 0;
	double n;

	// Gerador local de semente
	pts[worker] = &status[worker];
	srand48_r(seed*worker,pts[worker]);

	ptx = &x;
	pty = &y;

	for (n=worker; n <= NUM_ITERACOES; n+= NUM_THREADS) {
		count++;
		drand48_r(pts[worker],ptx);
		drand48_r(pts[worker],pty);

		if (y > sqrt(1. - x*x)) over++;
		else under++;
	}

	// Adiciona as variáveis locais às variavéis globais
	pthread_mutex_lock(&mutex);

	total_over += over;
	total_under += under;

	pthread_mutex_unlock(&mutex);

	pthread_exit(NULL);
}

