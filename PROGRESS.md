# Neural Network C++ Project - Progress Report

This document tracks the detailed progress, implemented features, and future roadmap of the Neural Network project in C++.

**Last Updated:** 2026-01-29

## 📊 Current Status
- **Build System:** `g++` (MinGW/GCC)
- **Test Framework:** Custom `mini_gtest.h` (mocking Google Test)
- **Compilation:** ✅ Successful
- **Tests:** ✅ All Passing (Creation, Addition)

---

## 🛠 Implemented Components

### 1. **Tensor Class (`tensor.h` / `tensor.cpp`)**
The core data structure for the neural network.

#### **Data Management**
- **Storage:** Uses a flat `std::vector<float> _data` for contiguous memory layout (row-major).
- **Shape & Stride:** Maintains `_shape` and `_stride` vectors (type `size_t`) to handle multi-dimensional indexing.
- **Memory Management:** Efficient use of `std::shared_ptr<Tensor>` for operations to manage types and potentially separate data ownership (autograd preparation).

#### **Constructors**
- **Scalar:** `Tensor(float data)` - Creates a 0-dimensional tensor.
- **1D Vector:** `Tensor(vector<float> data)` - Creates a 1D tensor from a list.
- **2D Matrix:** `Tensor(vector<vector<float>> data)` - Creates a 2D tensor from a nested list.
- **Shape Only:** `Tensor(vector<size_t> shape)` - Allocates a zero-initialized tensor of the given shape.

#### **Accessors & Utilities**
- **`item()`**: Retrieves the value of a scalar tensor (throws error if not scalar).
- **`operator()(i)`**: Access element in 1D tensor.
- **`operator()(i, j)`**: Access element in 2D tensor.
- **`shape()` / `stride()`**: Inspect tensor dimensions.
- **`operator<<`**: Friend overload for `std::ostream` to print tensors formatted (e.g., `[1.0, 2.0]`).

#### **Operations**
- **Addition (`operator+`)**:
    - **Signature:** `shared_ptr<Tensor> operator+(shared_ptr<Tensor> other)`
    - **Broadcasting Support:**
        - **Scalar + Scalar**
        - **Scalar + 1D / 2D** (Scalar added to every element)
        - **1D / 2D + Scalar**
    - **Element-wise:**
        - **1D + 1D**
        - **2D + 2D**
    - **Logic:** Returns a new `shared_ptr<Tensor>` containing the result.

---

## 🧪 Testing Infrastructure

### **Framework (`mini_gtest.h`)**
A header-only replacement for Google Test created to avoid external dependency issues.
- **`TEST` macro**: Registers test cases and test names.
- **`EXPECT_EQ`**: Checks for equality and prints expected/actual on failure.
- **`EXPECT_THROW`**: Verifies that specific exceptions are thrown.
- **`RUN_ALL_TESTS()`**: Executes all registered tests and reports pass/fail status.
- **Exception Handling**: Catches and prints `std::exception::what()` for easier debugging.

### **Test Suite (`tests.cpp`)**
- **`TensorTest.Creation`**:
    - Verifies 0D, 1D, and 2D creation.
    - Checks `shape()` correctness.
    - Validates exception throwing for out-of-bounds access.
- **`TensorTest.Addition`**:
    - Verifies Scalar+Scalar.
    - Verifies Scalar+Tensor broadcasting (commutative).
    - Verifies Element-wise addition for 1D and 2D.
    - Uses `shared_ptr` syntax: `(*t1) + t2`.

---

## 📋 usage Instructions

### Compilation
```bash
# Compile Main Application
g++ main.cpp tensor.cpp -o main.exe

# Compile Tests
g++ tests.cpp tensor.cpp -o tests.exe
```

### Execution
```bash
# Run Main
./main.exe

# Run Tests
./tests.exe
```

---

## 📝 TODO / Roadmap

- [ ] **Multiplication (`operator*`)**: Implement element-wise and matrix multiplication (MatMul).
- [ ] **Autograd Engine**:
    - [ ] Create `Value` or `Node` struct for computational graph.
    - [ ] Implement `backward()` pass.
- [ ] **Activation Functions**: Implement ReLU, Sigmoid.
- [ ] **Loss Functions**: Implement MSE, CrossEntropy.
- [ ] **Optimizer**: Implement SGD or Adam.
- [ ] **Refactoring**: Move `mini_gtest.h` to a dedicated `tests/` directory if the project grows.
