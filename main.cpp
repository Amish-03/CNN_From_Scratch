#include "tensor.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
  vector<vector<float>> test_v{
      {1.0, 2.0, 3.0, 4.0}, {1.0, 2.0, 3.0, 4.0}, {1.0, 2.0, 3.0, 4.0}};
  Tensor test_tensor(test_v);
  cout << test_tensor << endl;
  return 0;
}