#pragma once
#include "shader.h"
#include "camera.h"

struct sLight {
    Vector3 position;
    Vector3 color_int;
};

struct sUniformData {
	Matrix44 modelmat;
	Matrix44 viewproj;
	Vector3 ambient;
	int nLights;
	Vector3 cam_eye;
	sLight light;
	Camera* cam;
};

class Material {

public:
	
	Shader* shader;
	Texture* texture;
    Texture* normalTexture;
	Vector3 Ka;
	Vector3 Ks;
	Vector3 Kd;
	float shine;
    
    bool useTexture;
    bool useSpecular;
    bool useNormal;

	Material(Shader* shad, Texture* text, float shi, Vector3 a_coef, Vector3 s_coef, Vector3 d_coef);
	void Enable(const sUniformData& uniformData);
	void Disable();
	
};
