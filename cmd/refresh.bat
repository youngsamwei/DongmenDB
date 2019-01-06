@echo off
cd  %~dp0
%~d0
cd..
set projectdir=%cd%
rmdir /s/q cmake-build-debug
mkdir cmake-build-debug

cd cmake-build-debug
cmake.exe  -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - MinGW Makefiles" %projectdir%
cd ..
