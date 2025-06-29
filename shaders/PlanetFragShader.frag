#version 330 core
out vec4 FragColor;

in vec2 TextCoord;

// Parâmetros de iluminação
uniform sampler2D tex0;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColor;

// Normais e posição do fragmento
in vec3 FragPos;
in vec3 Normal;

// Novo parâmetro: intensidade de emissão
uniform float emissiveStrength;

void main()
{
    // Componente ambiente
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    // Componente difusa
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Cor da textura
    vec3 texColor = texture(tex0, TextCoord).rgb;

    // Emissão: cor do próprio objeto, sem depender da luz
    vec3 emission = emissiveStrength * texColor;

    // Resultado final
    vec3 result = (ambient + diffuse) * texColor + emission;
    FragColor = vec4(result, 1.0);
}
