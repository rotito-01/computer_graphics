/*  
	+ This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#pragma once

#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "mesh.h"
#include "entity.h"


class Application
{
public:
    /*
    int borderWidth_real; // border width
    bool fill_shape; // it will change if we press 'F' and the next shape we paint will be draw as filled.
    int application_task;
    
    bool ismousepressed; // changes if we are pressing or not the mouse left button
    Vector2 mouse_start;
    int tri_clicks;
    
    
    
    // Vectors that will be used hold triangle positions
    Vector2 v1;
    Vector2 v2;
    Vector2 v3;
    
    int count;
    
	//Buttons:
    Button clear;
    Button load;
    Button save;
    Button erase;
    Button pencil;
    Button line;
    Button rect;
    Button trian;
    Button black;
    Button white;
    Button pink;
    Button yellow;
    Button red;
    Button blue;
    Button cyan;
    Button green;
    
    // Color selected from the palette
    Color pen_color;
    
    // Each mode (triangle, rectangle...) has a number associated
    int shape_mode;
    
    ParticleSystem partSys;
    */
    bool lab;
    bool occlusion;
    bool wire;
    bool texture;
    
    int application_task;
    bool ismousepressedL;
    bool ismousepressedR;
    FloatImage Zbuffer;

    int mode;
    float near_p;
    float far_p;
    float fov_aux;

    Vector3 eye;
    Vector3 center;
    Vector3 up;

    Camera cam;
    Entity Jose;
    Entity Xesca;
    Entity Sandalio;
    Entity Pedro;
    
    // Window
    
    SDL_Window* window = nullptr;
	int window_width;
	int window_height;

	float time;
    bool uiReload = false;

	// Input
	const Uint8* keystate;
	int mouse_state; // Tells which buttons are pressed
	Vector2 mouse_position; // Last mouse position
	Vector2 mouse_delta; // Mouse movement in the last frame

	void OnKeyPressed(SDL_KeyboardEvent event);
	void OnMouseButtonDown(SDL_MouseButtonEvent event);
	void OnMouseButtonUp(SDL_MouseButtonEvent event);
	void OnMouseMove(SDL_MouseButtonEvent event);
	void OnWheel(SDL_MouseWheelEvent event);
	void OnFileChanged(const char* filename);

	// CPU Global framebuffer
	Image framebuffer;

	// Constructor and main methods
	Application(const char* caption, int width, int height);
	~Application();

	void Init( void );
	void Render( void );
	void Update( float dt );

	// Other methods to control the app
	void SetWindowSize(int width, int height) {
		glViewport( 0,0, width, height );
		this->window_width = width;
		this->window_height = height;
		this->framebuffer.Resize(width, height);
        this->Zbuffer.Resize(width, height);
        this->cam.SetPerspective(60, float(this->window_width) / this->window_height, 0.1, 100);
	}

	Vector2 GetWindowSize()
	{
		int w,h;
		SDL_GetWindowSize(window,&w,&h);
		return Vector2(float(w), float(h));
	}

	//void LoadUI();
};
