@echo off
set BUILD_DIR=%~dp0%\..\build
mkdir %BUILD_DIR%\debug
cd %BUILD_DIR%\debug
cmake ../..
cmake --build . --config Debug
ctest
cd ..\..
