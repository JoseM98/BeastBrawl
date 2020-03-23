#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out mat3 TBN;

uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 MVP;

void main()
{
    //gl_Position = projection * view * model * vec4(aPos, 1.0);
    gl_Position = MVP * vec4(aPos,1.0);
    FragPos = vec3(model * vec4(aPos,1.0));

    // Hacemos esto para ajustar las normales en caso de escalar las mallas
    //Source: http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
    //TODO: Cambiar este calculo a la CPU ya que inversas por cada vertice es muy costoso
    Normal = mat3(transpose(inverse(model))) * aNormal; 

    //Normal mapping
    //NOTA: Tecnicamente no necesitamos la bitangente ya que se calcularia como vec3 B = cross(N,T)
    vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));
    mat3 TBN = mat3(T, B, N);

    TexCoords = aTexCoord;
}