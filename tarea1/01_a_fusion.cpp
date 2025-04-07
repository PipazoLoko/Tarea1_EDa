#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>


using namespace std;

// Struct que almacena la data de una imagen
struct Imagen {
    unsigned char* data;
    int width, height, channels;
};

// Cargar imagen desde disco
Imagen* load(const char* filename) {
    Imagen* img = new Imagen();
    img->data = stbi_load(filename, &img->width, &img->height, &img->channels, 3);
    img->channels = 3;
    return img;
}

//funcion para operacion1 horizontalmente la imagen
void operacion1(Imagen* imagen){
    int canales = imagen->channels, alto = imagen->height, ancho = imagen->width; // Valores: 3 256 256 
    unsigned char* array_pixeles = imagen->data; // Pixeles por canales en un array (puntero hacia un array)
    for (int fila = 0; fila < alto ;++fila){ //itera por filas
      for (int clmIzq = 0; clmIzq < (ancho / 2) ; ++clmIzq){ //itera columnas de la mitad izquierda
        int clmDer = ancho - 1 - clmIzq, indIzq = (ancho * fila + clmIzq) * canales , indDer = (ancho * fila + clmDer) * canales; // calcula columna derecha asociada a la izquierda; e indices o "cordenadas" de los R (de 3 en 3) de cada pixel para ser utilizada en el array que almacena los canales de los pixeles
        //swap de canales por pixel
        for(int i = 0; i < canales; ++i){
          unsigned char temp = array_pixeles[indIzq + i];  //almacena temporalmente los valores de los canales del pixel izquierdo
          array_pixeles[indIzq + i] = array_pixeles[indDer + i ]; //asigna valores de canales del pixel derecho al izq
          array_pixeles[indDer + i] = temp; // asigna valore de canales del pixel izq al derecho 
        }
      } 
    }
  }

//Funcion rotar 90 antihorario
void operacion2(Imagen* imagen){
    int alto_original = imagen->height, ancho_original = imagen->width, canales = imagen->channels; //alto, ancho y canales de original
    int alto_rotado = ancho_original, ancho_rotado = alto_original;
    unsigned char* array_pixeles_rotados = new unsigned char[alto_rotado * ancho_rotado * canales]; // puntero al array de canales RGB por cantidad de pixeles totales, como es cuadrada son iguales
    for (int fila = 0 ; fila < ancho_original; ++fila){ //iteramos fila y columna del original, para calcular el indice R del original y el rotado
        for(int columna = 0; columna < alto_original; ++columna){
            int indice_original = (fila * ancho_original + columna) * canales; 
            int indice_rotado = (columna * ancho_rotado + (ancho_rotado - 1 - fila)) * canales;
        for(int i = 0; i < canales; ++i){ // copian los valores RGB al rotado que esta vacio 
            array_pixeles_rotados[indice_rotado + i] = imagen->data[indice_original + i];
        }
        }
    }
    imagen->data = array_pixeles_rotados; // asigna pixeles rotados al original
}

// Funcion para atenuar la imagen con valor arbitrario
void operacion3(Imagen* imagen, float valor){
    int alto = imagen->height, ancho = imagen->width, canales = imagen->channels;
    unsigned char* array_de_pixeles = imagen->data;
    for(int indice = 0; indice < alto * ancho * canales; ++indice){ // itero los indices RGB por pixel
        float v_atenuado = array_de_pixeles[indice] * valor; // aplicamos atenuacion
        imagen->data[indice] = (unsigned char)v_atenuado; // cambiamos valores del original al atenuado
    }
}


