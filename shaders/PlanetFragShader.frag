#version 330 core
out vec4 FragColor;

in vec2 TextCoord;

// Parâmetros de iluminação
uniform sampler2D tex0;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos; // posição da câmera
uniform vec3 objectColor;

// Normais interpoladas e posição do fragmento (ainda vamos passar isso)
in vec3 FragPos;
in vec3 Normal;

void main()
{
    // Ambiente
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    // Difuso
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Cor final
    vec3 texColor = texture(tex0, TextCoord).rgb;
    vec3 result = (ambient + diffuse) * texColor;

    FragColor = vec4(result, 1.0);
}
