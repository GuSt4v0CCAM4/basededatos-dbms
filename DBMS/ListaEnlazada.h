#ifndef BASEDEDATOS_DBMS_LISTAENLAZADA_H
#define BASEDEDATOS_DBMS_LISTAENLAZADA_H
#include<bits/stdc++.h>
using namespace std;
class NodoL{
public:
    char* dato;
    NodoL* siguiente;
    string diskName;

    NodoL(const char* dato) {
        this->dato = new char[strlen(dato) + 1];
        strcpy(this->dato, dato);
        this->siguiente = nullptr;
    }

    ~NodoL() {
        delete[] dato;
    }
};

class ListaEnlazada {
private:
    NodoL* cabeza;

public:
    ListaEnlazada() {
        cabeza = nullptr;
    }

    void insertar(const char* dato) {
        string vaciostr = "vacio";
        const char* vacio = vaciostr.c_str();

        NodoL* nuevoNodoL = new NodoL(dato);

        if (cabeza == nullptr) {
            cabeza = nuevoNodoL;
        } else {
            NodoL* temp = cabeza;
            while (temp->siguiente != nullptr) {
                if(strcmp(temp->dato,vacio)==0)
                {
                    strcpy(temp->dato, dato);
                    return;
                }
                temp = temp->siguiente;
            }
            temp->siguiente = nuevoNodoL;
        }
    }
//xd
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

        NodoL* actual = cabeza;

        while (actual != nullptr && strcmp(actual->dato, data) != 0) {
            //prev = actual;
            actual = actual->siguiente;
        }

        if (actual == nullptr) {
            return;
        }
        strcpy(actual->dato, vacio);
    }
    void eliminar_NodoL(const char* dato){
        if (cabeza == nullptr) {
            return;
        }

        if (strcmp(cabeza->dato, dato) == 0) {
            NodoL* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            return;
        }

        NodoL* prev = nullptr;
        NodoL* actual = cabeza;

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
        NodoL *tmp = cabeza;
        int contar = 1;
        while(true){
            if(contar==pos_i){
                break;
            }
            contar++;
            tmp = tmp->siguiente;
        }

        NodoL* temp = cabeza;
        int contador = 1;
        eliminar_NodoL(tmp->dato);
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
        NodoL* temp = cabeza;
        while (temp != nullptr) {
            std::cout << temp->dato << endl;
            temp = temp->siguiente;
        }
        std::cout << std::endl;
    }

    bool find(int lim){
        NodoL *temp = cabeza;
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
        NodoL* temp = cabeza;
        while (temp != nullptr) {
            archivo<<temp->dato<<endl;
            temp = temp->siguiente;
        }
        archivo.close();
    }
};

#endif //BASEDEDATOS_DBMS_LISTAENLAZADA_H
