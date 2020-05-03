#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>  
using namespace std;

//Esta clase crea una tabla y la modifica de acuerdo a los valores de index y valor actual de T o N respectivamente. 
class BHT {
    //instancias privadas
    public:
        vector<char> tabla; //se usa un vector para poder modificar el tamaño del array en otras instancias
        BHT(); //constructor
        BHT(int); 
        char current_predic(long);      
        void refresh_table(char,long);
        void show_table(int);
};
    //estado default de la clase
BHT::BHT(){
    int filas= 1;
    tabla.resize(filas);
    for(int i=0;i<filas;i++){
        tabla[i]='N';
    }
}

    //aqui se ingresa de parametros el size el cual nos da el tamaño de la tabla que es 2^size
BHT::BHT(int ROWS){         
    int filas=pow(2,ROWS)+0.5;//se le agrega 0.5 para que siempre redondee hacia arriba
    tabla.resize(filas);   
    for(int i=0;i<filas;i++){
        tabla[i]='N';   //se inicializa la tabla en "Strongly Not Taken" o 'N'
    }
}

//esta funcion imprime todas las filas en el array
void BHT::show_table(int ROWS){
    int filas=pow(2,ROWS)+0.5;      
    for(int i=0;i<filas;i++){
        cout << tabla[i];    
    }    
}

//esta funcion retorna un caracter que puede ser T o N dependiendo del valor actual de la fila 
//evaluada, entonces si es Weakly o Strong Taken o Not taken, retorna el valor adecuado.
char BHT::current_predic(long row){
    if (tabla[row]=='T'|tabla[row]=='t'){
        return 'T';
    }    
    else if (tabla[row]=='N'|tabla[row]=='n') {
        return 'N';
    }  
}

//esta funcion actualiza el valor de la tabla segun el estado actual de branch (T o N) y ademas se  le da
//de parametro el valor de index correcto desde el main.
void BHT::refresh_table(char valor_actual,long row){
    char pred_act=tabla[row];
    if(valor_actual=='T'){
        if(pred_act=='T'){
            tabla[row]='T';
        }
        if (pred_act=='t'){
            tabla[row]='T';
        }
        if (pred_act=='N'){
            tabla[row]='n';
        }
        else if(pred_act=='n'){
            tabla[row]='t';
        }
    }
    else if(valor_actual=='N'){
        if(pred_act=='N'){
            tabla[row]='N';
        }
        if (pred_act=='n'){
            tabla[row]='N';
        }
        if (pred_act=='T'){
            tabla[row]='t';
        }
        else if(pred_act=='t'){
            tabla[row]='n';
        }
    }
}