//Funcion para asignar blanco arriba del limite, negro de lo contrario
void operacion4(Imagen* imagen, int limite){
    int alto = imagen->height, ancho = imagen->width, canales = imagen->channels;
    unsigned char* array_de_pixeles = imagen->data; 
    for(int p = 0; p < alto * ancho; ++p){ // iteramos cada pixel 'p' (sin canal o *3)
        int indiceR = p * canales; //calculamos el R de cada pixel 'p'
        int suma_RGB = array_de_pixeles[indiceR] + array_de_pixeles[indiceR + 1] + array_de_pixeles[indiceR + 2]; // calculamos el valor del pixel 'p' = suma de sus valores RGB. util para el array unidimensional
        unsigned char valor_blanco_negro; // creamos una variable que pude tomar valor de negro o blanco
        if (suma_RGB > limite){ //asigna segun arriba o no arriba del limite
            valor_blanco_negro = 255; //blanco
        }else{
            valor_blanco_negro = 0; //negro
        }
        for(int c = 0 ; c < canales; ++c){ //itero canales por pixel 'p' y le asigno el blanco o negro calculado previamente
            array_de_pixeles[indiceR + c] = valor_blanco_negro ;
        }
    }
}

//Funcion para convertir imagen en ASCII
char** to_ascii(const Imagen* img, const char ascii_chars[]) {
    // Número de caracteres en el arreglo
    const int num_chars = sizeof(ascii_chars) / sizeof(ascii_chars[0]);
    char** ascii = new char*[img->height];//asignamos memeoria para las filas
    for (int h = 0; h < img->height; h++) { 
        ascii[h] = new char[img->width + 1];//incluir caracter nulo
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

                gris = img->data[pos];//si la imagen ya esta en escala de grises
            }
            int  posdiv_caracter= (int)(gris * num_chars / 256); //indice del caracter 
            if ( posdiv_caracter >= num_chars)
            posdiv_caracter= num_chars - 1;
            ascii[h][w] = ascii_chars[ posdiv_caracter];
        }

        ascii[h][img->width] = '\0';//finalizamos con caracter nulo
    }
    
    return ascii;
}


void save_ascii(char** ascii_art, int height, const char* filename) {
    FILE* file = fopen(filename, "w");

    for (int h = 0; h < height; h++) {
        fprintf(file, "%s\n", ascii_art[h]);
    }
}
// Guardar imagen en memoria local
void save(Imagen* img, const char* filename) {
    stbi_write_png(filename, img->width, img->height, img->channels, img->data, img->width * img->channels);
    cout << "Imagen guardada: " << filename << endl;
}

      int main() {
        //operación 1
        Imagen* img1 = load("Pikachu.png");
        operacion1(img1);
        save(img1, "Pikachu_invertido.png");
        delete[] img1->data; //libera contenido
        delete img1; //elimina el objeto, la estructura

        //operacion 2
        Imagen* img2 = load("Pikachu.png");
        operacion2(img2);
        save(img2, "Pikachu_rotada.png");
        delete[] img2->data;
        delete img2;
         
        //operación 3
        Imagen* img3 = load("Pikachu.png");
        operacion3(img3, (float)(0.5));
        save(img3, "Pikachu_atenuado.png");
        delete[] img3->data;
        delete img3;
    
        //operación 4
        Imagen* img4 = load("Pikachu.png");
        operacion4(img4, 400);
        save(img4, "Pikachu_bn.png");
        delete[] img4->data;
        delete img4;
        //operación 5
    char ascii_chars[] = { '@', 'X', '&', '=', '*', '|', '.', ' ' };//elegimos el arreglo de caracteres ASCII y lo ordemanos de mas oscuro a claro

    Imagen* img5 = load("Pikachu.png"); // Cargar la imagen para la operación ASCII
    char** ascii = to_ascii(img5, ascii_chars);//convertimos la imagen a ASCII
    
    for (int h = 0; h < img5->height; h++) {//imprimimos el resultado
        cout << ascii[h] << "\n";
    }

    save_ascii(ascii, img5->height, "Pikachuassci.txt");//lo guardamos en un archivo .txt
    
    for (int h = 0; h < img5->height; h++) {//liberamos memoria de cada fila
        delete[] ascii[h];
    }
    delete[] ascii;//liberamos memoria del arreglo
    
    
    stbi_image_free(img5->data);//liberamos memoria de la imagen
    delete img5;//liberamos memoria de la estructura
    
        return 0;
    }
    
