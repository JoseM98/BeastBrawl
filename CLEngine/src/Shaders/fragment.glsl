#version 450 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float specularStrength;
uniform int shininess;

void main()
{
    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);
    // ambient
    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm,lightDir), 0.0);
    vec3 diffuse = diff*lightColor;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos); //Vector entre nosotros y el punto del objeto
    vec3 reflectDir = reflect(-lightDir, norm);  //Angulo reflectado
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess); //Formula de la luz especular
    vec3 specular = specularStrength * spec * lightColor;  //Multiplicamos todo 

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}