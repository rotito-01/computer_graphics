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
    
    this->shape_mode = 0;
    this->ismousepressed = false;
    
    this->borderWidth_real = 10;
    this->tri_clicks = 0;
}

Application::~Application()
{
    
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;
    framebuffer.Fill(Color::BLACK);
    partSys.Init(this->window_width, this->window_height);
    LoadUI();
    
}

// Render one frame
void Application::Render(void)
{
    if (application_task == 1){
        LoadUI();
    }
    if (application_task == 2){
        framebuffer.Fill(Color::BLACK);
        partSys.Render(&framebuffer);
    }
    framebuffer.Render();
    if (uiReload) {
            LoadUI();
            uiReload = false;
    }

}

// Called after render
void Application::Update(float seconds_elapsed)
{
    if (application_task == 2) {
        partSys.Update(seconds_elapsed, this->window_width, this->window_height);
    }
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
        case SDLK_1:
            application_task = 1;
            break;
        case SDLK_2:
            application_task = 2;
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
            Button some = Button(this->window_width, this->window_height, "../res/images/fruits.png", 0, 0, 100);
            framebuffer.DrawImage(some.image, some.x, some.y);
            LoadUI();
    
        }
        
        else if (save.IsMouseInside(mouse_position)) {
            framebuffer.DrawRect(0, 0, window_width, 45, Color::BLACK, 1, true, Color::BLACK);
            std::string filename = std::string("NewImage") + std::to_string(count++) + std::string(".png");
            framebuffer.SaveTGA(filename.c_str());
            LoadUI();
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
            tri_clicks = 0;
        }
        
        
        
        //Colors
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
            if (shape_mode == 1 and mouse_position.y > 40) {
                framebuffer.DrawLineDDA(mouse_start.x, mouse_start.y, mouse_position.x, mouse_position.y, pen_color);
            }
            
            if (shape_mode == 2 && mouse_position.y > 40){
                int width = mouse_position.x - mouse_start.x;
                
                int height = mouse_position.y - mouse_start.y;
                
                framebuffer.DrawRect(mouse_start.x, mouse_start.y, width, height, pen_color, borderWidth_real, fill_shape, pen_color);
            }
            
            if (shape_mode == 3 && mouse_position.y > 40){
                
                if (tri_clicks == 1){
                    v1 = mouse_position;
                    
                    
                }
                if (tri_clicks == 2){
                    v2 = mouse_position;
                    

                }
                if (tri_clicks == 3){
                    v3 = mouse_position;
                    framebuffer.DrawTriangle(v1, v2, v3, pen_color, fill_shape, pen_color);
                    tri_clicks = 0;
                }
                tri_clicks++;
            }
            if (mouse_position.y < 40){
                tri_clicks = 1; //reset thee drawing of the triangle if we click in the menu bar
            }
            ismousepressed = false;
            
        }
    }
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
    int curr_x = event.x;
    int curr_y = this->framebuffer.height - event.y;
    
    if (ismousepressed == true && mouse_position.y > 40) {
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


