#include <iostream>

using namespace std;
float calProm (int num1,  int num2){
  float promedio = (num1 + num2) / 2.0;
  return promedio;
}
int main(){
  cout << "El promedio es: "<< calProm(3, 10);
  return 0;
}