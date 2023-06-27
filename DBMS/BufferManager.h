#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H
#include "iostream"
#include "fstream"
#include "sstream"
#include "vector"
using namespace std;
namespace fs = filesystem;
class BufferManager{
private:
    int numFrames;
    int FrameSize;
    int capacidadBufferPool;
public:
    BufferManager(int num, int size) : numFrames(num), FrameSize(size) {
        capacidadBufferPool = numFrames * FrameSize;
    }
    void createBufferPool(string diskname){
        string carpetaBufferPool = diskname + "/BufferPool";
        fs::create_directory(carpetaBufferPool);
        ofstream infoBufferPool(carpetaBufferPool + "/info.txt");
        infoBufferPool << numFrames << " " << FrameSize << endl;
        for (int frame = 0; frame < numFrames; ++frame) {
            string carpetaFrame = carpetaBufferPool + "/Frame " + to_string(frame + 1);
            fs::create_directory(carpetaFrame);
        }

        cout<<"Se creo satisfactoriamente el BufferPool de " << numFrames << " Frames, con una capacidad total de " << capacidadBufferPool << " Bytes."<<endl;
        /*if(paginasExists(diskname)){
            cout<<"Existen paginas en el disco."<<endl;
        }else{
            cout<<"No existen paginas en el disco."<<endl;
        }*/
    };
    bool paginasExists(string diskname){
        for (int frame = 0; frame < numFrames; ++frame) {
            string carpetaFrame = diskname + "/BufferPool/Frame " + to_string(frame + 1);
            bool archivosTxtEncontrados = false;

            for (const auto& entrada : fs::directory_iterator(carpetaFrame)) {
                if (!fs::is_directory(entrada) && entrada.path().extension() == ".txt") {
                    archivosTxtEncontrados = true;
                    break;
                }
            }

            if (!archivosTxtEncontrados) {
                return false;
            }
        }

        return true;
    }

};


#endif
