#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <direct.h> //mkdir
#include <cstring>
#include <string>
#include <cstdlib>
#include <unordered_map>
#include <list>
#include <algorithm>
using namespace std;

class LRUBufferManager {
private:
    int capacity; // capacidad del buffer pool
    unordered_map<int, list<int>::iterator> buffer; // Mapeo para almacenar las páginas en el buffer pool
    list<int> lruList; // Lista para realizar el seguimiento de la LRU

public:
    LRUBufferManager(int capacity) : capacity(capacity) {}

    void refer(int page) {
        // Verificar si la página ya está en el buffer pool
        if (buffer.find(page) == buffer.end()) {
            // La página no está en el buffer pool
            // Verificar si el buffer pool está lleno
            if (buffer.size() == capacity) {
                // El buffer pool está lleno, se debe reemplazar una página

                int lruPage = lruList.front(); // Obtener la página menos reciente utilizada (LRU)

                // Eliminar la página menos reciente utilizada del buffer pool
                buffer.erase(lruPage);
                lruList.pop_front();

                cout << "Reemplazar página LRU: " << lruPage << " con página: " << page << endl;
            }
        } else {
            // La página ya está en el buffer pool, se mueve al final de la lista LRU
            lruList.erase(buffer[page]);
        }

        // Agregar la página al final de la lista LRU y actualizar el mapa
        lruList.push_back(page);
        buffer[page] = prev(lruList.end());
    }
    void printBuffer() {
        cout << "LRU columna: ";
        for (int page : lruList) {
            cout << page << " ";
        }
        cout << endl;
    }
};
class MRUBufferManager {
private:
    int capacity; // capacidad del buffer pool
    unordered_map<int, bool> buffer; // Mapeo para almacenar las páginas en el buffer pool
    vector<int> mruStack; // Pila para realizar el seguimiento de la MRU

public:
    MRUBufferManager(int capacity) : capacity(capacity) {}

    void refer(int page) {
        // Verificar si la página ya está en el buffer pool
        if (buffer.find(page) == buffer.end()) {
            // La página no está en el buffer pool

            // Verificar si el buffer pool está lleno
            if (buffer.size() == capacity) {
                // El buffer pool está lleno, se debe reemplazar una página

                int mruPage = mruStack.back(); // Obtener la página más reciente utilizada (MRU)

                // Eliminar la página más reciente utilizada del buffer pool
                buffer.erase(mruPage);
                mruStack.pop_back();

                cout << "Reemplazar página MRU: " << mruPage << " con página: " << page << endl;
            }
        }

        // Agregar la página al principio de la pila MRU y actualizar el mapa
        mruStack.push_back(page);
        buffer[page] = true;
    }
    void printBuffer() {
        cout << "MRU columna: ";
        for (int i = mruStack.size() - 1; i >= 0; i--) {
            cout << mruStack[i] << " ";
        }
        cout << endl;
    }
};

class ClockBufferManager {
private:
    int capacity; // Capacidad del buffer pool
    unordered_map<int, pair<bool, bool>> buffer; // Mapa para almacenar las páginas en el buffer pool
    vector<int> clock; // Lista de páginas en el buffer pool en orden circular
    int hand; // Posición actual del puntero del algoritmo CLOCK

public:
    ClockBufferManager(int capacity) : capacity(capacity), hand(0) {
        clock.resize(capacity, -1); // Inicializar la lista con páginas vacías
    }

    void refer(int page) {
        // Verificar si la página ya está en el buffer pool
        if (buffer.find(page) != buffer.end()) {
            buffer[page].first = true; // Establecer el bit de referencia
        } else {
            // La página no está en el buffer pool

            // Verificar si el buffer pool está lleno
            if (buffer.size() == capacity) {
                // El buffer pool está lleno, se debe reemplazar una página

                while (true) {
                    int currentPage = clock[hand]; // Obtener la página actual

                    if (buffer[currentPage].first) {
                        // La página tiene el bit de referencia establecido
                        buffer[currentPage].first = false; // Resetear el bit de referencia
                    } else {
                        // La página no tiene el bit de referencia establecido

                        buffer.erase(currentPage); // Eliminar la página del buffer pool
                        buffer[page] = make_pair(true, true); // Agregar la nueva página al buffer pool

                        clock[hand] = page; // Actualizar la página en la posición actual del puntero
                        hand = (hand + 1) % capacity; // Mover el puntero al siguiente

                        cout<<"Reemplazar página " << currentPage << " con página: " << page << endl;
                        break;
                    }

                    hand = (hand + 1) % capacity; // Mover el puntero al siguiente
                }
            } else {
                // El buffer pool aún tiene capacidad disponible

                buffer[page] = make_pair(true, true); // Agregar la página al buffer pool

                clock[hand] = page; // Actualizar la página en la posición actual del puntero
                hand = (hand + 1) % capacity; // Mover el puntero al siguiente
            }
        }
    }
    void printBuffer() {
        cout << "Clock columna: ";
        int start = hand;
        while (true) {
            cout << clock[start] << " ";
            start = (start + 1) % capacity;
            if (start == hand)
                break;
        }
        cout << endl;
    }
};

