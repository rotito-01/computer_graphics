#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h"
#include "entity.h"

Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	int w,h;
	SDL_GetWindowSize(window,&w,&h);

	this->mouse_state = 0;
	this->time = 0.f;
	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(nullptr);

	this->framebuffer.Resize(w, h);
    this->mesh.CreateQuad();
    this->cam = Camera();
    this->fov_aux = 60;
    this->near_p = 0.1;
    this->far_p = 10;
    this->cam.SetPerspective(fov_aux, float(this->window_width) / this->window_height, near_p, far_p);
    this->eye = Vector3(0, 0, 1);
    this->center = Vector3(0, 0, 0);
    this->up = Vector3(0, 1, 0);
    this->cam.LookAt(eye, center, up);
    this->shader = Shader::Get("shaders/quad.vs", "shaders/quad.fs");
    this->subtask = 0;
    this->texture = Texture::Get("images/fruits.png");
    this->ismousepressedL = false;
    this->ismousepressedR = false;
    this->lab = 0;
    
    this->task = 1;
    this->lights = 1;
    this->textColor = false;
    this->specular = false;
    this->normal = false;
    
    // Configrar las luces
    this->light_array[0].position = Vector3(1, 2, 0);
    this->light_array[0].color_int = Vector3(1, 0, 0);
    
    this->light_array[1].position = Vector3(-1, 2, 0);
    this->light_array[1].color_int = Vector3(0, 0.6, 0);
    
    this->light_array[2].position = Vector3(0, -2, 1);
    this->light_array[2].color_int = Vector3(0, 0.3, 0);
    
    this->light_array[3].position = Vector3(2, 1, 1);
    this->light_array[3].color_int = Vector3(0.2, 0.1, 0);
    
    this->light_array[4].position = Vector3(-2, 1, -1);
    this->light_array[4].color_int = Vector3(0, 0.6, 0.4);
    
    this->light_array[5].position = Vector3(0, 3, 2);
    this->light_array[5].color_int = Vector3(0.3, 0, 0.2);
    
    this->light_array[6].position = Vector3(1, -1, 2);
    this->light_array[6].color_int = Vector3(0.1, 0.5, 0);
    
    this->light_array[7].position = Vector3(-1, -1, -2);
    this->light_array[7].color_int = Vector3(0.5, 0, 0.4);
    
    this->light_array[8].position = Vector3(3, 0, 0);
    this->light_array[8].color_int = Vector3(1, 1, 1);
    
    this->data.viewproj = cam.viewprojection_matrix;
    this->data.cam_eye = this->eye;
    this->data.light.color_int = Vector3(1,0,0);
    this->data.light.position = Vector3(1, 2, 0);
    this->data.ambient = Vector3(0.3, 0.3, 0.3);
    this->data.cam = &this->cam;
    this->activePhong = false;
    this->render = Shader::Get("shaders/render.vs", "shaders/render.fs");
    this->gouraud = Shader::Get("shaders/gouraud.vs", "shaders/gouraud.fs");
    this->phong = Shader::Get("shaders/phong.vs", "shaders/phong.fs");
}

Application::~Application()
{
    
}

void Application::Init(void)
{
    Mesh* mesh1 = new Mesh();
    mesh1->LoadOBJ("../res/meshes/lee.obj");
    Matrix44 matrix = Matrix44();
    Texture* t1 = Texture::Get("../res/textures/lee_color_specular.tga");
    Material* m1 = new Material(render, t1, 0.2, Vector3(1, 1, 1), Vector3(1, 1, 1), Vector3(1, 1, 1));
    m1->normalTexture = Texture::Get("../res/textures/lee_normal.tga");
    Entity* temp1 = new Entity(mesh1, matrix, render, m1);
    this->Manuel = temp1;
    Mesh* mesh2 = new Mesh();
    mesh2->LoadOBJ("../res/meshes/lee.obj");
    Texture* t2 = Texture::Get("../res/textures/lee_color_specular.tga");
    Material* m2 = new Material(gouraud, t2, 0.2, Vector3(1, 1, 1), Vector3(1, 1, 1), Vector3(1, 1, 1));
    m2->normalTexture = Texture::Get("../res/textures/lee_normal.tga");
    Entity* temp2 = new Entity(mesh2, matrix, gouraud, m2);
    this->Pedro = temp2;
    glEnable(GL_DEPTH_TEST);
}

