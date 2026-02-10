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
    this->mode = 0;

    this->application_task = 2;
    this->ismousepressedL = false;
    this->ismousepressedR = false;
}

Application::~Application()
{
    
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;
    Mesh* mesh1 = new Mesh();
    mesh1->LoadOBJ("../res/meshes/lee.obj");
    Matrix44 matrix = Matrix44();
    matrix.MakeTranslationMatrix(0, -0.3, 0);
    Image texture1 = Image();
    texture1.LoadTGA("../res/textures/lee_color_specular.tga", false);
    Entity temp1 = Entity(mesh1, matrix, texture1);
    this->Jose = temp1;
    matrix.SetIdentity();
    matrix.MakeTranslationMatrix(0, -0.2, 0);
    Entity temp = Entity(mesh1, matrix, texture1);
    this->Pedro = temp;

    Mesh* mesh2 = new Mesh();
    mesh2->LoadOBJ("../res/meshes/anna.obj");
    matrix.SetIdentity();
    matrix.MakeTranslationMatrix(0.4, -0.3, 0.3);
    Image texture2 = Image();
    texture2.LoadTGA("../res/textures/anna_color_specular.tga", false);
    Entity temp2 = Entity(mesh2, matrix, texture2);
    this->Xesca = temp2;
    
    Mesh* mesh3 = new Mesh();
    mesh3->LoadOBJ("../res/meshes/cleo.obj");
    matrix.SetIdentity();
    matrix.MakeTranslationMatrix(-0.4, -0.2, -0.4);
    Image texture3 = Image();
    texture3.LoadTGA("../res/textures/cleo_color_specular.tga", false);
    Entity temp3 = Entity(mesh3, matrix, texture3);
    this->Sandalio = temp3;

    this->cam = Camera();
    this->fov_aux = 60;
    this->near_p = 0.1;
    this->far_p = 10;
    this->cam.SetPerspective(fov_aux, float(this->window_width) / this->window_height, near_p, far_p);
    this->eye = Vector3(0,0,1);
    this->center = Vector3(0,0,0);
    this->up = Vector3(0,1,0);
    this->cam.LookAt(eye, center, up);
    this->Zbuffer = FloatImage(this->window_width, this->window_height);
}

// Render one frame
void Application::Render(void)
{
    this->Zbuffer = FloatImage(this->window_width, this->window_height);
    if (application_task == 1) {
        framebuffer.Fill(Color::BLACK);
        this->Pedro.Render(&framebuffer, &cam, &Zbuffer);
        framebuffer.Render();
    }
    else if (application_task == 2) {
        framebuffer.Fill(Color::BLACK);
        this->Jose.Render(&framebuffer, &cam, &Zbuffer);
        this->Xesca.Render(&framebuffer, &cam, &Zbuffer);
        this->Sandalio.Render(&framebuffer, &cam, &Zbuffer);
        framebuffer.Render();
    }
    
}

// Called after render
void Application::Update(float seconds_elapsed)
{
    if (application_task == 2) {
        this->Jose.Update(seconds_elapsed, 1);
        this->Sandalio.Update(seconds_elapsed, 2);
        this->Xesca.Update(seconds_elapsed, 3);
    }
    
}

//keyboard press event 
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
    // KEY CODES: https://wiki.libsdlon.org/SDL2/SDL_Keycode
    switch(event.keysym.sym) {
        case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
        case SDLK_n:
            mode = 1;
            break;
        case SDLK_f:
            mode = 2;
            break;
        case SDLK_v:
            mode = 3;
            break;
        case SDLK_PLUS:
            if (mode == 1) {
                this->near_p = this->near_p + 0.5;
                this->cam.SetPerspective(fov_aux, float(this->window_width) / this->window_height, near_p, far_p);
            }
            else if (mode == 2) {
                this->far_p = this->far_p + 0.5;
                this->cam.SetPerspective(fov_aux, float(this->window_width) / this->window_height, near_p, far_p);
            }
            else if (mode == 3) {
                this->fov_aux = this->fov_aux + 5;
                this->cam.SetPerspective(fov_aux, float(this->window_width) / this->window_height, near_p, far_p);
            }
            break;
        case SDLK_MINUS:
            if (mode == 1) {
                this->near_p = this->near_p - 0.5;
                this->cam.SetPerspective(fov_aux, float(this->window_width) / this->window_height, near_p, far_p);
            }
            else if (mode == 2) {
                this->far_p = this->far_p - 0.5;
                this->cam.SetPerspective(fov_aux, float(this->window_width) / this->window_height, near_p, far_p);
            }
            else if (mode == 3) {
                this->fov_aux = this->fov_aux - 5;
                this->cam.SetPerspective(fov_aux, float(this->window_width) / this->window_height, near_p, far_p);
            }
            break;
        case SDLK_1:
            this->application_task = 1;
            break;
        case SDLK_2:
            this->application_task = 2;
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
        
        float angleY = mouse_delta.y * 0.005;

        this->cam.Orbit(angleX, top);

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