class Nodo {
public:
    char* dato;
    Nodo* siguiente;

    Nodo(const char* dato) {
        this->dato = new char[strlen(dato) + 1];
        strcpy(this->dato, dato);
        this->siguiente = nullptr;
    }

    ~Nodo() {
        delete[] dato;
    }
};

class ListaEnlazada {
private:
    Nodo* cabeza;

public:
    ListaEnlazada() {
        cabeza = nullptr;
    }

    void insertar(const char* dato) {
        string vaciostr = "vacio";
        const char* vacio = vaciostr.c_str();

        Nodo* nuevoNodo = new Nodo(dato);

        if (cabeza == nullptr) {
            cabeza = nuevoNodo;
        } else {
            Nodo* temp = cabeza;
            while (temp->siguiente != nullptr) {
                if(strcmp(temp->dato,vacio)==0)
                {
                    strcpy(temp->dato, dato);
                    return;
                }
                temp = temp->siguiente;
            }
            temp->siguiente = nuevoNodo;
        }
    }

    void eliminar(const char* data) {
        cout<<data<<endl;
        if (cabeza == nullptr) {
            return;
        }
        string vaciostr = "vacio";
        const char* vacio = vaciostr.c_str();
        if (strcmp(cabeza->dato, data) == 0) {;
            cout<<"entro aqui"<<endl;
            strcpy(cabeza->dato, vacio);
            return;
        }

        Nodo* actual = cabeza;

        while (actual != nullptr && strcmp(actual->dato, data) != 0) {
            //prev = actual;
            actual = actual->siguiente;
        }

        if (actual == nullptr) {
            return;
        }
        strcpy(actual->dato, vacio);
    }
    void eliminar_nodo(const char* dato){
        if (cabeza == nullptr) {
            return;
        }

        if (strcmp(cabeza->dato, dato) == 0) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            return;
        }

        Nodo* prev = nullptr;
        Nodo* actual = cabeza;

        while (actual != nullptr && strcmp(actual->dato, dato) != 0) {
            prev = actual;
            actual = actual->siguiente;
        }

        if (actual == nullptr) {
            return;
        }

        prev->siguiente = actual->siguiente;
        delete actual;
    }


    void moverregistro(int pos_i, int pos_f){
        Nodo *tmp = cabeza;
        int contar = 1;
        while(true){
            if(contar==pos_i){
                break;
            }
            contar++;
            tmp = tmp->siguiente;
        }

        Nodo* temp = cabeza;
        int contador = 1;
        eliminar_nodo(tmp->dato);
        while(temp != nullptr){
            if(contador == pos_f){
                strcpy(temp->dato,tmp->dato);
                break;
            }
            contador++;
            temp = temp->siguiente;
        }
        if(temp==nullptr){
            insertar(tmp->dato);
        }
    }
    void mostrar() {
        Nodo* temp = cabeza;
        while (temp != nullptr) {
            std::cout << temp->dato << endl;
            temp = temp->siguiente;
        }
        std::cout << std::endl;
    }

    bool find(int lim){
        Nodo *temp = cabeza;
        int cont =1;
        while(temp != nullptr){
            if(cont==lim){
                return true;
            }
            cont++;
        }
        return false;
    }
    void guardar_archivo(string aux){
        ofstream archivo(aux,ios::out);
        Nodo* temp = cabeza;
        while (temp != nullptr) {
            archivo<<temp->dato<<endl;
            temp = temp->siguiente;
        }
        archivo.close();
    }
};



