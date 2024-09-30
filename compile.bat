@echo off

cd /d %~dp0

cmake .

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
move ALL_BUILD.vcxproj output
move ALL_BUILD.vcxproj.filters output
move ALL_BUILD.vcxproj.user output
move cmake_install.cmake output
move CMakeCache.txt output
move CMakeLists.txt output
move CPackConfig.cmake output
move CPackSourceConfig.cmake output
move INSTALL.vcxproj output
move INSTALL.vcxproj.filters output
move PACKAGE.vcxproj output
move PACKAGE.vcxproj.filters output
move RayLib.sln output
move RayLib.vcxproj output
move RayLib.vcxproj.filters output
move RayLib.vcxproj.user output
move ZERO_CHECK.vcxproj output
move ZERO_CHECK.vcxproj.filters output

echo Compilation and file move complete!
pause