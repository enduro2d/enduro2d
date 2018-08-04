@echo off
set SCRIPT_DIR=%~dp0%
%SCRIPT_DIR%\build_debug.bat
%SCRIPT_DIR%\build_release.bat
