#include <iostream>
#include <vector>
#include <string>
#include <fstream>//I/O
#include <sstream>//para convertir de un entero a string
#include <stdlib.h>
#include<thread>//para la concurrencia
#include <mutex>// esto es para la modificacion
///Integrantes:
///Erika Tamo Turpo
///David Tamo Turpo

using namespace std;


///////////////////////simulador de bloqueos
//mi funcion hashin
int clasificador(string identificador)
{
    int numero=0;
    for( auto spell : identificador)
    {
       numero=numero+ spell;
    }
    numero=numero%10;//dilema al cambiar
    //cout<<"el numero de hashing = "<<numero<<endl;
    return numero;
}


class transacciones{
public:

    transacciones(string id2,bool is_C2,string tipo_bloqueo1 )
    {
       this->id= id2;
       this->is_C= is_C2;///esta como bloqueo compartido
       this->is_wait =0;///por defecto no esta en espera
       this->tipo_bloqueo=tipo_bloqueo1;
    }
    void set_espera(bool espera){
    this->is_wait = espera; }
    string id;
    bool is_wait ;
    string tipo_bloqueo;
    bool is_C;

};


class element{
public:
    element(string dato2,bool is_C2 ,string transaccion_id ,string tipo_bloqueo1 )
    {this->dato = dato2;
     this->is_C = is_C2;
     transacciones nueva_transblo(transaccion_id,is_C2,tipo_bloqueo1);
     this->lista_transacciones.push_back(nueva_transblo);
    }
    bool push_element(bool is_C2 ,string transaccion_id,string tipo_bloqueo);
    vector<transacciones> lista_transacciones ;
    string dato;
    int is_C ;///va a hacer 1 variable que va a detectar en que bloqueo esta.
};


bool element::push_element(bool is_C2 ,string transaccion_id,string tipo_bloqueo)
{
    transacciones nueva_transblo(transaccion_id,is_C2,tipo_bloqueo);
    ///cambios segun la tabla de las diapositivas aca estoy dudanlo
    bool espera=false;
    if(this->is_C == 2)  //ver abajo
    {
        nueva_transblo.set_espera(true);
    }
    else{
        if(is_C2 && is_C)///la unica que funciona es de compartida a compartida
            this->is_C=1;
        else
        {//this->is_C=2; ///esto depende
            espera=true;
         nueva_transblo.set_espera(espera);
        }
    }
    lista_transacciones.push_back(nueva_transblo);
    return espera;
}

///clase principal
class fila_registro {
public:
    string identificador;
    vector<vector<element> > elemento ;
    fila_registro(string identificador2 )
    {
        this->identificador = identificador2 ;
        for(int i=0;i<11;i++){
            vector<element> nueva_lista;
            elemento.push_back(nueva_lista) ;
        }
    }
    bool solicitud_bloque(string dato ,string tipo_bloqueo,string Transaccion_id );
    void solicituddesbloqueo(string dato,string Transaccion_id );
    void imprime_transacciones();
    bool buscar(string dato, string transaccion_id);
};

void fila_registro::imprime_transacciones(){

    for(int i=0;i<elemento.size();i++)
    {
        int j=0;
        for(;j<elemento[i].size();j++)
        {
            int z=0;
            cout<<"dato "<<elemento[i][j].dato<<endl;
            for(;z<elemento[i][j].lista_transacciones.size() ;z++)
            {
                cout<<elemento[i][j].lista_transacciones[z].id<<"  "<<
                elemento[i][j].lista_transacciones[z].is_wait<<"  "<<
                elemento[i][j].lista_transacciones[z].tipo_bloqueo<<endl;

            }
        }
    }
}


