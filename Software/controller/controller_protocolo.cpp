#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <queue>
#include <ctime>
using namespace std;
int resultado;
int bit_descritor = 0;
int entradaCopia;
int count = 1;
unsigned int entrada;


void aplicar_protocolo(int bit_descritor,int resultado_magnitude){
    switch (bit_descritor){
    

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
        cout << "entrada inválida é igual a :" << endl;
    }
}

void leitura(){
    int magnitude=0;
    int resultado_magnitude=0;
    cin >> entrada;
    entradaCopia = entrada;
    do
    {
        entrada = entrada / 10;
        count++;
    } while (entrada / 10 != 0);

    cout << count << endl;

    int *p;
    int N, numero;

    N = count;
    p = (int *)malloc(N * sizeof(double));
    *p = entradaCopia;
    // pega o primeiro bit de cada entrada
    bit_descritor = *p / pow(10, N - 1);
    //calcula a magnitude
    resultado_magnitude =*p - bit_descritor*pow(10,N-1);
    cout << bit_descritor << endl;
    //cout << resultado_magnitude << endl;
    
     aplicar_protocolo(bit_descritor,resultado_magnitude);
}

int main()
{

    leitura();
    return 0;
}