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

/*Para nuestro caso es una imagen cuadrada de 256x256 (alto = ancho).
Con 4 canales, es decir, 4 componentes por cada pixel, siendo el ultimo canal
Alpha o canal de transparencia.*/

// Funcion para invertir horizontalmente
void espejo_horizontal(Imagen* imagen) {
    // Memoria temporal de los pixeles mientras se hace el intercambio
    unsigned char* memoria_temporal = new unsigned char[imagen->channels];
    // Iterar sobre cada fila de la imagen
    for (int fila_actual = 0; fila_actual < imagen->height; ++fila_actual) {
        
        // Iterar hasta la mitad de las columnas,para no repetir
        for (int columna_izquierda = 0; columna_izquierda < imagen->width / 2; ++columna_izquierda) {
            
            // Calcular la columna derecha correspondiente (espejo)
            int columna_derecha = imagen->width - 1 - columna_izquierda;
            /* Ancho es 255, al restarle 1, nos movemos de izquierda a derecha,
            es decir estamos calculando las columnas de la parte derecha*/

            // Calcular índices de memoria para ambos píxeles
            int indice_izquierdo = (fila_actual * imagen->width + columna_izquierda) * imagen->channels;
            int indice_derecho = (fila_actual * imagen->width + columna_derecha) * imagen->channels;

            // Intercambiar todos los canales (RGBA, RGB, etc.)
            for (int canal = 0; canal < imagen->channels; ++canal) {
                // Almacenar temporalmente el píxel izquierdo
                memoria_temporal[canal] = imagen->data[indice_izquierdo + canal];
                
                // Copiar píxel derecho al izquierdo
                imagen->data[indice_izquierdo + canal] = imagen->data[indice_derecho + canal];
                
                // Copiar píxel temporal al derecho
                imagen->data[indice_derecho + canal] = memoria_temporal[canal];
            }
        }
    }
    // Liberar memoria del buffer temporal
    delete[] memoria_temporal;
}

// Guardar imagen en memoria local
void save(Imagen* img, const char* filename) {
    stbi_write_png(filename, img->width, img->height, img->channels, img->data, img->width * img->channels);
    cout << "Imagen guardada:  " << filename << "\n";
}

int main() {
    // Cargar imagen
    Imagen* pikachu = load("Pikachu2.png");
    
    // Aplicar efecto espejo horizontal
    espejo_horizontal(pikachu);

    // Guardar resultado
    save(pikachu, "Pikachu2.png");

    // Liberar recursos
    delete[] pikachu->data;
    delete pikachu;

    return 0;
}