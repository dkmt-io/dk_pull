#!/bin/sh

cmake --build cmake-build -j$(nproc) $@
