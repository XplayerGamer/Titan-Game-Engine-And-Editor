@echo off
cd /d "%~dp0"
echo Starting Titan Editor...
echo.
Titan.Editor.exe > output.log 2>&1
echo.
echo Exit code: %errorlevel%
echo.
echo Press any key to continue...
pause > nul