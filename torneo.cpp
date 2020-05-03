#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>  
using namespace std;

//esta clase es la que hace una tabla metapredictor para determinar cual tipo de prediccion se prefiere actualmente
//entre PShared y GShared. Como es una tabla que se desea modificar se usa vectores. El tamaño de la tabla tambien
//se da por size, y tiene 2^size filas. Esta tabla es muy parecida a la BHT, por lo menos en la forma que 
//retorna datos y como se actualiza, nada mas que tiene una forma de actualizarse un poco mas peculiar. Cuando
//ambos predictores son incorrectos o correctos no se debe actualizar el peso de la fila actual, solo cuando
//uno es correcto y el otro incorrecto y el peso se modifica hacia el que es correcto en una unidad o estado.
class Torneo {
    public:
        vector<char> tabla; //se usan vectores para poder modificar la tabla
        Torneo();
        Torneo(int);
        char current_predic(long);
        void refresh_table(char,long);
        void show_table(int);
};
//constructor
Torneo::Torneo(){
    int filas=1;
    tabla.resize(filas);
    for(int i=0;i<filas;i++){
        tabla[i]='P';
    }
}

//se inicializa la tabla de metapredictor en Strongly Prefer PShared 
Torneo::Torneo(int size){
    int filas=pow(2,size)+0.5;
    tabla.resize(filas);
    for(int i=0;i<filas;i++){
        tabla[i]='P';       
    }
}

//Se tienen 4 estados que son Strongly Prefer GShared/PShared y Weakly Prefer GShared/PShared
//Retorna valores igual que el BHT, si es P o p retorna P, si es G o g retorna G. 
char Torneo::current_predic(long row){
    if (tabla[row]=='P'|tabla[row]=='p'){
        return 'P';
    }    
    else if (tabla[row]=='G'|tabla[row]=='g') {
        return 'G';
    }  
}

//imprime todas las filas
void Torneo::show_table(int ROWS){
    int filas=pow(2,ROWS)+0.5;
    for(int i=0;i<filas;i++){
        cout << tabla[i];    
    }
    cout << endl;    
}
//en este caso el algoritmo de peso esta en el main y solo se recibe el resultado de cual preferimos de 
//parametro ademas de la fila indexada utilizando el PC y la mascara de size bits. 
void Torneo::refresh_table(char status,long row){
    char pred_act=tabla[row];
    if (status=='P'){
        if (pred_act=='P'){
            tabla[row]='P';
        }
        if (pred_act=='p'){
            tabla[row]='P';
        }
        if (pred_act=='g'){
            tabla[row]='p';
        }
        else if (pred_act=='G'){
            tabla[row]='g';
        }
        
    }
    else if(status=='G'){
        if (pred_act=='P'){
            tabla[row]='p';
        }
        if (pred_act=='p'){
            tabla[row]='g';
        }
        if (pred_act=='g'){
            tabla[row]='G';
        }
        else if (pred_act=='G'){
            tabla[row]='G';
        }
    }

}
