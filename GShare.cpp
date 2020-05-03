#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>  

using namespace std;

//Esta clase crea un registro donde se guarda la historia pasada de saltos. Se utiliza este valor y el PC
//actual para hacer una operacion XOR y que se obtenga el valor que indexa la tabla BHT. 

class GShare {
    //instancias privadas
    long h_register;        //esta variable se usa para guardar el valor actual de bits de historia 
    public:
        GShare(); //constructor
        GShare(int, int);
        long current_reg();
        void refresh_reg(char, long);
};

GShare::GShare(){
    h_register=0;    
}
//cuando se llama esta funcion solo es necesario devolver el valor actual del registro
long GShare::current_reg(){
    return h_register;
}
//para actualizar el registro siempre se hace shift a la izquierda de 1 bit y ademas se debe aplicar la
//mascara que deje solo deje los gh bits menos significativos disponibles. Cuando se tiene eso si el valor
//actual del string es T entonces se le debe agregar un 1 al valor shifteado, si es N entonces queda en 0. 
void GShare::refresh_reg(char result, long mask){
    h_register=h_register<<1;
    h_register=h_register&mask;
    if (result=='T'){
        h_register=h_register|0b1;
    }
    else if (result=='N'){
        h_register=h_register|0b0;
    }          
}
