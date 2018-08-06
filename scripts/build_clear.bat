@echo off
set BUILD_DIR=%~dp0%\..\build
rmdir /s /q %BUILD_DIR% || goto :error

goto :EOF

:error
echo Failed with error #%errorlevel%.
exit /b %errorlevel%
