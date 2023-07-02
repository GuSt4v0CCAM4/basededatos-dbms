#ifndef DBMS_H
#define DBMS_H
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include "BufferManager.h"
#include "Disco.h"
using namespace std;
namespace fs = filesystem;

class Dbms {
private:
    string diskName;
    int capacidadDelSector;
    int numSectores;
    int numPistas;
    int numSuperficies;
    int numPlatos;

public:
    Dbms(const string& diskName) : diskName(diskName) { //constructor donde esta el valro del nombre del dico a utilizar
        ifstream diskFile(diskName + "/" + diskName + ".txt");
        if (!diskFile) {
            cout << "No se pudo abrir el archivo del disco." << endl;
            return;
        }

        diskFile >> capacidadDelSector >> numSectores >> numPistas >> numSuperficies >> numPlatos;
        //recuperamos los valores de los diferentes partes del disco
        diskFile.close();
    }

    void fillSectors(string dbName) {
        int pesoLine = 0, linePerSector = 0, totalLineCount = 0;
        ifstream weightFile("weightline.txt");
        if (!weightFile) {
            cout << "No se pudo abrir el archivo de pesos." << endl;
            return;
        }
        weightFile >> pesoLine;
        weightFile.close();
        ifstream countFile(dbName);
        string countLineFile, currentPosition;
        while (getline(countFile, countLineFile)){
            totalLineCount++;
        }
        if(pesoLine > capacidadDelSector){
            cout<<"El peso del registro excede al peso del Sector"<<endl;
        }else{
            ifstream databaseFile(dbName);
            string line;
            int lineCount = 0;
            getline(databaseFile, line);
            linePerSector = capacidadDelSector / pesoLine;
            bool copyCompleted = false;

            while (getline(databaseFile, line) && !copyCompleted) {
                for (int plato = 0; plato < numPlatos; ++plato) {
                    string carpetaPlato = diskName + "/Plato " + to_string(plato + 1);

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
                                //cout << "Archivo: " << archivoSector << endl;
                                ofstream sectorFile(archivoSector);

                                for (int i = 0; i < linePerSector && !copyCompleted; ++i) {
                                    getline(databaseFile, line);
                                    //cout << line << endl;
                                    sectorFile << line << endl;
                                    lineCount++;

                                    if (lineCount >= totalLineCount) {
                                        copyCompleted = true;
                                        currentPosition = position;
                                        //cout << position << endl;
                                        break;
                                    }
                                }

                                //cout << "\n" << endl;
                                sectorFile.close();

                                if (copyCompleted) {
                                    break;
                                }
                            }

                            if (copyCompleted) {
                                break;
                            }
                        }

                        if (copyCompleted) {
                            break;
                        }
                    }

                    if (copyCompleted) {
                        break;
                    }
                }
            }

            databaseFile.close();

        }
    }
    void info() {
        string diskname = diskName;
        int opc=0;
        cout << "Elige la opcion que desea: " << endl;
        cout << "1. Info de del Disco" << endl;
        cout << "2. Info de un Plato" << endl;
        cout << "3. Info de una Superficie" << endl;
        cout << "4. Info de una Pista" << endl;
        cout << "5. Info de un Sector" << endl;
        cout << "6. Salir" << endl;
        cout << "Opcion: "; cin >> opc;

        if(opc == 1){
            int tamanioDisco=0;
            tamanioDisco = capacidadDelSector * numSectores * numPistas * numSuperficies * numPlatos;
            cout<<"El tamaño del disco '"<<diskname<<"' es de: "<<tamanioDisco<<" bytes"<<endl;
            cout<<"Y esta estructurado de la siguiente manera: "<<endl;
            for (int plato = 0; plato < numPlatos; ++plato) {
                string carpetaPlato = diskname + "/Plato " + to_string(plato + 1);
                cout<<"Plato "<<to_string(plato +1 )<<": "<<endl;

                for (int superficie = 0; superficie < numSuperficies; ++superficie) {
                    string carpetaSuperficie = carpetaPlato + "/Superficie " + to_string(superficie + 1);
                    cout<<" Superficie "<<to_string(superficie + 1)<<": "<<endl;

                    for (int pista = 0; pista < numPistas; ++pista) {
                        string carpetaPista = carpetaSuperficie + "/Pista " + to_string(pista + 1);
                        cout<<"     Pista "<<to_string(pista + 1)<<": "<<endl;

                        for(int sector = 0; sector < numSectores; ++sector){
                            string position;
                            position += (plato < 9 ? "0" : "") + to_string(plato + 1);
                            position += (superficie < 9 ? "0" : "") + to_string(superficie + 1);
                            position += (pista < 9 ? "0" : "") + to_string(pista + 1);
                            position += (sector < 9 ? "0" : "") + to_string(sector + 1);
                            string archivoSector = carpetaPista + "/Sector " +  position + ".txt";
                            ifstream file(archivoSector);
                            //cout<<archivoSector<<endl;
                            cout<<"         Sector "<<to_string(sector + 1)<<": "<<endl;
                            if(file){
                                string line;
                                while (getline(file, line)) {
                                    cout<<"             "<<line<<endl;
                                }
                            }
                            file.close();
                        }
                    }
                }
            }
        }else if(opc == 2) {
            int tamanioPlato = 0, plato;
            tamanioPlato = capacidadDelSector * numSectores * numPistas * numSuperficies;
            cout << "Ingrese el numero de plato: ";
            cin >> plato;
            cout << "El tamaño del Plato " << plato << " es de: " << tamanioPlato << " bytes" << endl;
            cout << "Y tiene la siguiente estructura: " << endl;
            string carpetaPlato = diskname + "/Plato " + to_string(plato);
            cout << "Plato " << plato << ": " << endl;

            for (int superficie = 0; superficie < numSuperficies; ++superficie) {
                string carpetaSuperficie = carpetaPlato + "/Superficie " + to_string(superficie + 1);
                cout << " Superficie " << to_string(superficie + 1) << ": " << endl;

                for (int pista = 0; pista < numPistas; ++pista) {
                    string carpetaPista = carpetaSuperficie + "/Pista " + to_string(pista + 1);
                    cout << "     Pista " << to_string(pista + 1) << ": " << endl;

                    for (int sector = 0; sector < numSectores; ++sector) {
                        string position;
                        position += (plato < 9 ? "0" : "") + to_string(plato);
                        position += (superficie < 9 ? "0" : "") + to_string(superficie + 1);
                        position += (pista < 9 ? "0" : "") + to_string(pista + 1);
                        position += (sector < 9 ? "0" : "") + to_string(sector + 1);
                        string archivoSector = carpetaPista + "/Sector " + position + ".txt";
                        ifstream file(archivoSector);
                        //cout<<archivoSector<<endl;
                        cout << "         Sector " << to_string(sector + 1) << ": " << endl;
                        if (file) {
                            string line;
                            while (getline(file, line)) {
                                cout << "             " << line << endl;
                            }
                        }
                        file.close();
                    }
                }
            }
        } else if (opc == 3) {
            int tamanioSuperficie = 0, superficie, plato;
            tamanioSuperficie = capacidadDelSector * numSectores * numPistas;
            cout << "Ingrese el numero de plato: ";
            cin >> plato;
            cout << "Ingrese el numero de superficie: ";
            cin >> superficie;
            cout << "El tamaño de la Superficie " << plato << " del Plato " << plato << " es de: " << tamanioSuperficie << " bytes" << endl;
            cout << "Y tiene la siguiente estructura: " << endl;

            string carpetaPlato = diskname + "/Plato " + to_string(plato);
            cout << "Plato " << plato << ": " << endl;
            string carpetaSuperficie = carpetaPlato + "/Superficie " + to_string(superficie);
            cout << " Superficie " << superficie<< ": " << endl;

            for (int pista = 0; pista < numPistas; ++pista) {
                string carpetaPista = carpetaSuperficie + "/Pista " + to_string(pista + 1);
                cout << "     Pista " << to_string(pista + 1) << ": " << endl;

                for (int sector = 0; sector < numSectores; ++sector) {
                    string position;
                    position += (plato < 9 ? "0" : "") + to_string(plato);
                    position += (superficie < 9 ? "0" : "") + to_string(superficie);
                    position += (pista < 9 ? "0" : "") + to_string(pista + 1);
                    position += (sector < 9 ? "0" : "") + to_string(sector + 1);
                    string archivoSector = carpetaPista + "/Sector " + position + ".txt";
                    ifstream file(archivoSector);
                    //cout<<archivoSector<<endl;
                    cout << "         Sector " << to_string(sector + 1) << ": " << endl;
                    if (file) {
                        string line;
                        while (getline(file, line)) {
                            cout << "             " << line << endl;
                        }
                    }
                    file.close();
                }
            }

        } else if (opc == 4) {
            int tamanioPista = 0, pista, superficie, plato;
            tamanioPista = capacidadDelSector * numSectores;
            cout << "Ingrese el numero de plato: ";
            cin >> plato;
            cout << "Ingrese el numero de superficie: ";
            cin >> superficie;
            cout << "Ingrese el numero de pista: ";
            cin >> pista;
            cout << "El tamaño de la Pista "<<pista<<" de la Superficie " << plato << " del Plato " << plato << " es de: " << tamanioPista << " bytes" << endl;
            cout << "Y tiene la siguiente estructura: " << endl;

            string carpetaPlato = diskname + "/Plato " + to_string(plato);
            cout << "Plato " << plato << ": " << endl;
            string carpetaSuperficie = carpetaPlato + "/Superficie " + to_string(superficie);
            cout << " Superficie " << superficie<< ": " << endl;
            string carpetaPista = carpetaSuperficie + "/Pista " + to_string(pista);
            cout << "     Pista " << to_string(pista) << ": " << endl;

            for (int sector = 0; sector < numSectores; ++sector) {
                string position;
                position += (plato < 9 ? "0" : "") + to_string(plato);
                position += (superficie < 9 ? "0" : "") + to_string(superficie);
                position += (pista < 9 ? "0" : "") + to_string(pista);
                position += (sector < 9 ? "0" : "") + to_string(sector + 1);
                string archivoSector = carpetaPista + "/Sector " + position + ".txt";
                ifstream file(archivoSector);
                //cout<<archivoSector<<endl;
                cout << "         Sector " << to_string(sector + 1) << ": " << endl;
                if (file) {
                    string line;
                    while (getline(file, line)) {
                        cout << "             " << line << endl;
                    }
                }
                file.close();
            }

        } else if (opc == 5) {
            int tamanioSector = 0, sector, pista, superficie, plato;
            tamanioSector = capacidadDelSector;
            cout << "Ingrese el numero de plato: ";
            cin >> plato;
            cout << "Ingrese el numero de superficie: ";
            cin >> superficie;
            cout << "Ingrese el numero de pista: ";
            cin >> pista;
            cout << "Ingrese el numero de sector: ";
            cin >> sector;
            cout << "El tamaño del Sector "<<sector<<" de la Pista "<<pista<<" de la Superficie " << plato << " del Plato " << plato << " es de: " << tamanioSector<< " bytes" << endl;
            cout << "Y tiene la siguiente estructura: " << endl;

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
            ifstream file(archivoSector);
            //cout<<archivoSector<<endl;
            cout << "         Sector " << to_string(sector) << ": " << endl;
            if (file) {
                string line;
                while (getline(file, line)) {
                    cout << "             " << line << endl;
                }
            }
            file.close();
        } else if (opc == 6) {
            return;
        }
    }
    void select(string diskname, string search) {
        //recuperamos la info del bufferpool
        string nombredisco = diskname;
        string busqueda = search;
        int numframes = 0, sizeframes = 0;
        ifstream fileBufferPool(diskName + "/BufferPool/info.txt");
        fileBufferPool >> numframes >> sizeframes;
        fileBufferPool.close();
        cout <<"se recupero la info del buffer" <<numframes<<sizeframes<<endl;
        BufferManager bufferManager(numframes, sizeframes);
        //comprobamos si existe los archivos txt que simulan los frames del bufferpool
        if(bufferManager.paginasExists(diskName)){
            cout <<"Si existe buffer"<<endl;
        }else{
            cout <<"No existe buffer"<<endl;
            Disco disco(capacidadDelSector, numSectores, numPistas, numPlatos);
            cout <<capacidadDelSector<<"  "<<numSectores<<"  "<<numPistas<<"  "<<numPlatos<<endl;
            disco.select(nombredisco, busqueda);
            cout <<nombredisco<<"  "<<busqueda<<endl;
        }
    }
};




#endif
