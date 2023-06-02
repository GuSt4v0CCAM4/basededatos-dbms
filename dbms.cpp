#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Clase Sector
class Sector {
private:
    std::string lines[5];

public:
    Sector() {
        for (int i = 0; i < 5; i++) {
            lines[i] = "";
        }
    }

    bool isFull() {
        for (int i = 0; i < 5; i++) {
            if (lines[i] == "")
                return false;
        }
        return true;
    }

    void addLine(const std::string& line) {
        for (int i = 0; i < 5; i++) {
            if (lines[i] == "") {
                lines[i] = line;
                break;
            }
        }
    }
};

// Clase Pista
class Pista {
private:
    Sector sectors[10];

public:
    bool isFull() {
        for (int i = 0; i < 10; i++) {
            if (!sectors[i].isFull())
                return false;
        }
        return true;
    }

    void addLine(const std::string& line) {
        for (int i = 0; i < 10; i++) {
            if (!sectors[i].isFull()) {
                sectors[i].addLine(line);
                break;
            }
        }
    }
};

// Clase Superficie
class Superficie {
private:
    Pista pistas[10];

public:
    bool isFull() {
        for (int i = 0; i < 10; i++) {
            if (!pistas[i].isFull())
                return false;
        }
        return true;
    }

    void addLine(const std::string& line) {
        for (int i = 0; i < 10; i++) {
            if (!pistas[i].isFull()) {
                pistas[i].addLine(line);
                break;
            }
        }
    }
};

// Clase Disco
class Disco {
private:
    Superficie superficies[2];

public:
    bool isFull() {
        for (int i = 0; i < 2; i++) {
            if (!superficies[i].isFull())
                return false;
        }
        return true;
    }

    void addLine(const std::string& line) {
        for (int i = 0; i < 2; i++) {
            if (!superficies[i].isFull()) {
                superficies[i].addLine(line);
                break;
            }
        }
    }
};

// Clase Megatron
class Megatron {
private:
    Disco disco;
    std::string filePath;

    void createFile(const std::string& fileName) {
        std::ofstream file(fileName);
        file.close();
    }

    std::string getPosition(int plato, int superficie, int pista, int sector) {
        return std::to_string(plato) + std::to_string(superficie) +
               std::to_string(pista) + std::to_string(sector);
    }

    void writeFile(const std::string& fileName, const std::string& content) {
        std::ofstream file(fileName, std::ios::app);
        file << content << std::endl;
        file.close();
    }

    void processLine(const std::string& line) {
        std::vector<std::string> attributes;
        std::string attribute;

        for (char c : line) {
            if (c == '#') {
                attributes.push_back(attribute);
                attribute = "";            }
            else {
                attribute += c;
            }
        }
    }

    if (!attribute.empty()) {
        attributes.push_back(attribute);
    }

    if (disco.isFull()) {
        std::cout << "Disco lleno" << std::endl;
        return;
    }

    std::string fileName = getPosition(0, 0, 0, 0) + ".txt";
    createFile(fileName);

    for (const std::string& attr : attributes) {
        writeFile(fileName, attr);
    }

    disco.addLine(line);
}
public:
Megatron(const std::string& file) {
    filePath = file;
}
void read() {
    std::ifstream file(filePath);
    std::string line;

    if (!file) {
        createFile(filePath);
        return;
    }

    while (std::getline(file, line)) {
        processLine(line);
    }

    file.close();
}

void create(const std::string& fileName) {
    std::string input;
    std::cout << "Ingrese los atributos separados por numeral: ";
    std::getline(std::cin, input);

    std::vector<std::string> attributes;
    std::string attribute;

    for (char c : input) {
        if (c == '#') {
            attributes.push_back(attribute);
            attribute = "";
        } else {
            attribute += c;
        }
    }

    if (!attribute.empty()) {
        attributes.push_back(attribute);
    }

    std::string file = fileName + ".txt";
    createFile(file);

    for (const std::string& attr : attributes) {
        writeFile(file, attr);
    }
}

void insert(const std::string& fileName) {
    std::string input;
    std::cout << "Ingrese los atributos separados por numeral: ";
    std::getline(std::cin, input);

    std::string file = fileName + ".txt";
    std::vector<std::string> attributes;
    std::string attribute;

    for (char c : input) {
        if (c == '#') {
            attributes.push_back(attribute);
            attribute = "";
        } else {
            attribute += c;
        }
    }

    if (!attribute.empty()) {
        attributes.push_back(attribute);
    }

    for (const std::string& attr : attributes) {
        writeFile(file, attr);
    }
}

void select(const std::string& fileName, const std::string& search) {
    std::ifstream file(fileName);
    std::string line;

    while (std::getline(file, line)) {
        if (line.find(search) != std::string::npos) {
            std::cout << line << std::endl;
        }
    }

    file.close();
}

void location(const std::string& search) {
    for (int plato = 0; plato < 2; plato++) {
        for (int superficie = 0; superficie < 2; superficie++) {
            for (int pista = 0; pista < 10; pista++) {
                for (int sector = 0; sector < 5; sector++) {
                    std::string fileName = getPosition(plato, superficie, pista, sector) + ".txt";
                    std::ifstream file(fileName);

                    std::string line;
                    while (std::getline(file, line)) {
                        if (line.find(search) != std::string::npos) {
                            std::cout << "Archivo: " << fileName << std::endl;
                            file.close();
                            return;
                        }
                    }

                    file.close();
                }
            }
        }
    }
}
};

int main() {
    Megatron megatron("titanic.txt");
    std::string query;
    std::string fileName;
    std::string search;

    while (true) {
        std::cout << "Ingrese una consulta (READ, CREATE, INSERT, SELECT, LOCATION): ";
        std::getline(std::cin, query);

        if (query == "READ") {
            megatron.read();
        } else if (query == "CREATE") {
            std::cout << "Ingrese el nombre del archivo: ";
            std::getline(std::cin, fileName);
            megatron.create(fileName);
        } else if (query == "INSERT") {
            std::cout << "Ingrese el nombre del archivo: ";
            std::getline(std::cin, fileName);
            megatron.insert(fileName);
        } else if (query == "SELECT") {
            std::cout << "Ingrese el nombre del archivo: ";
            std::getline(std::cin, fileName);
            std::cout << "Ingrese la cadena de búsqueda: ";
            std::getline(std::cin, search);
            megatron.select(fileName, search);
        } else if (query == "LOCATION") {
            std::cout << "Ingrese la cadena de búsqueda: ";
            std::getline(std::cin, search);
            megatron.location(search);
        }

        std::cout << "Continua (s/n)? ";
        std::string response;
        std::getline(std::cin, response);

        if (response != "s")
            break;
    }

    return 0;
}