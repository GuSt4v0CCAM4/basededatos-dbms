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
        int weightLine = 0;
        ofstream dirFile("weightline.txt");

            while (getline(iss, attribute, ',')) {
                int opc=0;
                string attributeName = attribute, attributeType;
                float attributeLength;

                cout << "Elige el tipo de variable para el atributo '" << attributeName << "': "<<endl;
                cout << "1. VARCHAR" << endl;
                cout << "2. INT" << endl;
                cout << "3. DECIMAL" << endl;
                cout <<"Opcion: ";
                cin >> opc;

                switch (opc) {
                    case 1: attributeType = "varchar";
                    break;
                    case 2: attributeType = "int";
                    break;
                    case 3: attributeType = "decimal";
                    break;
                    default:
                        cout<<"Opcion incorrecta."<<endl;
                        break;
                }

                cout << "Ingrese la longitud para el atributo '" << attributeName << "': "<<endl;
                cin >> attributeLength;
                if (attributeType == "decimal") {
                    weightLine += (attributeLength + 2);
                }else if(attributeType == "varchar") {
                    weightLine += (attributeLength + 1);
                }else if(attributeType == "int") {
                    weightLine += 4;
                }


                Attribute attr(attributeName, attributeType, attributeLength);
                attributes.push_back(attr);
                cout<<weightLine<<endl;

            }
        dirFile << weightLine;


        dirFile.close();

        cout << "El resultado se ha guardado en weightline.txt." << endl;
    }



    void processEsquema(const string& inputFile, const string& outputFile) {
        ifstream esquemaFile(outputFile);
        if(esquemaFile) {
            cout<<"Ya existe el archivo esquema perteneciente a "<<inputFile<<" asi que se continuara..."<<endl;
            return;
        } else{
            ifstream input(inputFile);
            ofstream output(outputFile);

            if (!input || !output) {
                cout << "Error al abrir los archivos." << endl;
                return;
            }

            string header, dataLine;

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

    }

};

#endif
