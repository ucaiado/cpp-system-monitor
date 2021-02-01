#!/bin/bash
BuildDir="build"

cd /CppND-System-Monitor-Project-Updated/

# build project
make build

# run monitor
./build/monitor
