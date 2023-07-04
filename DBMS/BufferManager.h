#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H
#include "iostream"
#include "fstream"
#include "sstream"
#include "vector"
#include "cstring"
#include <cstdlib>
#include <unordered_map>
#include <list>
#include <algorithm>

using namespace std;
namespace fs = filesystem;

//comentario

class Nodo {
public:
    char* dato;
    Nodo* siguiente;
    string diskName;

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

class BufferManager{
private:
    int numFrames;
    int FrameSize;
    int capacidadBufferPool;
    string diskname;
public:
    BufferManager(int num, int size) : numFrames(num), FrameSize(size) {
        capacidadBufferPool = numFrames * FrameSize;
    }
    BufferManager(string a){
        diskname = a;
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
        //Para la impresion de un sector es necesario datos como
        //la superficie, pista, sector donde se ubica
        int tamanioSector = 0, sector, pista, superficie, plato;
        cout << "Ingrese el numero de plato: ";
        cin >> plato;
        cout << "Ingrese el numero de superficie: ";
        cin >> superficie;
        cout << "Ingrese el numero de pista: ";
        cin >> pista;
        cout << "Ingrese el numero de sector: ";
        cin >> sector;

        string carpetaPlato = diskname + "/Plato " + to_string(plato);
        cout << "Plato " << plato << ": " << endl;
        string carpetaSuperficie = carpetaPlato + "/Superficie " + to_string(superficie);
        cout << " Superficie " << superficie<< ": " << endl;
        string carpetaPista = carpetaSuperficie + "/Pista " + to_string(pista);
        cout << "     Pista " << to_string(pista) << ": " << endl;
        string position;
        position += (plato < 9 ? "0" : "") + to_string(plato);
        position += (superficie < 9 ? "0" : "") + to_string(superficie);
        position += (pista < 9 ? "0" : "") + to_string(pista);
        position += (sector < 9 ? "0" : "") + to_string(sector);
        string archivoSector = carpetaPista + "/Sector " + position + ".txt";


        int opc;
        ListaEnlazada lista;
        insertar_lista(lista,archivoSector);

        while(true){
            cout<<"\nMenu:\n1.Insertar\n2.Eliminar\n3.Mover\n4.Mostrar\n5.Actualizar sector\n6.Salir\nOpcion: ";cin>>opc;
            cin.ignore();
            if(opc==1){
                string aux;
                int opc2;
                cout<<"\n1.Insertar registro por consola.\n2.Insertar registro de un file.\nOpcion";cin>>opc2;
                if(opc2 == 1){
                    cout<<"Registro: ";getline(cin,aux);
                }else if(opc2 == 2){
                    string FileBD;
                    cin.ignore();
                    cout<<"Nombre del file.";
                    getline(cin,FileBD);
                    ifstream filenombre(FileBD);
                    getline(filenombre,aux);
                    int numero_de_registro;
                    cout<<"Numero de registro: ";cin>>numero_de_registro;
                    for(int i=0;i<numero_de_registro;i++){
                        getline(filenombre,aux);
                        cout<<"El registo a insertar es: "<<aux<<endl;
                    }
                }

                const char* textoChar = aux.c_str();
                lista.insertar(textoChar);
            }else if(opc==2){
                eliminar_re(lista,archivoSector);
            }else if(opc==3){
                mover_registro(lista);
            }else if(opc==4){
                lista.mostrar();
            }else if(opc==5){
                lista.guardar_archivo(archivoSector);
            }else if(opc==6){
                break;
            }else{
                cout<<"Opcion incorrecta."<<endl;
            }
        }
    }
    void Algoritmo_LRU(){
        int n,capacity,N_bloque;
        string fileNumBloques = diskname + "/infoBloque.txt";
        ifstream sectors(fileNumBloques);
        sectors >> n;
        sectors.close();
        //cout<<"Numero de sectores en un bloque: ";cin>>n;
        //cout<<"Capacidad del Buffer manager: ";cin>>capacity;
        string infoFrame = diskname + "/BufferPool/info.txt";
        ifstream frame(infoFrame);
        frame >> capacity;
        frame.close();
        LRUBufferManager lruBufferManager(capacity);
        while(true){
            cout<<"Numero de bloque a ingresar en el buffer manager: ";cin>>N_bloque;
            if(N_bloque<1){
                cout<<"buffer pool bloques: "<<endl;
                lruBufferManager.printBuffer();

                int n;
                cout<<"numero de bloque a modificar: ";cin>>n;
                string line;
                ifstream archivo(diskname+"/DirectorioDeBloques.txt");
                for(int i=0;i<n;i++){
                    getline(archivo,line);
                }
                cout<<line<<endl;
                int opc;
                cout<<"\nSe modificara algun registro\n1.Si\n2.No: ";cin>>opc;
                if(opc == 1){
                    menu_IEM();
                }else{
                    break;
                }
            }
            lruBufferManager.refer(N_bloque);
            lruBufferManager.printBuffer();
        }
    }

