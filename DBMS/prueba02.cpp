#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

class Attribute {
public:
    size_t position;
    size_t length;

    Attribute(size_t attributePosition, size_t attributeLength) {
        position = attributePosition;
        length = attributeLength;
    }
};

void writeSchema(const string& inputFile, const string& outputFile) {
    ifstream input(inputFile);
    ofstream output(outputFile);

    if (!input || !output) {
        cout << "Error al abrir los archivos." << endl;
        return;
    }

    vector<Attribute> variableAttributes;

    string line;
    size_t recordIndex = 1;

    while (getline(input, line)) {
        istringstream lineStream(line); //creamos un objeto de tipo istringstream, para leer y analizar
        string value;
        size_t position = 1;
        size_t sum = 0; // Variable para almacenar la sumatoria de longitudes por línea

        while (getline(lineStream, value, ',')) {
            if (!value.empty() && value.front() == '"' && value.back() != '"') {
                string nextValue;
                while (getline(lineStream, nextValue, ',')) {
                    value += "," + nextValue;
                    if (nextValue.back() == '"') {
                        break;
                    }
                }
            }

            // Eliminar comillas iniciales y finales si están presentes
            if (!value.empty() && value.front() == '"' && value.back() == '"') {
                value = value.substr(1, value.length() - 2);
            }

            if (recordIndex == 1) {
                output << position << "|";
            }

            if (!value.empty()) {
                output << value.length();
                sum += value.length(); // Sumar la longitud actual al total de la línea
            }

            output << "|";

            position++;
        }

        output << " #" << sum << "#" << endl; // Imprimir la sumatoria de longitudes en la línea

        if (recordIndex == 1) {
            for (size_t i = 1; i < position; i++) {
                if (i != position - 1) {
                    output << i << "|";
                }
                else {
                    output << i;
                }
            }
            output << endl;
        }

        recordIndex++;
    }

    output.close();

    cout << "Se ha creado el archivo de esquema exitosamente." << endl;
}


void removeFirstLine(const string& filename) {
    ifstream input(filename);
    if (!input) {
        cout << "Error al abrir el archivo." << endl;
        return;
    }

    vector<string> lines;
    string line;

    // Leer todas las líneas del archivo
    while (getline(input, line)) {
        lines.push_back(line);
    }

    input.close();

    // Eliminar la primera línea
    lines.erase(lines.begin());

    ofstream output(filename);
    if (!output) {
        cout << "Error al abrir el archivo." << endl;
        return;
    }

    // Escribir las líneas restantes en el archivo
    for (const string& line : lines) {
        output << line << endl;
    }

    output.close();

    cout << "Se ha eliminado la primera línea del archivo exitosamente." << endl;
}

int main() {
    string inputFile = "titanic.txt";
    string outputFile = "schema.txt";

    writeSchema(inputFile, outputFile);
    removeFirstLine(outputFile);
    removeFirstLine(outputFile);

    return 0;
}
