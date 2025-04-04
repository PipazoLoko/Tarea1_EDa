#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

// importacion de librerias. Los archivos correspondientes deben estar en el mismo directorio que el .cpp.
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>

using namespace std;

// Struct que almacena la data de una imagen
struct Imagen{
    unsigned char* data;
    int width, height, channels;
};

// abrimos la imagen desde la memoria local
Imagen* load(const char* filename) {
    Imagen* img = new Imagen();
    img->data = stbi_load(filename, &img->width, &img->height, &img->channels, 0);
    return img;
}

// guardardamos la imagen en la memoria local
void save(Imagen* img, const char* filename) {
    stbi_write_png(filename, img->width, img->height, img->channels, img->data, img->width * img->channels);
    cout << "Imagen guardada:  " << filename << "\n";
}

// funcion para mdificar la imagen (blanco/negro)
void operacion4(Imagen* imagen, int limite) {
    // aseguramos el límite [0, 255]
    if (limite < 0) limite = 0;
    else if (limite > 255) limite = 255;


    // procesamos cada píxel
    for (int i = 0; i < imagen->width * imagen->height; ++i) {
        // calcular la posición base del píxel
        int indice_base = i * imagen->channels;

        // Convertir a escala de grises si es necesario
        float valor_gris;
        if (imagen->channels >= 3) {  // RGB
            valor_gris = 0.299f * imagen->data[indice_base] +   // R
                         0.587f * imagen->data[indice_base + 1] + // G
                         0.114f * imagen->data[indice_base + 2];  // B
        } else {  // Escala de grises (1 o 2 canales)
            valor_gris = imagen->data[indice_base];
        }
        unsigned char nuevo_valor; // Determinar nuevo valor (blanco o negro)

        if (valor_gris > limite) {
            nuevo_valor = 255;
        } else {
            nuevo_valor = 0;   
        }

        for (int c = 0; c < min(3, imagen->channels); ++c) {//aplicar a RGB
            imagen->data[indice_base + c] = nuevo_valor;
        }
        }
    }
int main() {
    // Cargar imagen
    Imagen* img = load("Pikachu5.png");

    // Umbralizar con límite de 128 (puedes cambiar este valor)
    int limite = 100;
    operacion4(img, limite);
    save(img, "Pikachu5.png");
    delete[] img->data;// liberamos memoria
    delete img;

    return 0;
}
