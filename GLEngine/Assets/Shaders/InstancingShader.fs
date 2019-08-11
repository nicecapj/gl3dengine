#version 450 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 WorldPos;

out vec4 color;

uniform sampler2D Texture;
uniform vec3 lightPos;


void main(){		
	color = texture(Texture, TexCoord);	
}