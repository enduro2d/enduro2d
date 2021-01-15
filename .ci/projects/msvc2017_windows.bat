@echo off
set DIR=%~dp0

set ROOT_DIR=%DIR%..\..\
set BUILD_DIR=%ROOT_DIR%build\msvc2017_windows\

if not exist %BUILD_DIR% mkdir %BUILD_DIR% || goto :error

pushd %BUILD_DIR% || goto :error
cmake %ROOT_DIR% -G "Visual Studio 15 2017" || goto :error
start enduro2d.sln || goto :error
popd || goto :error

goto :EOF

:error
echo Failed with error #%errorlevel%.
exit /b %errorlevel%
