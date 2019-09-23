#include "pch.h"
#include "TextureLoader.h"
#include "gli/gli.hpp"
#include <gli/generate_mipmaps.hpp>
#include <SOIL.h>


TextureLoader::TextureLoader()
{
}


TextureLoader::~TextureLoader()
{
}

GLuint TextureLoader::GetTextureID(std::string textureFileName, bool useGLI)
{
	gli::texture Texture = gli::load(textureFileName.c_str());
	if (Texture.empty())
		return 0;

	gli::gl GL(gli::gl::PROFILE_GL33);
	gli::gl::format const Format = GL.translate(Texture.format(), Texture.swizzles());
	GLenum Target = GL_TEXTURE_2D;
	Target = GL.translate(Texture.target());

	GLuint TextureName = 0;
	glGenTextures(1, &TextureName);
	//glBindTexture(GL_TEXTURE_2D, TextureName);
	glBindTexture(Target, TextureName);

	//MIPMAP
	//UI�� ���� ó�� ���ϵ��� ���߿� ó������
	//glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glGenerateMipmap(Target);
	//gli::texture2d TextureMipmaped = gli::generate_mipmaps((gli::texture2d)Texture, gli::FILTER_LINEAR);

	//FILTER
	//�ϴ� �ݺ��� �ص�. ���߿� �����ؼ�, �����Ҽ� �ֵ���
	glTexParameteri(Target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(Target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	//GLI���� ����ϴ� �⺻ ������
	glTexParameteri(Target, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(Target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(Texture.levels() - 1));
	glTexParameteri(Target, GL_TEXTURE_SWIZZLE_R, Format.Swizzles[0]);
	glTexParameteri(Target, GL_TEXTURE_SWIZZLE_G, Format.Swizzles[1]);
	glTexParameteri(Target, GL_TEXTURE_SWIZZLE_B, Format.Swizzles[2]);
	glTexParameteri(Target, GL_TEXTURE_SWIZZLE_A, Format.Swizzles[3]);

	glm::tvec3<GLsizei> const Extent(Texture.extent());
	GLsizei const FaceTotal = static_cast<GLsizei>(Texture.layers() * Texture.faces());

	switch (Texture.target())
	{
	case gli::TARGET_1D:
		glTexStorage1D(
			Target, static_cast<GLint>(Texture.levels()), Format.Internal, Extent.x);
		break;
	case gli::TARGET_1D_ARRAY:
	case gli::TARGET_2D:
	case gli::TARGET_CUBE:
		glTexStorage2D(
			Target, static_cast<GLint>(Texture.levels()), Format.Internal,
			Extent.x, Texture.target() == gli::TARGET_2D ? Extent.y : FaceTotal);
		break;
	case gli::TARGET_2D_ARRAY:
	case gli::TARGET_3D:
	case gli::TARGET_CUBE_ARRAY:
		glTexStorage3D(
			Target, static_cast<GLint>(Texture.levels()), Format.Internal,
			Extent.x, Extent.y,
			Texture.target() == gli::TARGET_3D ? Extent.z : FaceTotal);
		break;
	default:
		assert(0);
		break;
	}

	for (std::size_t Layer = 0; Layer < Texture.layers(); ++Layer)
		for (std::size_t Face = 0; Face < Texture.faces(); ++Face)
			for (std::size_t Level = 0; Level < Texture.levels(); ++Level)
			{
				GLsizei const LayerGL = static_cast<GLsizei>(Layer);
				glm::tvec3<GLsizei> Extent(Texture.extent(Level));
				Target = gli::is_target_cube(Texture.target())
					? static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + Face)
					: Target;

				switch (Texture.target())
				{
				case gli::TARGET_1D:
					if (gli::is_compressed(Texture.format()))
						glCompressedTexSubImage1D(
							Target, static_cast<GLint>(Level), 0, Extent.x,
							Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
							Texture.data(Layer, Face, Level));
					else
						glTexSubImage1D(
							Target, static_cast<GLint>(Level), 0, Extent.x,
							Format.External, Format.Type,
							Texture.data(Layer, Face, Level));
					break;
				case gli::TARGET_1D_ARRAY:
				case gli::TARGET_2D:
				case gli::TARGET_CUBE:
					if (gli::is_compressed(Texture.format()))
						glCompressedTexSubImage2D(
							Target, static_cast<GLint>(Level),
							0, 0,
							Extent.x,
							Texture.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
							Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
							Texture.data(Layer, Face, Level));
					else
						glTexSubImage2D(
							Target, static_cast<GLint>(Level),
							0, 0,
							Extent.x,
							Texture.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
							Format.External, Format.Type,
							Texture.data(Layer, Face, Level));
					break;
				case gli::TARGET_2D_ARRAY:
				case gli::TARGET_3D:
				case gli::TARGET_CUBE_ARRAY:
					if (gli::is_compressed(Texture.format()))
						glCompressedTexSubImage3D(
							Target, static_cast<GLint>(Level),
							0, 0, 0,
							Extent.x, Extent.y,
							Texture.target() == gli::TARGET_3D ? Extent.z : LayerGL,
							Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
							Texture.data(Layer, Face, Level));
					else
						glTexSubImage3D(
							Target, static_cast<GLint>(Level),
							0, 0, 0,
							Extent.x, Extent.y,
							Texture.target() == gli::TARGET_3D ? Extent.z : LayerGL,
							Format.External, Format.Type,
							Texture.data(Layer, Face, Level));
					break;
				default: assert(0); break;
				}
			}
	return TextureName;
}

GLuint TextureLoader::GetTextureID(std::string textureFileName)
{
	return SOIL_load_OGL_texture(textureFileName.c_str(), 0, 0, SOIL_FLAG_TEXTURE_REPEATS);
}

GLuint TextureLoader::GetCubemapTextureID(std::string textureFileName)
{
	//SOIL_load_OGL_cubemap()	//6�� ���Ϸ� �����Ǿ� ������, 1���� ������ �Ϳ� ������ �ѱ�� ���� ������.
	char face_order[6] = { 'N', 'S', 'W', 'E', 'U', 'D'};
	return SOIL_load_OGL_single_cubemap(textureFileName.c_str(), face_order, 0, 0, 0);
}

GLuint TextureLoader::GetCubemapTextureID(std::string cubemapOrderedFileName[6])
{
	//GL_TEXTURE_CUBE_MAP_NEGATIVE_X~
	return SOIL_load_OGL_cubemap(
		cubemapOrderedFileName[0].c_str(),
		cubemapOrderedFileName[1].c_str(),
		cubemapOrderedFileName[2].c_str(),
		cubemapOrderedFileName[3].c_str(),
		cubemapOrderedFileName[4].c_str(),
		cubemapOrderedFileName[5].c_str(), 0, 0, 0);
}
