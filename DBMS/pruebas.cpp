#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class Attribute {
public:
    std::string name;
    size_t length;

    Attribute(const std::string& attributeName, size_t attributeLength) {
        name = attributeName;
        length = attributeLength;
    }
};

void writeSchema(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream input(inputFile);
    std::ofstream output(outputFile);

    if (!input || !output) {
        std::cout << "Error al abrir los archivos." << std::endl;
        return;
    }

    std::vector<Attribute> attributes;
    std::vector<size_t> attributeLengths;

    std::string header;
    std::getline(input, header);
    std::istringstream headerStream(header);

    std::string attributeName;
    while (std::getline(headerStream, attributeName, ',')) {
        attributes.push_back(Attribute(attributeName, 0));
        attributeLengths.push_back(0);
    }

    std::string line;
    while (std::getline(input, line)) {
        std::istringstream lineStream(line);
        std::string value;
        size_t attributeIndex = 0;

        while (std::getline(lineStream, value, ',')) {
            size_t length = value.length();
            if (length > attributeLengths[attributeIndex]) {
                attributeLengths[attributeIndex] = length;
            }
            attributeIndex++;
        }
    }

    for (size_t i = 0; i < attributes.size(); i++) {
        output << attributes[i].name << " (" << attributeLengths[i] << ")" << std::endl;
    }

    output.close();

    std::cout << "Se ha creado el archivo de esquema exitosamente." << std::endl;
}

int main() {
    std::string inputFile = "titanic.txt";
    std::string outputFile = "schema.txt";

    writeSchema(inputFile, outputFile);


    return 0;
}
