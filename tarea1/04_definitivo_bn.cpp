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
        int suma_RGB = array_de_pixeles[indiceR] + array_de_pixeles[indiceR + 1] + array_de_pixeles[indiceR + 2]; // calculamos el valor del pixel 'p' = suma de sus valores RGB. util para el array unidimensional
        unsigned char valor_blanco_negro; // creamos una variable que pude tomar valor de negro o blanco
        if (suma_RGB > limite){ //asigna segun arriba o no arriba del limite
            valor_blanco_negro = 255; //blanco
        }else{
            valor_blanco_negro = 0; //negro
        }
        for(int c = 0 ; c < canales; ++c){ //itero canales por pixel 'p' y le asigno el blanco o negro calculado previamente
            array_de_pixeles[indiceR + c] = valor_blanco_negro ;
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
    operacion4(img, 400);
    // Almacenamos el resultado
    save(img, "Pikachu51.png");
    delete[] img->data;
    delete img;
}
