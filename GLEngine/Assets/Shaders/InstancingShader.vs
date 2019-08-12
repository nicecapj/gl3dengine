#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in mat4 instanceMatrix;

out vec2 TexCoord;
out vec3 Normal;
out vec3 WorldPos;

uniform mat4 vp;
//uniform mat4 model;
//uniform vec3 instancingPos[128];

void main(void){	
	//vec3 offset = instancingPos[gl_InstanceID];		
	gl_Position = vp * instanceMatrix * vec4(position, 1.0);
	TexCoord = texCoord;		
	Normal = mat3(transpose(inverse(instanceMatrix))) * normal;
	WorldPos = vec3(instanceMatrix *vec4(position, 1.0));
}