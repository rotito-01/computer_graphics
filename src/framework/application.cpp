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
}

Application::~Application()
{
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;
    
    borderWidth_real = 10;
    fill_shape = false;
    application_task = 0;
}

// Render one frame
void Application::Render(void)
{
    /*
    switch(application_task){
        case 1:
            framebuffer.Fill(Color(0, 0, 0));
            framebuffer.DrawRect(mouse_position.x, mouse_position.y, 100, 100, Color(128, 0, 128), borderWidth_real, fill_shape, Color(255, 255, 255));
            framebuffer.Render();
            break;
        case 2:
            framebuffer.Fill(Color::BLACK);
            framebuffer.DrawLineDDA(500, 500, 500 + 100 * cos(time), 500 + 100 * sin(time), Color::RED);
            framebuffer.Render();



    }
     */
    framebuffer.Fill(Color::BLACK);
    framebuffer.DrawLineDDA(this->window_width/2, this->window_height/2, this->window_width/2 + 100 * cos(time), this->window_height/2 + 100 * sin(time), Color::RED);
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
