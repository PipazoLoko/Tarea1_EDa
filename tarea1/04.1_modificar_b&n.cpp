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
void umbralizar(Imagen* imagen, int limite) {
    // validamos parámetros
    if (imagen == nullptr || imagen->data == nullptr) {
        std::cerr << "Error: Imagen no válida para umbralizar." << std::endl;
        return;
    }

    // aseguramos el límite [0, 255]
    limite = std::max(0, std::min(255, limite));

    // procesamos cada píxel
    for (int i = 0; i < imagen->width * imagen->height; ++i) {
        // calcular la posición base del píxel
        int indice_base = i * imagen->channels;

        // Convertir a escala de grises si es necesario
        float valor_gris;
        if (imagen->channels >= 3) {  // RGB o RGBA
            valor_gris = 0.299f * imagen->data[indice_base] +   // R
                         0.587f * imagen->data[indice_base + 1] + // G
                         0.114f * imagen->data[indice_base + 2];  // B
        } else {  // Escala de grises (1 o 2 canales)
            valor_gris = imagen->data[indice_base];
        }

        // Determinar nuevo valor (blanco o negro)
        unsigned char nuevo_valor = (valor_gris > limite) ? 255 : 0;

        // Aplicar a todos los canales de color (excepto alpha si existe)
        for (int c = 0; c < std::min(3, imagen->channels); ++c) {
            imagen->data[indice_base + c] = nuevo_valor;
        }

        // Mantener canal alpha sin cambios si existe
        if (imagen->channels == 4) {
            imagen->data[indice_base + 3] = imagen->data[indice_base + 3];
        }
    }
}

int main() {
    // Cargar imagen
    Imagen* img = load("Pikachu5.png");

    // Umbralizar con límite de 128 (puedes cambiar este valor)
    int limite_umbral = 100;
    umbralizar(img, limite_umbral);

    // Guardar resultado
    save(img, "Pikachu5.png");

    // Liberar memoria
    delete[] img->data;
    delete img;

    return 0;
}