bool fila_registro::buscar(string dato ,string transaccion_id)
{
   int k=clasificador(dato);
        int i=0;
        for(;i<elemento[k].size();i++)
        {
            int j=0;
            cout<<"dato "<<elemento[i][j].dato<<endl;
            for(;j<elemento[k][i].lista_transacciones.size() ;j++)
            {
                if(elemento[k][i].lista_transacciones[j].id==transaccion_id)
                    return elemento[k][i].lista_transacciones[j].is_wait;
            }
        }
}
bool fila_registro::solicitud_bloque(string dato ,string tipo_bloqueo,string Transaccion_id )
{
    int fila = clasificador(dato);
    int i,canti;
    canti=elemento[fila].size();
    bool wait_bloqueo=false;
    for(i=0;i<canti;i++)
    {
        if(elemento[fila][i].dato == dato)
        {
            break;
        }
    }

    if(i==canti)
    {
        element nuevo_elemento(dato,tipo_bloqueo=="S",Transaccion_id,tipo_bloqueo);
        elemento[fila].push_back(nuevo_elemento);
    }
    else
    {
        wait_bloqueo=(elemento[fila][i]).push_element(tipo_bloqueo=="S",Transaccion_id,tipo_bloqueo);
    }
    return wait_bloqueo;
}


void fila_registro::solicituddesbloqueo(string dato ,string Transaccion_id )
{
    int i;
    for(i=0; i<elemento[clasificador(dato)].size();i++)
    {
        int j=0;
        for(;j<elemento[clasificador(dato)][i].lista_transacciones.size();j++)
        {
            if(elemento[clasificador(dato)][i].lista_transacciones[j].id==Transaccion_id)
            {
            (elemento[clasificador(dato)][i].lista_transacciones).erase((elemento[clasificador(dato)][i].lista_transacciones).begin()+j);
             int canti=elemento[clasificador(dato)][i].lista_transacciones.size();
             if(canti==1)
             {elemento[clasificador(dato)][i].lista_transacciones[0].is_wait=0;
              elemento[clasificador(dato)][i].is_C=elemento[clasificador(dato)][i].lista_transacciones[0].is_C;
              return;
             }

             if(elemento[clasificador(dato)][i].is_C=1)
             {
              int s=0;
             for(int k=0;k<canti;k++)
             {
                if(elemento[clasificador(dato)][i].lista_transacciones[k].tipo_bloqueo == "X")
                    s++;
             }
             if(s==canti)
             {
              elemento[clasificador(dato)][i].lista_transacciones[0].is_wait=0;
              elemento[clasificador(dato)][i].is_C=elemento[clasificador(dato)][i].lista_transacciones[0].is_C;
             }
             return;
             }
            }

        }
    }
}


typedef float tipo_dato;
//////////////////////////////////// simulador de transacciones
string datofile_A="datos_A.txt";
string datofile_B="datos_B.txt";
fila_registro gestor_bloqueos("Gestor de bloqueos");
//template <typename T>
void mostrar(vector<tipo_dato> datoA,vector<tipo_dato> datoB)
{
    int answer_sum = 0;
    for(tipo_dato num : datoA)
    {
        answer_sum=answer_sum+num;
    }
    for(tipo_dato num : datoB)
    {
        answer_sum=answer_sum+num;
    }
    cout<<"La suma entre A y B ="<<answer_sum<<endl;
}

void mostrar(vector<tipo_dato> datoA)
{
    for(tipo_dato num : datoA)
    {
        cout<<num<<" ";
    }
}

void read(string name_file,vector<tipo_dato> & dato_A )
{
    ifstream fichero1;
    fichero1.open(name_file.c_str() );
    while(!fichero1.eof() )
    {
        string linea;
        string strnum="";
        getline(fichero1, linea);
        for( auto num : linea)
        {
            if(num !=' ')
            strnum=strnum+num;
            else
           {
               dato_A.push_back(atof(strnum.c_str()) ) ;//tiene que ser de tipo_dato
               strnum="";
           }
        }
    }
    cout<<"lectura"<<name_file<<endl;
    mostrar(dato_A);
    cout<<"termino la lectura satifactoriamente"<<endl;
    fichero1.close();
}

void write(string name_file,vector<tipo_dato> & dato_A)
{
    string numstr;
    string resultado;
    ofstream writefile;
    writefile.open(name_file.c_str(),ios::out);
    static mutex m;

    for(tipo_dato num : dato_A)
    {
        m.lock();
        numstr= static_cast<std::ostringstream*>(&(std::ostringstream() << num))->str();
        resultado = resultado+numstr+" ";
        m.unlock();
    }
    writefile<<resultado;
    writefile.close();
    cout<<"la escritura fue hecha bien";
}

