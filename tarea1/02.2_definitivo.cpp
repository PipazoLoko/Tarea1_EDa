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
    int width, height, channels;
};

// Abrir imagen desde memoria local
Imagen* load(const char* filename) {
    Imagen* img = new Imagen();
    img->data = stbi_load(filename, &img->width, &img->height, &img->channels, 3);
    img->channels = 3;
    return img;
}

//Funcion rotar 90 antihorario
void operacion_2(Imagen* imagen){
    int alto_original = imagen->height, ancho_original = imagen->width, canales = imagen->channels; //alto, ancho y canales de original
    int alto_rotado = ancho_original, ancho_rotado = alto_original;
    unsigned char* array_pixeles_rotados = new unsigned char[alto_rotado * ancho_rotado * canales]; // puntero al array de canales RGB por cantidad de pixeles totales, como es cuadrada son iguales
    for (int fila = 0 ; fila < ancho_original; ++fila){
        for(int columna = 0; columna < alto_original; ++columna){
            int indice_original = (fila * ancho_original + columna) * canales; 
            int indice_rotado = (columna * ancho_rotado + (ancho_rotado - 1 - fila)) * canales;
        for(int i = 0; i < canales; ++i){ // copian los valores RGB al rotado que esta vacio 
            array_pixeles_rotados[indice_rotado + i] = imagen->data[indice_original + i];
        }
        }
    }
    imagen->data = array_pixeles_rotados; // asigna pixeles rotados al original
}

// Guardar imagen en memoria local
void save(Imagen* img, const char* filename) {
    stbi_write_png(filename, img->width, img->height, img->channels, img->data, img->width * img->channels);
    cout << "Imagen guardada:  " << filename << "\n";
}

int main() {
    //Cargamos la imagen
    Imagen* img = load("Pikachu3.png");
    operacion_2(img);
    save(img, "Pikachu3.png");
    delete[] img->data; // función de la librería stb_image.h que libera memoria de la funcion stbi_load.
    delete img; //elimina el objeto, la estructura o el "esqueleto", mientras que la anterior deletea su contenido
}
