#ifndef ESQUEMA_H
#define ESQUEMA_H
#include "iostream"
#include "fstream"
#include "sstream"
#include "vector"

class Attribute {
public:
    string name;
    string type;
    size_t length;

    Attribute(const string& attributeName, const string& attributeType, size_t attributeLength) {
        name = attributeName;
        type = attributeType;
        length = attributeLength;
    }
};

class Esquema {
private:
    vector<Attribute> attributes;

public:
    void extractAttributes(const string& header) {
        istringstream iss(header);
        string attribute;

        while (getline(iss, attribute, ',')) {
            string attributeName = attribute;
            string attributeType;
            size_t attributeLength;

            cout << "Ingrese el tipo de variable para el atributo '" << attributeName << "': ";
            cin >> attributeType;

            cout << "Ingrese la longitud para el atributo '" << attributeName << "': ";
            cin >> attributeLength;

            Attribute attr(attributeName, attributeType, attributeLength);
            attributes.push_back(attr);
        }
    }

    string convertToFixedLength(const string& line) {
        istringstream iss(line);
        string value;
        ostringstream oss;

        int i = 0;
        while (getline(iss, value, ',')) {
            const Attribute& attr = attributes[i++];
            string fixedValue = value;
            fixedValue.resize(attr.length, ' ');

            oss << fixedValue;
        }

        return oss.str();
    }

    void processEsquema(const string& inputFile, const string& outputFile) {
        ifstream input(inputFile);
        ofstream output(outputFile);

        if (!input || !output) {
            cout << "Error al abrir los archivos." << endl;
            return;
        }

        string header;
        string dataLine;

        // Leer la primera línea del archivo CSV para obtener los nombres de los atributos
        if (getline(input, header)) {
            extractAttributes(header);

            // Escribir los atributos junto con sus tipos de variables y longitud en el archivo de esquema
            for (const Attribute& attr : attributes) {
                output << attr.name << " (" << attr.type << ", longitud: " << attr.length << ")" << endl;
            }


            cout << "Se ha creado el archivo de esquema exitosamente." << endl;
        } else {
            cout << "No se encontraron atributos en el archivo CSV." << endl;
        }

        input.close();
        output.close();
    }
};

#endif
