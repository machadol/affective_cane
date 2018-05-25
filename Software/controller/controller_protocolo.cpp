#include <iostream>
#include <math.h>
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <queue>
#include <time.h>
#include <sys/timeb.h>
#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions 	   */ 
#include <errno.h>

using namespace std;
int resultado;
int bit_descritor = 0;
int entradaCopia;
int count = 1;
unsigned int entrada;
float tempo;
float ml_segundos=0;
time_t tInicio, tFim; 
char caracteres;
int contador=0;
int n_bits;
int longitude=0;

void open_serial_port(){
    int fd;
    fd =open("/dev/ttyUSB0",O_RDWR);
    if (fd==-1){
        cout<<"deu ruim"<<endl;
    }else
        cout<<"funfou"<<endl;
    char read_buffer[32];
    int bytes_read;
    bytes_read = read(fd,&read_buffer,32);
    for(int i=0;i<bytes_read;i++){	 /*printing only the received characters*/ 		 
         printf("%c",read_buffer[i]);
    }
}



void salvar_dados(int resultado_magnitude,unsigned int milliseconds){
    FILE *file;
    file=fopen("batimento_cardiaco.txt","a");
    //fseek(file,)

    fprintf(file,"%d %u\n",resultado_magnitude,milliseconds);
    fclose(file);

}
int ler_arquivo(){
    FILE *file;
    file=fopen("batimento_cardiaco.txt","r");
   
    while((caracteres=fgetc(file))!=EOF){
        contador++;
    }
    return contador;
}

void aplicar_protocolo(int bit_descritor,int resultado_magnitude,unsigned int milliseconds){
    
    switch (bit_descritor){
    case 0:
        cout << "\t\t Fim do programa\t\t :"<< endl;
        
        break;
    case 1:
        cout << "Batimentos Cardiacos é igual a :"<<resultado_magnitude<< endl;
        printf("%u milliseconds\n", milliseconds);
        n_bits=ler_arquivo();
        salvar_dados(resultado_magnitude,milliseconds);
        cout<<n_bits<<endl;
        break;

    case 2:
        cout << "Dados de temperatura da mão é igual a :"<<resultado_magnitude << endl;
        printf("%u milliseconds\n", milliseconds);
        break;

    case 3://recebe numero negativo
        cout << "GSR" <<resultado_magnitude<< endl;
        printf("%u milliseconds\n", milliseconds);
        break;

    case 4:
        cout << "temperatura ambiente é igual a :"<<resultado_magnitude << endl;
        printf("%u milliseconds\n", milliseconds);
        break;

    case 5:
        cout << "Umidade ambiente é igual a :"<<resultado_magnitude << endl;
        printf("%u milliseconds\n", milliseconds);
        break;

    case 6://2 dados latitude pegar o valor latitude e e longitude dividir/100
    //recebe numero negativo
        
        cin >> longitude;
        cout << "latitude é igual a :" <<resultado_magnitude/100<< endl;
        cout << "longitude é igual a :" <<longitude/100<< endl;
        printf("%u milliseconds\n", milliseconds);
        break;

    case 7:
        cout << "Luminosidade é igual a :" <<resultado_magnitude<< endl;
        printf("%u milliseconds\n", milliseconds);
        break;

    case 8:
        //cout << "FSR é igual a :" <<resultado_magnitude<< endl;
       // printf("%u milliseconds\n", milliseconds);
       // break;

    case 9://recebe 4 entradas decimais e dividir por 100 
        cout << "Rotação é igual a :" <<resultado_magnitude<< endl;
        printf("%u milliseconds\n", milliseconds);
        break;

    case 10:
        cout << "Batime é igual a :"<<resultado_magnitude << endl;
        printf("%u milliseconds\n", milliseconds);
        break;

    default:
        cout << "Entrada inválida" << endl;
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
        
        //tFim=clock();
        int milliSeconds = getMilliSpan(start);
        //tempo =   difftime(tFim,tInicio)/100;
        aplicar_protocolo(bit_descritor,resultado_magnitude,milliSeconds);
        

        free(p);
        p==NULL;
       
        //corrigir o erro de incremento do count
        count=1;
       
          
    }while(bit_descritor!=0);
    
}
int main(){

    //leitura();
    open_serial_port();
return 0;
}

// ler porta serial
// plotar valores
// salvar dados em BD ou arquivo 