#include <iostream>
using namespace std;
int main(){
  struct punto{
    float x, y;
    int peso;
  };  
  punto p[10];
  p[0] = {3.2, 3.8, 9};
  cout << p[0].x << p[0].y << p[0].peso;
  return 0;
}