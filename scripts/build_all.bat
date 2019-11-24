@echo off
set SCRIPT_DIR=%~dp0%
call %SCRIPT_DIR%\build_debug_x86.bat || goto :error
call %SCRIPT_DIR%\build_debug_x64.bat || goto :error
call %SCRIPT_DIR%\build_release_x86.bat || goto :error
call %SCRIPT_DIR%\build_release_x64.bat || goto :error

goto :EOF

:error
echo Failed with error #%errorlevel%.
exit /b %errorlevel%
