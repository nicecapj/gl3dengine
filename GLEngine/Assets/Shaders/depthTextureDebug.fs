#version 330 core
layout (location = 0) out float FragColor;
//out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D depthMap;

void main()
{             
    float depthValue = texture(depthMap, TexCoord).r;
    //FragColor = vec4(vec3(depthValue), 1.0);

	FragColor = gl_FragCoord.z;
}  