    void Algoritmo_MRU(){
        int n,capacity,N_bloque;
        string fileNumBloques = diskname + "/infoBloque.txt";
        ifstream sectors(fileNumBloques);
        sectors >> n;
        sectors.close();
        //cout<<"Numero de sectores en un bloque: ";cin>>n;
        string infoFrame = diskname + "/BufferPool/info.txt";
        ifstream frame(infoFrame);
        frame >> capacity;
        frame.close();
        //cout<<"Capacidad del Buffer manager: ";cin>>capacity;
        //int accessPattern[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
        MRUBufferManager mruBufferManager(capacity);
        while(true){
            cout<<"Numero de bloque a ingresar en el buffer manager: ";cin>>N_bloque;
            if(N_bloque<1){
                cout<<"buffer pool bloques: "<<endl;
                mruBufferManager.printBuffer();
                int opc;
                cout<<"\nSe modificara algun registro\n1.Si\n2.No\nOpcion: ";cin>>opc;
                if(opc == 1){
                    int n;
                    cout<<"numero de bloque a modificar: ";cin>>n;
                    string line;
                    ifstream archivo(diskname+"/DirectorioDeBloques.txt");
                    for(int i=0;i<n;i++){
                        getline(archivo,line);
                    }
                    cout<<line<<endl;
                    menu_IEM();
                }else{
                    break;
                }
            }
            mruBufferManager.refer(N_bloque);
            mruBufferManager.printBuffer();
        }
    }

    void Algoritmo_CLOCK(){
        int capacity,N_bloque, n;
        string fileNumBloques = diskname + "/infoBloque.txt";
        ifstream sectors(fileNumBloques);
        sectors >> n;
        sectors.close();
        string infoFrame = diskname + "/BufferPool/info.txt";
        ifstream frame(infoFrame);
        frame >> capacity;
        frame.close();
        //cout<<capacity;
        //cout<<"Capacidad del Buffer manager: ";cin>>capacity;
        //int accessPattern[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
        ClockBufferManager clockBufferManager(capacity);
        while(true){
            cout<<"Numero de bloque a ingresar en el buffer manager: ";cin>>N_bloque;
            if(N_bloque<1){
                cout<<"buffer pool bloques: "<<endl;
                clockBufferManager.printBuffer();

                int n;
                cout<<"numero de bloque a modificar: ";cin>>n;
                string line;
                ifstream archivo(diskname+"/DirectorioDeBloques.txt");
                for(int i=0;i<n;i++){
                    getline(archivo,line);
                }
                cout<<line<<endl;
                cin.ignore();
                int opc;
                cout<<"\nSe modificara algun registro\n1.Si\n2.No: ";cin>>opc;
                if(opc == 1){
                    menu_IEM();
                }else{
                    break;
                }
            }
            clockBufferManager.refer(N_bloque);
            clockBufferManager.printBuffer();
        }
    }
    void menu_buffer(){
        int opc;
        cout<<"\nEstrategia a usar:\n1.LRU\n2.MRU\n3.CLOCK\nOpcion:";cin>>opc;
        if(opc==1){
            Algoritmo_LRU();
        }else if(opc==2){
            Algoritmo_MRU();
        }else if(opc==3){
            Algoritmo_CLOCK();
        }else{
            menu_buffer();
        }
    }
    void createBufferPool(string diskname){
        string carpetaBufferPool = diskname + "/BufferPool";
        fs::create_directory(carpetaBufferPool);
        ofstream infoBufferPool(carpetaBufferPool + "/info.txt");
        infoBufferPool << numFrames << " " << FrameSize << endl;
        for (int frame = 0; frame < numFrames; ++frame) {
            string carpetaFrame = carpetaBufferPool + "/Frame " + to_string(frame + 1);
            fs::create_directory(carpetaFrame);
        }

        cout<<"Se creo satisfactoriamente el BufferPool de " << numFrames << " Frames, con una capacidad total de " << capacidadBufferPool << " Bytes."<<endl;
        /*if(paginasExists(diskname)){
            cout<<"Existen paginas en el disco."<<endl;
        }else{
            cout<<"No existen paginas en el disco."<<endl;
        }*/
    };
    bool paginasExists(string diskname){
        for (int frame = 0; frame < numFrames; ++frame) {
            string carpetaFrame = diskname + "/BufferPool/Frame " + to_string(frame + 1);
            bool archivosTxtEncontrados = false;

            for (const auto& entrada : fs::directory_iterator(carpetaFrame)) {
                if (!fs::is_directory(entrada) && entrada.path().extension() == ".txt") {
                    archivosTxtEncontrados = true;
                    break;
                }
            }

            if (!archivosTxtEncontrados) {
                return false;
            }
        }

        return true;
    }

};


#endif
