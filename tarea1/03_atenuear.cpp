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
    img->data = stbi_load(filename, &img->width, &img->height, &img->channels, 0);
    return img;
}

// Guardar imagen en memoria local
void save(Imagen* img, const char* filename) {
    stbi_write_png(filename, img->width, img->height, img->channels, img->data, img->width * img->channels);
    cout << "Imagen guardada:  " << filename << "\n";
}

// Función para atenuar los valores de los píxeles
void atenuarImagen(Imagen* imagen, float factor) {

  // Asegurar que el factor esté en el rango [0, 1]
  if (factor < 0.0f) factor = 0.0f;
  if (factor > 1.0f) factor = 1.0f;

  // Iterar sobre todos los píxeles de la imagen
  for (int indice = 0; indice < imagen->width * imagen->height * imagen->channels; ++indice) {
      // Aplicar atenuación y asegurar que se mantenga en [0, 255]
      float valor_atenuado = imagen->data[indice] * factor;
      imagen->data[indice] = static_cast<unsigned char>(valor_atenuado + 0.5f); // +0.5 para redondeo
  }
}

int main() {
  // Cargar imagen
  Imagen* pikachu = load("Pikachu4.png");

  // Ejemplo: atenuar al 50% de intensidad
  atenuarImagen(pikachu, 0.5f);

  // Guardar resultado
  save(pikachu, "Pikachu4.png");

  // Liberar recursos
  delete[] pikachu->data;
  delete pikachu;

  return 0;
}


