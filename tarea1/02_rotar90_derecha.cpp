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

// Función para rotar 90 grados a la derecha
Imagen* rotar90Derecha(Imagen* original) {
    // Crear nueva imagen con dimensiones invertidas
    Imagen* rotada = new Imagen();
    rotada->width = original->height;   // El alto original será el nuevo ancho
    rotada->height = original->width;   // El ancho original será el nuevo alto
    rotada->channels = original->channels;
    
    // Asignar memoria para los datos rotados
    rotada->data = new unsigned char[rotada->width * rotada->height * rotada->channels];

    // Algoritmo de rotación
    for (int y = 0; y < original->height; ++y) {
        for (int x = 0; x < original->width; ++x) {
            // Calcular índices
            int indice_original = (y * original->width + x) * original->channels;
            int indice_rotado = (x * rotada->width + (rotada->width - 1 - y)) * rotada->channels;
            
            // Copiar todos los canales
            for (int c = 0; c < original->channels; ++c) {
                rotada->data[indice_rotado + c] = original->data[indice_original + c];
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
    // Cargar imagen
    Imagen* pikachu = load("Pikachu3.png");
    
    // Validar carga
    if (pikachu->data == nullptr) {
        std::cerr << "Error al cargar la imagen." << std::endl;
        return 1;
    }

    // Rotar 90° a la derecha
    Imagen* rotada = rotar90Derecha(pikachu);

    // Guardar resultado
    save(rotada, "Pikachu3.png");

    // Liberar recursos
    delete[] pikachu->data;
    delete pikachu;
    delete[] rotada->data;
    delete rotada;

    return 0;
}