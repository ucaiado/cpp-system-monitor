#!/bin/bash
BuildDir="build"

cd /CppND-System-Monitor-Project-Updated/
if [ ! -d "$BuildDir" ]; then
mkdir $BuildDir
fi
cd $BuildDir

# build project
cmake ..
make

# run A*
./test
