#include "pch.h"
#include "TextRenderer.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include "freetype/freetype.h"
#include <iostream>

TextRenderer::TextRenderer(std::string str, std::string fontName, int size, glm::vec3 color, GLuint program)
{
    text_ = str;
    color_ = color;
    program_ = program;
	fontScale_ = 1.0f;
    //SetPosition(position_);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(800.0f), 0.0f, static_cast<GLfloat>(600.0f));
    glUseProgram(program_);

    GLuint projectMatrix = glGetUniformLocation(program_, "projection");
    glUniformMatrix4fv(projectMatrix, 1, GL_FALSE, glm::value_ptr(projection));

    //TODO : 언리얼에서도 텍스트 랜더객체 만들때마다, 라이브러리 초기화하진 않을것 같은데 찾아보자
    FT_Library ft;
    if (FT_Init_FreeType(&ft))	//0이면 성공...
    {
        std::cout << "Error : FT_Init_FreeType" << std::endl;
    }

    FT_Face face;
    if (FT_New_Face(ft, fontName.c_str(), 0, &face))//0이면 성공...
    {
        std::cout << "Error : FT_New_Face" << std::endl;
    }

    FT_Set_Pixel_Sizes(face, 0, size);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLuint i = 0; i < 128; ++i)
    {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER))//0이면 성공...
        {
            std::cout << "Error : FT_Load_Char : " << i <<  std::endl;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character =
        {
            texture,
            {face->glyph->bitmap.width, face->glyph->bitmap.rows},
            {face->glyph->bitmap_left, face->glyph->bitmap_top},
            face->glyph->advance.x,
        };

        characters_.insert(std::pair<GLchar, Character>(i, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);


    //VBO
    //버텍스어래이 오브젝트를 만들고, 어플리케이션에 바인드하면
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    //버텍스버퍼 오브젝트를 만들어서 바인드 할수 있다. 메모리 형태로 관리됨으로, GL_ARRAY_BUFFER같은 타입 명시가 필요하다.
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    //버텍스버퍼 오브젝트가 만들어지면 실제 데이터를 넘겨서. 버퍼에 복사시킨다.
    //glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices_.size(), &vertices_[0], GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);	//버텍스6개 사각평면 x rgba4 - 글자1개 크기의 버퍼


    glEnableVertexAttribArray(0);	//Vertex 구조체 속성 0번 멤버 활성화 -> position으로 사용

    //normalize안된 값이라서 GL_FALSE
    glVertexAttribPointer(0,	//index of vertex array. 어플리케이션에 여러게 vao가 바인드되었을수 있음으로, 0번에 이 포인터 사용
                          4,	// 4?
                          GL_FLOAT,	//
                          GL_FALSE,	//normalized. 0~1이 아닌 값이면 false한다
                          4 * sizeof(GLfloat),	//stride는 보폭이란 뜻으로, 버퍼에서 어느 사이즈로 뛰어가면서 읽을지를 결정하는 사이즈
                          (GLvoid*)0);	//버텍스 struct안에서 버텍스속성의 옵셋()

    //Unbind buffer and vertex array as a precaution
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

TextRenderer::~TextRenderer()
{
}

void TextRenderer::Draw()
{
    glm::vec2 textPos = this->position2D_;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //set shader
    glUseProgram(program_);

    //model ->  view -> projection matrix를 uniform variables를 통해 쉐이더로 정보를 보낼수 있다.
    //glGetUniformLocation()함수로 쉐이더프로그램 안에 선언된 변수의 이름으로 가져올 수 있다. ex) uniform mat4 model;
    GLuint textColor = glGetUniformLocation(program_, "textColor");
    //glUniformXXX형식의 함수로 쉐이더에  연결된 유니폼 변수에, 값을 설정할 수 있다.
    glUniform3f(textColor, color_.r, color_.g, color_.b);
    glActiveTexture(GL_TEXTURE0);//텍스쳐 0번 활성화. static mesh에서는 이렇게 안했는데?

    glBindVertexArray(vao_);


    for (auto c = text_.begin(); c != text_.end(); c++)
    {
        Character ch = characters_[*c];

        GLfloat xPos = textPos.x + ch.Bearing.x * this->fontScale_;
        GLfloat yPos = textPos.y + (ch.Size.y - ch.Bearing.y) * this->fontScale_;
        GLfloat width = ch.Size.x;
        GLfloat height = ch.Size.y;

        //quad
        GLfloat vertices[6][4] =
        {
            { xPos, yPos + height, 0.0, 0.0 },
            { xPos, yPos, 0.0, 1.0 },
            { xPos + width, yPos, 1.0, 1.0 },

            { xPos, yPos + height, 0.0, 0.0 },
            { xPos + width, yPos, 1.0, 1.0 },
            { xPos + width, yPos + height, 1.0, 0.0 }
        };

        //texture
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);	//vbo업데이트
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);	//1개씩 추가하는 것이 아니라, 6개를 모았다가 그리기 위해 glBufferData대신 glBufferSubData사용

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);	//6개 배열 그려준다.

        //다음 글자가 출력될 위치 결정
        textPos.x += (ch.Advance >> 6) * fontScale_;	//2^6 = 64. advance가 1/64에 해당하는 값을 가지고 있다.

    }


    //static draw의 경우
    //한번만 그릴 데이터를 전부 요구한다.(vao_) 이후 glDrawElements를 통해 그린다.
    //glBindVertexArray(vao_);
    //glDrawElements(GL_TRIANGLES, indicies_.size(), GL_UNSIGNED_INT, 0);

    ////marks end of draw function
    glBindVertexArray(0);
    //glUseProgram(0);
}

void TextRenderer::SetPosition(glm::vec2 position)
{
	position2D_ = position;
}

void TextRenderer::SetText(std::string str)
{
    text_ = str;
}

void TextRenderer::PreDraw()
{
}

void TextRenderer::PostDraw()
{
}
