#version 450 core

uniform sampler2D screenTexture;

out vec4 color;
in vec2 TexCoord;

//offset가 1이 아닌 이유는 텍셀임으로, 텍스쳐의 offset가 1로 안 떨어짐으로
const float offset = 1.0/300.0;


void main(){
	//프레임버퍼에서 일반 이미지와 틀리게 뒤집혀 있을것임으로 1-x해서 상하 반전 시킨다.	
	color = texture(screenTexture, vec2(TexCoord.r, 1-TexCoord.g));
	
	//inverse
	//color = 1 - texture(screenTexture, vec2(TexCoord.r, 1-TexCoord.g));
	//color.a = 1.0;
	
	//gray	
	//color = (color.r + color.g + color.b)/3;	simple gray
	//float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
	//color = vec4(average,average,average,1.0);
	
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 좌측 상단
        vec2( 0.0f,    offset), // 중앙 상단
        vec2( offset,  offset), // 우측 상단
        vec2(-offset,  0.0f),   // 좌측 중앙
        vec2( 0.0f,    0.0f),   // 정중앙
        vec2( offset,  0.0f),   // 우측 중앙
        vec2(-offset, -offset), // 좌측 하단
        vec2( 0.0f,   -offset), // 중앙 하단
        vec2( offset, -offset)  // 우측 하단   
    );
	
	//커널의 합은 보통1(원래 색이 나와야 하는데 비율로 주변 픽셀값 조정)
	//sharpern
	//float kernel[9] = float[](
    //    -1, -1, -1,
    //    -1,  9, -1,
    //    -1, -1, -1
    //);
	
	//blur
	float kernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
	);
	
	//edge dection
	//float kernel[9] = float[](
    //    1, 1, 1,
    //    1, 1-8, 1,
    //    1, 1, 1
    //);
	1111−81111
	
	vec3 sampleTex[9];
	for(int i = 0 ; i < 9 ; i++)
	{
		sampleTex[i] = vec3(texture(screenTexture, vec2(TexCoord.s + offsets[i].x, 1-TexCoord.t+ + offsets[i].y)));
	}
	
	vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
		
	color = vec4(col, 1.0);
}