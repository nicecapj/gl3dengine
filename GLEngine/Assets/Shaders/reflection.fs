#version 450 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 WorldPos;
out vec4 color;

uniform vec3 cameraPos;
uniform samplerCube cubemap;

void main()
{             
    vec3 I = normalize(WorldPos - cameraPos);
    vec3 R = reflect(I, normalize(Normal));    
    color = texture(cubemap, R);
} 