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
//Funcion para asignar blanco arriba del limite, negro de lo contrario
void operacion4(Imagen* imagen, int limite){
    int alto = imagen->height, ancho = imagen->width, canales = imagen->channels;
    unsigned char* array_de_pixeles = imagen->data; 
    for(int p = 0; p < alto * ancho; ++p){ // iteramos cada pixel 'p' (sin canal o *3)
        int indiceR = p * canales; //calculamos el R de cada pixel 'p'
        for (int c = 0; c < canales; ++c){ // Evaluamos por canal y cambiamos 255 o 0 por canal
            if (array_de_pixeles[indiceR + c] > limite){
                array_de_pixeles[indiceR + c ] = 255 ;
            }else{
                array_de_pixeles[indiceR + c] = 0;
            }
        }
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
    operacion4(img, 80);
    // Almacenamos el resultado
    save(img, "Pikachu_error_bn.png");
    delete[] img->data;
    delete img;
}
