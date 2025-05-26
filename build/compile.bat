@echo off

:: Cria a pasta build se não existir
if not exist build mkdir build

:: Compilação
g++ src\main.cpp src\glad.c src\VAO.cpp src\VBO.cpp src\ShaderClass.cpp src\EBO.cpp src\planet.cpp src\stb.cpp src\camera.cpp src\time.cpp src\texture.cpp -o build\universe.exe -Iinclude -Llib -lglfw3 -lopengl32 -lgdi32

echo Compilado com sucesso!
build\universe.exe
