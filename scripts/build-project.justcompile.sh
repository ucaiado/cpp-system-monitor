#!/bin/bash
BuildDir="build"

cd /CppND-System-Monitor-Project-Updated/

# format the project
make clean
make format

# build project
make build
