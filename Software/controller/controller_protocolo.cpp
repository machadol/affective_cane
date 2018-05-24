#include <iostream>
#include <math.h>
#include<stdio.h>
#include <stdlib.h>
#include <queue>
#include <time.h>
#include <sys/timeb.h>

using namespace std;
int resultado;
int bit_descritor = 0;
int entradaCopia;
int count = 1;
unsigned int entrada;
float tempo;
float ml_segundos=0;
time_t tInicio, tFim; 


void aplicar_protocolo(int bit_descritor,int resultado_magnitude){
    
    switch (bit_descritor){
    case 0:
        cout << "Fim do programa :"<< endl;
        break;
    case 1:
        cout << "Batimentos Cardiacos é igual a :"<<resultado_magnitude<< endl;
        break;

    case 2:
        cout << "Dados de temperatura da mão é igual a :"<<resultado_magnitude << endl;
        break;

    case 3:
        cout << "HRV" <<resultado_magnitude<< endl;
        break;

    case 4:
        cout << "temperatura ambiente é igual a :"<<resultado_magnitude << endl;
        break;

    case 5:
        cout << "Umidade ambiente é igual a :"<<resultado_magnitude << endl;
        break;

    case 6:
        cout << "GPS é igual a :" <<resultado_magnitude<< endl;
        break;

    case 7:
        cout << "Luminosidade é igual a :" <<resultado_magnitude<< endl;
        break;

    case 8:
        cout << "FSR é igual a :" <<resultado_magnitude<< endl;
        break;

    case 9:
        cout << "Rotação é igual a :" <<resultado_magnitude<< endl;
        break;

    case 10:
        cout << "Batime é igual a :"<<resultado_magnitude << endl;
        break;

    default:
        cout << "Entrada inválida:" << endl;
    }
}
int getMilliCount(){
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

int getMilliSpan(int nTimeStart){
	int nSpan = getMilliCount() - nTimeStart;
	if(nSpan < 0)
		nSpan += 0x100000 * 1000;
	return nSpan;
}

void leitura(){
    double Segundos, Milissegundos, Microssegundos;
    int *p;
    int N=0, numero=0;
    int magnitude=0;
    int resultado_magnitude=0;
   // tInicio = clock();
    int start = getMilliCount();
    do{ 
        
        cin>>entrada;
        
        // cout<<entrada<<endl;
        entradaCopia = entrada;
        do{
            entrada = entrada / 10;
            count++;
        } while (entrada / 10 != 0);

        N = count;
        
        p = (int *)malloc(N * sizeof(double));
        *p = entradaCopia;
        // pega o primeiro bit de cada entrada
        bit_descritor = *p / pow(10, N - 1);
        //calcula a magnitude
        resultado_magnitude =*p - bit_descritor*pow(10,N-1);
        aplicar_protocolo(bit_descritor,resultado_magnitude);
        //tFim=clock();
        int milliSecondsElapsed = getMilliSpan(start);
        //tempo =   difftime(tFim,tInicio)/100;
        printf("%u milliseconds\n", milliSecondsElapsed);

        free(p);
        p==NULL;
       
        //corrigir o erro de incremento do count
        count=1;
       
          
    }while(entrada!=0);
}
int main(){

    leitura();

return 0;
}

// ler porta serial
// plotar valores
// salvar dados em BD ou arquivo 