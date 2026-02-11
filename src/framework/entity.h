#pragma once
#include "mesh.h"
#include "framework.h"
#include "image.h"

class Entity {

public:
	Mesh* mesh;
	Matrix44 model_matrix;
	float displacement = 0;
	float growth = 0;
	Image texture;

	Entity();
	Entity(Mesh* m, Matrix44 mm, Image text);


	void Render(Image* framebuffer, Camera* camera, FloatImage* zBuffer, bool lab_mode, bool wire, bool occlusion, const Color& c, bool texture);
	void Update(float seconds_elapsed, int mode);
};
