#include <iostream>
#include <vector>

class Disk {
private:
    int capacity;  // Capacidad del disco en bytes
    std::vector<std::vector<char>> platters;  // Platos del disco
    int sectorsPerTrack;  // Sectores por pista
    int tracksPerPlatter;  // Pistas por plato
    int bytesPerSector;  // Bytes por sector
    int currentSector;  // Sector actual para el siguiente acceso

public:
    Disk(int diskCapacity, int sectorSize, int sectorsPerTrack, int tracksPerPlatter) :
            capacity(diskCapacity),
            sectorsPerTrack(sectorsPerTrack),
            tracksPerPlatter(tracksPerPlatter),
            bytesPerSector(sectorSize),
            currentSector(0)
    {
        // Calcular el número de platos necesarios
        int numPlatters = (diskCapacity + (sectorsPerTrack * sectorSize * tracksPerPlatter) - 1) / (sectorsPerTrack * sectorSize * tracksPerPlatter);

        // Inicializar los platos del disco
        platters.resize(numPlatters);
        for (int i = 0; i < numPlatters; ++i) {
            platters[i].resize(sectorsPerTrack * tracksPerPlatter * bytesPerSector);
        }
    }

    void readData(int sector, int size) {
        if (sector < 0 || sector >= capacity || size <= 0) {
            std::cout << "Error: Sector o tamaño inválido." << std::endl;
            return;
        }

        int platterIndex = sector / (sectorsPerTrack * tracksPerPlatter);
        int trackIndex = (sector / sectorsPerTrack) % tracksPerPlatter;
        int sectorIndex = sector % sectorsPerTrack;

        std::cout << "Leyendo " << size << " bytes desde el sector " << sector << std::endl;
        // Aquí se realizaría la lectura de los datos del disco utilizando los índices calculados
        std::vector<char>& platter = platters[platterIndex];
        for (int i = 0; i < size; ++i) {
            char data = platter[(trackIndex * sectorsPerTrack + sectorIndex) * bytesPerSector + i];
            // Hacer algo con los datos leídos
            std::cout << data;
        }
        std::cout << std::endl;
    }

    void writeData(int sector, const std::vector<char>& newData) {
        if (sector < 0 || sector >= capacity) {
            std::cout << "Error: Sector inválido." << std::endl;
            return;
        }

        int platterIndex = sector / (sectorsPerTrack * tracksPerPlatter);
        int trackIndex = (sector / sectorsPerTrack) % tracksPerPlatter;
        int sectorIndex = sector % sectorsPerTrack;

        std::cout << "Escribiendo " << newData.size() << " bytes en el sector " << sector << std::endl;
        // Aquí se realizaría la escritura de los datos en el disco utilizando los índices calculados
        std::vector<char>& platter = platters[platterIndex];
        for (int i = 0; i < newData.size(); ++i) {
            platter[(trackIndex * sectorsPerTrack + sectorIndex) * bytesPerSector + i] = newData[i];
        }
    }

    void seekToSector(int sector) {
        if (sector < 0 || sector >= capacity) {
            std::cout << "Error: Sector inválido." << std::endl;
            return;
        }

        currentSector = sector;
        std::cout << "Moviendo el cabezal al sector " << currentSector << std::endl;
        // Aquí se realizaría el movimiento del cabezal del disco a la posición del sector especificado
    }

    void printDiskInfo() {
        std::cout << "Capacidad del disco: " << capacity << " bytes" << std::endl;
        std::cout << "Tamaño de sector: " << bytesPerSector << " bytes" << std::endl;
        std::cout << "Sectores por pista: " << sectorsPerTrack << std::endl;
        std::cout << "Pistas por plato: " << tracksPerPlatter << std::endl;
        std::cout << "Número de platos: " << platters.size() << std::endl;
    }
};

int main() {
    int diskCapacity = 1024;  // Capacidad del disco en bytes
    int sectorSize = 512;  // Tamaño de sector en bytes
    int sectorsPerTrack = 10;  // Sectores por pista
    int tracksPerPlatter = 20;  // Pistas por plato

    Disk disk(diskCapacity, sectorSize, sectorsPerTrack, tracksPerPlatter);

    disk.printDiskInfo();  // Imprimir información del disco

    std::vector<char> newData = { 'A', 'B', 'C', 'D', 'E' };

    disk.seekToSector(5);  // Mover el cabezal al sector 5
    disk.writeData(5, newData);  // Escribir datos en el sector 5

    disk.seekToSector(5);  // Mover el cabezal al sector 5
    disk.readData(5, newData.size());  // Leer datos del sector 5

    return 0;
}
