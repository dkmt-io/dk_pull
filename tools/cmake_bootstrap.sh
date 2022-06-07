#!/bin/sh

mkdir -p cmake-build

OS=$(uname -s)

if [[ "$OS" =~ ^Darwin ]]; then
  LIBCXX=libc++
fi

if [[ "$OS" =~ ^Linux ]]; then
  LIBCXX=libstdc++11
fi

conan install .                          \
  --install-folder cmake-build           \
  --build                                \
  --settings compiler.libcxx=$LIBCXX

cmake                                                      \
  -B cmake-build                                           \
  -S .

cmake --build cmake-build -j$(nproc)
