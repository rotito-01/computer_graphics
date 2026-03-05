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
	shader->SetMatrix44("u_viewprojection", uniformData.cam->viewprojection_matrix);
	shader->SetVector3("u_ka", Ka);
	shader->SetVector3("u_ks", Ks);
	shader->SetVector3("u_kd", Kd);
	shader->SetVector3("u_eye", uniformData.cam_eye);
	shader->SetVector3("u_ambient", uniformData.ambient);
	shader->SetVector3("u_light_pos", uniformData.light.position);
	shader->SetVector3("u_light_col", uniformData.light.color_int);
	shader->SetFloat("u_shine", shine);
	shader->SetTexture("u_texture", texture);

}

void Material::Disable() {
	shader->Disable();
}