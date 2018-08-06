@echo off
set SCRIPT_DIR=%~dp0%
%SCRIPT_DIR%\build_debug.bat || goto :error
%SCRIPT_DIR%\build_release.bat || goto :error

goto :EOF

:error
echo Failed with error #%errorlevel%.
exit /b %errorlevel%
