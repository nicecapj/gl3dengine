#version 450 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 WorldPos;
out vec4 color;

uniform vec3 cameraPos;
uniform samplerCube cubemap;

void main()
{             
    //공기	1.00
    //물	1.33
    //얼음	1.309
    //유리	1.52
    //다이아몬드	2.42
    float refractRatio = 1.0/1.52;  //유리
    vec3 I = normalize(WorldPos - cameraPos);
    vec3 R = refract(I, normalize(Normal), refractRatio);    
    color = texture(cubemap, R);
} 