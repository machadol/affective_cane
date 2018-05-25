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
char batimento[]= "Batimento_cardiaco.txt";
char temperatura_mao[] ="Temperatura_mao.txt";
char GSR[] ="GSR.txt";
char temperatura_ambiente[]="Temperatura_ambiente.txt";
char umidade_ambiete[]="Umidade_ambiente.txt";
char GPS[]="GPS.txt";
char luminosidade[]="Luminosidade.txt";
char Rotation[]="Rotation.txt";

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



void save_data_one_input(float first_input,unsigned int milliseconds,char arquivo[]){
    FILE *file;
    file=fopen(arquivo,"a");
    //fseek(file,)
    if (file==NULL){
        cout<<"arquivo não exite"<<endl;
    }else{
    fprintf(file,"%u %.2f\n",milliseconds,first_input);
    fclose(file);
    }
}
void save_data_two_inputs(float first_input,float second_input,unsigned int milliseconds,char arquivo[]){
    FILE *file;
    file=fopen(arquivo,"a");
    //fseek(file,)
    if (file==NULL){
        cout<<"arquivo não exite"<<endl;
    }else{
    fprintf(file,"%u %.2f %.2f\n",milliseconds,first_input,second_input);
    fclose(file);
    }
}




void aplicar_protocolo(int entrada,unsigned int milliseconds){
     float first_input=0.0;
     float second_input=0;
    switch (entrada){
    case 0:
        cout << "\t\t Fim do programa\t\t :"<< endl;
        
        break;
    case 1:
        //char arquivo[] = "batimento_cardiaco.txt";
        cin>>first_input;
        cout << "Batimentos Cardiacos é igual a :"<<first_input<< endl;
        printf("%u milliseconds\n", milliseconds);
        save_data_one_input(first_input,milliseconds,batimento);
      
        break;

    case 2: 
        cin>>first_input;
        cout << "Dados de temperatura da mão é igual a :"<<first_input << endl;
        printf("%u milliseconds\n", milliseconds);
        save_data_one_input(first_input,milliseconds,temperatura_mao);
        break;

    case 3://recebe numero negativo
        cin>>first_input;
        cout << "GSR" <<first_input<< endl;
        printf("%u milliseconds\n", milliseconds);
        save_data_one_input(first_input,milliseconds,GSR);
        break;

    case 4:
        cin>>first_input;
        cout << "temperatura ambiente é igual a :"<<first_input << endl;
        printf("%u milliseconds\n", milliseconds);
        save_data_one_input(first_input,milliseconds,temperatura_ambiente);
        break;

    case 5:
        cin>>first_input;
        cout << "Umidade ambiente é igual a :"<<first_input << endl;
        printf("%u milliseconds\n", milliseconds);
        save_data_one_input(first_input,milliseconds,umidade_ambiete);
        break;

    case 6://2 dados latitude pegar o valor latitude e e longitude dividir/100
    //recebe numero negativo
        cin>>first_input;
        cin >> second_input;
        first_input=first_input/100;
        second_input=second_input/100;
        cout << "latitude é igual a :" <<first_input<< endl;
        cout << "longitude é igual a :" <<second_input<< endl;
        printf("%u milliseconds\n", milliseconds);
        save_data_two_inputs(first_input,second_input,milliseconds,GPS);
        break;

    case 7:
        cout << "Luminosidade é igual a :" <<first_input<< endl;
        printf("%u milliseconds\n", milliseconds);
        save_data_one_input(first_input,milliseconds,luminosidade);
        break;

    case 8:
        //cout << "FSR é igual a :" <<first_input<< endl;
       // printf("%u milliseconds\n", milliseconds);
       // break;

    case 9://recebe 4 entradas decimais e dividir por 100 
        cout << "Rotação é igual a :" <<first_input<< endl;
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
   
    int N=0, numero=0;
    int magnitude=0;
   
    int start = getMilliCount();
    do{ 
        
        cin>>entrada;
        
        int milliSeconds = getMilliSpan(start);
      
        aplicar_protocolo(entrada,milliSeconds);

          
    }while(entrada!=0);
    
}
int main(){

    leitura();
    //open_serial_port();
return 0;
}

// ler porta serial
// plotar valores
// salvar dados em BD ou arquivo 