#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

// Importacion de librerias. Los archivos correspondientes deben estar en el mismo directorio que el .cpp.
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>

using namespace std;

// Struct que almacena la data de una imagen
struct Imagen
{
    unsigned char *data;
    int width, height, channels;
};

Imagen *rotada;
Imagen *original;

// Abrir imagen desde memoria local
Imagen *load(const char *filename){
Imagen *img = new Imagen();
img->data = stbi_load(filename, &img->width, &img->height, &img->channels, 0);
return img;
}
// Guardar imagen en memoria local
void save(Imagen * img, const char *filename)
{
    stbi_write_png(filename, img->width, img->height, img->channels, img->data, img->width * img->channels);
    cout << "Imagen guardada:  " << filename << "\n";
}

// Función para rotar imagen 90
Imagen *rotate90(const Imagen *original){ 
    rotada->width = original->height;
    rotada->height = original->width;
    rotada->channels = original->channels;

    // Asignar memoria para la imagen rotada
    rotada->data = new unsigned char[rotada->width * rotada->height * rotada->channels];

    // Rotación
    for (int h = 0; h < original->height; h++) {
        for (int w = 0; w < original->width; w++) {
            for (int c = 0; c < original->channels; c++) {
                // Calcular posición en la imagen original
                int iOriginal = (h * original->width + w) * original->channels + c;

                // Calcular posición en la imagen rotada
                int iRotado = (w * rotada->width + (rotada->width - 1 - h)) * rotada->channels + c;

                // Copiar el valor del píxel
                rotada->data[iRotado] = original->data[iOriginal];      
            }
        }   
}

    return rotada;
}

int main(){
    // Cargamos la imagen
    Imagen *img = load("Pikachu.png");

    // Modificamos un par de pixeles en el borde superior de la imagen como ejemplo
    img->data[101] = 0;
    img->data[102] = 0;
    img->data[103] = 0;
    img->data[104] = 0;
    img->data[105] = 0;
    img->data[106] = 0;

    // Rotar la imagen 90 grados
    Imagen *img2 = rotate90(img);

    // Almacenamos el resultado
    save(img2, "Pikachu_output.png");
    delete[] img->data;
    delete img;
    delete[] img2->data; // Libera los datos de la imagen rotada
    delete img2;

return 0;
}

