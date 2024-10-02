@echo off

cd /d %~dp0

if exist output (
    rd /s /q output
)

cd build
cmake ..

if %errorlevel% neq 0 (
    echo ERROR generation compilation files
    pause
    exit /b %errorlevel%
)

cmake --build .

if %errorlevel% neq 0 (
    echo ERROR during the build process
    pause
    exit /b %errorlevel%
)

:: Crear la carpeta "output" donde moveremos los archivos
mkdir output

:: Mover los archivos generados a la carpeta "output"
:: move *.vcxproj output
:: move *.vcxproj.filters output
:: move *.vcxproj.user output
:: move *.cmake output
:: move *.md output
:: move CMakeCache.txt output

echo Compilation and file move complete!
pause