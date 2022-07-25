@echo off

mkdir .\build_release
cd .\build_release
conan install ..\conanfileWin.txt  --build=missing --settings arch=x86 --settings arch_build=x86 --settings build_type=Release --build=missing --settings compiler="Visual Studio" --settings compiler.version=15 
cmake -G "Visual Studio 15 2017" .. -DCMAKE_BUILD_TYPE=Release -DPODOFO_BUILD_SHARED=FALSE -DPODOFO_USE_VISIBILITY=1 -DPODOFO_BUILD_LIB_ONLY=True -DQT_PATH=C:\Qt\5.9.9
cmake --build . --config Release
