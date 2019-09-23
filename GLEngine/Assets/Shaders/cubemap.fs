#version 450 core

uniform samplerCube cubemap;
in vec3 TextureDir; // 3D 텍스처 좌표를 나타내는 방향 벡터

out vec4 color;

void main()
{             
    color = texture(cubemap, TextureDir);
} 