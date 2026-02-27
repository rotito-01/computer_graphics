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
}

Application::~Application()
{
    
}

void Application::Init(void)
{
    Mesh* mesh1 = new Mesh();
    mesh1->LoadOBJ("../res/meshes/lee.obj");
    Matrix44 matrix = Matrix44();
    Image texture1 = Image();
    texture1.LoadTGA("../res/textures/lee_normal.tga", false);
    Shader* render = Shader::Get("shaders/render.vs", "shaders/render.fs");
    Entity temp1 = Entity(mesh1, matrix, texture1, render);
    this->Pedro = temp1;
    glEnable(GL_DEPTH_TEST);
}

// Render one frame
void Application::Render(void)
{
    if (task == 4) {
        Pedro.Render(&cam);
    }
    else {
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
            break;
        case SDLK_2:
            if (lab == 0) {
                task = 2;
            }
            break;
        case SDLK_3:
            if (lab == 0) {
                task = 3;
            }
            break;
        case SDLK_4:
            if (lab == 0) {
                task = 4;
            }
            break;
        case SDLK_l:
            if (lab == 0) {
                lab == 1;
                break;
            }
            if (lab == 1) {
                lab == 0;
                break;
            }
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

