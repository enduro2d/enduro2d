@echo off
set BUILD_DIR=%~dp0%\..\build
mkdir %BUILD_DIR%\Release\x64 || goto :error
cd %BUILD_DIR%\Release\x64 || goto :error
cmake ..\..\.. -A x64 || goto :error
cmake --build . --config Release || goto :error
ctest --verbose || goto :error
cd ..\..\.. || goto :error

goto :EOF

:error
echo Failed with error #%errorlevel%.
exit /b %errorlevel%
