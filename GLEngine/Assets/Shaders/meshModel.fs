#version 450 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 WorldPos;


uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_diffuse4;
uniform sampler2D texture_diffuse5;
uniform sampler2D texture_diffuse6;

uniform samplerCube cubemap;

uniform vec3 lightPos;
uniform vec3 cameraPos;

float specularPower = 64.0f;
float ambientPower = 0.33f;
vec4 ambientColor = vec4(1.0, 1.0, 1.0, 1.0);
vec4 rimLightColor = vec4(1.0, 1.0, 1.0, 1.0);

out vec4 color;

void main(){		
		color = texture(texture_diffuse1, TexCoord);		
		ambientColor = ambientColor * ambientPower;
		
		vec3 normal = normalize(Normal);		
		vec3 lightDir = normalize(lightPos - WorldPos);
		float ndl = max(dot(normal, lightDir), 0);
		//vec4 diffuse = color * ndl;
		vec4 diffuse = color * ndl;
		
		
		//rim light
		vec3 viewDir = normalize(cameraPos - WorldPos);
		//float ndc = max(dot(normal, viewDir), 0);
		//float rim;
		//if(ndc < 0.3)
		//	rim = 1-ndc;
		//else
		//	rim = 0;
		
		//half lambert
		diffuse = diffuse*0.5 + 0.5;

		//reflect env
		//vec3 I = -viewDir;			
		//vec3 R = reflect(I, normalize(Normal));
		//vec4 envColor = texture(cubemap, R);
		//diffuse *= envColor;
		
		//refraction
		//float refractRatio = 1.0/1.52;  //유리    
		//vec3 R = refract(I, normalize(Normal), refractRatio); 
		//vec4 envColor = texture(cubemap, R);
					
		//PHONG Specular
		vec3 reflectionDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectionDir),0.0),specularPower);					
		
		//color = vec4(vec3(gl_FragCoord.z), 1.0);  fragment내장 쉐이더 gl_FragCoord.z 에 깊이버퍼값 저장되 있음.
		
		//color = color * (diffuse + ambientColor + spec) + (rim * rimLightColor);
		color = color * (diffuse + ambientColor + spec);
		color.a = 1.0;				
}