//Tarea 1 Predictor de Saltos. Alejandro Cedeño Uribe, B41672.
//Este codigo deberia poder usar 1 de 4 tipos de prediccion seleccionable(0. Bimodal, 1. Global, 2. Privado, 3. Torneo) y determinar la cantidad de branches, T's correctos e incorrectos
//N's correctos e incorrectos y la cantidad de saltos del archivo seleccionado. Ademas puede generar un archivo final que da las estadisticas del archivo ingresado. 
#include "BHT.cpp"
#include "GShare.cpp"
#include "PShare.cpp"
#include "torneo.cpp"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

//esta funcion se encarga de extraer la parte numeral de 
//la entrada del gunzip. De entrada tiene el string completo
//de una linea del archivo, posteriormente usa la funcion find 
//para encontrar un espacio y utiliza la funcion substring
//para sacar un "string del string" que va desde el primer caracter hasta el punto donde encontro el espacio. 
//Luego convierte ese string a long usando stol(string to long). Devuleve el long con el PC.
//Se usa long porque la cantidad de numeros supera 32 bits. 
long num_parse(string actual){
    if (actual.empty()) {
        return 0;
    }
    while(!actual.empty()){
        int found = actual.find(' ');
        string result = actual.substr(0, found); //agarra solo los numeros del string proveniente del archivo
        long pc_now=stol(result,nullptr);   //numeros del string a long
        return pc_now; 
    }
}


//Esta funcion tiene el mismo funcionamiento de num_parse, parsea una
//linea del archivo gunzip pero en este caso solo quiere agarrar la
//ultima letra o caracter del string y lo guarda en un char porque
//siempre es T o N y devuelve ese char. Utilizamos find otra vez para
//encontrar el espacio y luego del espacio siempre esta el caracter que
//queremos, posteriormente se guarda ese valor con substring. 
char char_parse(string actual){
    if (actual.empty()) {
        return 0;
    }
    while(!actual.empty()){
        int found = actual.find(' ');
        string result = actual.substr(found+1);
        char state_now=result[0];
        return state_now;
    }
}


