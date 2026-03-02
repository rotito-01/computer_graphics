#pragma once
#include "shader.h"

struct sLight {
	Vector2 position;
	Vector3 color_int;
};

struct sUniformData {
	Matrix44 modelmat;
	Matrix44 viewproj;
	Vector3 ambient;
	int nLights;
};

class Material {

public:
	
	Shader* shader;
	Texture* texture;
	Vector3 Ka;
	Vector3 Ks;
	Vector3 Kd;
	float shine;

	Material(Shader* shad, Texture* text, float shi, Vector3 a_coef, Vector3 s_coef, Vector3 d_coef);
	void Enable(const sUniformData& uniformData);
	void Disable();
	
};
