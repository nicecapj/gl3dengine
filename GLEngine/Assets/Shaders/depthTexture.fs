#version 450 core
layout (location = 0) out vec4 color;

void main()
{             
    // gl_FragDepth = gl_FragCoord.z;	//depthbuffer
	color = vec4(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z,1);	//depth texture
}