#ifndef DISCO_H
#define DISCO_H
#include "algorithm"
#include "cctype"
#include "iostream"
#include "filesystem"
#include "fstream"
#include "sstream"
#include "string"
#include "vector"
using namespace std;
namespace fs = filesystem;
class Disco {
private:
    int capacidadDisco, capacidadSector, numSectores, numPistas, numSuperficies=2, numPlatos;
    string diskname;
public:
    Disco(int capacidadSector, int numSectores, int numPistas,  int numPlatos) :
    capacidadSector(capacidadSector), numSectores(numSectores), numPistas(numPistas), numPlatos(numPlatos)
    {
        capacidadDisco = capacidadSector * numSectores * numPistas * numPistas * numPlatos;


    }
    bool diskExists(const string& diskName)
    {
        return fs::exists(diskName);
    }
    void createFolder(string diskname){
        if (diskExists(diskname)) {
            cout << "El disco ya existe." << endl;
            return;
        }else{
            fs::create_directory(diskname);
            for (int plato = 0; plato < numPlatos; ++plato) {
                string carpetaPlato = diskname + "/Plato " + to_string(plato + 1);
                fs::create_directory(carpetaPlato);

                for (int superficie = 0; superficie < numSuperficies; ++superficie) {
                    string carpetaSuperficie = carpetaPlato + "/Superficie " + to_string(superficie + 1);
                    fs::create_directory(carpetaSuperficie);

                    for (int pista = 0; pista < numPistas; ++pista) {
                        string carpetaPista = carpetaSuperficie + "/Pista " + to_string(pista + 1);
                        fs::create_directory(carpetaPista);
                        for(int sector = 0; sector < numSectores; ++sector){
                            string position;
                            position += (plato < 9 ? "0" : "") + to_string(plato + 1);
                            position += (superficie < 9 ? "0" : "") + to_string(superficie + 1);
                            position += (pista < 9 ? "0" : "") + to_string(pista + 1);
                            position += (sector < 9 ? "0" : "") + to_string(sector + 1);
                            string archivoSector = carpetaPista + "/Sector " +  position + ".txt";
                            ofstream file(archivoSector);
                            file.close();
                        }
                    }
                }
            }
        }
        ofstream file;
        file.open(diskname + "/" + diskname + ".txt", ios::out);
        file << capacidadSector << " " << numSectores << " " << numPistas << " " << numSuperficies << " " <<numPlatos << endl;
        file.close();
        cout<<"Disco creado con exito"<<endl;
        cout<< "El disco "<<diskname<<" tiene "<<capacidadDisco<<" bytes."<<endl;
    }

    void asignarBloques(int numSectoresPerBloque, string diskname) {
        string directoryFile = diskname + "/DirectorioDeBloques.txt";
        ofstream dirFile(directoryFile.c_str(), ios::app);
        int numSectoresTotal = numSectores * numPistas * numSuperficies * numPlatos;
        int numBloques = numSectoresTotal / numSectoresPerBloque;

        for (int bloque = 0; bloque < numBloques; ++bloque) {
            string bloqueFixed;
            bloqueFixed = (bloque <= 9 ? "0" : "") + to_string(bloque);
            dirFile << "Bloque " << bloqueFixed << ": ";
            int sectorInicio = bloque * numSectoresPerBloque;

            for (int i = 0; i < numSectoresPerBloque; ++i) {
                int sector = sectorInicio + i;
                int plato = sector / (numSectores * numSuperficies * numPistas);
                int residuo = sector % (numSectores * numSuperficies * numPistas);
                int superficie = residuo / (numSectores * numPistas);
                residuo = residuo % (numSectores * numPistas);
                int pista = residuo / numSectores;
                int sectorEnPista = residuo % numSectores;

                string position;
                position += (plato < 9 ? "0" : "") + to_string(plato + 1);
                position += (superficie < 9 ? "0" : "") + to_string(superficie + 1);
                position += (pista < 9 ? "0" : "") + to_string(pista + 1);
                position += (sectorEnPista < 9 ? "0" : "") + to_string(sectorEnPista + 1);

                dirFile << position << " ";
            }

            dirFile << endl;
        }

        dirFile.close();
    }
    void select(string diskname, string search) {

        for (int plato = 0; plato < numPlatos; ++plato) {
            string carpetaPlato = diskname + "/Plato " + to_string(plato + 1);

            for (int superficie = 0; superficie < numSuperficies; ++superficie) {
                string carpetaSuperficie = carpetaPlato + "/Superficie " + to_string(superficie + 1);

                for (int pista = 0; pista < numPistas; ++pista) {
                    string carpetaPista = carpetaSuperficie + "/Pista " + to_string(pista + 1);

                    for(int sector = 0; sector < numSectores; ++sector){
                        string position;
                        position += (plato < 9 ? "0" : "") + to_string(plato + 1);
                        position += (superficie < 9 ? "0" : "") + to_string(superficie + 1);
                        position += (pista < 9 ? "0" : "") + to_string(pista + 1);
                        position += (sector < 9 ? "0" : "") + to_string(sector + 1);
                        string archivoSector = carpetaPista + "/Sector " +  position + ".txt";
                        ifstream file(archivoSector);
                        //cout<<archivoSector<<endl;
                        if(file){
                            string line;
                            while (getline(file, line)) {
                                line.erase(std::remove_if(line.begin(), line.end(), [](unsigned char c) { return std::isspace(c) || c == '\"'; }), line.end());
                                if (line.find(search) != string::npos) {
                                    //cout<<line<<endl;

                                    cout<<"El archivo "<<archivoSector<<" contiene el bloque "<<line<<endl;
                                }
                            }
                        }
                        file.close();
                    }
                }
            }
        }
    }





};
#endif
