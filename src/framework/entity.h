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
	Shader* shader;
	Material* material;

	Entity();
	Entity(Mesh* m, Matrix44 mm, Shader* shad, Material* mat);

	void Render(Camera* camera); // Lab 4
	void Render(sUniformData& uniformData); // Lab 5 lighting
	void Update(float seconds_elapsed, int mode);
};
