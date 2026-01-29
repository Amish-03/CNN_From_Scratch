#include "mini_gtest.h"
#include "tensor.h"

int main() { return RUN_ALL_TESTS(); }

using namespace std;

TEST(TensorTest, Creation) {
  Tensor tensor = Tensor(5.0);
  EXPECT_EQ(tensor.shape(), vector<size_t>({}));
  EXPECT_THROW(tensor(0), invalid_argument);
  EXPECT_EQ(tensor.item(), 5.0);

  // 1d
  vector<float> v = {1.0, 2.0, 3.0};
  Tensor tensor1d = Tensor(v);
  EXPECT_EQ(tensor1d.shape(), vector<size_t>({3}));
  EXPECT_EQ(tensor1d(0), 1.0);
  EXPECT_EQ(tensor1d(1), 2.0);
  EXPECT_EQ(tensor1d(2), 3.0);
  EXPECT_THROW(tensor1d(3), invalid_argument);
  EXPECT_THROW(tensor1d(0, 0), runtime_error);

  // 2d
  vector<vector<float>> v2d = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
  Tensor tensor2d = Tensor(v2d);
  EXPECT_EQ(tensor2d.shape(), vector<size_t>({2, 3}));
  EXPECT_EQ(tensor2d(0, 0), 1.0);
  EXPECT_EQ(tensor2d(0, 1), 2.0);
  EXPECT_EQ(tensor2d(0, 2), 3.0);
  EXPECT_EQ(tensor2d(1, 0), 4.0);
  EXPECT_EQ(tensor2d(1, 1), 5.0);
  EXPECT_EQ(tensor2d(1, 2), 6.0);
  EXPECT_THROW(tensor2d(2, 0), invalid_argument);
  EXPECT_THROW(tensor2d(0, 3), invalid_argument);
  EXPECT_THROW(tensor2d.item(), runtime_error);
}

TEST(TensorTest, Addition) {
  // scalar + scalar
  shared_ptr<Tensor> tensor1 = make_shared<Tensor>(1.0);
  shared_ptr<Tensor> tensor2 = make_shared<Tensor>(2.0);
  shared_ptr<Tensor> tensor3 = (*tensor1) + (tensor2);
  EXPECT_EQ(tensor3->item(), 3.0);

  // scalar + 1d
  shared_ptr<Tensor> tensor4 = make_shared<Tensor>(2.0);
  shared_ptr<Tensor> tensor5 =
      make_shared<Tensor>(vector<float>({1.0, 2.0, 3.0}));
  shared_ptr<Tensor> tensor6 = (*tensor4) + (tensor5);
  EXPECT_EQ(tensor6->shape(), vector<size_t>({3}));
  EXPECT_EQ((*tensor6)(0), 3.0);
  EXPECT_EQ((*tensor6)(1), 4.0);
  EXPECT_EQ((*tensor6)(2), 5.0);

  // 1d + 1d
  shared_ptr<Tensor> tensor7 =
      make_shared<Tensor>(vector<float>({1.0, 2.0, 3.0}));
  shared_ptr<Tensor> tensor8 =
      make_shared<Tensor>(vector<float>({4.0, 5.0, 6.0}));
  shared_ptr<Tensor> tensor9 = (*tensor7) + (tensor8);
  EXPECT_EQ(tensor9->shape(), vector<size_t>({3}));
  EXPECT_EQ((*tensor9)(0), 5.0);
  EXPECT_EQ((*tensor9)(1), 7.0);
  EXPECT_EQ((*tensor9)(2), 9.0);

  // 1d + scalar
  shared_ptr<Tensor> tensor10 =
      make_shared<Tensor>(vector<float>({1.0, 2.0, 3.0}));
  shared_ptr<Tensor> tensor11 = make_shared<Tensor>(2.0);
  shared_ptr<Tensor> tensor12 = (*tensor10) + (tensor11);
  EXPECT_EQ(tensor12->shape(), vector<size_t>({3}));
  EXPECT_EQ((*tensor12)(0), 3.0);
  EXPECT_EQ((*tensor12)(1), 4.0);
  EXPECT_EQ((*tensor12)(2), 5.0);

  // 2d + 2d
  shared_ptr<Tensor> tensor13 = make_shared<Tensor>(
      vector<vector<float>>({{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}}));
  shared_ptr<Tensor> tensor14 = make_shared<Tensor>(
      vector<vector<float>>({{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}}));
  shared_ptr<Tensor> tensor15 = (*tensor13) + (tensor14);
  EXPECT_EQ(tensor15->shape(), vector<size_t>({2, 3}));
  EXPECT_EQ((*tensor15)(0, 0), 2.0);
  EXPECT_EQ((*tensor15)(0, 1), 4.0);
  EXPECT_EQ((*tensor15)(0, 2), 6.0);
  EXPECT_EQ((*tensor15)(1, 0), 8.0);
  EXPECT_EQ((*tensor15)(1, 1), 10.0);
  EXPECT_EQ((*tensor15)(1, 2), 12.0);
}