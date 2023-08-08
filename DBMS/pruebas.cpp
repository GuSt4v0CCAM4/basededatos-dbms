#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
using namespace std;

struct nodo {
    string clave;
    string nombreArchivo;
};

map<string, nodo> BPlusTree;

void imprimirRaiz(){
    cout<<BPlusTree.find("1")->second.clave<<endl;
}
void searchBPlusTree(const string& clave) {
    auto it = BPlusTree.find(clave);
        if (it != BPlusTree.end()) {
        string archivo = it->second.nombreArchivo;
        ifstream file(archivo);
        if (file.is_open()) {
            string linea;

            while (getline(file, linea)) {
                istringstream ss(linea);
                string atributo;
                getline(ss,atributo,',');
                if(atributo == clave){
                    cout<<atributo<<" ";
                    cout << " (En archivo: " << archivo << ")" << endl;
                }

            }
            file.close();
        }
        else {
            cout << "Error al abrir el archivo: " << archivo << endl;
        }
    }
    else {
        cout << "Registro no encontrado en el B+ tree." << endl;
    }
}

void createFolder(string diskname, int capacidadSector, int numSectores, int numPistas, int numSuperficies, int numPlatos) {
    int numBloque=0,s=0, limBloque=0;
    string infoBloque = diskname + "/DirectorioDeBloques.txt";
    ifstream infoB(infoBloque);
    infoB >> limBloque;
    infoB.close();
    for (int plato = 0; plato < numPlatos; ++plato) {
        string carpetaPlato = diskname + "/Plato " + to_string(plato + 1);

        for (int superficie = 0; superficie < numSuperficies; ++superficie) {
            string carpetaSuperficie = carpetaPlato + "/Superficie " + to_string(superficie + 1);

            for (int pista = 0; pista < numPistas; ++pista) {
                string carpetaPista = carpetaSuperficie + "/Pista " + to_string(pista + 1);
                for (int sector = 0; sector < numSectores; ++sector) {
                    string position;
                    position += (plato < 9 ? "0" : "") + to_string(plato + 1);
                    position += (superficie < 9 ? "0" : "") + to_string(superficie + 1);
                    position += (pista < 9 ? "0" : "") + to_string(pista + 1);
                    position += (sector < 9 ? "0" : "") + to_string(sector + 1);
                    string archivoSector = carpetaPista + "/Sector " + position + ".txt";

                    ifstream file(archivoSector);
                    s++;

                    if (s==limBloque){
                        numBloque++;
                        s = 0;
                    }
                    if (file.is_open()) {
                        string linea;
                        while (getline(file, linea)) {
                            istringstream ss(linea);
                            string clave;
                            getline(ss, clave, ','); // La clave es el primer atributo
                            nodo registro = { clave, archivoSector };
                            BPlusTree.insert({ clave, registro });
                        }
                        file.close();
                    }
                    else {
                        cout << "Error al abrir el archivo: " << archivoSector << endl;
                    }
                }
            }
        }
    }
}


int main() {
    string diskname = "parcial2";

    int capacidadSector = 1351;
    int numSectores = 15;
    int numPistas = 7;
    int numSuperficies = 2;
    int numPlatos = 5;

    createFolder(diskname, capacidadSector, numSectores, numPistas, numSuperficies, numPlatos);

    int opcion;
    string clave;
    do {
        cout << "====== MENÚ ======" << endl;
        cout << "1. Buscar registro por clave" << endl;
        cout << "2. Imprimir Raiz."<<endl;
        cout << "0. Salir" << endl;
        cout << "===================" << endl;
        cout << "Ingrese opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "Ingrese la clave del registro a buscar: ";
                cin >> clave;
                searchBPlusTree(clave);
                break;
            case 2:
                imprimirRaiz();
                break;
            case 0:
                cout << "Saliendo del programa." << endl;
                break;
            default:
                cout << "Opción inválida. Intente nuevamente." << endl;
                break;
        }
    } while (opcion != 0);

    return 0;
}
