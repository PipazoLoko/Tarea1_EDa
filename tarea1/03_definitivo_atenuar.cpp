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

// Funcion para atenuar la imagen con valor arbitrario
void operacion3(Imagen* imagen, float valor){
    int alto = imagen->height, ancho = imagen->width, canales = imagen->channels;
    unsigned char* array_de_pixeles = imagen->data;
    for(int indice = 0; indice < alto * ancho * canales; ++indice){
        float v_atenuado = array_de_pixeles[indice] * valor;
        imagen->data[indice] = (unsigned char)v_atenuado;
    }
}

// Guardar imagen en memoria local
void save(Imagen* img, const char* filename) {
    stbi_write_png(filename, img->width, img->height, img->channels, img->data, img->width * img->channels);
    cout << "Imagen guardada:  " << filename << "\n";
}

int main() {
    //Cargamos la imagen
    Imagen* img = load("Pikachu.png");
    operacion3(img, (float)(0.5));
    // Almacenamos el resultado
    save(img, "Pikachu4.png");
    delete[] img->data;
    delete img;
}
