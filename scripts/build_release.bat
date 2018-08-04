@echo off
set BUILD_DIR=%~dp0%\..\build
mkdir %BUILD_DIR%\release
cd %BUILD_DIR%\release
cmake ../..
cmake --build . --config Release
ctest
cd ..\..
