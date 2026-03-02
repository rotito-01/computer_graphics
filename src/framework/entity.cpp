#include "entity.h"
#include "mesh.h"
#include "image.h"
#include <math.h>

// Default constructor so that we can have a variable Entity in application.h

Entity::Entity() {
    this->mesh = new Mesh();
    this->model_matrix = Matrix44();
}

Entity::Entity(Mesh* m, Matrix44 mm, Image text, Shader* shad, Material* mat) {

	this->mesh = m;
	this->model_matrix = mm;
    this->texture = text;
    this->shader = shad;
    this->material = mat;

}

// AÑADIR RENDER(CAM) PARA LAB 4

void Entity::Render(sUniformData& uniformData) {
    uniformData.modelmat = model_matrix;
    material->Enable(uniformData);
    mesh->Render();
    material->Disable();
}

/*
void Entity::Render(Image* framebuffer, Camera* camera, FloatImage* zBuffer, bool lab_mode, bool wire, bool occlusion, const Color& c, bool texture) {
    
    Vector3 cur_vec1;
    Vector3 cur_vec2;
    Vector3 cur_vec3;
    
    Vector3 proj_v1;
    Vector3 proj_v2;
    Vector3 proj_v3;
    
    Vector3 v1;
    Vector3 v2;
    Vector3 v3;
    
    for (int i = 0; i < mesh->GetVertices().size(); i = i + 3) {
        cur_vec1 = mesh->GetVertices()[i];
        cur_vec2 = mesh->GetVertices()[i+1];
        cur_vec3 = mesh->GetVertices()[i+2];
        
        proj_v1 = model_matrix * cur_vec1; // world coordinates
        proj_v2 = model_matrix * cur_vec2;
        proj_v3 = model_matrix * cur_vec3;
        
        
        proj_v1 = camera->ProjectVector(proj_v1); // View Matrix and Projection Matrix all in one
        proj_v2 = camera->ProjectVector(proj_v2);
        proj_v3 = camera->ProjectVector(proj_v3);
        // Clamp already done ..?
        
        // camera project pv
        // clamp [-1, 1]
        
        
        
         proj_v2.x = proj_v2.x / proj_v2.w;
         proj_v2.y = proj_v2.y / proj_v2.w;
         proj_v2.z = proj_v2.z / proj_v2.w;
         
         proj_v3.x = proj_v3.x / proj_v3.w;
         proj_v3.y = proj_v3.y / proj_v3.w;
         proj_v3.z = proj_v3.z / proj_v3.w;
         
         
        if ((proj_v1.x < -1 || proj_v1.x > 1) || (proj_v1.y < -1 || proj_v1.y > 1) || (proj_v1.z < -1 || proj_v1.z > 1) ||
            (proj_v2.x < -1 || proj_v2.x > 1) || (proj_v2.y < -1 || proj_v2.y > 1) || (proj_v2.z < -1 || proj_v2.z > 1) ||
            (proj_v3.x < -1 || proj_v3.x > 1) || (proj_v3.y < -1 || proj_v3.y > 1) || (proj_v3.z < -1 || proj_v3.z > 1)) {
            continue;
        }
        
        // viewport [-1,1] -> [0, w]
        
        proj_v1.x = (proj_v1.x - -1) / (1 - (-1));
        proj_v1.y = (proj_v1.y - -1) / (1 - (-1));
        proj_v1.z = (proj_v1.z - -1) / (1 - (-1));
        
        proj_v2.x = (proj_v2.x - -1) / (1 - (-1));
        proj_v2.y = (proj_v2.y - -1) / (1 - (-1));
        proj_v2.z = (proj_v2.z - -1) / (1 - (-1));
        
        proj_v3.x = (proj_v3.x - -1) / (1 - (-1));
        proj_v3.y = (proj_v3.y - -1) / (1 - (-1));
        proj_v3.z = (proj_v3.z - -1) / (1 - (-1));
        
        v1.x = proj_v1.x * framebuffer->width;
        v1.y = proj_v1.y * framebuffer->height;
        v1.z = proj_v1.z;
        
        v2.x = proj_v2.x * framebuffer->width;
        v2.y = proj_v2.y * framebuffer->height;
        v2.z = proj_v2.z;
        
        v3.x = proj_v3.x * framebuffer->width;
        v3.y = proj_v3.y * framebuffer->height;
        v3.z = proj_v3.z;
        
        Vector2 lab2_1 = Vector2(v1.x, v1.y);
        Vector2 lab2_2 = Vector2(v2.x, v2.y);
        Vector2 lab2_3 = Vector2(v3.x, v3.y);
        
        //framebuffer Set Pixel
        //framebuffer->SetPixel(v1.x, v1.y, c);
        
        
        
        if (lab_mode == false){
            
            if (wire == true){ // WIREFRAME MODE
                framebuffer->DrawTriangle(lab2_1, lab2_2, lab2_3, Color::WHITE, false, Color::WHITE);
            }
            else { // PLAIN COLOR MODE
                framebuffer->DrawTriangle(lab2_1, lab2_2, lab2_3, c, true, c);
            }
        }
        
        
        
        
        if (lab_mode == true) { // INTERPOLATED MODE - LAB3
            if (occlusion == true ) {// OCCLUSION MODE - USING ZBUFFER
                if (texture == true){ // USING TEXTYRE
                    framebuffer->DrawTriangleInterpolated(v1, v2, v3, Color::BLUE, Color::GREEN, Color::RED, zBuffer, &this->texture, mesh->GetUVs()[i], mesh->GetUVs()[i+1], mesh->GetUVs()[i+2]);
                }
                else {
                    framebuffer->DrawTriangleInterpolated(v1, v2, v3, Color::YELLOW, Color::CYAN, Color::BLUE, zBuffer);
                }

                
            }
            else { // NO OCCLUSION - NOT USING ZBUFFER
                framebuffer->DrawTriangleInterpolated(v1, v2, v3, Color::RED, Color::WHITE, Color::GREEN);
            }
            
        }
    }
}
*/

