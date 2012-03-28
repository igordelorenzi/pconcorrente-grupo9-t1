#include <stdio.h>
#include <math.h>
#include <gmp.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_THREADS	8
#define NUM_ITERACOES	1000

// Como compilar: gcc borwein_paralelo.c -o borwein_paralelo -lgmp -pthread
// Como executar: ./borwein_paralelo

pthread_mutex_t mutexA, mutexB, mutexC;
mpf_t pi, aAnt, aPos, yAnt, yPos; // Variaveis principais
mpf_t aAntAux, aAntAux2, yAntAux, yAntAux2; // Variaveis auxiliares
int i, x;


void *calculo0 (void *threadParametro){
	
	pthread_mutex_lock(&mutexA);

	mpf_pow_ui(aAntAux, yPos, 2);
	mpf_add(aAntAux, aAntAux, yPos);
	mpf_add_ui(aAntAux, aAntAux, 1);
	mpf_mul(aAntAux, yPos, aAntAux);

	pthread_mutex_unlock(&mutexA);

	pthread_exit(NULL);
}

void *calculo1 (void *threadParametro){

	mpf_set_ui(aAntAux2, 2);

	x = (2*i) + 3;
	mpf_pow_ui(aAntAux2, aAntAux2, x);

}

int main(void)
{
	struct timeval tv1, tv2; // Variaveis para calcular o tempo de execucao
	double t1, t2;
        pthread_t threads[NUM_THREADS];  // Declarando as threads

        // Flag de Erro das Threads
        int codRetorno;

	/* Registra a marca de tempo t1 antes de iniciar os calculos */
        gettimeofday(&tv1, NULL);
	t1 = (double)(tv1.tv_sec) + (double)(tv1.tv_usec) / 1000000.00;

	// Inicializacoes de variaveis GMP
	mpf_init2(pi, 100000);
	mpf_init2(aAnt, 100000);
	mpf_init2(aPos, 100000);
	mpf_init2(yAnt, 100000);
	mpf_init2(yPos, 100000);

	mpf_init2(aAntAux, 100000);
	mpf_init2(aAntAux2, 100000);
	mpf_init2(yAntAux, 100000);
	mpf_init2(yAntAux2, 100000);
	
	// Primeiros valores
	mpf_sqrt_ui(aAnt, 2);
	mpf_mul_ui(aAnt, aAnt, 4);
	mpf_ui_sub(aAnt, 6, aAnt);

	mpf_sqrt_ui(yAnt, 2);
	mpf_sub_ui(yAnt, yAnt, 1);

	mpf_set_ui(pi, 0);
	i = 0;
	x = 0;


	// Iteracoes para calculo do numero Pi
	while(i < NUM_ITERACOES){

	//      yPos = (1 - pow(1-pow(yAnt,4),0.25)) / (1 + pow(1-pow(yAnt,4),0.25));

		mpf_pow_ui(yAntAux, yAnt, 4);
		mpf_ui_sub(yAntAux, 1, yAntAux);
	
		mpf_sqrt(yAntAux, yAntAux);
		mpf_sqrt(yAntAux, yAntAux);
		mpf_add_ui(yAntAux2, yAntAux, 1);
		mpf_ui_sub(yAntAux, 1, yAntAux);

		mpf_div(yPos, yAntAux, yAntAux2);

      	//	aPos = (aAnt*pow((1+yPos),4)) - pow(2,(2*i+3))*yPos*(1 + yPos + pow(yPos,2));

        	
		// Criando Thread 0
		codRetorno = pthread_create(&threads[0], NULL, calculo0, NULL);
		if (codRetorno){
                	printf("ERRO de codigo %d na criacao da thread.\n", codRetorno);
                	exit(-1);
        	}
		

		// Criando Thread 1
        	codRetorno = pthread_create(&threads[1], NULL, calculo1, NULL);
		if(codRetorno){
			printf("ERRO de codigo %d na criacao da thread.\n", codRetorno);
                	exit(-1);
		}
        	pthread_join(threads[0], NULL);
		pthread_join(threads[1], NULL);
			
		mpf_mul(aAntAux, aAntAux, aAntAux2);

		mpf_add_ui(aAntAux2, yPos, 1);

		mpf_pow_ui(aAntAux2, aAntAux2, 4);
		mpf_mul(aAntAux2, aAnt, aAntAux2);

		mpf_sub(aPos, aAntAux2, aAntAux);
 
		mpf_set(aAnt, aPos);
		mpf_set(yAnt, yPos);

		i++;
    	}

	mpf_ui_div(pi, 1, aAnt);

	/* Registra a marca de tempo t2 depois de terminar os calculos */
        gettimeofday(&tv2, NULL);
        t2 = (double)(tv2.tv_sec) + (double)(tv2.tv_usec) / 1000000.00;

	gmp_printf("PI: %.100Ff\n", pi);
	printf("Tempo: %lf\n", (t2-t1));

        /* Last thing that main() should do */
        pthread_exit(NULL);

}
