#include <iostream>
#include <fstream>
#include <string>
#include <vector>
class Disco {
private:
    int cantidadPlatos;
    int cantidadSuperficies = 2;
    int cantidadPistas;
    int cantidadSectores;
    int cantidadBytes;
public:
    Disco(int platos, int pistas, int sectores, int bytes){
        cantidadPlatos = platos;
        cantidadPistas = pistas;
        cantidadSectores = sectores;
        cantidadBytes = bytes;
    }
};
class Sector : public Disco{
private:
    std::string lines[5];
    int currentLine;

public:
    Sector() {
        for (int i = 0; i < 5; i++) {
            lines[i] = "";
        }
        currentLine = 0;
    }
    bool isFull() {
        for (int i = 0; i < 5; i++) {
            if (lines[i] == "")
                return false;
        }
        return true;
    }

    void addLine(const std::string& line) {
        if (currentLine < 5) {
            lines[currentLine] = line;
            currentLine++;  // Incrementar el índice de la línea actual
        }
    }

    int getCurrentLine() const {
        return currentLine;
    }
};

class Pista : public Disco{
private:
    Sector sectors[10];
    int currentSector;

public:
    Pista() {
        currentSector = 0;
    }

    bool isFull() {
        for (int i = 0; i < 10; i++) {
            if (!sectors[i].isFull())
                return false;
        }
        return true;
    }
    void addLine(const std::string& line) {
        for (int i = currentSector; i < 11; i++) {
            if (!sectors[i].isFull()) {
                sectors[i].addLine(line);
                currentSector = i;  // Actualizar el índice del sector
                break;
            }
        }
    }

    int getCurrentSector() const {
        return currentSector;
    }
};

class Superficie : public Disco{
private:
    Pista pistas[10];
    int currentPista;
    int currentSector;

public:
    Superficie() {
        currentPista = 0;
        currentSector = 0;
    }

    bool isFull() {
        for (int i = 0; i < 10; i++) {
            if (!pistas[i].isFull())
                return false;
        }
        return true;
    }
    void addLine(const std::string& line) {
        for (int i = currentPista; i < 10; i++) {
            if (!pistas[i].isFull()) {
                pistas[i].addLine(line);
                currentPista = i;  // Actualizar el índice de la pista
                currentSector = pistas[i].getCurrentSector();  // Obtener el índice del sector actual desde la clase Pista
                break;
            }
        }
    }

    int getCurrentPista() const {
        return currentPista;
    }

    int getCurrentSector() const {
        return currentSector;
    }
};

class Plato : public Disco {
private:
    Superficie superficies[2];
    int currentPlato;
    int currentSuperficie;
    int currentPista;
    int currentSector;

public:
    Plato() {
        currentPlato = 0;
        currentSuperficie = 0;
        currentPista = 0;
        currentSector = 0;

    }

    bool isFull() {
        for (int i = 0; i < 2; i++) {
            if (!superficies[i].isFull())
                return false;
        }
        return true;
    }
    void addLine(const std::string& line) {
        for (int i = currentSuperficie; i < 2; i++) {
            if (!superficies[i].isFull()) {
                superficies[i].addLine(line);
                currentPlato = i;  // Actualizar el índice del plato
                currentSuperficie = i;  // Actualizar el índice de la superficie
                currentPista = superficies[i].getCurrentPista();  // Obtener el índice de la pista actual desde la clase Superficie
                currentSector = superficies[i].getCurrentSector();  // Obtener el índice del sector actual desde la clase Superficie
                break;
            }
        }
    }


    int getCurrentPlato() const {
        return currentPlato;
    }

    int getCurrentSuperficie() const {
        return currentSuperficie;
    }

    int getCurrentPista() const {
        return currentPista;
    }

    int getCurrentSector() const {
        return currentSector;
    }
};
