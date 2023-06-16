#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <algorithm>

class Sector {
private:
    int cantidadSectores;
    int cantidadBytes;
    std::string lines[5];
    int currentLine;

public:
    Sector() {
        for (int i = 0; i < 5; i++) {
            lines[i] = "";
        }
        currentLine = 0;
    }
    void setSectores(int cantidad) {
        cantidadSectores = cantidad;
    }
    void setBytes(int cantidad) {
        cantidadBytes = cantidad;
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

class Pista {
private:
    int cantidadPistas;
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
    void setPistas(int cantidad) {
        cantidadPistas = cantidad;
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

class Superficie {
private:
    int cantidadSuperficies;
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
    void setSuperficies(int cantidad) {
        cantidadSuperficies = cantidad;
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

class Disco {
private:
    int cantidadPlatos;
    Superficie superficies[2];
    int currentPlato;
    int currentSuperficie;
    int currentPista;
    int currentSector;

public:
    Disco() {
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
    void setPlatos(int cantidad) {
        cantidadPlatos = cantidad;
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


class Megatron {
private:
    Disco discog;
    std::string filePath;
    int currentPlato;
    int currentSuperficie;
    int currentPista;
    int currentSector;
    int numPlatos;
    int numSuperficies;
    int numPistas;
    int numSectores;

    void createFile(const std::string& fileName) {
        std::ofstream file(fileName);
        file.close();
    }

    std::string getPosition(int plato, int superficie, int pista, int sector) {
        std::string position;
        position += (plato < 9 ? "0" : "") + std::to_string(plato + 1);
        position += (superficie < 9 ? "0" : "") + std::to_string(superficie + 1);
        position += (pista < 9 ? "0" : "") + std::to_string(pista + 1);
        position += (sector < 9 ? "0" : "") + std::to_string(sector + 1);

        return position;

    }


    void writeFile(const std::string& fileName, const std::string& content) {
        std::ofstream file(fileName, std::ios::app);
        file << content << std::endl;
        file.close();

    }

public:
    Megatron(const std::string& file) {
        filePath = file;
        currentPlato = 0;
        currentSuperficie = 0;
        currentPista = 0;
        currentSector = 0;
    }
    void create_disk() {
        int cantidadDiscos, pistas, sectores, superficies = 2, bytes;
        std::cout<< "Ingrese la cantidad de platos: ";
        std::cin >> cantidadDiscos;
        std::cout<< "Ingrese la cantidad de pistas por superficie: ";
        std::cin >> pistas;
        std::cout<< "Ingrese la cantidad de sectores por pista: ";
        std::cin>> sectores;
        std::cout<< "Ingrese el tamaño de un sector (en bytes): ";
        std::cin>> bytes;
        Disco *disco = new Disco[cantidadDiscos];
        Superficie *superficie = new Superficie[superficies];
        Pista *pista = new Pista[pistas];
        Sector *sector = new Sector[sectores];

        disco->setPlatos(cantidadDiscos);
        superficie->setSuperficies(superficies);
        pista->setPistas(pistas);
        sector->setSectores(sectores);
        sector->setBytes(bytes);
        int capacidadTotal = 0;
        capacidadTotal = cantidadDiscos * pistas * sectores * bytes;
        std::cout << "Capacidad total del disco: " << capacidadTotal << " bytes"<<std::endl;
    }

    void read() {
        std::ifstream file(filePath);
        std::string line;

        if (!file) {
            createFile(filePath);
            return;
        }

        std::vector<std::string> lines;
        int lineCount = 0;

        while (std::getline(file, line)) {
            lines.push_back(line);
            lineCount++;

            if (lineCount == 5) {
                processLines(lines);
                lines.clear();
                lineCount = 0;
            }
        }

        if (!lines.empty()) {
            processLines(lines);
        }

        file.close();
    }
    void processLines(const std::vector<std::string>& lines) {
        std::vector<std::string> processedLines;

        for (const std::string& line : lines) {
            std::vector<std::string> attributes;
            std::string attribute;
            std::string processedLine;

            for (char c : line) {
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
                std::string processedAttr = processAttribute(attr);
                processedLine += attr + processedAttr + "#";
            }

            processedLines.push_back(processedLine);
        }

        storeLines(processedLines);
    }
    std::string processAttribute(const std::string& attribute) {
        std::string processedAttr;

        // Identificar tipo de variable y cantidad de caracteres
        if (attribute.find_first_not_of("0123456789") == std::string::npos) {
            processedAttr = "<int>";
        } else {
            processedAttr = "<char>";
        }

        processedAttr += std::to_string(attribute.length());

        return processedAttr;
    }
    void storeLines(const std::vector<std::string>& lines) {
        // Leer el archivo de respaldo "backup"
        std::ifstream backupFile("backup.txt");
        if (backupFile.is_open()) {
            backupFile >> currentPlato >> currentSuperficie >> currentPista >> currentSector;
            backupFile.close();
        }

        int plato = currentPlato;
        int superficie = currentSuperficie;
        int pista = currentPista;
        int sector = currentSector;

        std::ofstream directoryFile("DirectorioDeBloques.txt", std::ios_base::app);
        int fileCount = 0;
        int blockCount = 0;
        int num = rand() % 10;

        std::string blockName;
        int linenumber=0;

        for (const std::string& line : lines) {
            if (discog.isFull()) {
                std::cout << "Disco lleno" << std::endl;
                return;
            }

            std::string position = getPosition(plato, superficie, pista, sector);
            std::string fileName = position + ".txt";
            writeFile(fileName, line);
            linenumber++;
            discog.addLine(position);
            savePositionToFile(linenumber, position);


            sector++;
            if (sector > 30) {
                sector = 0;
                pista++;
                if (pista > 20) {
                    pista = 0;
                    superficie++;
                    if (superficie > 2) {
                        superficie = 0;
                        plato++;
                        if (plato > 4) {
                            std::cout << "Disco lleno" << std::endl;
                            return;
                        }
                    }
                }
            }

            fileCount++;
            blockName += fileName + "#";

            if (fileCount % 20 == 0) {
                if (!blockName.empty()) {
                    blockName.pop_back(); // Eliminar el último carácter '#' extra
                    if (directoryFile.is_open()) {
                        directoryFile << "Bloque" << num << "#" << blockName << std::endl;
                        blockCount++;
                    }
                    blockName.clear();
                }
            }



            std::ofstream backup("backup.txt");
            if (backup.is_open()) {
                backup << plato << " " << superficie << " " << pista << " " << sector;
                backup.close();
            }
        }

        if (!blockName.empty()) {
            blockName.pop_back(); // Eliminar el último carácter '#' extra
            if (directoryFile.is_open()) {
                directoryFile << "Bloque" << num << "#" << blockName << std::endl;
                directoryFile.close();
            }
        }
    }
    void savePositionToFile(int lineNumber, const std::string& position)
    {
        std::ofstream positionFile("pos_line.txt", std::ios::app);
        if (positionFile)
        {
            positionFile << lineNumber << "#" << position << std::endl;
            positionFile.close();
        }
        else
        {
            std::cout << "Error al abrir el archivo de posición." << std::endl;
            return;
        }
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

    void insert() {
        std::string filename;
        std::cout << "Ingrese el nombre del archivo de texto: ";
        std::cin >> filename;

        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "No se pudo abrir el archivo: " << filename << std::endl;
            return;
        }

        std::string attributes;
        std::getline(file, attributes);
        file.close();

        std::cout << "Atributos: " << attributes << std::endl;

        std::vector<std::string> newData;

        std::istringstream iss(attributes);
        std::string attribute;
        while (std::getline(iss, attribute, '#')) {
            std::string value;
            std::cout << "Ingrese el valor para " << attribute << ": ";
            std::cin >> value;
            newData.push_back(value);
        }

        std::ofstream outputFile(filename, std::ios::app);
        if (outputFile.is_open()) {
            std::ostringstream oss;
            for (const std::string& value : newData) {
                oss << value << "#";
            }
            std::string line = oss.str();
            line.pop_back(); // Eliminar el último símbolo numeral (#)
            outputFile << std::endl<< line << std::endl;
            outputFile.close();

            storeLines({line}); // Llamar a la función storeLines con la nueva línea en un vector
            std::cout << "Datos insertados correctamente." << std::endl;
        } else {
            std::cerr << "No se pudo abrir el archivo: " << filename << std::endl;
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

    void location() {
        std::ifstream directoryFile("DirectorioDeBloques.txt");
        if (directoryFile.is_open()) {
            std::string line;
            int blockNumber = 0;
            while (std::getline(directoryFile, line)) {
                size_t separatorPos = line.find('#');
                if (separatorPos != std::string::npos) {
                    std::string blockName = line.substr(0, separatorPos);
                    std::cout << "Bloque " << blockNumber << ": " << blockName << std::endl;
                }
                blockNumber++;
            }
            directoryFile.close();
        }

        // Pedir al usuario el número de bloque a consultar
        int blockChoice;
        std::cout << "Ingrese el número de bloque que desea consultar: ";
        std::cin >> blockChoice;

        std::ifstream directoryFile2("DirectorioDeBloques.txt");
        if (directoryFile2.is_open()) {
            std::string line;
            int currentBlock = 0;
            while (std::getline(directoryFile2, line)) {
                if (currentBlock == blockChoice) {
                    size_t separatorPos = line.find('#');
                    if (separatorPos != std::string::npos) {
                        std::string blockName = line.substr(0, separatorPos);
                        std::cout << "Contenido del Bloque " << blockChoice << ": " << blockName << std::endl;

                        std::string fileNames = line.substr(separatorPos + 1);
                        std::string fileName;
                        std::stringstream ss(fileNames); // Objeto stringstream para procesar los nombres de archivo
                        while (std::getline(ss, fileName, '#')) {
                            std::ifstream file(fileName);
                            if (file.is_open()) {
                                std::cout << "Contenido de " << fileName << ":" << std::endl;

                                std::string fileContent;
                                std::string fileLine;
                                while (std::getline(file, fileLine)) {
                                    fileContent += fileLine + "\n";
                                }
                                file.close();

                                std::cout << fileContent << std::endl;
                                std::cout << "Tamaño: " << fileContent.size() << " bytes" << std::endl;
                            }
                        }
                    }
                    break;
                }
                currentBlock++;
            }
            directoryFile2.close();
        }
    }
    void del() {
        std::string filename;
        std::cout << "Ingrese el nombre del archivo de texto: ";
        std::cin >> filename;

        std::ifstream inFile(filename);
        if (!inFile)
        {
            std::cout << "Error al abrir el archivo de texto." << std::endl;
            return;
        }

        std::vector<std::string> lines;
        std::string line;

        // Omitimos la primera línea que contiene los atributos
        std::getline(inFile, line);

        while (std::getline(inFile, line))
        {
            lines.push_back(line);
        }

        inFile.close();

        std::cout << "Ingrese el contenido a buscar: ";
        std::string searchContent;
        std::cin.ignore();
        std::getline(std::cin, searchContent);

        std::vector<int> matchingLines;

        for (int i = 0; i < lines.size(); i++)
        {
            if (lines[i].find(searchContent) != std::string::npos)
            {
                matchingLines.push_back(i);
                std::cout << "Opción " << matchingLines.size() << ": " << lines[i] << std::endl;
            }
        }

        if (matchingLines.empty())
        {
            std::cout << "No se encontraron coincidencias." << std::endl;
            return;
        }

        std::cout << "Ingrese el rango de líneas a eliminar (ejemplo: 1-3): ";
        std::string range;
        std::getline(std::cin, range);

        int startLine = std::stoi(range.substr(0, range.find('-')));
        int endLine = std::stoi(range.substr(range.find('-') + 1));

        std::cout << "¿Está seguro de eliminar las líneas " << startLine << " a " << endLine << "? (S/N): ";
        std::string confirmation;
        std::getline(std::cin, confirmation);

        if (confirmation == "S" || confirmation == "s")
        {
            std::ofstream outFile(filename);

            for (int i = 0; i < lines.size(); i++)
            {
                if (i < startLine - 1 || i > endLine - 1)
                {
                    outFile << lines[i] << std::endl;
                }
                else
                {
                    outFile << std::endl; // Reemplazamos la línea con un salto de línea
                }
            }

            outFile.close();

            std::ofstream logFile("del_log.txt", std::ios::app);

            for (int i = startLine - 1; i < endLine; i++)
            {
                int lineNumber = matchingLines[i];
                logFile << filename << "#" << lineNumber << std::endl;
            }

            logFile.close();

            std::cout << "Se han eliminado satisfactoriamente las líneas del archivo." << std::endl;

            std::ifstream posLineFile("pos_line.txt");
            std::string posLineContent;

            if (posLineFile)
            {
                std::stringstream buffer;
                buffer << posLineFile.rdbuf();
                posLineContent = buffer.str();
                posLineFile.close();
            }

            std::ofstream updatedPosLineFile("pos_line.txt");

            for (int i = startLine - 1; i < endLine; i++)
            {
                int lineNumber = matchingLines[i];
                std::string searchString = std::to_string(lineNumber);
                std::size_t pos = posLineContent.find(searchString);

                if (pos != std::string::npos)
                {
                    // Buscamos la posición correspondiente
                    std::string position = posLineContent.substr(pos - 8, 8);

                    // Eliminamos el registro del archivo pos_line.txt
                    posLineContent.replace(pos - 8, 9, "");

                    // Imprimimos el mensaje de eliminación
                    std::string blockNumber = position.substr(0, 2);
                    std::string surfaceNumber = position.substr(2, 2);
                    std::string trackNumber = position.substr(4, 2);
                    std::string sectorNumber = position.substr(6, 2);
                    std::cout << "Se eliminó satisfactoriamente el registro del " << sectorNumber << " sector de la "
                              << trackNumber << " pista del " << surfaceNumber << " superficie del " << blockNumber
                              << " plato del disco, perteneciente al bloque " << blockNumber << "." << std::endl;
                }
            }

            updatedPosLineFile << posLineContent;
            updatedPosLineFile.close();
        }
        else
        {
            std::cout << "Operación de eliminación cancelada." << std::endl;
        }
    }
};

int main() {
    Megatron megatron("titanic.txt");
    std::string query;
    std::string fileName;
    std::string search;
    while (true) {
        std::cout << "Ingrese una consulta (CREATE_DISK, READ, CREATE, INSERT, SELECT, LOCATION): ";
        std::getline(std::cin, query);

        if (query == "READ") {
            megatron.read();
        } else if (query == "CREATE") {
            std::cout << "Ingrese el nombre del archivo: ";
            std::getline(std::cin, fileName);
            megatron.create(fileName);
        } else if (query == "INSERT") {
            megatron.insert();
        } else if (query == "SELECT") {
            std::cout << "Ingrese el nombre del archivo: ";
            std::getline(std::cin, fileName);
            std::cout << "Ingrese la cadena de búsqueda: ";
            std::getline(std::cin, search);
            megatron.select(fileName, search);
        } else if (query == "LOCATION") {
            megatron.location();
        } else if (query == "CREATE_DISK"){
            megatron.create_disk();
        }else if (query == "DELETE"){
            megatron.del();
        }

        std::cout << "Continua (s/n)? ";
        std::string response;
        std::getline(std::cin, response);

        if (response != "s")
            break;
    }

    return 0;
}
