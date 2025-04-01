#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

// Importacion de librerias. Los archivos correspondientes deben estar en el mismo directorio que el .cpp.
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>

using namespace std;

// Struct que almacena la data de una imagen
struct Imagen{
    unsigned char* data;
    int width, height, channels; //ancho, altura y canales
};

// Abrir imagen desde memoria local
Imagen* load(const char* filename) {
    Imagen* img = new Imagen();
    img->data = stbi_load(filename, &img->width, &img->height, &img->channels, 0);
    return img;
}

Imagen* rotar(Imagen* original) {
    //invertido
    Imagen* rotada = new Imagen();
    rotada->width = original->height;//alto sera ancho
    rotada->height = original->width;//ancho sera alto
    rotada->channels = original->channels;//mantendremos los canales
    
    //asignamos memoria
    rotada->data = new unsigned char[rotada->width * rotada->height * rotada->channels];

    for (int h = 0; h < original->height; ++h) { //recorremos cada fila hasta llegar a la altura
        for (int w = 0; w < original->width; ++w) {//recorremos cada columna hasta llegar al ancho
            int pos_original = (h* original->width +w) * original->channels; //calculamos los indices exactos
            int pos_rotada = (w* rotada->width + (rotada->width -1 -h)) * rotada->channels;//calculamos indices ya rotados
        
            for (int c = 0; c < original->channels; ++c) {
                rotada->data[pos_rotada + c] = original->data[pos_original + c];//copiamos canales
            }
        }
    }
    
    return rotada;
}

// Guardar imagen en memoria local
void save(Imagen* img, const char* filename) {
    stbi_write_png(filename, img->width, img->height, img->channels, img->data, img->width * img->channels);
    cout << "Imagen guardada:  " << filename << "\n";
}

int main() {
    //cargamos imagen
    Imagen* pikachu = load("Pikachu3.png");
    Imagen* rotada = rotar(pikachu);//rotamos
    save(rotada, "Pikachu3.png");//guardamos

    //liberamos memoria
    delete[] pikachu->data;
    delete pikachu;
    delete[] rotada->data;
    delete rotada;

    return 0;
}



