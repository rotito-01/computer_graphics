#include "entity.h"
#include "mesh.h"
#include "image.h"

Entity::Entity(Mesh* m, Matrix44 mm) {

	this->mesh = m;
	this->model_matrix = mm;

}


void Entity::Render(Image* framebuffer, Camera* camera, const Color& c) {
	std::vector<Vector3> vert = mesh->GetVertices();
	Vector4 cur_vec;
	Vector4 proj_v;
	for (int i = 0; i < vert.size(); i++) {
		cur_vec.x = vert[i].x;
		cur_vec.y = vert[i].y;
		cur_vec.z = vert[i].z;
		cur_vec.w = 1;
		proj_v = model_matrix * cur_vec; // world coordinates
		// camera project pv
		// clamp [-1, 1]
		// viewport [-1,1] -> [0, w]
		//framebuffer Set Pixel
	}
}