class Disco{
private:
    //dentro de los atributos se tendra
    string name; //el nombre del disco
    int platos;
    int superficies = 2; //la cantidad de superficies, pistas, sectores y registros por sector
    int pistas;
    int sectores;
    int registros;
    int tamanio_registros = 0; //tamañio de cada registros

public:
    Disco(){}
    Disco(string name_){ name = name_;}
    void setName(string nombre_){name = nombre_; }
    string getName(){ return name; }
    int getplatos(){ return platos; }
    void setplatos(int p){ platos = p; }
    int getsuperficies(){ return superficies; }
    void setsuperficies(int s){ superficies = s; }
    int getpistas(){ return pistas; }
    void setpistas(int p){ pistas = p; }
    int getsectores(){ return sectores; }
    void setsectores(int s){ sectores = s; }
    int getregistros(){ return registros; }
    void setregistros(int r){ registros = r; }
    int gettamanio_registros(){ return tamanio_registros; }
    void settamanio_registros(int r){ tamanio_registros = r; }

    //la funcion de crear se utiliza para la creacion de carpetas dentro de la ejecucion
    //se utiliza el Directorio direct.h para crear carpetas con mkdir
    void crear(){
        string aux=name;
        //primero las superficies
        if (mkdir(aux.c_str()) == 0) cout << "Carpeta creada correctamente" << endl;
        else cout << "Ha ocurrido un error al crear la carpeta nombre" << endl;
        for(int u=1;u<=platos;u++){
            string aux_;
            aux_ = aux +"//"+to_string(u);

            if (mkdir(aux_.c_str()) == 0) cout << "Carpeta creada correctamente" << endl;
            else cout << "Ha ocurrido un error al crear la carpeta plato" << endl;
            for(int i=1;i<=superficies;i++){
                string aux1;
                aux1 = aux_ + "//" + to_string(i);
                //luego las pistas
                if (mkdir(aux1.c_str()) == 0) cout << "Carpeta creada correctamente" << endl;
                else cout << "Ha ocurrido un error al crear la carpeta superficie" << endl;
                for(int j=1;j<=pistas;j++){
                    string aux2;
                    aux2 = aux1+"//"+to_string(j);
                    //y ultimo en sectores
                    if (mkdir(aux2.c_str()) == 0) cout << "Carpeta creada correctamente" << endl;
                    else cout << "Ha ocurrido un error al crear la carpeta pista" << endl;
                    for(int k=1;k<=sectores;k++){
                        string chares = aux2+"//"+to_string(k)+".txt";
                        ofstream arch(chares);
                        arch.close();
                        //funcion para crear txt que simularan los sectores de un disco
                    }
                }
            }
        }
    }
    void imprimir_disco(){
        for(int l=1;l<=platos;l++){
            for(int k=1;k<=superficies;k++){
                for(int j=1;j<=pistas;j++){
                    for(int i=1;i<=sectores;i++){
                        string sector_= name+"//"+to_string(l)+"//"+to_string(k)+"//"+to_string(j)+"//"+to_string(i)+".txt"; // se abre el archivo.txt con esta direccion
                        ifstream archivo(sector_);
                        string linea;
                        cout<<"Direccion: "<<sector_<<endl;
                        while(getline(archivo,linea)){
                            cout<<linea<<endl;
                        }
                        cout<<"\n\n";
                    }
                }
            }
        }
        mostrar_informacion();
        cout<<"Tamanio en bytes del disco: "<<tamanio_sector()*sectores*pistas*superficies*platos<<endl;
    }
    void imprimir_plato(){
        int plato;
        cout<<"plato: ";cin>>plato;

        for(int k=1;k<=superficies;k++){
            for(int j=1;j<=pistas;j++){
                for(int i=1;i<=sectores;i++){
                    string sector_= name+"//"+to_string(plato)+"//"+to_string(k)+"//"+to_string(j)+"//"+to_string(i)+".txt"; // se abre el archivo.txt con esta direccion
                    ifstream archivo(sector_);
                    string linea;
                    cout<<"Direccion: "<<sector_<<endl;
                    while(getline(archivo,linea)){
                        cout<<linea<<endl;
                    }
                    cout<<"\n\n";
                }
            }
        }
        cout<<"Tamanio de pista "<<tamanio_sector()*sectores*pistas*superficies<<endl;
    }
    void imprimir_superficie(){
        int array[2];
        cout<<"plato: ";cin>>array[1];
        cout<<"superficie: ";cin>>array[0];
        for(int j=1;j<=pistas;j++){
            for(int i=1;i<=sectores;i++){
                string sector_= name+"//"+to_string(array[1])+"//"+to_string(array[0])+"//"+to_string(j)+"//"+to_string(i)+".txt"; // se abre el archivo.txt con esta direccion
                ifstream archivo(sector_);
                string linea;
                cout<<"Direccion: "<<sector_<<endl;
                while(getline(archivo,linea)){
                    cout<<linea<<endl;
                }
                cout<<"\n\n";
            }
        }
        cout<<"Tamanio de pista "<<tamanio_sector()*sectores*pistas;
    }
    void imprimir_pista(){
        int array[3];
        cout<<"plato: ";cin>>array[2];
        cout<<"superficie: ";cin>>array[0];
        cout<<"pista: ";cin>>array[1];
        for(int i=1;i<=sectores;i++){
            string sector_= name+"//"+to_string(array[2])+"//"+to_string(array[0])+"//"+to_string(array[1])+"//"+to_string(i)+".txt"; // se abre el archivo.txt con esta direccion
            ifstream archivo(sector_);
            string linea;
            cout<<"Direccion: "<<sector_<<endl;
            while(getline(archivo,linea)){
                cout<<linea<<endl;
            }
            cout<<"\n\n";
        }
        cout<<"Tamanio de pista "<<tamanio_sector()*sectores;
    }
    void imprimir_sector(){
        int array[4];
        //Para la impresion de un sector es necesario datos como
        //la superficie, pista, sector donde se ubica
        cout<<"plato: ";cin>>array[3];
        cout<<"superficie: ";cin>>array[0];
        cout<<"pista: ";cin>>array[1];
        cout<<"sector: ";cin>>array[2];
        string sector_= name+"//"+to_string(array[3])+"//"+to_string(array[0])+"//"+to_string(array[1])+"//"+to_string(array[2])+".txt"; // se abre el archivo.txt con esta direccion
        ifstream archivo(sector_);
        string linea;
        //Se imprime la informacion y el tamañio del sector
        cout<<"Informacion: "<<sector_<<endl;
        cout<<"Tamanio del sector es: "<<tamanio_sector()<<endl;
        while(getline(archivo,linea)){
            cout<<linea<<endl;
        }
    }

