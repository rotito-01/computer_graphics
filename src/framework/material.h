#pragma once

class Material {

public:
	Shader* shader;
	Texture* texture;
	Vector3 Ka;
	Vector3 Ks;
	Vector3 Kd;
	int shine;

	Material(Shader* shad, Texture* text, int shi, Vector3 a_coef, Vector3 s_coef, Vector3 d_coef);
	void Enable();
	void Disable();

	struct sLight {
		
	};
};
