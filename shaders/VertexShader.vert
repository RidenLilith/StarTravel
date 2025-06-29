#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;
// Normais calculadas no planeta.cpp — vamos já resolver isso
layout(location = 2) in vec3 aNormal;

out vec2 TextCoord;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 camMatrix;

void main() {
    gl_Position = camMatrix * model * vec4(aPos, 1.0);
    TextCoord = aTex;

    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
}
