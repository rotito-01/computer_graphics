#include "entity.h"
#include "mesh.h"
#include "image.h"

Entity::Entity(Mesh* m, Matrix44 mm) {

	this->mesh = m;
	this->model_matrix = mm;

}


void Entity::Render(Image* framebuffer, Camera* camera, const Color& c) {
	
	Vector4 cur_vec1;
    Vector4 cur_vec2;
    Vector4 cur_vec3;

	Vector4 proj_v1;
    Vector4 proj_v2;
    Vector4 proj_v3;

    Vector2 v1;
    Vector2 v2;
    Vector2 v3;

	for (int i = 0; i < mesh->GetVertices().size(); i=i+3) {
		cur_vec1.x = mesh->GetVertices()[i].x;
		cur_vec1.y = mesh->GetVertices()[i].y;
		cur_vec1.z = mesh->GetVertices()[i].z;
		cur_vec1.w = 1;
		proj_v1 = model_matrix * cur_vec1; // world coordinates
        
        cur_vec2.x = mesh->GetVertices()[i + 1].x;
        cur_vec2.y = mesh->GetVertices()[i + 1].y;
        cur_vec2.z = mesh->GetVertices()[i + 1].z;
        cur_vec2.w = 1;
        proj_v2 = model_matrix * cur_vec2;

        cur_vec3.x = mesh->GetVertices()[i + 2].x;
        cur_vec3.y = mesh->GetVertices()[i + 2].y;
        cur_vec3.z = mesh->GetVertices()[i + 2].z;
        cur_vec3.w = 1;
        proj_v3 = model_matrix * cur_vec3;

        proj_v1 = camera->GetViewProjectionMatrix() * proj_v1; // View Matrix and Projection Matrix all in one
        proj_v2 = camera->GetViewProjectionMatrix() * proj_v2;
        proj_v3 = camera->GetViewProjectionMatrix() * proj_v3;
        // Clamp already done ..?
        
		// camera project pv
		// clamp [-1, 1]
        proj_v1.x = proj_v1.x / proj_v1.w;
        proj_v1.y = proj_v1.y / proj_v1.w;
        proj_v1.z = proj_v1.z / proj_v1.w;

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

        v2.x = proj_v2.x * framebuffer->width;
        v2.y = proj_v2.y * framebuffer->height;

        v3.x = proj_v3.x * framebuffer->width;
        v3.y = proj_v3.y * framebuffer->height;
        

        
		//framebuffer Set Pixel
        framebuffer->DrawTriangle(v1, v2, v3, c, true, c);
	}
}
