#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 

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
    this->application_task = 0;
    Image::BGColor = Color::BLACK;
    this->borderWidth_real = 10;
    this->fill_shape = false;
}

Application::~Application()
{
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;
    
    LoadUI();
    for (int i = 0; i < Image::table.size(); i++) {
        Image::table[i].minx = window_width;
        Image::table[i].maxx = 0;
    }
    
}

// Render one frame
void Application::Render(void)
{
    if (uiReload) {
        LoadUI();
        uiReload = false;
    }
    /*
    switch(application_task){
        case 1:
            framebuffer.Fill(Color(0, 0, 0));
            framebuffer.DrawRect(mouse_position.x, mouse_position.y, 100, 100, Color(128, 0, 128), borderWidth_real, fill_shape, Color(255, 255, 255));
            framebuffer.Render();
            break;
        case 2:
            framebuffer.Fill(Color::BLACK);
            framebuffer.DrawLineDDA(this->window_width/2, this->window_height/2, this->window_width/2 + 100 * cos(time), this->window_height/2 + 100 * sin(time), Color::RED);
            framebuffer.Render();



    }
     */

    Vector2 v0 = Vector2(200, 200);
    Vector2 v1 = Vector2(400, 300);
    Vector2 v2 = Vector2(300, 500);

    

    framebuffer.Fill(Image::BGColor);
    //framebuffer.DrawTriangle(v0, v1, v2, Color::GREEN, true, Color::CYAN);
    framebuffer.Render();

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
        case SDLK_PLUS: borderWidth_real = borderWidth_real + 5; break;
        case SDLK_MINUS: borderWidth_real = borderWidth_real - 5; break;
        case SDLK_f:
            if (fill_shape == false){
                fill_shape = true;
            }
            else {
                fill_shape = false;
            }
            break;
            
        case SDLK_r: // Rectangle
            application_task = 1;
            break;
            
        case SDLK_l: // Line
            application_task = 2;
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

	// ...
}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}

void Application::LoadUI() {
    for (int i = 0; i < 40;i++){
        for (int j = 0; j < window_width; j++) {
            framebuffer.SetPixel(i, j, Color::GRAY);
        }
    }
    Button clear = Button(30, 30, "clear.png", 5, 5, 0);
    Button load = Button(30, 30, "load.png", 55, 5, 1);
    Button save = Button(30, 30, "save.png", 105, 5, 2);
    Button erase = Button(30, 30, "eraser.png", 155, 5, 3);
    Button pencil = Button(30, 30, "pencil.png", 205, 5, 4);
    Button line = Button(30, 30, "line.png", 255, 5, 5);
    Button rect = Button(30, 30, "rectangle.png", 305, 5, 6);
    Button trian = Button(30, 30, "triangle.png", 355, 5, 7);
    Button black = Button(30, 30, "black.png", 405, 5, 8);
    Button white = Button(30, 30, "white.png", 455, 5, 9);
    Button pink = Button(30, 30, "pink.png", 505, 5, 10);
    Button yellow = Button(30, 30, "yellow.png", 555, 5, 11);
    Button red = Button(30, 30, "red.png", 605, 5, 12);
    Button blue = Button(30, 30, "blue.png", 655, 5, 13);
    Button cyan = Button(30, 30, "cyan.png", 705, 5, 14);
    Button green = Button(30, 30, "green.png", 755, 5, 15);

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