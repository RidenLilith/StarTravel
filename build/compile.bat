@echo off

pushd %~dp0\..

set "MINGW64_GPP=C:\msys64\mingw64\bin\g++.exe"
if exist "%MINGW64_GPP%" (
	set "CXX=%MINGW64_GPP%"
) else (
	set "CXX=g++"
)

"%CXX%" src\main.cpp src\glad.c src\VAO.cpp src\VBO.cpp src\ShaderClass.cpp src\EBO.cpp src\planet.cpp src\stb.cpp src\camera.cpp src\time.cpp src\texture.cpp src\window.cpp src\skybox.cpp src\bezier.cpp src\bezierPath.cpp src\cameraController.cpp src\billBoard.cpp -o build\universe.exe -Iinclude "%~dp0\..\lib\libglfw3.a" -lopengl32 -lgdi32 -lwinmm -luser32 -lshell32

build\universe.exe

popd
