@echo off
REM Titan Engine Build Script for Windows

echo.
echo ========================================
echo   Titan Engine Build System
echo ========================================
echo.

REM Check if build directory exists
if not exist "build" (
    echo Creating build directory...
    mkdir build
)

REM Change to build directory
cd build

REM Run CMake configuration
echo.
echo Running CMake configuration...
echo.
cmake .. -G "Visual Studio 16 2019" -A x64

REM Check if configuration succeeded
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: CMake configuration failed!
    cd ..
    exit /b 1
)

REM Build the project
echo.
echo Building project...
echo.
cmake --build . --config Release

REM Check if build succeeded
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Build failed!
    cd ..
    exit /b 1
)

REM Return to root directory
cd ..

echo.
echo ========================================
echo   Build Complete!
echo ========================================
echo.
echo To run the example game:
echo   .\build\Release\TitanGame.exe
echo.
echo To clean the build:
echo   rmdir /s /q build
echo.
