@echo off
set BUILD_DIR=%~dp0%\..\build
mkdir %BUILD_DIR%\debug || goto :error
cd %BUILD_DIR%\debug || goto :error
cmake ../.. || goto :error
cmake --build . --config Debug || goto :error
ctest --verbose || goto :error
cd ..\.. || goto :error

goto :EOF

:error
echo Failed with error #%errorlevel%.
exit /b %errorlevel%