    void imprimir_registro(){
        int num,tamanio;
        cout<<"Disco: "<<superficies<<" "<<pistas<<" "<<sectores<<" "<<registros<<endl;
        cout<<"numero de registro: ";cin>>num; //se pide el numero de registro a imprimir
        tamanio = superficies*pistas*sectores*registros; //se multiplica para saber donde se ubica
        cout<<"tamanio del disco: "<<tamanio<<endl;
        int array[5];
        int contador1=1;
        //en las siguientes lineas se buscara extraer la direccion del registro
        if(num<=tamanio){
            for(int u=1;u<=platos;u++){
                for(int i=1;i<=superficies;i++){
                    for(int j=1;j<=pistas;j++){
                        for(int k=1;k<=sectores;k++){
                            for(int l=1;l<=registros;l++){
                                if(contador1==num){
                                    array[0]=i;
                                    array[1]=j;
                                    array[2]=k;
                                    array[3]=l;
                                    array[4]=u;
                                }
                                contador1++;
                            }
                        }
                    }
                }
            }
            // se abre el archivo
            ifstream archivo(name+"//"+to_string(array[4])+"//"+to_string(array[0])+"//"+to_string(array[1])+"//"+to_string(array[2])+".txt");
            int con = 1;
            string linea;
            //se recorre hasta encontrarlo para luego salir del bucle
            while(true){
                getline(archivo,linea);
                if(con==array[3]){
                    cout<<linea<<endl;
                    break;
                }
                con++;
            }
        }else{
            cout<<"EL registro supera el limite del disco"<<endl;
        }
    }

