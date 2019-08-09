#version 450 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 WorldPos;


uniform sampler2D Texture;

uniform vec3 lightPos;
uniform vec3 cameraPos;

float specularPower = 3.0f;
float ambientPower = 0.3f;
vec4 ambientColor = vec4(1.0, 0.0, 0.0, 1.0);
vec4 rimLightColor = vec4(0.0, 0.0, 1.0, 1.0);

out vec4 color;

void main(){
		
		color = texture(Texture, TexCoord);
		vec3 normal = normalize(Normal);		
		vec3 lightDir = normalize(lightPos - WorldPos);
		float ndl = max(dot(normal, lightDir), 0);
		//vec4 diffuse = color * ndl;
		vec4 diffuse = color * ndl;
		
		vec4 ambient = vec4(0.3, 0.3, 0.3, 1.0);		
		
		//rim light
		vec3 viewDir = normalize(cameraPos - WorldPos);
		float ndc = max(dot(normal, viewDir), 1);
		float rim;
		if(ndc < 0.05)
			rim = 1;
		else
			rim = 0;
			
		
		vec3 reflectionDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectionDir),0.0),128);			
		spec = spec * specularPower;
		
		
		color = color * (diffuse + (ambient * ambientColor) + spec) + (rim * rimLightColor);
		color.a = 1.0;
		
}