#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 WorldPos;

uniform mat4 vp;
uniform mat4 model;


void main(){

	gl_Position = vp * model *vec4(position, 1.0);
	
	TexCoord = texCoord;	
	
	//노말을 단위 노말로 바꾼다
	Normal = mat3(transpose(inverse(model))) * normal;
	WorldPos = vec3(model *vec4(position, 1.0));
}