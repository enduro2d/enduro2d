@echo off
set BUILD_DIR=%~dp0%\..\build
mkdir %BUILD_DIR%\msvc2017
cd %BUILD_DIR%\msvc2017
cmake -G "Visual Studio 15 2017" ..\..
start enduro2d.sln
