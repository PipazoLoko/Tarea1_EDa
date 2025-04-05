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
    

    const char ascii_chars[] = { '@', 'X', '&', '=', '*', '|', '.', ' ' };//elegimos el arreglo de caracteres ASCII y lo ordemanos de mas oscuro a claro

    char** ascii_art = operacion7(img, ascii_chars);//convertimos la imagen a ASCII
    
    for (int h = 0; h < img->height; h++) {//imprimimos el resultado
        cout << ascii_art[h] << "\n";
    }

    save_ascii(ascii_art, img->height, "Pikachuassci.txt");//lo guardamos en un archivo .txt
    
    for (int h = 0; h < img->height; h++) {//liberamos memoria de cada fila
        delete[] ascii_art[h];
    }
    delete[] ascii_art;//liberamos memoria del arreglo
    
    
    stbi_image_free(img->data);//liberamos memoria de la imagen
    delete img;//liberamos memoria de la estructura
    
    return 0;
}

