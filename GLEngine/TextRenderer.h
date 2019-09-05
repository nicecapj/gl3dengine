#pragma once
#include "Mesh.h"
#include "ShaderLoader.h"
#include "Renderer.h"
#include <vector>
#include <map>

struct Character
{
    GLuint TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    GLuint Advance;
};

//생성될때마다 freetype 초기화한다. 이건 성능 개선하려면 공유구조로 만들수 있을듯
//Draw할때마다 폰트수만큼 돌면서, 생성시 만들어두긴 했지만, 텍스처를 글자수만큼 설정하고, 그려줘야 한다.
//static과 틀리게 VBO를 업데이트 해주어야 하기때문에 성능 저하가 있다.
class TextRenderer : public Renderer
{
public:
    TextRenderer(std::string str, std::string fontName, int size, glm::vec3 color, GLuint program);
    ~TextRenderer();
    	
	virtual void PreDraw() override;	
	virtual void Draw() override;
	virtual void PostDraw() override;

	void SetPosition(glm::vec2 position);	
    void SetText(std::string str);



private:
    std::string text_;        
    glm::vec2 position2D_;
	GLfloat fontScale_;


    //동적으로 만들어낼것임으로 필요없다.dynamicMesh
    //    std::vector<Vertex> vertices_;
    //    std::vector<GLuint> indicies_;
    //    GLuint texture_;

    std::map<GLchar, Character> characters_;    
};