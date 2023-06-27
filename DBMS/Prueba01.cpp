#include "iostream"
#include "filesystem"
#include "fstream"
#include "sstream"
#include "string"
#include "Disco.h"
#include "Esquema.h"
#include "Dbms.h"
#include "BufferManager.h"
using namespace std;
namespace fs = std::filesystem;

class File {
private:
    int numSectores, numPistas, numSuperficies, numPlatos, capacidadSector;
    string bd;
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
        getline(std::cin, bd);
        disco.createFolder(bd);
        cout << "Ingrese el numero de sectores por Bloque: "<<endl;
        int numSectoresPerBloque, sizeFrame, numFrames;
        cin >> numSectoresPerBloque;
        disco.asignarBloques(numSectoresPerBloque, bd);
        cout << "Ahora s eprocedera a crear el BufferPool..."<<endl;
        cout << "Ingrese el numero de Frames del BufferPool: "<<endl;
        cin >> numFrames;
        sizeFrame = capacidadSector * numSectoresPerBloque;
        BufferManager bufferManager(numFrames, sizeFrame);
        bufferManager.createBufferPool(bd);
    }
};
int main() {
    int numSuperficies=2, numPistas, numSectores, capacidadSector, numPlatos;
    string query, fileName, search, DataBaseName, bd, response;;
    File file;
    Esquema esquema;
    while (true) {
        cout << "Ingrese una consulta (CREATE_DISK, READ, CREATE, INSERT, SELECT, LOCATION): ";
        getline(std::cin, query);

        if (query == "CREATE_DISK") {
            file.createDisk();

        }else if(query == "READ") {
            cout<<"Ingrese el nombre de la Base de Datos para cargar (sin extensión): ";
            getline(cin, DataBaseName);
            string DataBase = DataBaseName + ".txt";
            string EsquemaName = DataBaseName + "_esquema.txt";
            esquema.processEsquema(DataBase,EsquemaName);
            cout<<"Ingrese el nombre del Disco a utilizar:";
            getline(cin,bd);
            Dbms dbms(bd);
            dbms.fillSectors(DataBase);
        } else if (query == "CREATE") {
            cout << "Ingrese el nombre del archivo: ";
            getline(std::cin, fileName);
            //megatron.create(fileName);
        } else if (query == "INSERT") {
            //megatron.insert();
        } else if (query == "SELECT") {
            cout << "Ingrese el nombre del disco: ";
            getline(std::cin, fileName);
            cout << "Ingrese la cadena de búsqueda: ";
            getline(std::cin, search);
            Dbms dbms(fileName);
            dbms.select(fileName, search);
            //megatron.select(fileName, search);
        } else if (query == "LOCATION") {
            //megatron.location();
        } else if (query == "DELETE"){
            //megatron.del();
        }

        cout << "Continua (s/n)? ";
        getline(std::cin, response);

        if (response != "s")
            break;
    }
    return 0;
}