//El main se encarga de guardar y acomodar los valores que se metan
//de parametros en consola ademas de cambiarlos a integers. Tambien
//hace mascaras a partir de ellos, esto para extraer los bits menos
//significativos del PC cuando se necesiten (como por ejemplo para
//las tablas BHT, PHT).
int main(int argc, char * argv[]) {
    ofstream myfile;    //se usa esto para el archivo de salida
       
    //para entrada: "branch -s < # > -bp < # > -gh < # > -ph < # > -o < # >" se tiene que
    //cada # se reemplaza por 2, 4, 6, 8 y 10.   
    string s=argv[2]; //tamaño de la tabla
    int size= s[0]- '0';  //pasar string a int
    long size_l=s[0]- '0'; //string to long

    //calculamos el BHT size
    int BHT_size=pow(2,size)+0.5;

    string bp=argv[4]; //tipo de predicción

    string gh=argv[6]; //tamaño del registro global
    int size_G= gh[0]- '0';  //pasar string a int
    long size_G_l=gh[0]- '0'; //string to long
    
    string ph=argv[8]; //bits de historia
    int size_P=ph[0] - '0'; //pasar string a int
    long size_P_l=ph[0] - '0'; //string to long

    string o=argv[10]; //output type
    int size_O=o[0] - '0'; //pasar string a int
    
    string line;    //string donde se guarda la linea actual del file

    long pc;        //PC actual
    char state;     //T o Not Taken
    long index;     //Indice actual de la tabla BHT
    char row;       //Valor en la tabla T o N

    long mask=1;            //mascara 2n-1 para size en binario.
    for(int i=0;i<size-1;i++){        
        mask=mask<<1;
        mask++;
    }
    long mask_G=1;          //mascara 2n-1 para gh en binario.
    for(int i=0;i<size_G-1;i++){        
        mask_G=mask_G<<1;
        mask_G++;
    }    
        
    long mask_P=1;          //mascara 2n-1 para ph en binario.
    for(int i=0;i<size_P-1;i++){        
        mask_P=mask_P<<1;
        mask_P++;    
    }
  
    //variables extra para calcular las estadisticas del final
    int counter_branches=0;
    int counter_correct_T=0;
    int counter_correct_N=0;
    int counter_incorrect_T=0;
    int counter_incorrect_N=0;
    double correct_percent=0;
    string C_or_N;
    string pred_type; 

    //se utiliza bp para seleccionar el tipo de predictor con los numeros deseados.  
    //se hace uso de la clase BHT para crear el objeto BHT_Bimodal, se usan las funciones del mismo para realizar la tabla BHT que se actualiza con los valores de pc y state.
    if (o=="1"){
        myfile.open("stats.txt", ios::out);
        myfile << "PC           Outcome   Prediction   correct/incorrect"<<endl;
    }
    if (bp=="0"){
        pred_type="Bimodal";
        BHT BHT_Bimodal(size); //aqui se designa el objeto
        while (getline(cin, line)){
            //se llama la funcion num parse para obtener el valor de pc
            pc=num_parse(line);
            //se usa la funcion char_parse para obtener el T o N actual
            state=char_parse(line);
            index=pc&mask;  //se usa la mascara para sacar los LSB del PC actual, eso nos da el indice de la tabla BHT que queremos usar para predecir el branch. 
            row=BHT_Bimodal.current_predic(index);   //se guarda la prediccion en row 
            if (row==state) {
                C_or_N="correct"; 
                if (state=='T'){
                    counter_correct_T++;        
                }
                else{
                    counter_correct_N++;
                }; 
            }
            else {
                C_or_N="incorrect";
                if (state=='T'){
                    counter_incorrect_T++;
                }
                else{
                    counter_incorrect_N++;
                }                  
            }
            //logica del archivo de salida
            if (counter_branches<5000){
                myfile << pc <<"   "<<state<<"         "<<row<<"            "<<C_or_N<<endl; 
            }
            BHT_Bimodal.refresh_table(state, index);    //se manda el indice de la tabla y el valor actual de branch
            counter_branches++; 
        }
    }
    //predictor de prediccion global, funciona con la clase BHT y otra clase nueva llamada GShare la cual es un registro que guarda bits de historia segun el salto actual es decir segun el valor de T o N.
    //este registro se usa para obtener el puntero de la tabla BHT, para obtener este puntero se debe hacer una xor entre el PC actual y el valor del registro de historia actual. Se le debe aplicar una 
    //mascara de tamaño size a la xor para que apunte a la tabla correctamente.
    if (bp=="1"){
        pred_type="Historia Global";
        BHT BHT_Global(size);//se crea el objeto BHT
        GShare GShare_Global;//se crea el objeto GShare
        while (getline(cin, line)){
            pc=num_parse(line);         //PC actual
            state=char_parse(line);     //T o N
            long reg=GShare_Global.current_reg();   //se guarda el valor del registro de GShare actual
            long xor_temp=pc^reg;                   //se hace la xor 
            index=xor_temp&mask;                    //se sacan los LSB de la xor
            row=BHT_Global.current_predic(index);   //con los LSB de la xor se obtiene el valor de prediccion de la tabla actual.
            //ifs para contadores
            if (row==state) { 
                C_or_N="correct";                      
                if (state=='T'){
                    counter_correct_T++;
                }
                else{
                    counter_correct_N++;
                } 
            }
            else {
                C_or_N="incorrect";
                if (state=='T'){
                    counter_incorrect_T++;
                }
                else{
                    counter_incorrect_N++;
                }     
            }
            //logica del archivo de salida
            if (counter_branches<5000){
                myfile << pc <<"   "<<state<<"         "<<row<<"            "<<C_or_N<<endl; 
            }
            GShare_Global.refresh_reg(state, mask_G);   //obtiene los valores nuevos para el registro 
            BHT_Global.refresh_table(state, index);     //obtiene los valores nuevos para la tabla BHT    
            counter_branches++;         
        }
    }
    //Predictor de historia privada, es basicamente una expansion del predictor anterior nada mas que el registro es una tabla.               
    if (bp=="2"){
        pred_type="Historia Privada";
        BHT BHT_Privado(size);      //se crea el objeto BHT 
        PShare PShare_Privado(size);        //objeto PHT
        while (getline(cin, line)){
            pc=num_parse(line);         //PC actual
            state=char_parse(line);     //T o N  
            //se usa el pc para ubicar cual registro de la tabla PHT se va a usar junto al pc para sacar el resultado de la xor. 
            long pc_reduc=pc&mask;      //es necesario usar la mascara de size porque la tabla PHT es de tamaño 2^size. 
            long xor_temp=pc^PShare_Privado.current_reg(pc_reduc);  
            index=xor_temp&mask;        //ya con el resultado de la xor solo debemos reducirlo a solo los LSB para indexar la tabla BHT
            row=BHT_Privado.current_predic(index);      //se obtiene el valor de prediccion actual
            //contadores
            if (row==state) {
                C_or_N="correct"; 
                if (state=='T'){
                    counter_correct_T++;
                }
                else{
                    counter_correct_N++;
                } 
            }
            else {
                C_or_N="incorrect"; 
                if (state=='T'){
                    counter_incorrect_T++;
                }
                else{
                    counter_incorrect_N++;
                }     
            }
            //logica del archivo de salida
            if (counter_branches<5000){
                myfile << pc <<"   "<<state<<"         "<<row<<"            "<<C_or_N<<endl; 
            }

            PShare_Privado.refresh_reg(state,mask_P,pc_reduc);      //se actualiza la tabla PHT
            BHT_Privado.refresh_table(state, index);                //se actualiza la tabla BHT
            
            counter_branches++;
        }    
    }
    //este predictor es el de torneo, basicamente hay que inicializar y actualizar ambos predictores al mismo tiempo, y ademas se debe usar otra clase para 
    //escoger cual predictor se va a usar, este metapredictor es una tabla que se inicializa con valores de strongly prefer PShared y se actualiza de fila a fila.
    //Si ambos predictores tienen el valor correcto o incorrecto la preferencia de prediccion actual no cambia, pero si uno de los dos esta mal y el otro bien entonces
    //se cambia el estado en esa fila hacia el predictor preferido, esto puede ser de strong a weak, de weak a weak, o de weak a strong. 
    if (bp=="3"){
        pred_type="Torneo";
        //se construye un BHT para global y tambien el objeto GShare
        BHT BHT_Global(size);
        GShare GShare_Global;
        //se construye un BHT para privado y tambien el objeto PShare
        BHT BHT_Privado(size);
        PShare PShare_Privado(size);
        //se hace el objeto Torneo 
        Torneo Tourney(size);
        
        while (getline(cin, line)){
            //se hacen las variables que funcionan para ambos predictores primero por cuestiones de orden
            pc=num_parse(line);      
            state=char_parse(line);
            long pc_reduc=pc&mask;

            //primero se implementa el GShare
        
            long xor_temp_G=pc^GShare_Global.current_reg();
            long index_G=xor_temp_G&mask;
            char row_G=BHT_Global.current_predic(index_G);
            
            //ahora se implementa el PShare
            long xor_temp_P=pc^PShare_Privado.current_reg(pc_reduc);
            long index_P=xor_temp_P&mask;
            char row_P=BHT_Privado.current_predic(index_P);

            //ahora la logica de torneo
            char Current_predictor=Tourney.current_predic(pc_reduc);
            char status_G; 
            char status_P;

            //Esta es la logica de los contadores, en general solo se necesitan 4 ifs para determinar cual contador aumentar pero como son
            //dos predictores al mismo tiempo se necesitan 8 ifs para actualizarlos correctamente.                                    
            if(Current_predictor=='G'){
                //cout << row_G;
                if (row_G==state) {
                    C_or_N="correct"; 
                    row=row_G;//esto es necesario para el archivo de salida
                    status_G='G';
                    if (row_G=='T'){
                        counter_correct_T++;
                    }
                    else{
                        counter_correct_N++;
                    } 
                }
                else {
                    C_or_N="incorrect"; 
                    status_G='P';
                    row=row_G;
                    if (state=='T'){
                        counter_incorrect_T++;
                    }
                    else{
                        counter_incorrect_N++;
                    } 
                }
            }
            else if(Current_predictor=='P'){
                if (row_P==state) {
                    row=row_P;
                    C_or_N="correct"; 
                    status_P='P';
                    if (row_P=='T'){
                        counter_correct_T++;
                    }
                    else{
                        counter_correct_N++;
                    } 
                }
                else {
                    row=row_P;
                    C_or_N="incorrect"; 
                    status_P='G';
                    if (state=='T'){
                        counter_incorrect_T++;
                    }
                    else{
                        counter_incorrect_N++;
                    }  
                }
            }

            //esta logica nos permite saber cual es la preferencia actual, se tienen 2 estados G y P, dependiendo de sus valores se actualizara el torneo como se explico arriba
            if (row_G==state) {
                status_G='G'; 
            }
            else {
                status_G='P';
            }
            if (row_P==state) {         
                status_P='P'; 
            }
            else {
                status_P='G'; 
            }
            //logica del archivo de salida
            if (counter_branches<5000){
                myfile << pc <<"   "<<state<<"         "<<row<<"            "<<C_or_N<<endl; 
            }

            GShare_Global.refresh_reg(state, mask_G);  //se actualiza el GShare y su tabla BHT
            BHT_Global.refresh_table(state, index_G);

            PShare_Privado.refresh_reg(state,mask_P,pc_reduc);  //se actualiza el PShare y su tabla BHT
            BHT_Privado.refresh_table(state, index_P);

            //se actualiza el torneo solamente si s_G=G y s_P=G (es decir si el predictor G es correcto y el P incorrecto) o 
            //si s_G=P y s_P=P (es decir si el predictor G es incorrecto y el P correcto)            
            if (status_G=='G'&&status_P=='G'){
                Tourney.refresh_table(status_G,pc_reduc);
            }
            else if (status_G=='P'&&status_P=='P'){    
                Tourney.refresh_table(status_P,pc_reduc);
            }

            counter_branches++;
        }     
    }
    
    
    //Imprimir valores finales en consola
    cout<<"---------------------------------------------------------------------------"<<endl;
    cout<<"Prediction parameters:"<<endl;
    cout<<"---------------------------------------------------------------------------"<<endl;
    cout<<"Branch Prediction type:                           "<< pred_type <<endl;
    cout<<"BHT size (entries):                               "<< BHT_size <<endl;
    cout<<"Global history register size:                     "<< size_G <<endl;
    cout<<"Private history register size:                    "<< size_P <<endl;
    cout<<"---------------------------------------------------------------------------"<<endl;
    cout<<"Simulation Results:                               "<<endl;
    cout<<"---------------------------------------------------------------------------"<<endl;
    cout<<"Number of branches: "<<counter_branches<<endl;
    cout<<"Number of correct prediction of taken branches:   "<<counter_correct_T<<endl;
    cout<<"Number of incorrect prediction of taken branches: "<<counter_incorrect_T<<endl;   
    cout<<"Correct prediction of not taken branches:         " <<counter_correct_N<<endl;
    cout<<"Incorrect prediction of not taken branches:       "<<counter_incorrect_N<<endl;
    int temp=counter_correct_N+counter_correct_T;
    correct_percent=temp/(double)counter_branches;
    cout<<"Percentage of correct predictions:                "<<correct_percent<<endl;    
}


