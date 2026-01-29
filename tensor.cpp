#include "tensor.h"
#include <iostream>
#include <vector>

using namespace std;

Tensor::Tensor(float data) : _data{data}, _shape{}, _stride{} {};

Tensor::Tensor(vector<float> data)
    : _data{data}, _shape{data.size()}, _stride{1} {};

Tensor::Tensor(vector<size_t> shape) : _shape{shape} {
  size_t size = 1;
  for (size_t s : shape)
    size *= s;
  _data.resize(size, 0.0f);

  if (shape.size() == 1) {
    _stride = {1};
  } else if (shape.size() == 2) {
    _stride = {shape[1], 1};
  } else {
    _stride.resize(shape.size());
    size_t s = 1;
    for (int i = shape.size() - 1; i >= 0; --i) {
      _stride[i] = s;
      s *= shape[i];
    }
  }
};

Tensor::Tensor(vector<vector<float>> data)
    : _shape{data.size(), data[0].size()}, _stride{data[0].size(), 1} {
  size_t n_expected_columns = data[0].size();
  for (size_t i = 1; i < data.size(); i++) {
    if (data[i].size() != n_expected_columns) {
      throw invalid_argument("Dimensions are inconsistent.");
    }
  }
  // store in row major foramt like pytorch or numpy
  for (size_t i = 0; i < data.size(); i++) {
    for (size_t j = 0; j < data[i].size(); j++) {
      _data.push_back(data[i][j]);
    }
  }
};

const float &Tensor::item() const {
  if (_data.size() != 1) {
    throw runtime_error(
        "item() can only be called on tensors with a single element.");
  }
  return _data[0];
}

float &Tensor::item() {
  if (_data.size() != 1) {
    throw runtime_error(
        "item() can only be called on tensors with a single element.");
  }
  return _data[0];
}

const float &Tensor::operator()(size_t i) const {
  if (_shape.size() == 0) {
    throw invalid_argument("Can't index into a scalar. Use item() instead");
  }
  if (_shape.size() == 1) {
    if (i >= _shape[0]) {
      throw invalid_argument("Index" + to_string(i) +
                             "is out of bounds for array of size " +
                             to_string(_shape[0]));
    }
    return _data[i];
  }

  throw invalid_argument(
      "This is a 1D tensor. Use two indices for 2D tensors.");
}

float &Tensor::operator()(size_t i) {
  if (_shape.size() != 1) {
    throw runtime_error(
        "operator() can only be called on tensors with a single dimension.");
  }
  if (i >= _shape[0]) {
    throw out_of_range("Index out of range.");
  }
  return _data[i];
}

const float &Tensor::operator()(size_t i, size_t j) const {
  if (_shape.size() == 2) {
    if (i >= _shape[0]) {
      throw invalid_argument("Row index " + to_string(i) +
                             " is out of bounds for tensor with " +
                             to_string(_shape[0]) + " rows");
    }
    if (j >= _shape[1]) {
      throw invalid_argument("Column index " + to_string(j) +
                             " is out of bounds for tensor with " +
                             to_string(_shape[1]) + " columns");
    }
    return _data[i * _stride[0] + j * _stride[1]];
  }

  throw invalid_argument("Can only double index into 2D tensors.");
}

float &Tensor::operator()(size_t i, size_t j) {
  if (_shape.size() == 2) {
    if (i >= _shape[0]) {
      throw invalid_argument("Row index " + to_string(i) +
                             " is out of bounds for tensor with " +
                             to_string(_shape[0]) + " rows");
    }
    if (j >= _shape[1]) {
      throw invalid_argument("Column index " + to_string(j) +
                             " is out of bounds for tensor with " +
                             to_string(_shape[1]) + " columns");
    }
    return _data[i * _stride[0] + j * _stride[1]];
  }

  throw invalid_argument("Can only double index into 2D tensors.");
}

const vector<size_t> &Tensor::shape() const { return _shape; }

const vector<size_t> &Tensor::stride() const { return _stride; }

ostream &operator<<(ostream &os, const Tensor &obj) {
  string string_repr = "[";
  if (obj.shape().size() == 0) {
    os << obj.item();
    return os;
  }

  else if (obj.shape().size() == 1) {
    for (size_t i = 0; i < obj.shape()[0]; i++) {
      string_repr += to_string(obj(i));
      if (i < obj.shape()[0] - 1) {
        string_repr += ", ";
      }
    }
    string_repr += "]";
  } else {
    for (size_t i = 0; i < obj.shape()[0]; i++) {
      string_repr += "[";
      for (size_t j = 0; j < obj.shape()[1]; j++) {
        string_repr += to_string(obj(i, j));
        if (j < obj.shape()[1] - 1) {
          string_repr += ", ";
        }
      }
      string_repr += "]";
      if (i < obj.shape()[0] - 1) {
        string_repr += ", ";
      }
    }
    string_repr += "]";
  }

  os << string_repr;
  return os;
}

shared_ptr<Tensor> Tensor::operator+(shared_ptr<Tensor> other) {
  // scaler + scalar
  if (_shape.size() == 0 && other->shape().size() == 0) {
    float result = item() + other->item();
    return make_shared<Tensor>(result);
  }

  // scalar + 1d
  if (_shape.size() == 0 && other->shape().size() == 1) {
    vector<float> result;
    for (size_t i = 0; i < other->shape()[0]; i++) {
      result.push_back(item() + ((*other)(i)));
    }
    return make_shared<Tensor>(result);
  }

  // scaler + 2D
  if (_shape.size() == 0 && other->shape().size() == 2) {
    vector<vector<float>> result;
    for (size_t i = 0; i < other->shape()[0]; i++) {
      vector<float> result_i;
      for (size_t j = 0; j < other->shape()[1]; j++) {
        result_i.push_back(item() + ((*other)(i, j)));
      }
      result.push_back(result_i);
    }
    return make_shared<Tensor>(result);
  }

  // 1D + scaler
  if (_shape.size() == 1 && other->shape().size() == 0) {
    vector<float> result;
    for (size_t i = 0; i < _shape[0]; i++) {
      result.push_back(operator()(i) + other->item());
    }
    return make_shared<Tensor>(result);
  }

  // 2D + scalar
  if (_shape.size() == 2 && other->shape().size() == 0) {
    vector<vector<float>> result;
    for (size_t i = 0; i < _shape[0]; i++) {
      vector<float> result_i;
      for (size_t j = 0; j < _shape[1]; j++) {
        result_i.push_back(operator()(i, j) + other->item());
      }
      result.push_back(result_i);
    }
    return make_shared<Tensor>(result);
  }

  // 1D + 1D
  if (_shape[0] != other->shape()[0]) {
    throw invalid_argument("Dimensions do not match.");
  }
  if (_shape.size() == 1) {
    vector<float> result;
    for (size_t i = 0; i < _shape[0]; i++) {
      result.push_back(operator()(i) + (*other)(i));
    }

    return make_shared<Tensor>(result);
  }

  // 2D + 2D
  else {
    if (shape()[1] != other->shape()[1]) {
      throw invalid_argument("Dimensions do not match.");
    }
    vector<vector<float>> result;
    for (size_t i = 0; i < _shape[0]; i++) {
      vector<float> result_i;
      for (size_t j = 0; j < _shape[1]; j++) {
        result_i.push_back(operator()(i, j) + (*other)(i, j));
      }
      result.push_back(result_i);
    }
    return make_shared<Tensor>(result);
  }
}