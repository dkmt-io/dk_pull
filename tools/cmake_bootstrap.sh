#!/bin/sh

mkdir -p cmake-build

conan install .                          \
  --install-folder cmake-build           \
  --build                                \
  --settings compiler.libcxx=libstdc++11

cmake                                                      \
  -B cmake-build                                           \
  -S .

cmake --build cmake-build -j$(nproc)
