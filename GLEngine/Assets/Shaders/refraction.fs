#version 450 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 WorldPos;
out vec4 color;

uniform vec3 cameraPos;
uniform samplerCube cubemap;

void main()
{             
    //����	1.00
    //��	1.33
    //����	1.309
    //����	1.52
    //���̾Ƹ��	2.42
    float refractRatio = 1.0/1.52;  //����
    vec3 I = normalize(WorldPos - cameraPos);
    vec3 R = refract(I, normalize(Normal), refractRatio);    
    color = texture(cubemap, R);
} 