// Render one frame
void Application::Render(void)
{
    if (lab == 1) {

        this->data.viewproj = this->cam.viewprojection_matrix;
        this->data.cam_eye = cam.eye;
        Pedro->material->useTexture = textColor;
        Pedro->material->useSpecular = specular;
        Pedro->material->useNormal = normal;

        //process each frame x # of lights
        for (int i = 0; i < lights; i++) {
            this->data.light = light_array[i];

            if (i == 0) { //only one light
                this->data.ambient = Vector3(0.3, 0.3, 0.3);
                glDisable(GL_BLEND); // no need to blend
                Pedro->Render(data);
            }
            else { // we have more than 1 light
                this->data.ambient = Vector3(0, 0, 0);
                glEnable(GL_BLEND); //activate accumulative frame processing
                glBlendFunc(GL_ONE, GL_ONE); // factor of (1,1,1,1) for both destination and source
                // the pixel values are in the same depth as before. to ensure that we update the colors of the pixels
                // we say that even if they are at the same depth, we can update the values
                glDepthFunc(GL_LEQUAL);
                Pedro->Render(data);
            }
        }

        glDisable(GL_BLEND);
        glDepthFunc(GL_LESS);
        
    }
    else if (lab == 0) {
        if (task == 4) {
            Manuel->Render(&cam);
        } else {
            shader->Enable();
            shader->SetVector2("u_res", Vector2(this->window_width, this->window_height));
            shader->SetTexture("u_texture", texture);
            shader->SetInt("u_task", task);
            shader->SetInt("u_subtask", subtask);
            shader->SetFloat("u_time", time);
            mesh.Render();
            shader->Disable();
        }
    }
    
    
}

// Called after render
void Application::Update(float seconds_elapsed)
{
    
    
}

//keyboard press event 
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
    // KEY CODES: https://wiki.libsdlon.org/SDL2/SDL_Keycode
    switch(event.keysym.sym) {
        case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
        case SDLK_a:
            if (lab == 0){
                subtask = 1;
            }
            break;
        case SDLK_b:
            if (lab == 0) {
                subtask = 2;
            }
            break;
        case SDLK_c:
            if (lab == 0) {
                subtask = 3;
            }
            else if (lab == 1 && activePhong == true) {
                 textColor = !textColor;
            }
            break;
        case SDLK_d:
            if (lab == 0) {
                subtask = 4;
            }
            break;
        case SDLK_e:
            if (lab == 0) {
                subtask = 5;
            }
            break;
        case SDLK_f:
            if (lab == 0) {
                subtask = 6;
            }
            break;
        case SDLK_1:
            if (lab == 0) {
                task = 1;
            }
            else if (lab == 1 && activePhong == true) {
                lights = 1;
            }
            break;
        case SDLK_2:
            if (lab == 0) {
                task = 2;
            }
            else if (lab == 1 && activePhong == true) {
                lights = 2;
            }
            break;
        case SDLK_3:
            if (lab == 0) {
                task = 3;
            }
            else if (lab == 1 && activePhong == true) {
                lights = 3;
            }
            break;
        case SDLK_4:
            if (lab == 0) {
                task = 4;
            }
            else if (lab == 1 && activePhong == true) {
                lights = 4;
            }
            break;
        case SDLK_5:
            if (lab == 1 && activePhong == true) {
                lights = 5;
            }
            break;
        case SDLK_6:
            if (lab == 1 && activePhong == true) {
                lights = 6;
            }
            break;
        case SDLK_7:
            if (lab == 1 && activePhong == true) {
                lights = 7;
            }
            break;
        case SDLK_8:
            if (lab == 1 && activePhong == true) {
                lights = 8;
            }
            break;
        case SDLK_9:
            if (lab == 1 && activePhong == true) {
                lights = 9;
            }
            break;
        case SDLK_0:
            if (lab == 1 && activePhong == true) {
                lights = 0;
            }
            break;
        case SDLK_l:
            if (lab == 0) {
                lab = 1;
                break;
            }
            if (lab == 1) {
                lab = 0;
                break;
            }
        case SDLK_g:
            if (lab == 1) {
                Pedro->shader = gouraud;
                Pedro->material->shader = gouraud;
                activePhong = false;
            }
            break;
        case SDLK_p:
            if (lab == 1) {
                Pedro->shader = phong;
                Pedro->material->shader = phong;
                activePhong = true;
            }
            break;
        case SDLK_s:
            if (lab == 1 && activePhong == true) {
                specular = !specular;
            }
            break;
        case SDLK_n:
            if (lab == 1 && activePhong == true) {
                normal = !normal;
            }
            break;
    }
}
void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
    if (event.button == SDL_BUTTON_LEFT) {
        ismousepressedL = true;
    }
    if (event.button == SDL_BUTTON_RIGHT) {
        ismousepressedR = true;
    }
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
    if (event.button == SDL_BUTTON_LEFT) {
        ismousepressedL = false;
    }
    if (event.button == SDL_BUTTON_RIGHT) {
        ismousepressedR = false;
    }if (event.button == SDL_BUTTON_LEFT) {
        
    }
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
    Vector3 forward = (this->cam.eye - this->cam.center).Normalize();
    Vector3 right = this->cam.up.Cross(forward).Normalize();
    Vector3 top = forward.Cross(right);

    if (ismousepressedR == true) {

        float angleX = mouse_delta.x * 0.005;
        this->cam.Rotate(angleX, top);

        float angleY = mouse_delta.y * 0.005;
        this->cam.Rotate(angleY, right);

    }
    if (ismousepressedL == true) {

        float angleX = mouse_delta.x * 0.005;
        this->cam.Orbit(angleX, top);

        float angleY = mouse_delta.y * 0.005;

        this->cam.Orbit(angleY, right);

    }
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
    float dy = event.preciseY;
    Vector3 v = Vector3(0, 0, (dy * 0.1));
    this->cam.Move(v);
    
    // ...
}