    void insertar_datos(){
        //Imprimimos informacion del disco
        cout<<"Disco:\nSuperficies: "<<superficies<<"\npistas: "<<pistas<<"\nSectores: "<<sectores<<"\nRegistros:  "<<registros<<endl;

        int a;
        //Llamamos al metodo para generar carpetas
        crear();
        //leemos el archivo titanic para insertar
        //cout<<"Nombre de la tabla: ";getline(cin,aux);
        ifstream archivo("titanic.txt");
        string line;

        //Mediante el algoritmo insertamos los datos
        int ite=1,ite1=1,ite2=1,ite3=1,ite4=1;
        cout<<"INICIO LA INSERCION."<<endl;
        do{
            if(ite<=platos){
                if(ite1<=superficies){
                    if(ite2<=pistas){
                        if(ite3<=sectores){
                            if(ite4<=registros){
                                //Se guarda el registro en una variable
                                getline(archivo,line);
                                //Primero se abre uno en modo escritura que es el sector donde se guardara registros
                                ofstream archis(name+"//"+to_string(ite)+"//"+to_string(ite1)+"//"+to_string(ite2)+"//"+to_string(ite3)+".txt",ios::app);
                                if(ite4<=1){
                                    //La direccion se guarda en un Directorio
                                    //para luego solicitarlo en caso de los bloques
                                    ofstream direct("Directorio//Directorio_"+name+".txt",ios::app);
                                    direct<<name+"//"+to_string(ite)+"//"+to_string(ite1)+"//"+to_string(ite2)+"//"+to_string(ite3)+".txt"<<endl;
                                    direct.close();
                                }
                                //se escribe la linea de registro en el sector
                                archis<<line<<endl;
                                archis.close();
                                ite4++;
                            }else{
                                ite4=1;
                                ite3++;
                            }
                        }else{
                            ite4=1;
                            ite3=1;
                            ite2++;
                        }
                    }else{
                        ite4=1;
                        ite3=1;
                        ite2=1;
                        ite1++;
                    }
                }else{
                    ite4=1;
                    ite3=1;
                    ite2=1;
                    ite1=1;
                    ite++;
                }
            }else{
                cout<<"Disco lleno";
            }

        }while(line.size()!=0);

    }

    void imprimir_bloque(){
        //Para imprimir el bloque se usara el Directorio
        int tamanio, N_bloque;
        //se pide el numero de sectores que contiene el bloque
        cout<<"Numero de sectores: "<<superficies*pistas*sectores<<endl;
        cout<<"tamanio de bloque(numero de sectores): ";cin>>tamanio;
        //Luego el numero de bloque a imprimir
        cout<<"Numero de bloque: ";cin>>N_bloque;
        //Se abre el directorio con las direcciones de cada sector
        ifstream archivo("Directorio//Directorio_"+name+".txt");
        string line;
        //luego recorremos hasta obtener el bloque
        for(int i=0;i<N_bloque;i++){
            for(int j=1;j<=tamanio;j++){
                if(!archivo.eof()){
                    getline(archivo,line);
                    if(i==N_bloque-1){
                        //Y se imprime la informacion del mismo
                        //con los sectores que contiene
                        cout<<"Metadata: "<<line<<endl;

                        ifstream arch(line);
                        string aux;
                        while(getline(arch,aux)){
                            cout<<aux<<endl;
                        }
                        cout<<"\n";
                    }
                }
            }
        }
        cout<<"Tamanio del bloque: "<<tamanio*registros*tamanio_registros<<endl;
    }
    void insertar_lista(ListaEnlazada &lista,string direc){
        ifstream archivo(direc);
        if (!archivo) {
            std::cout << "No se pudo abrir el archivo." << std::endl;
            return;
        }
        string cadena;
        int cont = 0;
        while(getline(archivo,cadena)){
            const char* textoChar = cadena.c_str();
            lista.insertar(textoChar);
        }
        archivo.close();
    }

    void mover_registro(ListaEnlazada &lista){
        int n;
        cout<<"Numero de registro a mover(n): ";cin>>n;
        if(!lista.find(n)){
            cout<<"Registro no encontrado"<<endl;
            return;
        }else{
            int nro2;
            cout<<"Nro de registro a sobreescribir: ";cin>>nro2;
            lista.moverregistro(n,nro2);
        }
    }

