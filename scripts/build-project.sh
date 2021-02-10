#!/bin/bash
BuildDir="build"

cd /CppND-System-Monitor-Project-Updated/

# build project
make clean
make build

# start some process to monitor
for i in {1..3}; do echo -n ""; date ; sleep 5; done > /dev/null 2>&1 &
watch free -m > /dev/null 3>&1 &

# run monitor
./build/monitor
