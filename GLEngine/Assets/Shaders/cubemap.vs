#version 450 core
layout (location = 0) in vec3 position;

out vec3 TextureDir; // 3D 텍스처 좌표를 나타내는 방향 벡터

uniform mat4 vp;
uniform mat4 model;
uniform vec3 textureDir;


void main(){

	gl_Position = vp * model *vec4(position, 1.0);	
	TextureDir = position;
}