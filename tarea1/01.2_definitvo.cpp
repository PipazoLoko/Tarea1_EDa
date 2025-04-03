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

//funcion para invertir horizontalmente la imagen
void invertir(Imagen* imagen){
  int canales = imagen->channels, alto = imagen->height, ancho = imagen->width; // Valores: 4 256 256 
  cout<<canales<<endl;
  unsigned char* array_pixeles = imagen->data; // Pixeles por canales en un array (puntero hacia un array)
  for (int fila = 0; fila < alto ;++fila){ //itera por filas
    for (int clmIzq = 0; clmIzq < (ancho / 2) ; ++clmIzq){ //itera columnas de la mitad izquierda
      int clmDer = ancho - 1 - clmIzq, indIzq = (ancho * fila + clmIzq) * canales , indDer = (ancho * fila + clmDer) * canales; // calcula columna derecha asociada a la izquierda; "cordenadas" de los pixeles para ser utilizada en el array que almacena los canales de los pixeles
      //swap de canales por pixel
      for(int i = 0; i < canales; ++i){
        unsigned char temp = array_pixeles[indIzq + i];  //almacena temporalmente los valores de los canales del pixel izquierdo
        array_pixeles[indIzq + i] = array_pixeles[indDer + i ]; //asigna valores de canales del pixel derecho al izq
        array_pixeles[indDer + i] = temp; // asigna valore de canales del pixel izq al derecho 
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
    Imagen* img = load("Pikachu2.png");
    invertir(img);
    save(img, "Pikachu2.png");// Almacenamos el resultado
    stbi_image_free(img->data); // función de la librería stb_image.h que libera memoria de la funcion stbi_load. 
    delete img; //elimina el objeto, la estructura o el "esqueleto", mientras que la anterior deletea su contenido fuente: https://solarianprogrammer.com/2019/06/10/c-programming-reading-writing-images-stb_image-libraries/
}