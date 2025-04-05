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
    img->data = stbi_load(filename, &img->width, &img->height, &img->channels, 0);
    return img;
}

// Guardar imagen en memoria local
void save(Imagen* img, const char* filename) {
    stbi_write_png(filename, img->width, img->height, img->channels, img->data, img->width * img->channels);
    cout << "Imagen guardada: " << filename << endl;
}


//funcion para invertir la imagen
void operacion1(Imagen* imagen){
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

// Operación 2: Rotación 90° antihoraria
Imagen* operacion2(Imagen* original) {
    Imagen* rotada = new Imagen();
    rotada->width = original->height;//alto sera ancho
    rotada->height = original->width;//ancho sera alto
    rotada->channels = original->channels;//mantendremos los canales

    //asignar memoria
    rotada->data = new unsigned char[rotada->width * rotada->height * rotada->channels];

    for (int h = 0; h < original->height; ++h) {//recorreremos cada fila hasta llegar a la altura
        for (int w = 0; w < original->width; ++w) {//recorreremos cada columna hasta llegar al ancho
            int pos_original = (h * original->width + w) * original->channels;//calculamos los indices exactos
            int pos_rotada = (w * rotada->width + (rotada->width - 1 - h)) * rotada->channels;//calculamos indices ya rotados

            for (int c = 0; c < original->channels; ++c) {
                rotada->data[pos_rotada + c] = original->data[pos_original + c];//copiamos canales
            }
        }
    }

    return rotada;
}

//función para atenuar
void operacion3(Imagen* imagen, float factor) {
  if (factor < 0.0f) factor = 0.0f; // Rango [0, 1]
    if (factor > 1.0f) factor = 1.0f;
    for (int indice = 0; indice < imagen->width * imagen->height * imagen->channels; ++indice) {
    int canal = indice % imagen->channels;//posicion del canal 
    if (canal < 3) {//aplicamos a los canales RGB
          float v_atenuado = imagen->data[indice] * factor;//atenuamos
        if (v_atenuado < 0) v_atenuado = 0;//valor en el rango [0, 255]
        if (v_atenuado > 255) v_atenuado = 255;
            imagen->data[indice] = static_cast<unsigned char>(v_atenuado + 0.5f);//asignamos el nuevo valor al canal correspondiente
        }
    }
}


//funcion para mdificar la imagen (blanco/negro)
void operacion4(Imagen* imagen, int limite) {
  // aseguramos el límite [0, 255]
if (limite < 0) limite = 0;
else if (limite > 255) limite = 255;


  //procesamos cada píxel
  for (int i = 0; i < imagen->width * imagen->height; ++i) {
      // calcular la posición base del píxel
      int indice_base = i * imagen->channels;

      //convertir a escala de grises si es necesario
    float valor_gris;
      if (imagen->channels >= 3) {  // RGB
          valor_gris = 0.299f * imagen->data[indice_base] +   
                       0.587f * imagen->data[indice_base + 1] + 
                       0.114f * imagen->data[indice_base + 2];  
      } else {  //escala de grises (1 o 2 canales)
        valor_gris = imagen->data[indice_base];
    }
      unsigned char nuevo_valor; //determinar nuevo valor (blanco o negro)

    if (valor_gris > limite) {
        nuevo_valor = 255;
    } else {
        nuevo_valor = 0;   
    }

    for (int c = 0; c < min(3, imagen->channels); ++c) {//aplicar a RGB
        imagen->data[indice_base + c] = nuevo_valor;
    }
    }
}

//operacion ASCII
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
    int main() {
        //operación 1
        Imagen* img1 = load("Pikachu.png");
        operacion1(img1);
        save(img1, "Pikachu_invertido.png");
        stbi_image_free(img1->data);
    
        //operacion 2
        Imagen* img2 = load("Pikachu.png");
        Imagen* rotada = operacion2(img2);
        save(rotada, "Pikachu_rotada.png");
        stbi_image_free(img2->data); 
        delete img2;
        delete[] rotada->data; 
        delete rotada;
    
        //operación 3
        Imagen* img3 = load("Pikachu.png");
        float valor;
        cout << "Ingrese el valor de atenuación (0.0 a 1.0): ";
        cin >> valor;
        operacion3(img3, valor);
        save(img3, "Pikachu_atenuado.png");
        stbi_image_free(img3->data);
        delete img3;
    
        //operación 4
        Imagen* img4 = load("Pikachu.png");
        int limite = 100;
        operacion4(img4, limite);
        save(img4, "Pikachu_bn.png");
        stbi_image_free(img4->data);
        delete img4;

    const char ascii_chars[] = { '@', 'X', '&', '=', '*', '|', '.', ' ' };//elegimos el arreglo de caracteres ASCII y lo ordemanos de mas oscuro a claro

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
    