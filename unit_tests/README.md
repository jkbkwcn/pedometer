## Unit tests using GoogleTest

Tests are prepared to run on PC and build independently of the ESP-IDF environment in unit_tests directory.

### Requirements

- C++17 compiler (e.g. g++, clang++)
- cmake
- make or ninja
- Internet connection (to download googletests)

### Compilation and running tests

Each test is prepared for given file or class. To run tests go into class test directory e.g.

```bash
cd unit_tests
cd system_data
```

Then in the specyfic directory type

```bash
mkdir build
cd built
cmake ..
cmake --build .
ctest
```