    void eliminar_re(ListaEnlazada &lista,string direc){
        ifstream archivo1(direc);
        int nro;
        string cadex;
        cout<<"Nro de registro a eliminar: ";cin>>nro;
        cout<<"\n\n---Menu---"<<endl;
        for(int i=0;i<nro;i++){
            getline(archivo1,cadex);
            cout<<cadex<<endl;
        }
        const char* textoChar = cadex.c_str();
        lista.eliminar(textoChar);
    }
    void menu_IEM(){
        int array[4];
        //Para la impresion de un sector es necesario datos como
        //la superficie, pista, sector donde se ubica
        cout<<"plato: ";cin>>array[3];
        cout<<"superficie: ";cin>>array[0];
        cout<<"pista: ";cin>>array[1];
        cout<<"sector: ";cin>>array[2];
        string sector_= name+"//"+to_string(array[3])+"//"+to_string(array[0])+"//"+to_string(array[1])+"//"+to_string(array[2])+".txt"; // se abre el archivo.txt con esta direccion

        int opc;
        ListaEnlazada lista;
        insertar_lista(lista,sector_);

        while(true){
            cout<<"\nMenu:\n1.Insertar\n2.Eliminar\n3.Mover\n4.Mostrar\n5.Actualizar sector\n6.Salir\nOpcion: ";cin>>opc;
            cin.ignore();
            if(opc==1){
                string aux;
                cout<<"Registro: ";getline(cin,aux);
                const char* textoChar = aux.c_str();
                lista.insertar(textoChar);
            }else if(opc==2){
                eliminar_re(lis ta,sector_);
            }else if(opc==3){
                mover_registro(lista);
            }else if(opc==4){
                lista.mostrar();
            }else if(opc==5){
                lista.guardar_archivo(sector_);
            }else if(opc==6){
                break;
            }else{
                 cout<<"Opcion incorrecta."<<endl;
            }
        }
    }
    /*
    int accessPattern[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    int n = sizeof(accessPattern) / sizeof(accessPattern[0]);
    for (int i = 0; i < n; i++) {
    lruBufferManager.refer(accessPattern[i]);
    mruBufferManager.refer(accessPattern[i]);
    clockBufferManager.refer(accessPattern[i]);
    }
    */

    void Algoritmo_LRU(){
        int n,capacity,N_bloque;
        cout<<"Numero de sectores en un bloque: ";cin>>n;
        cout<<"Capacidad del Buffer manager: ";cin>>capacity;
        LRUBufferManager lruBufferManager(capacity);
        while(true){
            cout<<"Numero de bloque a ingresar en el buffer manager: ";cin>>N_bloque;
            if(N_bloque<1){
                break;
            }
            lruBufferManager.refer(N_bloque);
            lruBufferManager.printBuffer();
        }
    }

    void Algoritmo_MRU(){
        int n,capacity,N_bloque;
        cout<<"Numero de sectores en un bloque: ";cin>>n;
        cout<<"Capacidad del Buffer manager: ";cin>>capacity;
        //int accessPattern[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
        MRUBufferManager mruBufferManager(capacity);
        while(true){
            cout<<"Numero de bloque a ingresar en el buffer manager: ";cin>>N_bloque;
            if(N_bloque<1){
                break;
            }
            mruBufferManager.refer(N_bloque);
            mruBufferManager.printBuffer();
        }
    }

    void Algoritmo_CLOCK(){
        int n,capacity,N_bloque;
        cout<<"Numero de sectores en un bloque: ";cin>>n;
        cout<<"Capacidad del Buffer manager: ";cin>>capacity;
        //int accessPattern[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
        ClockBufferManager clockBufferManager(capacity);
        while(true){
            cout<<"Numero de bloque a ingresar en el buffer manager: ";cin>>N_bloque;
            if(N_bloque<1){
                break;
            }
            clockBufferManager.refer(N_bloque);
            clockBufferManager.printBuffer();
        }
    }


