#include <iostream>
#include <vector>
#include "fstream"
#include "sstream"
using namespace std;

class ClockBufferManager {
private:
    struct Frame {
        int frameID;
        int pageID;
        bool dirtyBit;
        int pinCount;
        bool refBit;
    };
    std::vector<Frame> buffer;
    int clockHand;

public:
    ClockBufferManager(int numFrames) : buffer(numFrames), clockHand(0) {
        // Initialize all frames with refBit = false and other values as needed.
        for (int i = 0; i < numFrames; ++i) {
            buffer[i].frameID = i;
            buffer[i].pageID = -1; // Set an invalid initial page ID.
            buffer[i].dirtyBit = false;
            buffer[i].pinCount = 0;
            buffer[i].refBit = false;
        }
    }

    void readPage(int pageID) {
        // Check if the page is already in the buffer pool.
        for (auto& frame : buffer) {
            if (frame.pageID == pageID) {
                frame.refBit = true; // Set ref bit = 1 without moving the clock hand.
                return;
            }
        }

        // Find the first unpinned frame with refBit = 0 using clock algorithm.
        while (true) {
            Frame& currentFrame = buffer[clockHand];
            if (currentFrame.pinCount == 0) {
                if (currentFrame.refBit) {
                    currentFrame.refBit = false;
                } else {
                    // Replace the existing page (write to disk if dirty) and read the new page.
                    currentFrame.pageID = pageID;
                    currentFrame.refBit = true;
                    // Additional logic can be added here to handle dirty bit and disk write.

                    // Move the clock hand to the next frame.
                    clockHand = (clockHand + 1) % buffer.size();
                    return;
                }
            }
            // Move the clock hand to the next frame.
            clockHand = (clockHand + 1) % buffer.size();
        }
    }
    void printBuffer(){
        cout << "Clock: "<<endl;
        int start = clockHand;
        for (int i = 0; i < buffer.size(); ++i) {
            std::cout << "Frame " << buffer[start].frameID << " | Page ID " << buffer[start].pageID << " | Ref Bit " << buffer[start].refBit << " | Dirty Bit " << buffer[start].dirtyBit << " | Pin Count " << buffer[start].pinCount << endl;
            start = (start + 1) % buffer.size();
        }
        std::cout << std::endl;
    }
    void Algoritmo_Clock(){

    }
    void menu_IEM(){
        //Para la impresion de un sector es necesario datos como
        //la superficie, pista, sector donde se ubica
        int tamanioSector = 0, sector, pista, superficie, plato;
        cout << "Ingrese el numero de plato: ";
        cin >> plato;
        cout << "Ingrese el numero de superficie: ";
        cin >> superficie;
        cout << "Ingrese el numero de pista: ";
        cin >> pista;
        cout << "Ingrese el numero de sector: ";
        cin >> sector;
        string diskname = "aldechi"; //eliminar
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

    }

    // Additional methods can be added to manage pin counts and other functionalities.
};

int main() {
    // Example usage:
    int capacity,N_bloque, n;
    string diskname = "aldechi";
    string fileNumBloques = diskname + "/infoBloque.txt";
    ifstream sectors(fileNumBloques);
    sectors >> n;
    sectors.close();
    string infoFrame = diskname + "/BufferPool/info.txt";
    ifstream frame(infoFrame);
    frame >> capacity;
    frame.close();
    ClockBufferManager clockBufferManager(capacity);
    while(true){
        cout<<"Numero de bloque a ingresar en el buffer manager: ";cin>>N_bloque;
        if(N_bloque<1){
            cout<<"buffer pool bloques: "<<endl;
            clockBufferManager.printBuffer();

            int n;
            cout<<"numero de bloque a modificar: ";cin>>n;
            string line;
            ifstream archivo(diskname+"/DirectorioDeBloques.txt");
            for(int i=0;i<n;i++){
                getline(archivo,line);
            }
            cout<<line<<endl;
            cin.ignore();
            int opc;
            cout<<"\nSe modificara algun registro\n1.Si\n2.No: ";cin>>opc;
            if(opc == 1){
                ClockBufferManager menu_IEM();
            }else{
                break;
            }
        }
        clockBufferManager.readPage(N_bloque);
        clockBufferManager.printBuffer();
    }
   /* int numFrames = 5;
    ClockBufferManager bufferManager(numFrames);

    // Simulate some page reads.
    bufferManager.readPage(1);
    bufferManager.readPage(3);
    bufferManager.readPage(6);
    bufferManager.readPage(10);
    bufferManager.readPage(3); // Page 3 is already in the buffer, ref bit set to 1.
    bufferManager.readPage(5); // Replaces a frame due to clock algorithm.

     */
    return 0;
}