void Entity::Update(float seconds_elapsed, int mode) {
    
    if (mode == 1) {
        // Rotation
        Matrix44 center = Matrix44();
        Matrix44 position = Matrix44();
        Matrix44 rot = Matrix44();
        rot.MakeRotationMatrix(seconds_elapsed * DEG2RAD * -100, Vector3(0, 1, 0));
        center.MakeTranslationMatrix(0, 0, 0.15);
        position.MakeTranslationMatrix(0, 0, -0.15);
        Matrix44 M = position * rot * center;
        model_matrix = M * model_matrix;
    }
    else if (mode == 2) {
        // Translation
        Matrix44 tran = Matrix44();
        displacement++;
        if (displacement <= 80) {
            tran.MakeTranslationMatrix(0, 0, seconds_elapsed* 0.7);
        }
        else {
            tran.MakeTranslationMatrix(0, 0, -seconds_elapsed * 0.7);
        }
        
        if (displacement == 160) {
            displacement = 0;
        }
        
        model_matrix = tran * model_matrix;
    }
    else if (mode == 3) {
        // Translation
        Matrix44 scale = Matrix44();
        Matrix44 center = Matrix44();
        Matrix44 position = Matrix44();
        float factor = 0.9;
        growth++;
        
        if (growth <= 50) {
            scale.MakeScaleMatrix(factor, factor, factor);
        }
        else {
            scale.MakeScaleMatrix(float(1/ factor), float(1 / factor), float(1 / factor));
        }
        
        if (growth == 100) {
            growth = 0;
        }
        center.MakeTranslationMatrix(-0.4, 0.3, -0.3);
        position.MakeTranslationMatrix(0.4, -0.3, 0.3);
        Matrix44 M = position * scale * center;
        model_matrix = M * model_matrix;
    }
    
}

