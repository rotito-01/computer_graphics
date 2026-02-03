#pragma once
#include "mesh.h"
#include "framework.h"
#include "image.h"

class Entity {

public:
	Mesh* mesh;
	Matrix44 model_matrix;

	Entity();
	Entity(Mesh* m, Matrix44 mm);


	void Render(Image* framebuffer, Camera* camera, const Color& c);
	void Update(float seconds_elapsed, int mode);
};