#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>
#include <cstdio>  // Para usar fopen, fprintf, fclose

using namespace std;

// Estructura para almacenar la imagen
struct Imagen {
    unsigned char* data;
    int width, height, channels;
};

// Función para cargar la imagen desde disco
Imagen* load(const char* filename) {
    Imagen* img = new Imagen();
    img->data = stbi_load(filename, &img->width, &img->height, &img->channels, 0);
    if (!img->data) {
        cerr << "Error al cargar la imagen: " << filename << "\n";
    }
    return img;
}


char** to_ascii(const Imagen* img, const char ascii_chars[]) {
    // Número de caracteres en el arreglo
    const int num_chars = sizeof(ascii_chars) / sizeof(ascii_chars[0]);
    // Reservar memoria para las filas del ASCII Art
    char** ascii_art = new char*[img->height];
    for (int y = 0; y < img->height; y++) {
        // Reservamos ancho + 1 para incluir el carácter nulo al final de cada líne
        ascii_art[y] = new char[img->width + 1];
    }
    
    // Recorrer la imagen y asignar un carácter ASCII a cada pixel
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            int pos = (y * img->width + x) * img->channels;
            float gray;
            if (img->channels >= 3) {
                // Conversión a gris: 0.299 * R + 0.587 * G + 0.114 * B
                unsigned char r = img->data[pos];
                unsigned char g = img->data[pos + 1];
                unsigned char b = img->data[pos + 2];
                gray = 0.299f * r + 0.587f * g + 0.114f * b;
            } else {
                // Si la imagen ya es en escala de grises
                gray = img->data[pos];
            }
            // Mapear el valor de gris [0, 255] a uno de los caracteres del arreglo
            int  posdiv_caracter= (int)(gray * num_chars / 256); //indice del caracter 
            if ( posdiv_caracter >= num_chars)
            posdiv_caracter= num_chars - 1;
            ascii_art[y][x] = ascii_chars[ posdiv_caracter];
        }
        // Finalizar cada línea con el carácter nulo
        ascii_art[y][img->width] = '\0';
    }
    
    return ascii_art;
}


void save_ascii(char** ascii_art, int height, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        cerr << "No se pudo abrir el archivo: " << filename << "\n";
        return;
    }
    for (int y = 0; y < height; y++) {
        // Escribir cada línea seguida de un salto de línea
        fprintf(file, "%s\n", ascii_art[y]);
    }
    fclose(file);
    cout << "ASCII Art guardado en: " << filename << "\n";
}

int main() {
    // Cargar la imagen (por ejemplo "Pikachu.png")
    Imagen* img = load("Pikachu.png");
    if (!img->data)
        return 1;
    
    // Definir el arreglo de caracteres ASCII ordenados por luminosidad.
    // Por ejemplo, de oscuro a claro: '#' y 'X' representan pixeles oscuros y '|' y ' ' los más luminosos.
    const char ascii_chars[] = { '#', 'X', 'O', '=', '+', '|', '.', ' ' };

    // Generar el ASCII Art
    char** ascii_art = to_ascii(img, ascii_chars);
    
    // Imprimir el resultado en consola
    for (int y = 0; y < img->height; y++) {
        cout << ascii_art[y] << "\n";
    }
    
    // Guardar el ASCII Art en un archivo de texto
    save_ascii(ascii_art, img->height, "output.txt");
    
    // Liberar la memoria reservada para el arreglo bidimensional
    for (int y = 0; y < img->height; y++) {
        delete[] ascii_art[y];
    }
    delete[] ascii_art;
    
    
    stbi_image_free(img->data);
    delete img;
    
    return 0;
}

