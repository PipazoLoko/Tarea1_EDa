#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

// Importacion de librerias. Los archivos correspondientes deben estar en el mismo directorio que el .cpp.
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>
#include <fstream>

using namespace std;

// Struct que almacena la data de una imagen
struct Imagen{
    unsigned char* data;
    int width, height, channels;
};

// Abrir imagen desde memoria local EN COLOR **GRIS**
Imagen* load2(const char* filename) {
    Imagen* img = new Imagen();
    img->data = stbi_load(filename, &img->width, &img->height, &img->channels, 1);
    img->channels = 1;
    return img;
}

//funcion que crea un arreglo bidimensional a partir de la imagen y un arreglo 
char** to_ascii(Imagen* imagen, char* arbitrario){
    int alto = imagen-> height, ancho = imagen->width;
    unsigned char* array_pixeles = imagen->data;
    char** arreglo_bidi = new char*[alto]; // arreglo de punteros por fila
    for (int i = 0; i < alto; i++) {     // cada arreglo de fila tiene un tamaño 'ancho' caracteres + 1 para el '\0'
        arreglo_bidi[i] = new char[ancho + 1]; // +1 para terminar la cadena con '\0'
    }
    for(int fila = 0; fila < alto; ++fila){
        for(int columna = 0; columna < ancho; ++columna){
            int indice_pixel = fila * ancho + columna;
            unsigned char contenido_pixel = array_pixeles[indice_pixel];
            int index = contenido_pixel / 32; // del 0 al 7
            arreglo_bidi[fila][columna] = arbitrario[index];
            // for(int paso = 32, menor = 0, mayor = 1; paso < 256; paso += 32, ++menor, ++mayor){
            //     if ((unsigned char)(paso * menor) < contenido_pixel && contenido_pixel <= (unsigned char)(paso * mayor)){
            //         arreglo_bidi[fila][columna] = arbitrario[menor];
            //         break;
            //     }
                arreglo_bidi[fila][ancho] = '\0';
            }
        }
        return arreglo_bidi;
}

// Funcion que crea un txt a partir del arreglo bidimensional
void save_ascii(char** matriz_cuadrada){
    ofstream salida;
    salida.open("salida.txt");
    for(int i = 0; i < 256; ++i){
        salida << matriz_cuadrada[i] << endl;
    }
    salida.close();
}

// Guardar imagen en memoria local
void save(Imagen* img, const char* filename) {
    stbi_write_png(filename, img->width, img->height, img->channels, img->data, img->width * img->channels);
    cout << "Imagen guardada:  " << filename << "\n";
}

int main() {
    //Cargamos la imagen
    Imagen* img5 = load2("Pikachu.png");
    char array_arbitrario[] = { '@', 'X', '&', '=', '*', '|', '.', ' ' }; //elegimos el arreglo de caracteres ASCII y lo ordemanos de mas oscuro a claro
    char** arreglo_bidimensional = to_ascii(img5, array_arbitrario);
    save_ascii(arreglo_bidimensional);
    for (int i = 0; i < img5->height; ++i) {// liberar la memoria de arreglo bidimensional
        delete[] arreglo_bidimensional[i]; // liberar cada fila
    } 
    delete[] arreglo_bidimensional; // liberar el arreglo de punteros
    // stbi_image_free(img5->data);  // Libera la data cargada por stbi_load
    delete img5;

}
