#include <iostream>

void swap(int& a, int& b){
  int temp;
  temp = a;
  a = b;
  b = temp;
}

using namespace std;
int main(){
  int v1 = 10, v2 =918;
  cout << v1 << " " << v2 << endl;
  swap(v1, v2);
  cout << v1 << " " << v2 << endl;
  return 0;
}