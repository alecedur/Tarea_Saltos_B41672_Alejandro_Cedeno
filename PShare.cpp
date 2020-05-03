#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>  
using namespace std;

//esta clase crea una tabla para guardar los bits de historia. La tabla es de tamaño 2^size y ademas se indexa
//con los size bits menos significativos ingresados en consola. 
class PShare {
    //instancias privadas
    public:
        vector<long> tabla; //nuevamente como es necesario utilizar una tabla que varia en tamaño se usan vectores
        PShare(); //constructor
        PShare(int);
        long current_reg(long);
        void refresh_reg(char,long,long);
        void show_table(int);
};

//constructor
PShare::PShare(){  
    int filas=1;
    tabla.resize(filas);
    for (int i=0;i<filas;i++){
        tabla[i]=0;
    }  
}

//la tabla cambia a tamaño 2^size
PShare::PShare(int size){
    int filas=pow(2,size);
    tabla.resize(filas);
    for (int i=0; i<filas;i++){
        tabla[i]=0;
    }
}

//imprime todas las filas de la tabla
void PShare::show_table(int size){
    int filas=pow(2,size)+0.5;
    for(int i=0;i<filas;i++){
        cout << tabla[i];    
    }    
}

//devuelve el valor actual segun el valor indexado dado por el PC y la mascara de size.
long PShare::current_reg(long reg_val){
    return tabla[reg_val];
}

//la tabla se actualiza como la GShare, nada mas que hay que tener en cuenta que solo se debe actualizar
//la fila indexada actual. Por lo tanto se necesitan de parametros el valor actual de T o N, la mascara de
//ph bits y el valor de index actual. 
void PShare::refresh_reg(char result, long mask, long reg_val){
    tabla[reg_val]=tabla[reg_val]<<1;
    tabla[reg_val]=tabla[reg_val]&mask;
    if (result=='T'){
        tabla[reg_val]=tabla[reg_val]|0b1;
    }
    else if (result=='N'){
        tabla[reg_val]=tabla[reg_val]|0b0;
    }          
}