    void BufferManager(){
        int opc;
        cout<<"\nEstrategia a usar:\n1.LRU\n2.MRU\n3.CLOCK\nOpcion:";cin>>opc;
        if(opc==1){
            Algoritmo_LRU();
        }else if(opc==2){
            Algoritmo_MRU();
        }else if(opc==3){
            Algoritmo_CLOCK();
        }else{
            BufferManager();
        }
    }
    void Esquema(string tabla){
        ifstream archivo(tabla);
        string line;
        string esquemaLine = tabla.substr(0,tabla.find("."));
        getline(archivo,line);
        int pos=0,cont;
        for(int i=0;i!=line.size();i++){
            if(line[i]==','){
                string aux;
                cout<<"Tipo de variable de "<<line.substr(pos,i-pos)<<": ";getline(cin,aux);
                if(aux=="int64"){
                    cont+=4;
                }
            }
        }
    }
    int Esquema_leer(){
        string nombre;
        //Para manipular un disco que ya existe
        //se abre un txt que tiene al disco y se busca por el nombre
        cout<<"Nombre de la base de datos: ";getline(cin,nombre);
        ifstream archivo("esquema.txt");
        string line,aux;
        //la informacion de cada disco(nombre,superficies,etc)
        while(!archivo.eof()){
            getline(archivo,line);
            if(nombre==line.substr(0,line.find("#"))){ //se divide por "/"
                aux=line;
                break;
            }
        }
        archivo.close();
        if(line.size()!=0){
            line.erase(0,line.find("#")+1);

            int cont =0, tam=line.size();
            string data;
            while(line.size()>0){
                data = line.substr(line.find("|")+1,line.find("#")-line.find("|")-1);
                cout<<data<<endl;
                if(data=="int64"){
                    cont +=4;
                }else if(data=="float64"){
                    cont +=4;
                }else{
                    string a = data.substr(data.find("(")+1,data.find(")")-data.find("(")-1);
                    cont += stoi(a);
                }
                line.erase(0,line.find("#")+1);

            }
            return cont;
        }
        return 1;
    }
    void mostrar_informacion(){
        cout<<"Informacion del disco: "<<endl;
        cout<<"Platos: "<<platos<<endl;
        cout<<"Pistas: "<<pistas<<endl;
        cout<<"sectores: "<<sectores<<endl;
        cout<<"Nro de registros por sector: "<<registros<<endl;
        cout<<"Nro de bytes por sector: "<<registros*tamanio_registros<<endl;
    }
    void menu(){
        //Menu para el disco
        int opc;
        string aux;
        if(tamanio_registros==0){
            tamanio_registros=Esquema_leer();
        }
        cout<<"\nBienvenido al Menu de Disco\n1. insertar datos: "<<endl;
        cout<<"2. Imprimir sector. "<<endl;
        cout<<"3. imprimir Registro. "<<endl;
        cout<<"4. imprimir tamanio disco."<<endl;
        cout<<"5. Imprimir bloque. "<<endl;
        cout<<"6. Modificar sector. "<<endl;
        cout<<"7. Buffer Manager."<<endl;
        cout<<"8. imprimir pista."<<endl;
        cout<<"9. imprimir superficie."<<endl;
        cout<<"10. imprimir plato."<<endl;
        cout<<"11. imprimir disco."<<endl;
        cout<<"12. mostrar informacion."<<endl;
        cout<<"Opcion: ";cin>>opc;
        cin.ignore();
        if(opc==1){
            insertar_datos();
        }else if(opc==2){
            imprimir_sector();
        }else if(opc==3){
            imprimir_registro();
        }else if(opc==4){
            cout<<tamanio()<<endl;
        }else if(opc==5){
            imprimir_bloque();
        }else if(opc==6){
            menu_IEM();
        }else if(opc==7){
            BufferManager();
        }else if(opc==8){
            imprimir_pista();
        }else if(opc==9){
            imprimir_superficie();
        }else if(opc==10){
            imprimir_plato();
        }else if(opc==11){
            imprimir_disco();
        }else if(opc==12){
            mostrar_informacion();
        }
        menu();
    }
    int tamanio(){
        return superficies*pistas*sectores*registros*tamanio_registros;
    }
    int tamanio_sector(){
        return tamanio_registros*registros;
    }

};

void disco_crear(Disco &disco){
    string name;
    int opc1;
    cin.ignore();
    cout<<"Nombre del disco: ";getline(cin,name);
    //funcion para crear un disco

    //se guarda en un archivo de texto plano para luego recuperar su informacion
    ifstream archidiscos("DiscoCreados.txt");
    string discos;
    bool band = false;
    //recorre el archivo para ver si es que el disco ya existe
    while(!archidiscos.eof()){
        getline(archidiscos,discos);
        if(name == discos.substr(0,discos.find("/"))){
            band = true;
            break;
        }
    }
    archidiscos.close();
    //Si no existe procede a solicitar informacion
    //para luego guardarla en el archivo
    if(band==false){
        disco.setName(name);
        int n_platos,n_pistas,n_sectores,n_registros;
        cout<<"Numero de platos: ";cin>>n_platos;
        cout<<"Numero de pistas: ";cin>>n_pistas;
        cout<<"Numero de sectores: ";cin>>n_sectores;
        cout<<"Numero de registros: ";cin>>n_registros;
        string Name_disco = name+"/"+to_string(n_platos)+"/"+to_string(n_pistas)+"/"+to_string(n_sectores)+"/"+to_string(n_registros);
        ofstream arc_("DiscoCreados.txt",ios::app);
        arc_<<Name_disco<<endl;
        arc_.close();
    }else{
        //En caso de que exista uno con el mismo nombre
        //Procede a enviar este mensaje
        cout<<"El disco ya fue creado"<<endl;
    }
}

