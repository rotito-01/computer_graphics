#include "entity.h"
#include "mesh.h"
#include "image.h"

Entity::Entity(Mesh* m, Matrix44 mm) {

	this->mesh = m;
	this->model_matrix = mm;

}


void Entity::Render(Image* framebuffer, Camera* camera, const Color& c) {
	
	Vector4 cur_vec;
	Vector4 proj_v;
	for (int i = 0; i < mesh->GetVertices().size(); i++) {
		cur_vec.x = mesh->GetVertices()[i].x;
		cur_vec.y = mesh->GetVertices()[i].y;
		cur_vec.z = mesh->GetVertices()[i].z;
		cur_vec.w = 1;
		proj_v = model_matrix * cur_vec; // world coordinates
        
        proj_v = camera->GetViewProjectionMatrix() * proj_v; // View Matrix and Projection Matrix all in one
        // Clamp already done ..?
        
		// camera project pv
		// clamp [-1, 1]
        proj_v.x = proj_v.x / proj_v.w;
        proj_v.y = proj_v.y / proj_v.w;
        proj_v.z = proj_v.z / proj_v.w;
        
		// viewport [-1,1] -> [0, w]
        
        proj_v.x = (proj_v.x - -1) / (1 - (-1));
        proj_v.y = (proj_v.y - -1) / (1 - (-1));
        proj_v.z = (proj_v.z - -1) / (1 - (-1));
        
        proj_v.x = proj_v.x * framebuffer->width;
        proj_v.y = proj_v.y * framebuffer->height;
        
		//framebuffer Set Pixel
        framebuffer->SetPixel(int(proj_v.x), int(proj_v.y), c);
	}
}
