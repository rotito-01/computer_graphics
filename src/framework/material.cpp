#include "material.h"
#include "shader.h"
#include "texture.h"

Material::Material(Shader* shad, Texture* text, int shi, Vector3 a_coef, Vector3 s_coef, Vector3 d_coef) {
	this->shader = shad;
	this->texture = text;
	this->shine = shi;
	this->Ka = a_coef;
	this->Ks = s_coef;
	this->Kd = d_coef;
}

void Material::Enable() {
	shader->Enable();
}

void Material::Disable() {
	shader->Disable();
}