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
    Dbms(const string& diskName) : diskName(diskName) {
        ifstream diskFile(diskName + "/" + diskName + ".txt");
        if (!diskFile) {
            cout << "No se pudo abrir el archivo del disco." << endl;
            return;
        }

        diskFile >> capacidadDelSector >> numSectores >> numPistas >> numSuperficies >> numPlatos;
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
