#version 450 core
layout (location = 0) out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace)
{
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;	//-1~1 Light Space, 직교는 w안 변하니 무의미, 원근은 의미 있다.
	projCoords = projCoords * 0.5 + 0.5;	//make NDC Space (0~1)
	
	//조명좌표에서 가장 가까운 깊이를 제공한다(-1~1인것을 텍스쳐좌표 0~1에 맞추었음으로, 꼭 정확하게 일치하는것은 아니다 - 오차생김)
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	
	//뎁스 텍스처에 기록된 값 말고, 원래 라이트 좌표기준 투영된 z값 가져온다.
	float currentDepth = projCoords.z;
	
	float shadow = currentDepth > closestDepth ? 1.0:0.0f;
	

    //return shadow;
	return 0.0;	//shadowMap 제대로 안 넘어오고 있음. 수정해야 함. 
}

void main()
{           
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;	
	
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(1.0);
    
	// ambient    
	vec3 ambient = 0.15 * color;
    
	// diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    
	// specular(blinn phong)
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 128.0);
    vec3 specular = spec * lightColor;    
    
	// calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);       
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);	
}