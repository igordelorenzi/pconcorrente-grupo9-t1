/**
 * Metodo Gauss-Legendre para aproximação de Pi
 * Implementação Pararela (Pthread)
 * 
 * Para compilar: gcc gl_paralelo.c -o gl_paralelo -lgmp -pthread -lm
 *
 */

/*
* Bibliotecas utilizadas
*/
#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <time.h>
 
/*
* Prototipos das funcoes
*/
static void rPi(unsigned);
void* rAT(void *);
void* rB(void *);
void* rP(void *);

/*
* Variaveis globais
*/
mpf_t an, an1;
mpf_t bn, bn1;
mpf_t tn, tn1;
mpf_t pn, pn1;
mpf_t pi;

/*
* Threads utilizadas
*/
pthread_t tAT, tB, tP;

int main(int argc, char **argv)
{	
	/* Bits para setar precisao das variaveis precisao */
	unsigned prec = 40000000;
	/* Variaveis para calcular o speedup*/
	clock_t begin, end;
	
	printf("Calculando pi...");
	fflush(stdout);
	/* Setando a precisao das variaveis (GMP)*/
	mpf_set_default_prec(prec);
	/* Comeca a contagem de tempo */
	begin = clock();
	/* Funcao que calcula o Pi por Gauss-Legendre usando Threads*/
	rPi(prec);
	/* Termina a contagem de tempo */
	end = clock();
	/* Imprime o speedup com as variaveis antes setadas */
	printf("\nSpeedUp: %.2f segundos\n", (double)(end - begin) / CLOCKS_PER_SEC);
	printf("\n");
	/* Limpa a variavel (GMP) */
	mpf_clear(pi);
	return 0;
}
 
/*
* Nucleo de Gauss-Legendre. 
* Input: 'prec', precisao desejada em bits.
*/
static void rPi(unsigned prec)
{
    	/* Variaveis locais*/
	int i = 0; /* Contador de iteracoes*/
	mpf_t lastPi; /* Comparador de pi nas iteracoes*/
	mpf_init_set_ui(lastPi, 0.0);	
	mpf_t aux;
	mpf_init(aux);

	/* Inicia as variaveis globais*/
	mpf_inits(pi,an,bn,tn,pn,an1,bn1,tn1,pn1,NULL);

	/* Valores iniciais */
	mpf_init_set_ui(an, 1);      /* an = 1*/
	mpf_init(bn);                /* n = 1 / sqrt(2)*/
	mpf_sqrt_ui(bn, 2);
	mpf_ui_div(bn, 1, bn);
	mpf_init_set_ui(tn, 4);      /* t = 1/4*/
	mpf_ui_div(tn, 1, tn);
	mpf_init_set_ui(pn,1);       

	/* Laco infinito, ou ate conseguir a precisao desejada */
	while(1) 
	{
		if(pthread_create(&tAT, NULL, rAT, NULL)){
			printf("Erro na criacao de tAT\n");
			exit(-1);
		}
		if(pthread_create(&tB, NULL, rB, NULL)){
			printf("Erro na criacao de tB\n");
			exit(-1);
		}
		if(pthread_create(&tP, NULL, rP, NULL)){
			printf("Erro na criacao de tP\n");
			exit(-1);
		}
		pthread_join(tAT, NULL);
		pthread_join(tB, NULL);
		pthread_join(tP, NULL);	
		/* Atualiza variaveis por valores obtidos*/
		mpf_set(bn, bn1);
		mpf_set(an, an1);
		mpf_set(pn, pn1);
		mpf_set(tn, tn1);
		/* Calculo de Pi final */
		mpf_add(pi, an, bn);
		mpf_pow_ui(pi, pi, 2);
		mpf_mul_ui(aux, tn, 4);
		mpf_div(pi, pi, aux);
		
		i++;
		/* Compara o resultado obtido com o anterior, com precisao desejada.*/
		if(mpf_eq(pi, lastPi, prec)) {
			/* Imprime na tela somente os 10 primeiros digitos depois da virgula */
			gmp_printf("\nIteracao %d  | pi = %.25Ff",i, pi);
			printf("\n%d iteracoes para alcancar 10 milhoes de digitos de corretos.", i);			
			break;
		}
		/* Atualiza o pi*/
		mpf_set(lastPi, pi);
		/* Imprime na tela somente os 10 primeiros digitos depois da virgula */
		gmp_printf("\nIteracao %d  | pi = %.25Ff",i, pi);
	}
	/* Libera memoria utilizada.*/	
	mpf_clears(aux,lastPi,an,bn,tn,pn,an1,bn1,tn1,pn1,NULL);
}

/*
* Funcao que calcula An e Tn, pois Tn depende de An.
*/
void* rAT(void *nada)
{
	/* an1 = (an + bn)/2.0 */
	mpf_add(an1,an,bn);
	mpf_div_ui(an1,an1,2);

	/* tn1 = tn - pn*(an - an1)^2 */
	mpf_sub(tn1,an,an1);
	mpf_mul(tn1,tn1,tn1);
	mpf_mul(tn1,tn1,pn);   
	mpf_sub(tn1,tn,tn1);  
}

/*
* Funcao que calcula Bn.
*/
void* rB(void *nada)
{
	/* bn1 = sqrt(an*bn) */
	mpf_mul(bn1,an,bn);
	mpf_sqrt(bn1,bn1);
}

/*
* Funcao que calcula Pn.
*/
void* rP(void *nada)
{
	/* pn1 = 2*pn */
	mpf_mul_ui(pn1,pn,2);
}