void Application::OnFileChanged(const char* filename)
{
    Shader::ReloadSingleShader(filename);
}
/*
void Application::LoadUI() {
    
    framebuffer.Fill(Color::BLACK);
    for (int i = 0; i < 40;i++){
        for (int j = 0; j < window_width; j++) {
            framebuffer.SetPixel(j, i, Color::GRAY);
        }
    }
    
    clear = Button(30, 30, "../res/images/clear.png", 5, 5, 0);
    load = Button(30, 30, "../res/images/load.png", 55, 5, 1);
    save = Button(30, 30, "../res/images/save.png", 105, 5, 2);
    erase = Button(30, 30, "../res/images/eraser.png", 155, 5, 3);
    pencil = Button(30, 30, "../res/images/pencil.png", 205, 5, 4);
    line = Button(30, 30, "../res/images/line.png", 255, 5, 5);
    rect = Button(30, 30, "../res/images/rectangle.png", 305, 5, 6);
    trian = Button(30, 30, "../res/images/triangle.png", 355, 5, 7);
    black = Button(30, 30, "../res/images/black.png", 405, 5, 8);
    white = Button(30, 30, "../res/images/white.png", 455, 5, 9);
    pink = Button(30, 30, "../res/images/pink.png", 505, 5, 10);
    yellow = Button(30, 30, "../res/images/yellow.png", 555, 5, 11);
    red = Button(30, 30, "../res/images/red.png", 605, 5, 12);
    blue = Button(30, 30, "../res/images/blue.png", 655, 5, 13);
    cyan = Button(30, 30, "../res/images/cyan.png", 705, 5, 14);
    green = Button(30, 30, "../res/images/green.png", 755, 5, 15);
    
    
    framebuffer.DrawImage(clear.image, clear.x, clear.y);
    framebuffer.DrawImage(load.image, load.x, load.y);
    framebuffer.DrawImage(save.image, save.x, save.y);
    framebuffer.DrawImage(erase.image, erase.x, erase.y);
    framebuffer.DrawImage(pencil.image, pencil.x, pencil.y);
    framebuffer.DrawImage(line.image, line.x, line.y);
    framebuffer.DrawImage(rect.image, rect.x, rect.y);
    framebuffer.DrawImage(trian.image, trian.x, trian.y);
    framebuffer.DrawImage(black.image, black.x, black.y);
    framebuffer.DrawImage(white.image, white.x, white.y);
    framebuffer.DrawImage(pink.image, pink.x, pink.y);
    framebuffer.DrawImage(yellow.image, yellow.x, yellow.y);
    framebuffer.DrawImage(red.image, red.x, red.y);
    framebuffer.DrawImage(blue.image, blue.x, blue.y);
    framebuffer.DrawImage(cyan.image, cyan.x, cyan.y);
    framebuffer.DrawImage(green.image, green.x, green.y);
    
}
*/
