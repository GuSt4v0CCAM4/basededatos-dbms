#include <iostream>
#include <unordered_map>
#include <list>

class BufferManager {
public:
    BufferManager(int bufferSize) : bufferSize(bufferSize) {}

    void ReadPage(int pageNumber) {
        if (pageTable.count(pageNumber) == 0) {
            LoadPage(pageNumber);
        }
        UpdateLRU(pageNumber);
        std::cout << "Leyendo la pagina " << pageNumber << " desde el buffer pool." << std::endl;
    }

    void WritePage(int pageNumber) {
        if (pageTable.count(pageNumber) == 0) {
            LoadPage(pageNumber);
        }
        UpdateLRU(pageNumber);
        pageTable[pageNumber] = true; // Mark the page as dirty
        std::cout << "Escribiendo la pagina " << pageNumber << " para el buffer pool." << std::endl;
    }

private:
    int bufferSize;
    std::unordered_map<int, bool> pageTable; // Tracks loaded pages and their dirty status
    std::list<int> lruList; // Doubly-linked list to maintain the LRU order

    void LoadPage(int pageNumber) {
        if (lruList.size() >= bufferSize) {
            int victimPage = lruList.back();
            lruList.pop_back();
            pageTable.erase(victimPage);
            std::cout << "Liberar pagina " << victimPage << " del buffer pool." << std::endl;
        }
        lruList.push_front(pageNumber);
        pageTable[pageNumber] = false; // Mark the page as clean
        std::cout << "Cargando pagina " << pageNumber << " en el buffer pool." << std::endl;
    }

    void UpdateLRU(int pageNumber) {
        lruList.remove(pageNumber);
        lruList.push_front(pageNumber);
    }
};

int main() {
    BufferManager bufferManager(3); // Buffer size of 3 pages

    bufferManager.ReadPage(1);
    bufferManager.ReadPage(2);
    bufferManager.WritePage(3);
    bufferManager.ReadPage(1);
    bufferManager.WritePage(4);
    bufferManager.ReadPage(2);
    bufferManager.ReadPage(3);
    bufferManager.ReadPage(4);

    return 0;
}
