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
    return img;
}

char** operacion7(const Imagen* img, const char ascii_chars[]) {
    // Número de caracteres en el arreglo
    const int num_chars = sizeof(ascii_chars) / sizeof(ascii_chars[0]);
    char** ascii_art = new char*[img->height];//asignamos memeoria para las filas
    for (int h = 0; h < img->height; h++) { 
        ascii_art[h] = new char[img->width + 1];//incluir caracter nulo
    }
    
    //recorremos la imagen para asignar un caracter ASCII
    for (int h = 0; h < img->height; h++) {
        for (int w = 0; w < img->width; w++) {
            int pos = (h * img->width + w) * img->channels;
            float gris;
            if (img->channels >= 3) {
                //conversión a gris: 0.299 * R + 0.587 * G + 0.114 * B
                unsigned char r = img->data[pos];
                unsigned char g = img->data[pos + 1];
                unsigned char b = img->data[pos + 2];
                gris = 0.299f * r + 0.587f * g + 0.114f * b;
            } else {
                //si la imagen ya esta en escala de grises
                gris = img->data[pos];
            }
                    // Convertir a escala de grises si es necesario
            // Mapear el valor de gris [0, 255] a uno de los caracteres del arreglo
            int  posdiv_caracter= (int)(gris * num_chars / 256); //indice del caracter 
            if ( posdiv_caracter >= num_chars)
            posdiv_caracter= num_chars - 1;
            ascii_art[h][w] = ascii_chars[ posdiv_caracter];
        }
        // Finalizar cada línea con el carácter nulo
        ascii_art[h][img->width] = '\0';
    }
    
    return ascii_art;
}


void save_ascii(char** ascii_art, int height, const char* filename) {
    FILE* file = fopen(filename, "w");

    for (int h = 0; h < height; h++) {
        fprintf(file, "%s\n", ascii_art[h]);
    }
}

int main() {
    Imagen* img = load("Pikachu.png");
    if (!img->data)
        return 1;
    
    // Definir el arreglo de caracteres ASCII ordenados por luminosidad.
    // Por ejemplo, de oscuro a claro: '#' y 'X' representan pixeles oscuros y '|' y ' ' los más luminosos.
    const char ascii_chars[] = { '@', 'X', '&', '=', '*', '|', '.', ' ' };

    // Generar el ASCII Art
    char** ascii_art = operacion7(img, ascii_chars);
    
    // Imprimir el resultado en consola
    for (int h = 0; h < img->height; h++) {
        cout << ascii_art[h] << "\n";
    }
    
    // Guardar el ASCII Art en un archivo de texto
    save_ascii(ascii_art, img->height, "Pikachuassci.txt");
    
    // Liberar la memoria reservada para el arreglo bidimensional
    for (int h = 0; h < img->height; h++) {
        delete[] ascii_art[h];
    }
    delete[] ascii_art;
    
    
    stbi_image_free(img->data);
    delete img;
    
    return 0;
}

