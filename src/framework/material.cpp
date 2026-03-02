#include "material.h"


Material::Material(Shader* shad, Texture* text, float shi, Vector3 a_coef, Vector3 s_coef, Vector3 d_coef) {
	this->shader = shad;
	this->texture = text;
	this->shine = shi;
	this->Ka = a_coef;
	this->Ks = s_coef;
	this->Kd = d_coef;
}

void Material::Enable(const sUniformData& uniformData) {
	shader->Enable();
	shader->SetMatrix44("u_model", uniformData.modelmat);
	shader->SetMatrix44("u_viewprojection", uniformData.viewproj);
	shader->SetTexture("u_texture", texture);

}

void Material::Disable() {
	shader->Disable();
}