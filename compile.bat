
@echo off

cd /d %~dp0

cmake .

if %errorlevel% neq 0 (
    echo ERROR generation compilation files
    pause
    exit /b %errorlevel%
)

cmake --build .
pause