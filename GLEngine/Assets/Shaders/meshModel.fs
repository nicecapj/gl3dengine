#version 450 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 WorldPos;


uniform sampler2D texture_diffuse1;

uniform vec3 lightPos;
uniform vec3 cameraPos;

float specularPower = 3.0f;
float ambientPower = 0.33f;
vec4 ambientColor = vec4(0.33, 0.33, 0.33, 1.0);
vec4 rimLightColor = vec4(0.0, 0.0, 1.0, 1.0);

out vec4 color;

void main(){
		color = texture(texture_diffuse1, TexCoord);
		
		ambientColor = color * ambientPower;
		
		vec3 normal = normalize(Normal);		
		vec3 lightDir = normalize(lightPos - WorldPos);
		float ndl = max(dot(normal, lightDir), 0);
		//vec4 diffuse = color * ndl;
		vec4 diffuse = color * ndl;
		
		vec4 ambient = vec4(0.3, 0.3, 0.3, 1.0);		
		
		//rim light
		vec3 viewDir = normalize(cameraPos - WorldPos);
		float ndc = max(dot(normal, viewDir), 0);
		float rim;
		if(ndc < 0.1)
			rim = 1;
		else
			rim = 0;
					
		//PHONG Specular
		vec3 reflectionDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectionDir),0.0),128);			
		spec = spec * specularPower;
		
		//color = vec4(vec3(gl_FragCoord.z), 1.0);  fragment내장 쉐이더 gl_FragCoord.z 에 깊이버퍼값 저장되 있음.
		
		color = color * (diffuse + (ambient * ambientColor) + spec) + (rim * rimLightColor);
		color.a = 1.0;
}