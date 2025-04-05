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
  stbi_image_free(imagen->data); // función de la librería stb_image.h que libera memoria de la funcion stbi_load.
}

//Funcion rotar 90 antihorario
void operacion_2(Imagen* imagen){
    int alto_original = imagen->height, ancho_original = imagen->width, canales = imagen->channels; //alto, ancho y canales de original
    int alto_rotado = ancho_original, ancho_rotado = alto_original;
    unsigned char* array_pixeles_rotados = new unsigned char[alto_rotado * ancho_rotado * canales]; // puntero al array de canales RGB por cantidad de pixeles totales, como es cuadrada son iguales
    for (int fila = 0 ; fila < ancho_original; ++fila){
        for(int columna = 0; columna < alto_original; ++columna){
            int indice_original = (fila * ancho_original + columna) * canales; 
            int indice_rotado = (columna * ancho_rotado + (ancho_rotado - 1 - fila)) * canales;
        for(int i = 0; i < canales; ++i){ // copian los valores RGB al rotado que esta vacio 
            array_pixeles_rotados[indice_rotado + i] = imagen->data[indice_original + i];
        }
        }
    }
    stbi_image_free(imagen->data);
    imagen->data = array_pixeles_rotados; // asigna pixeles rotados al original
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
      int main() {
        //operación 1
        Imagen* img1 = load("Pikachu.png");
        operacion1(img1);
        save(img1, "Pikachu_invertido.png");
        delete[] img1->data; //libera contenido
        delete img1; //elimina el objeto, la estructura

        //operacion 2
        Imagen* img2 = load("Pikachu.png");
        operacion_2(img2);
        save(img2, "Pikachu_rotada.png");
        delete img2;
        delete[] img2->data; 
        delete img2;
    
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
    
        return 0;
    }
    
