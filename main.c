#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

/*
*
* implementação de código RSA
*
*/


int is_prime(long long n)
{
    for(long long i=2;i * i<=n;i++)
        if(!n%2)
            return 0;
    return 1;
}

void calcula_quociente(long long *resto, long long *quociente, long long a, long long b){

	if(a >= 0){

		*quociente = 0;
		*resto = a;

		while(*resto >= b){

			*resto -= b;
			*quociente = *quociente + 1;
		}
	}

}

long potencia(long long a, long long e, long long n){
    long p = 1;
	while(1){
		if(e == 0)
			return p;
        
		else if(e%2 != 0){
			//Realizamos a redução módulo n de cada uma das multpilicações 			
			p = (p * a)%n;
			e = (e-1)/2;
		}
		else{
			e = e/2;
		}
		a = (a*a)%n;
	}
}
char * decodifica(int *cifra, int size, long long d, long long n){
	
	long long i;
	char *mensagem;

	mensagem = malloc(size + 1 * sizeof(char));
	
	for(i = 0; i < size; i++){
		mensagem[i] = potencia(cifra[i], d, n);
	}

	return mensagem;
}

int * codificar(char * msg, long long e, long long n)
{
    long long i;
    int sizeMsg = strlen(msg);
    int * cifra = (int*)calloc(sizeMsg + 1, sizeof(int));
    for(int i=0; i < sizeMsg; i++)
        cifra[i] = potencia(msg[i],e,n);
    return cifra;
}

long long menor_primo(long long phi, long long p, long long p2){

	long long i, e, n = p * p2;
	for(i = 2; i < phi; i++){

		if(phi%i != 0 && is_prime(i) && i != p && i != p2){
			e = i;
			break;
		}
	}

	return e;
}

long long mdc(long long a, long long b){
	
	long long resto, quociente, xB = 1, yB = 0, x = 0, y = 1, alpha, beta, phi;
	phi = a;

	resto = a;
	while(resto != 0){
		calcula_quociente(&resto, &quociente, a, b);
		a = b;
		b = resto;
		if(resto > 0){
			alpha = xB - quociente *x;
			beta = yB - quociente * y;

			xB = x;
			yB = y;
			x = alpha;
			y = beta;
		}
	}

	if(beta < 0)
		beta = phi + beta;

	return beta;

}

long long get_prime()
{
    long long s =  rand();
    while (!is_prime(s) || s <= 2)
    {
        s++;
    }
    return s;
}

int main(int * args, char *argv[])
{
    printf("\niniciando algoritimo RSA");
    char * frase = (char *)calloc(strlen(argv[1]) + 1 ,sizeof(char));
    int* cifra;
    strcpy(argv[1],frase);
    printf("\n copindo frase");
    printf("\nFrase: %s ",argv[0]);
    long long phi =0, n =0, e =0, d =0, p = get_prime(), p2 =get_prime();
    printf("\n p: %llu p2: %llu",p,p2);
    n = p * p2;
    phi = (p - 1)* (p2 -1);
    e = menor_primo(phi, p, p2);
    d = mdc(phi,e);
    printf("\nchave publica: %llu %llu",d,n);
    cifra = codificar(frase, e, n);
    printf("\nmensagem cifrada :");
    for(int i=0; i < strlen(frase) ; i++)
        printf("%d",cifra[i]);
    printf("\nmensagem descriptografada: %s", decodifica(cifra, strlen(frase), d , n));
    free(frase);
    return 0;
}