/*
 class ClockBufferManager {
private:
    int capacity; // Capacidad del buffer pool
    unordered_map<int, pair<bool, bool>> buffer; // Mapa para almacenar las páginas en el buffer pool
    vector<int> clock; // Lista de páginas en el buffer pool en orden circular
    int hand; // Posición actual del puntero del algoritmo CLOCK

public:
    ClockBufferManager(int capacity) : capacity(capacity), hand(0) {
        clock.resize(capacity, -1); // Inicializar la lista con páginas vacías
    }

    void refer(int page) {
        // Verificar si la página ya está en el buffer pool
        if (buffer.find(page) != buffer.end()) {
            buffer[page].first = true; // Establecer el bit de referencia
        } else {
            // La página no está en el buffer pool

            // Verificar si el buffer pool está lleno
            if (buffer.size() == capacity) {
                // El buffer pool está lleno, se debe reemplazar una página

                while (true) {
                    int currentPage = clock[hand]; // Obtener la página actual

                    if (buffer[currentPage].first) {
                        // La página tiene el bit de referencia establecido
                        buffer[currentPage].first = false; // Resetear el bit de referencia
                    } else {
                        // La página no tiene el bit de referencia establecido

                        buffer.erase(currentPage); // Eliminar la página del buffer pool
                        buffer[page] = make_pair(true, true); // Agregar la nueva página al buffer pool

                        clock[hand] = page; // Actualizar la página en la posición actual del puntero
                        hand = (hand + 1) % capacity; // Mover el puntero al siguiente

                        cout<<"Reemplazar página " << currentPage << " con página: " << page << endl;
                        break;
                    }

                    hand = (hand + 1) % capacity; // Mover el puntero al siguiente
                }
            } else {
                // El buffer pool aún tiene capacidad disponible

                buffer[page] = make_pair(true, true); // Agregar la página al buffer pool

                clock[hand] = page; // Actualizar la página en la posición actual del puntero
                hand = (hand + 1) % capacity; // Mover el puntero al siguiente
            }
        }
    }
    void printBuffer() {
        cout << "Clock columna: ";
        int start = hand;
        while (true) {
            cout << clock[start] << " ";
            start = (start + 1) % capacity;
            if (start == hand)
                break;
        }
        cout << endl;
    }
};
 void Algoritmo_CLOCK(){
        int capacity,N_bloque, n;
        string fileNumBloques = diskname + "/infoBloque.txt";
        ifstream sectors(fileNumBloques);
        sectors >> n;
        sectors.close();
        string infoFrame = diskname + "/BufferPool/info.txt";
        ifstream frame(infoFrame);
        frame >> capacity;
        frame.close();
        //cout<<capacity;
        //cout<<"Capacidad del Buffer manager: ";cin>>capacity;
        //int accessPattern[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
        ClockBufferManager clockBufferManager(capacity);
        while(true){
            cout<<"Numero de bloque a ingresar en el buffer manager: ";cin>>N_bloque;
            if(N_bloque<1){
                cout<<"buffer pool bloques: "<<endl;
                clockBufferManager.printBuffer();

                int n;
                cout<<"numero de bloque a modificar: ";cin>>n;
                string line;
                ifstream archivo(diskname+"/DirectorioDeBloques.txt");
                for(int i=0;i<n;i++){
                    getline(archivo,line);
                }
                cout<<line<<endl;
                cin.ignore();
                int opc;
                cout<<"\nSe modificara algun registro\n1.Si\n2.No: ";cin>>opc;
                if(opc == 1){
                    menu_IEM();
                }else{
                    break;
                }
            }
            clockBufferManager.refer(N_bloque);
            clockBufferManager.printBuffer();
        }
    }
 * */