#pragma once
#include "glm/glm.hpp"
#include "gl/glew.h"

const int NUM_BONES_PER_VEREX = 75;
struct VertexBoneData
{
	GLuint IDs[NUM_BONES_PER_VEREX];
	float Weights[NUM_BONES_PER_VEREX];
};


class Animation
{
public:
	Animation();
	~Animation();

private:
	glm::mat4 inverseRootTransformMatrix;
};