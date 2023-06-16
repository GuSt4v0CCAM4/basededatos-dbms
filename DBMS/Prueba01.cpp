#include "iostream"
#include "filesystem"
#include "fstream"
#include "sstream"
#include "string"
#include "Disco.h"
#include "Esquema.h"
using namespace std;
namespace fs = std::filesystem;

class File {
private:
    int numSectores, numPistas, numSuperficies, numPlatos, capacidadSector;
    string diskname;
public:
    void createDisk(){
        cout << "Ingrese la cantidad de Platos: " <<endl;
        cin >> numPlatos;
        cout << "Ingrese la cantidad de Pistas por Superficie: " <<endl;
        cin >> numPistas;
        cout << "Ingrese la cantidad de Sectores por Pista: " <<endl;
        cin >> numSectores;
        cout << "Ingrese la capacidad de Sector (bytes): " <<endl;
        cin >> capacidadSector;
        Disco disco(capacidadSector, numSectores, numPistas, numPlatos);
        cin.ignore();
        cout << "Ingrese el nombre del Disco: "<<endl;
        getline(std::cin, diskname);
        disco.createFolder(diskname);
        cout << "Ingrese el numero de bloques: "<<endl;
        int numSectoresPerBloque;
        cin >> numSectoresPerBloque;
        disco.asignarBloques(numSectoresPerBloque, diskname);
    }
};
int main() {
    int numSuperficies=2, numPistas, numSectores, capacidadSector, numPlatos;
    string query, fileName, search;
    while (true) {
        cout << "Ingrese una consulta (CREATE_DISK, READ, CREATE, INSERT, SELECT, LOCATION): ";
        getline(std::cin, query);

        if (query == "CREATE_DISK") {
            File file;
            file.createDisk();
        }else if(query == "READ") {
            string esquemaFile;
            cout<<"Ingrese el nombre de la Base de Datos: ";
            getline(cin, esquemaFile);
            Esquema esquema;

            string fileSave = "esquema.txt";
            esquema.processEsquema(esquemaFile,fileSave);
            //megatron.read();
        } else if (query == "CREATE") {
            cout << "Ingrese el nombre del archivo: ";
            getline(std::cin, fileName);
            //megatron.create(fileName);
        } else if (query == "INSERT") {
            //megatron.insert();
        } else if (query == "SELECT") {
            cout << "Ingrese el nombre del archivo: ";
            getline(std::cin, fileName);
            cout << "Ingrese la cadena de búsqueda: ";
            getline(std::cin, search);
            //megatron.select(fileName, search);
        } else if (query == "LOCATION") {
            //megatron.location();
        } else if (query == "CREATE_DISK"){
            //megatron.create_disk();
        }else if (query == "DELETE"){
            //megatron.del();
        }

        cout << "Continua (s/n)? ";
        string response;
        getline(std::cin, response);

        if (response != "s")
            break;
    }
    return 0;
}