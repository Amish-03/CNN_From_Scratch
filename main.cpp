#include "tensor.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
  vector<vector<float>> test_v{
      {1.0, 2.0, 3.0, 4.0}, {1.0, 2.0, 3.0, 4.0}, {1.0, 2.0, 3.0, 4.0}};
  vector<vector<float>> test_v2{
      {1.0, 2.0, 3.0, 4.0}, {1.0, 2.0, 3.0, 4.0}, {1.0, 2.0, 3.0, 4.0}};
  shared_ptr<Tensor> test_tensor = make_shared<Tensor>(test_v);
  shared_ptr<Tensor> test_tensor2 = make_shared<Tensor>(test_v2);
  cout << *test_tensor << endl;
  cout << *test_tensor2 << endl;
  cout << *((*test_tensor) + test_tensor2) << endl;
  return 0;
}