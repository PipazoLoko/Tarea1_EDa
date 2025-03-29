// por ley de morgan 
// (i <= 'T' || a) == (i > 'T' && !a)

#include <iostream>
using namespace std;
int main(){
  int cuenta, n;
  for (cuenta = 1; cuenta < 5; cuenta++){
    cout << 2*cuenta << ""; //Deberia de mostrar (2468)✅s
  }
  for (n = 10; n > 0; n = n-2) {
    cout << "Hola "; //Muestra: Hola 10, Hola 8... Hola 2
    cout << n << endl;
    }
  return 0;
}