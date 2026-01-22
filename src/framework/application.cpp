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
    this->pen_color = Color::WHITE; // Default color is white
    
    this->shape_mode = 1;
    this->ismousepressed = false;
    
    this->borderWidth_real = 10;
    
}

Application::~Application()
{
    
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;
    
    LoadUI();
    
}

// Render one frame
void Application::Render(void)
{
    
//    switch(application_task){
//        case 1:
//            framebuffer.Fill(Color(0, 0, 0));
//            framebuffer.DrawRect(mouse_position.x, mouse_position.y, 100, 100, Color(128, 0, 128), borderWidth_real, fill_shape, Color(255, 255, 255));
//            framebuffer.Render();
//            break;
//        case 2:
//            framebuffer.Fill(Color::BLACK);
//            framebuffer.DrawLineDDA(this->window_width/2, this->window_height/2, this->window_width/2 + 100 * cos(time), this->window_height/2 + 100 * sin(time), Color::RED);
//            framebuffer.Render();
//            break;
//        case 3:
//            Vector2 v0 = Vector2(200, 200);
//            Vector2 v1 = Vector2(400, 300);
//            Vector2 v2 = Vector2(300, 500);
//            framebuffer.Fill(Color::BLACK);
//            framebuffer.DrawTriangle(v0, v1, v2, Color::GREEN, fill_shape, Color::CYAN);
//            framebuffer.Render();
//            break;
//
//    }
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
        
    }
}
void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
        if (ismousepressed == false){
            mouse_start = mouse_position;
        }
        ismousepressed = true;
        
        if (clear.IsMouseInside(mouse_position)) {
            framebuffer.Fill(Color::BLACK);
            LoadUI();
        }
        
        else if (load.IsMouseInside(mouse_position)) {
            //
        }
        
        else if (save.IsMouseInside(mouse_position)) {
            //
        }
        else if (erase.IsMouseInside(mouse_position)) {
            pen_color = Color::BLACK;
            shape_mode = 0;
        }
        else if (pencil.IsMouseInside(mouse_position)) {
            shape_mode = 0;
        }
        else if (line.IsMouseInside(mouse_position)) {
            shape_mode = 1;
            
        }
        else if (rect.IsMouseInside(mouse_position)) {
            shape_mode = 2;
        }
        else if (trian.IsMouseInside(mouse_position)) {
            shape_mode = 3;
        }
        else if (black.IsMouseInside(mouse_position)) {
            pen_color = Color::BLACK;
        }
        else if (white.IsMouseInside(mouse_position)) {
            pen_color = Color::WHITE;
        }
        else if (pink.IsMouseInside(mouse_position)) {
            pen_color = Color::PURPLE;
        }
        else if (yellow.IsMouseInside(mouse_position)) {
            pen_color = Color::YELLOW;
        }
        else if (red.IsMouseInside(mouse_position)) {
            pen_color = Color::RED;
        }
        else if (blue.IsMouseInside(mouse_position)) {
            pen_color = Color::BLUE;
        }
        else if (cyan.IsMouseInside(mouse_position)) {
            pen_color = Color::CYAN;
        }
        else if (green.IsMouseInside(mouse_position)) {
            pen_color = Color::GREEN;
        }
        
	}
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
    if (event.button == SDL_BUTTON_LEFT) {
        if (ismousepressed == true){
            if (shape_mode == 1) {
                framebuffer.DrawLineDDA(mouse_start.x, mouse_start.y, mouse_position.x, mouse_position.y, pen_color);
            }
            if (shape_mode == 2){
                int width = mouse_position.x - mouse_start.x;
                
                int height = mouse_position.y - mouse_start.y;
                
                framebuffer.DrawRect(mouse_start.x, mouse_start.y, width, height, pen_color, borderWidth_real, fill_shape, Color::WHITE);
            }
            ismousepressed = false;
        }
    }
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
    int curr_x = event.x;
    int curr_y = this->framebuffer.height - event.y;
    
    if (ismousepressed == true) {
        if (shape_mode == 0){
            framebuffer.DrawLineDDA(curr_x, curr_y, mouse_position.x, mouse_position.y, pen_color);
        }
        
        
        
    }
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


