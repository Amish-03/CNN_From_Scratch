#include <iostream>
#include <memory>
#include <vector>

using namespace std;
class Tensor {
private:
  vector<float> _data;
  vector<size_t> _shape;
  vector<size_t> _stride;

public:
  Tensor(float data);
  Tensor(vector<float> data);
  Tensor(vector<size_t> shape);
  Tensor(vector<vector<float>> data);

  const float &item() const;
  float &item();
  const float &operator()(size_t i) const;
  float &operator()(size_t i);
  const float &operator()(size_t i, size_t j) const;
  float &operator()(size_t i, size_t j);

  shared_ptr<Tensor> operator+(shared_ptr<Tensor> other);

  const vector<size_t> &shape() const;
  const vector<size_t> &stride() const;

  friend ostream &operator<<(ostream &os, const Tensor &obj);
};
