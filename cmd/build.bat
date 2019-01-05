set exp_target=%1
cd  %~dp0
%~d0
cd..
cmake.exe  --build cmake-build-debug  --target %exp_target%  -- -j 2