void T1_instrucc_01(vector<tipo_dato> & dato_B)
{
    read(datofile_B,dato_B);
    static mutex m;
    for(int i=0;i<dato_B.size();i++)
    {
        m.lock();
        dato_B[i]=dato_B[i]-50;
        m.unlock();
    }
    write(datofile_B,dato_B);
}

void T2_instrucc_03(vector<tipo_dato> & dato_B,vector<tipo_dato> & dato_A)
{
    mostrar(dato_A,dato_B);//internamente se hace la suma
}

void T2_instrucc_02(vector<tipo_dato> & dato_B)
{
    read(datofile_B,dato_B);
}

void T1_instrucc_02(vector<tipo_dato> & dato_A  )
{
    read(datofile_A,dato_A);
    for(int i=0;i<dato_A.size();i++)
    {
        dato_A[i]=dato_A[i]-50;
    }
    write(datofile_A,dato_A);
}


void T2_instrucc_01(vector<tipo_dato> & dato_A,vector<tipo_dato> & dato_B )
{
    read(datofile_A,dato_A);
     gestor_bloqueos.solicituddesbloqueo("A","T2");
     while(!gestor_bloqueos.buscar("A","T2"))
     {
         cout<<" ";
     }
     dato_A.clear();
     dato_B.clear();
     if(gestor_bloqueos.solicitud_bloque("A","X","T1"))
      { vector<tipo_dato> dato_A2;
        thread t3{bind(T1_instrucc_02,dato_A2)};//T1_instrucc_02(A);
        gestor_bloqueos.solicituddesbloqueo("A","T1");
        t3.join();
      }
     gestor_bloqueos.solicitud_bloque("B","S","T2");
     T2_instrucc_02(dato_B);
     gestor_bloqueos.solicituddesbloqueo("B","T2");
     T2_instrucc_03(dato_B,dato_A);
}



int main()
{
    //tengo que implementar deberas
    vector<tipo_dato> A;
    vector<tipo_dato> B;
    //read("datos_A.txt",A);
    //write("datos_A.txt",A);
    cout<<"Primer grupo de instrucciones"<<endl;
    if( gestor_bloqueos.solicitud_bloque("B","X","T1") )
    {
        thread t1{bind(T1_instrucc_01,B)}; //T1_instrucc_01(A);
        if( gestor_bloqueos.solicitud_bloque("A","S","T2") ){
         thread t2{bind(T2_instrucc_01,A,B)}; // T2_instrucc_01(A);
         t2.join();
        }

        t1.join();
    }
    gestor_bloqueos.solicituddesbloqueo("B","T1");
    cout<<"Segundo grupo de instrucciones"<<endl;

    A.clear();
    B.clear();
    cout<<"Tercera grupo de instrucciones"<<endl;


    gestor_bloqueos.imprime_transacciones();

    /*
    //Anterior programa
    cout<<"Las solicitudes "<<endl;
    cout<<"Las transacciones  y el estado(esperando 1, no esperando 0)"<<endl;
    cout<<"T | E | Modo "<<endl;
    gestor_bloqueos.solicitud_bloque("A","S","T1");
    gestor_bloqueos.solicitud_bloque("A","X","T2");
    gestor_bloqueos.solicitud_bloque("A","S","T3");
    gestor_bloqueos.solicitud_bloque("A","S","T4");

    gestor_bloqueos.solicitud_bloque("B","X","T4");
    gestor_bloqueos.solicitud_bloque("B","X","T1");

    gestor_bloqueos.imprime_transacciones();
    cout<<endl<<"Desbloqueos !! "<<endl<<endl;
    gestor_bloqueos.solicituddesbloqueo("B","T4");
    gestor_bloqueos.solicituddesbloqueo("A","T1");
    gestor_bloqueos.solicituddesbloqueo("A","T3");
    gestor_bloqueos.solicituddesbloqueo("A","T4");
    gestor_bloqueos.imprime_transacciones();*/
    cout<<"muchas cosas por hacer tperika"<<endl;
    //gestor_bloques.imprime_transacciones();
    cout << "Hello world!" << endl;
    return 0;
}
