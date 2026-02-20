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
}

Application::~Application()
{
    
}

void Application::Init(void)
{
    
}

// Render one frame
void Application::Render(void)
{
    shader->Enable();
    shader->SetTexture("u_texture", texture);
    shader->SetInt("u_task", task);
    shader->SetInt("u_subtask", subtask);
    mesh.Render();
    shader->Disable();
    
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
            subtask = 1;
            break;
        case SDLK_b:
            subtask = 2;
            break;
        case SDLK_c:
            subtask = 3;
            break;
        case SDLK_d:
            subtask = 4;
            break;
        case SDLK_e:
            subtask = 5;
            break;
        case SDLK_f:
            subtask = 6;
            break;
        case SDLK_1:
            task = 1;
            break;
        case SDLK_2:
            task = 2;
            break;
        case SDLK_3:
            task = 3;
            break;
        case SDLK_4:
            task = 4;
            break;
    }
}
void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
        
	}
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
    if (event.button == SDL_BUTTON_LEFT) {
        
    }
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
    
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

