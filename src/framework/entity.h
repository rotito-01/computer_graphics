#pragma once
#include "mesh.h"
#include "framework.h"
#include "image.h"
#include "shader.h"
#include "material.h"

class Entity {

public:
	Mesh* mesh;
	Matrix44 model_matrix;
	float displacement = 0;
	float growth = 0;
	Image texture;
	Shader* shader;
	Material* material;

	Entity();
	Entity(Mesh* m, Matrix44 mm, Image text, Shader* shad, Material* mat);

	void Render(sUniformData& uniformData);
	void Update(float seconds_elapsed, int mode);
};
