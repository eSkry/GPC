#!/bin/bash

echo '\nRun Build GPC GCC Script\n'
mkdir ./build
cd ./build
conan install ../conanfile.txt --build=missing --settings compiler.libcxx=libstdc++11
cmake -G "Unix Makefiles" .. -DCMAKE_BUILD_TYPE=Release -DPODOFO_BUILD_SHARED=FALSE -DPODOFO_USE_VISIBILITY=1 -DPODOFO_BUILD_LIB_ONLY=True -DQT_PATH=${HOME}/Qt/5.12.12
cmake --build . --config Release -- -j `nproc`
