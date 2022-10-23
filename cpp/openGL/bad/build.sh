#!/bin/bash
g++ --std=c++20 -c main.cpp glad.c VBO.cpp EBO.cpp VAO.cpp shaderClass.cpp Texture.cpp camera.cpp -lglfw -Iinclude -lGL
g++ *.o -lglfw -Iinclude -lGL -o main
./main