void disco_manipular(Disco &disco){
    cin.ignore();
    string nombre;
    //Para manipular un disco que ya existe
    //se abre un txt que tiene al disco y se busca por el nombre
    cout<<"Nombre del disco: ";getline(cin,nombre);
    ifstream archivo("DiscoCreados.txt");
    string line,aux;
    //la informacion de cada disco(nombre,superficies,etc)
    while(!archivo.eof()){
        getline(archivo,line);
        if(nombre==line.substr(0,line.find("/"))){ //se divide por "/"
            aux=line;
            break;
        }
    }
    archivo.close();
    if(aux.size()>0){
        //si existe carga la informacion del txt en el disco abstracto
        //para luego manipularlo

        disco.setName(nombre);
        aux.erase(0,aux.find("/")+1);

        disco.setplatos(stoi(aux.substr(0,aux.find("/"))));
        aux.erase(0,aux.find("/")+1);

        disco.setpistas(stoi(aux.substr(0,aux.find("/"))));
        aux.erase(0,aux.find("/")+1);

        disco.setsectores(stoi(aux.substr(0,aux.find("/"))));
        aux.erase(0,aux.find("/")+1);

        disco.setregistros(stoi(aux));
        disco.menu();
    }
}


void menu_inicial(Disco &disco){

    //Menu para crear o manipular un disco
    int opc;
    cout<<"1. Crear disco magnetico:\n2. Manipular disco creado\nOpcion: ";cin>>opc;
    if(opc==1){
        disco_crear(disco);
    }else if(opc==2){
        disco_manipular(disco);

    }
    menu_inicial(disco);
}

struct Registro {
    int id;
    std::string nombre;
    std::string apellido;
};

void escribirRegistro(std::ofstream& archivo, const Registro& registro) {
    // Escribir el id
    archivo.write(reinterpret_cast<const char*>(&registro.id), sizeof(registro.id));

    // Escribir el nombre y el apellido
    archivo.write(registro.nombre.c_str(), registro.nombre.length() + 1);
    archivo.write(registro.apellido.c_str(), registro.apellido.length() + 1);
}

void leerRegistro(std::ifstream& archivo, Registro& registro) {
    // Leer el id
    archivo.read(reinterpret_cast<char*>(&registro.id), sizeof(registro.id));

    // Leer el nombre
    std::getline(archivo, registro.nombre, '\0');

    // Leer el apellido
    std::getline(archivo, registro.apellido, '\0');
}


void esquema(){
    string table,line;
    cout<<"Nombre de la tabla: ";
    getline(cin,table);

    ifstream archivo(table);
    getline(archivo,line);
    string cadena= table+"#";

    int pos=0;
    string aux;
    for(int i=0;i!=line.size()+1;i++){
        if(line[i]==',' || i==line.size()){
            aux = line.substr(pos,i-pos);
            pos = i+1;
            cadena += aux+"|";
            cout<<"Tipo de dato de "<<aux<<": ";getline(cin,aux);
            cadena += aux;
            if(aux == "VARCHAR"){
                cout<<"Cuantos bytes: ";getline(cin,aux);
                cadena += "("+aux+")";
            }
            cadena += "#";
        }

    }
    ofstream arch("esquema.txt",ios::app);
    arch<<cadena<<endl;
    arch.close();
}

void Esquema_leer(){
    string nombre;
    //Para manipular un disco que ya existe
    //se abre un txt que tiene al disco y se busca por el nombre
    cout<<"Nombre del disco: ";getline(cin,nombre);
    ifstream archivo("esquema.txt");
    string line,aux;
    //la informacion de cada disco(nombre,superficies,etc)
    while(!archivo.eof()){
        getline(archivo,line);
        if(nombre==line.substr(0,line.find("#"))){ //se divide por "/"
            aux=line;
            break;
        }
    }
    archivo.close();
    if(line.size()!=0){
        line.erase(0,line.find("#")+1);

        int cont =0, tam=line.size();
        string data;
        while(line.size()>0){
            data = line.substr(line.find("|")+1,line.find("#")-line.find("|")-1);
            cout<<data<<endl;
            if(data=="int64"){
                cont +=4;
            }else if(data=="float64"){
                cont +=4;
            }else{
                string a = data.substr(data.find("(")+1,data.find(")")-data.find("(")-1);
                cont += stoi(a);
            }
            line.erase(0,line.find("#")+1);

        }
        cout<<cont;
    }
}
int main()
{
    //se define un objero disco
    Disco disco;
    menu_inicial(disco);
    //Esquema_leer();
    return 0;
}
