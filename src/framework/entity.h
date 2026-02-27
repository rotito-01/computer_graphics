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

	Entity();
	Entity(Mesh* m, Matrix44 mm, Image text, Shader* shader);


	void Render(Image* framebuffer, Camera* camera, FloatImage* zBuffer, bool lab_mode, bool wire, bool occlusion, const Color& c, bool texture);
	void Render(Camera* camera);
	void Update(float seconds_elapsed, int mode);
};
