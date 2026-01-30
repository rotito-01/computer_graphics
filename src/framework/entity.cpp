#include "entity.h"
#include "mesh.h"
#include "image.h"

// Default constructor so that we can have a variable Entity in application.h

Entity::Entity() {
    this->mesh = new Mesh();
    this->model_matrix = Matrix44();
}

Entity::Entity(Mesh* m, Matrix44 mm) {

	this->mesh = m;
	this->model_matrix = mm;

}


void Entity::Render(Image* framebuffer, Camera* camera, const Color& c) {
	
    auto vertices = this->mesh->GetVertices();

	Vector3 cur_vec1;
    Vector3 cur_vec2;
    Vector3 cur_vec3;

	Vector3 proj_v1;
    Vector3 proj_v2;
    Vector3 proj_v3;

    Vector2 v1;
    Vector2 v2;
    Vector2 v3;

	for (int i = 0; i < vertices.size(); i = i+3) {
		cur_vec1 = vertices[i];
        cur_vec2 = vertices[i+1];
        cur_vec3 = vertices[i+2];

		proj_v1 = model_matrix * cur_vec1; // world coordinates
        proj_v2 = model_matrix * cur_vec2;
        proj_v3 = model_matrix * cur_vec3;
        
        
        proj_v1 = camera->ProjectVector(proj_v1); // View Matrix and Projection Matrix all in one
        proj_v2 = camera->ProjectVector(proj_v2);
        proj_v3 = camera->ProjectVector(proj_v3);
        // Clamp already done ..?
        
		// camera project pv
		// clamp [-1, 1]


        /*
        proj_v2.x = proj_v2.x / proj_v2.w;
        proj_v2.y = proj_v2.y / proj_v2.w;
        proj_v2.z = proj_v2.z / proj_v2.w;

        proj_v3.x = proj_v3.x / proj_v3.w;
        proj_v3.y = proj_v3.y / proj_v3.w;
        proj_v3.z = proj_v3.z / proj_v3.w;
         
        */
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
        //framebuffer->SetPixel(v1.x, v1.y, c);
        framebuffer->DrawTriangle(v1, v2, v3, c, true, c);
	}
}
