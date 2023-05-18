#!/bin/bash

# Install libnoise library

#check if libnoise already installed
if [[ $(ls /usr/local/include | grep noise) ]]; then
  # libnoise installed
  echo "libnoise library already installed"
else
  # libnoise not installed
  echo "trying to install libnoise library. If it fails, please install it manually"
  cd ..
  git clone git@github.com:qknight/libnoise.git
  cd libnoise
  mkdir build
  cd build
  cmake ..
  make
  sudo make install
  cd ../..
fi

# DIANA-Template Cpp - 2023
# Crafted on a stormy and dark night by Federico Mustich