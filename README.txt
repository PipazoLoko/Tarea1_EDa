Pregunta 1 (5 pts)
¿Por qué si los pixeles de la imagen son números enteros ∈ [0, 255], utilizamos el tipo de dato unsigned char?

El tipo char es un tipo especial de entero que almacena 1 byte (8 bits), significa que reserva una memoria de 8 digitos binarios (2^8 = 256 valores) para representar un entero, que al ser unsigned solo tomara valores no negativos, siendo el intervalo [0-255], el cual coincide con los valores posibles de un pixel. ("citar paginas 4 y 5 de apuntes")

Pregunta 2 (5 pts)
Cuales serían las diferencias entre usar unsigned int versus unsigned char? Responder cuantitativamente para una imagen RGB de dimensiones (N,N,3).

Pregunta 3 (5 pts)
Usando este esquema de almacenamiento unidimensional: ¿Cuál sería el índice del primer pixel del canal azul?

Pregunta 4 (5 pts)
¿Cómo podemos convertir una imagen RGB a escala de grises?

Al abrir y sobrescribir o escribir la imagen, hay que limitar los canales. En detalle, si cargamos una imagen de 4 canales, al abrirlo con la funcion stbi_load tenemos que modificar el ultimo parametro, indica los canales deseados, a 1 (escala de grises). Luego, al guardar la imagen con stbi_write_png reemplazamos todas sentencias llamadas img->channels por 1. 



Bibiografía

  * Aroyuelo, D., & Díaz, R. (s.f.). Tutorial del Lenguaje C++. INF-134 Estructuras de Datos. Departamento de Informática, Universidad Técnica Federico Santa María.