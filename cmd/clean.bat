@echo off
cd  %~dp0
%~d0
cd..
cmake.exe  --build cmake-build-debug  --target clean -- -j 2
