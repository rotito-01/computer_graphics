#include "material.h"


Material::Material(Shader* shad, Texture* text, float shi, Vector3 a_coef, Vector3 s_coef, Vector3 d_coef) {
    this->shader = shad;
    this->texture = text;
    this->normalTexture = nullptr;
    this->shine = shi;
    this->Ka = a_coef;
    this->Ks = s_coef;
    this->Kd = d_coef;
    this->useTexture = false;
    this->useSpecular = false;
    this->useNormal = false;
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
    // depending the detail we want we set one value or other to be used
    //check texture
    if (useTexture) {
        shader->SetFloat("u_use_texture", 1);
    }
    else {
        shader->SetFloat("u_use_texture", 0);
    }
    //check specular
    if (useSpecular) {
        shader->SetFloat("u_use_specular",1);
    }
    else {
        shader->SetFloat("u_use_specular",0);
    }
    //check normal
    if (useNormal) {
        shader->SetFloat("u_use_normal",1);
    }
    else {
        shader->SetFloat("u_use_normal",0);
    }
    shader->SetTexture("u_normal_texture", normalTexture);

}

void Material::Disable() {
    shader->Disable();
}
