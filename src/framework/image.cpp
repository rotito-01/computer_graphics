#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "GL/glew.h"
#include "../extra/picopng.h"
#include "image.h"
#include "utils.h"
#include "camera.h"
#include "mesh.h"

Image::Image() {
	width = 0; height = 0;
	pixels = NULL;
}

Image::Image(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new Color[width*height];
	memset(pixels, 0, width * height * sizeof(Color));
}

// Copy constructor
Image::Image(const Image& c)
{
	pixels = NULL;
	width = c.width;
	height = c.height;
	bytes_per_pixel = c.bytes_per_pixel;
	if(c.pixels)
	{
		pixels = new Color[width*height];
		memcpy(pixels, c.pixels, width*height*bytes_per_pixel);
	}
}

// Assign operator
Image& Image::operator = (const Image& c)
{
	if(pixels) delete[] pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	bytes_per_pixel = c.bytes_per_pixel;

	if(c.pixels)
	{
		pixels = new Color[width*height*bytes_per_pixel];
		memcpy(pixels, c.pixels, width*height*bytes_per_pixel);
	}
	return *this;
}

Image::~Image()
{
	if(pixels) 
		delete[] pixels;
}

void Image::Render()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(width, height, bytes_per_pixel == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

// Change image size (the old one will remain in the top-left corner)
void Image::Resize(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for(unsigned int x = 0; x < min_width; ++x)
		for(unsigned int y = 0; y < min_height; ++y)
			new_pixels[ y * width + x ] = GetPixel(x,y);

	delete[] pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

// Change image size and scale the content
void Image::Scale(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];

	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++y)
			new_pixels[ y * width + x ] = GetPixel((unsigned int)(this->width * (x / (float)width)), (unsigned int)(this->height * (y / (float)height)) );

	delete[] pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

Image Image::GetArea(unsigned int start_x, unsigned int start_y, unsigned int width, unsigned int height)
{
	Image result(width, height);
	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++y)
		{
			if( (x + start_x) < this->width && (y + start_y) < this->height) 
				result.SetPixelUnsafe( x, y, GetPixel(x + start_x,y + start_y) );
		}
	return result;
}

void Image::FlipY()
{
	int row_size = bytes_per_pixel * width;
	Uint8* temp_row = new Uint8[row_size];
#pragma omp simd
	for (int y = 0; y < height * 0.5; y += 1)
	{
		Uint8* pos = (Uint8*)pixels + y * row_size;
		memcpy(temp_row, pos, row_size);
		Uint8* pos2 = (Uint8*)pixels + (height - y - 1) * row_size;
		memcpy(pos, pos2, row_size);
		memcpy(pos2, temp_row, row_size);
	}
	delete[] temp_row;
}

bool Image::LoadPNG(const char* filename, bool flip_y)
{
	std::string sfullPath = absResPath(filename);
	std::ifstream file(sfullPath, std::ios::in | std::ios::binary | std::ios::ate);

	// Get filesize
	std::streamsize size = 0;
	if (file.seekg(0, std::ios::end).good()) size = file.tellg();
	if (file.seekg(0, std::ios::beg).good()) size -= file.tellg();

	if (!size){
		std::cerr << "--- Failed to load file: " << sfullPath.c_str() << std::endl;
		return false;
	}

	std::vector<unsigned char> buffer;

	// Read contents of the file into the vector
	if (size > 0)
	{
		buffer.resize((size_t)size);
		file.read((char*)(&buffer[0]), size);
	}
	else
		buffer.clear();

	std::vector<unsigned char> out_image;

	if (decodePNG(out_image, width, height, buffer.empty() ? 0 : &buffer[0], (unsigned long)buffer.size(), true) != 0){
		std::cerr << "--- Failed to load file: " << sfullPath.c_str() << std::endl;
		return false;
	}

	size_t bufferSize = out_image.size();
	unsigned int originalBytesPerPixel = (unsigned int)bufferSize / (width * height);
	
	// Force 3 channels
	bytes_per_pixel = 3;

	if (originalBytesPerPixel == 3) {
		if (pixels) delete[] pixels;
		pixels = new Color[bufferSize];
		memcpy(pixels, &out_image[0], bufferSize);
	}
	else if (originalBytesPerPixel == 4) {
		if (pixels) delete[] pixels;

		unsigned int newBufferSize = width * height * bytes_per_pixel;
		pixels = new Color[newBufferSize];

		unsigned int k = 0;
		for (unsigned int i = 0; i < bufferSize; i += originalBytesPerPixel) {
			pixels[k] = Color(out_image[i], out_image[i + 1], out_image[i + 2]);
			k++;
		}
	}

	// Flip pixels in Y
	if (flip_y)
		FlipY();

	std::cout << "+++ File loaded: " << sfullPath.c_str() << std::endl;

	return true;
}

// Loads an image from a TGA file
bool Image::LoadTGA(const char* filename, bool flip_y)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char TGAcompare[12];
	unsigned char header[6];
	unsigned int imageSize;
	unsigned int bytesPerPixel;

    std::string sfullPath = absResPath( filename );

	FILE * file = fopen( sfullPath.c_str(), "rb");
   	if ( file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
		fread(header, 1, sizeof(header), file) != sizeof(header))
	{
		std::cerr << "--- File not found: " << sfullPath.c_str() << std::endl;
		if (file == NULL)
			return NULL;
		else
		{
			fclose(file);
			return NULL;
		}
	}

	TGAInfo* tgainfo = new TGAInfo;
    
	tgainfo->width = header[1] * 256 + header[0];
	tgainfo->height = header[3] * 256 + header[2];
    
	if (tgainfo->width <= 0 || tgainfo->height <= 0 || (header[4] != 24 && header[4] != 32))
	{
		std::cerr << "--- Failed to load file: " << sfullPath.c_str() << std::endl;
		fclose(file);
		delete tgainfo;
		return NULL;
	}
    
	tgainfo->bpp = header[4];
	bytesPerPixel = tgainfo->bpp / 8;
	imageSize = tgainfo->width * tgainfo->height * bytesPerPixel;
    
	tgainfo->data = new unsigned char[imageSize];
    
	if (tgainfo->data == NULL || fread(tgainfo->data, 1, imageSize, file) != imageSize)
	{
		std::cerr << "--- Failed to load file: " << sfullPath.c_str() << std::endl;

		if (tgainfo->data != NULL)
			delete[] tgainfo->data;
            
		fclose(file);
		delete tgainfo;
		return false;
	}

	fclose(file);

	// Save info in image
	if(pixels)
		delete[] pixels;

	width = tgainfo->width;
	height = tgainfo->height;
	pixels = new Color[width*height];

	const char imageDescriptor = header[5];
	bool tgaFlipY = (imageDescriptor & 0x20) > 0; // bit 5 (0-7) -> true == origin on top
	bool tgaFlipX = (imageDescriptor & 0x10) > 0; // bit 4 (0-7) -> true == origin on right

	if (flip_y) {
		tgaFlipY = !tgaFlipY;
	}

	// Convert to float all pixels
	for (unsigned int y = 0; y < height; ++y) {
		for (unsigned int x = 0; x < width; ++x) {
			unsigned int offsetY = (tgaFlipY ? (height - 1 - y) : y) * width * bytesPerPixel;
			unsigned int offsetX = (tgaFlipX ? (width - 1 - x) : x) * bytesPerPixel;
			unsigned int pos = offsetY + offsetX;
			// Make sure we don't access out of memory
			if( pos + 2 < imageSize ) // assuming 1 bytes per channel
				SetPixelUnsafe(x, y, Color(tgainfo->data[pos + 2], tgainfo->data[pos + 1], tgainfo->data[pos]));
		}
	}

	delete[] tgainfo->data;
	delete tgainfo;

	std::cout << "+++ File loaded: " << sfullPath.c_str() << std::endl;

	return true;
}

// Saves the image to a TGA file
bool Image::SaveTGA(const char* filename)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	std::string fullPath = absResPath(filename);
	FILE *file = fopen(fullPath.c_str(), "wb");
	if ( file == NULL )
	{
		std::cerr << "--- Failed to save file: " << fullPath.c_str() << std::endl;
		return false;
	}

	unsigned short header_short[3];
	header_short[0] = width;
	header_short[1] = height;
	unsigned char* header = (unsigned char*)header_short;
	header[4] = 24;
	header[5] = 0; // image descriptor: origin in bottom-left

	fwrite(TGAheader, 1, sizeof(TGAheader), file);
	fwrite(header, 1, 6, file);

	// Convert pixels to unsigned char
	unsigned char* bytes = new unsigned char[width*height*3];
	for(unsigned int y = 0; y < height; ++y)
		for(unsigned int x = 0; x < width; ++x)
		{
			Color c = pixels[y*width+x];
			unsigned int pos = (y*width+x)*3;
			bytes[pos+2] = c.r;
			bytes[pos+1] = c.g;
			bytes[pos] = c.b;
		}

	fwrite(bytes, 1, width*height*3, file);
	fclose(file);

	delete[] bytes;

	std::cout << "+++ File saved: " << fullPath.c_str() << std::endl;

	return true;
}

void Image::DrawRect(int x, int y, int w, int h, const Color& borderColor, int borderWidth, bool isFilled, const Color& fillColor) {
    
    // The following 2 ifs are for handeling when we recieve negative values of coordinates
    // For example, when the starting point is on the left
    
    if (w < 0) {
        x += w;
        w = -w;
    }
    
    if (h < 0) {
        y += h;
        h = -h;
    }
    
    
    if (isFilled == true) { // Check a given variable that will be 'true' if we want to fill our shape
        // Fill
        for (int i = 0 + borderWidth; i < w - borderWidth; i++) {
            for (int j = 0 + borderWidth; j < h - borderWidth; j++) {
                SetPixel( x + i, y + j, fillColor);
            }
        }
    }
    
    // Drawing of the borders
    
    //Top
    for (int i = 0; i < w; ++i){
        for (int j = 0; j < borderWidth; ++j){
            SetPixel(x + i, y + j, borderColor);
        }
    }
    
    // Bottom
    for (int i = 0; i < w; ++i){
        for (int j = 0; j < borderWidth; ++j){
            SetPixel(x + i, y + h - borderWidth + j, borderColor);
        }
    }
    
    // Left
    for (int i = borderWidth; i < h - borderWidth; ++i){ // we subtract the borderwidth to avoid redrawing the pixels
        for (int j = 0; j < borderWidth; ++j){
            SetPixel(x + j, y + i, borderColor);
        }
    }
    // Right
    for (int i = borderWidth; i < h - borderWidth; ++i){
        for (int j = 0; j < borderWidth; ++j){
            SetPixel(x + w - borderWidth + j, y + i, borderColor);
        }
    }
}

void Image::DrawLineDDA(int x0, int y0, int x1, int y1, const Color& c){
    
    float dx = x1- x0;
    float dy = y1 - y0;
    float d = (std::max)(abs(dx), abs(dy));

    for(int i = 0; i <= d; i++) {
        int x = x0 + i * dx/d;
        int y = y0 + i * dy/d;
        SetPixel(x, y, c);
    }
}

void Image::DrawTriangle(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Color& borderColor, bool isFilled, const Color& fillColor) {
    DrawLineDDA(int(p0.x), int(p0.y), int(p1.x), int(p1.y), borderColor);
    DrawLineDDA(int(p1.x), int(p1.y), int(p2.x), int(p2.y), borderColor);
    DrawLineDDA(int(p0.x), int(p0.y), int(p2.x), int(p2.y), borderColor);
    
    std::vector<Cell>table;
    table.resize(this->height);
    
                 
    // Fill triangle
    
    if (isFilled) {
        // We will fill the table
        ScanLineDDA(int(p0.x), int(p0.y), int(p1.x), int(p1.y), table);
        ScanLineDDA(int(p1.x), int(p1.y), int(p2.x), int(p2.y), table);
        ScanLineDDA(int(p0.x), int(p0.y), int(p2.x), int(p2.y), table);
        
        // and now go through the table so we paint 
        for (int i = 0; i < this->height; i++) {
            for (int j = table[i].minx; j < table[i].maxx; j++) {
                SetPixel(j, i, fillColor);
            }
        }
    }

}

void Image::ScanLineDDA(int x0, int y0, int x1, int y1, std::vector<Cell>& table) {
    float dx = x1 - x0;
    float dy = y1 - y0;

    float d = (std::max)(abs(dx), abs(dy));

    for (int i = 0; i <= d; i++) {
		int x = x0 + i * dx / d;
        int y = y0 + i * dy / d;
		if (dx == 0 || d == 0) {
			x = x0;
		}
		if (dy == 0 || d == 0) {
			y = y0;
		}

        if (table[y].minx > x) {
            table[y].minx = x;
        }
        if (table[y].maxx < x) {
            table[y].maxx = x;
        }
        
    }

}
void Image::DrawImage(const Image& image, int x, int y) {
	for (int i = 0; i < image.width; i++) {
		for (int j = 0; j < image.height; j++) {
			SetPixel(x+i, y+j, image.GetPixel(i,j));
		}
	}
}

void Image::DrawTriangleInterpolated(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Color& c0, const Color& c1, const Color& c2, FloatImage* zbuffer) {
	Vector2 v0 = Vector2(p0.x, p0.y);
	Vector2 v1 = Vector2(p1.x, p1.y);
	Vector2 v2 = Vector2(p2.x, p2.y);
	
	Vector2 p01 = (v1 - v0);
	Vector2 p02 = (v2 - v0);
	Vector2 p12 = (v2 - v1);

	float total = p01.Perpdot(p02);
	float totArea = (total) / 2;

	std::vector<Cell>table;
	table.resize(this->height);

	ScanLineDDA(int(p0.x), int(p0.y), int(p1.x), int(p1.y), table);
	ScanLineDDA(int(p1.x), int(p1.y), int(p2.x), int(p2.y), table);
	ScanLineDDA(int(p0.x), int(p0.y), int(p2.x), int(p2.y), table);

	// and now go through the table so we paint 
	for (int i = 0; i < this->height; i++) {
		for (int j = table[i].minx; j <= table[i].maxx; j++) {
			Vector2 p = Vector2(j, i);

			float mod0 = p12.Perpdot(p - v1);
			float a0 = (mod0) / 2;

			float mod1 = p02.Perpdot(p - v0);
			float a1 = (mod1) / 2;

			float mod2 = p01.Perpdot(p - v0);
			float a2 = (mod2) / 2;

			float alpha = a0 / totArea;
			float beta = a1 / totArea;
			float gamma = a2 / totArea;

			if (alpha < 0) {
				alpha = -alpha;
			}
			if (beta < 0) {
				beta = -beta;
			}
			if (gamma < 0) {
				gamma = -gamma;
			}

			Color finalColor = alpha * c0 + beta * c1 + gamma * c2;
			float z = alpha * p0.z + beta * p1.z + gamma * p2.z;

			if ((zbuffer->GetPixel(j, i) > z) || (zbuffer->GetPixel(j, i) == NULL)) {
				zbuffer->SetPixel(j, i, z);
				SetPixel(j, i, finalColor);
			}
			
		}
	}

}


void Image::DrawTriangleInterpolated(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Color& c0, const Color& c1, const Color& c2, FloatImage* zbuffer, Image* texture, const Vector2& uv0, const Vector2& uv1, const Vector2& uv2) {
	Vector2 v0 = Vector2(p0.x, p0.y);
	Vector2 v1 = Vector2(p1.x, p1.y);
	Vector2 v2 = Vector2(p2.x, p2.y);
	
	Vector2 p01 = (v1 - v0);
	Vector2 p02 = (v2 - v0);
	Vector2 p12 = (v2 - v1);

	float total = p01.Perpdot(p02);
	float totArea = (total) / 2;

	std::vector<Cell>table;
	table.resize(this->height);

	ScanLineDDA(int(p0.x), int(p0.y), int(p1.x), int(p1.y), table);
	ScanLineDDA(int(p1.x), int(p1.y), int(p2.x), int(p2.y), table);
	ScanLineDDA(int(p0.x), int(p0.y), int(p2.x), int(p2.y), table);

	// and now go through the table so we paint 
	for (int i = 0; i < this->height; i++) {
		for (int j = table[i].minx; j <= table[i].maxx; j++) {
			Vector2 p = Vector2(j, i);

			float mod0 = p12.Perpdot(p - v1);
			float a0 = (mod0) / 2;

			float mod1 = p02.Perpdot(p - v0);
			float a1 = (mod1) / 2;

			float mod2 = p01.Perpdot(p - v0);
			float a2 = (mod2) / 2;

			float alpha = a0 / totArea;
			float beta = a1 / totArea;
			float gamma = a2 / totArea;

			if (alpha < 0) {
				alpha = -alpha;
			}
			if (beta < 0) {
				beta = -beta;
			}
			if (gamma < 0) {
				gamma = -gamma;
			}

			float sum = alpha + beta + gamma;
			alpha /= sum;
			beta /= sum;
			gamma /= sum;

			//make shure the values are from [0,1]
			alpha = clamp(alpha, 0.0, 1.0);
			beta = clamp(beta, 0.0, 1.0);
			gamma = clamp(gamma, 0.0, 1.0);

			Vector2 uv;
			uv.x = alpha * uv0.x + beta * uv1.x + gamma * uv2.x;
			uv.y = alpha * uv0.y + beta * uv1.y + gamma * uv2.y;

			float z = alpha * p0.z + beta * p1.z + gamma * p2.z;
			// Change to texture space
			uv.x = uv.x * (texture->width - 1);
			uv.y = uv.y * (texture->height - 1);

			Color finalColor = texture->GetPixel(uv.x, uv.y);

			if ((zbuffer->GetPixel(j, i) > z) || (zbuffer->GetPixel(j, i) == NULL)) {
				zbuffer->SetPixel(j, i, z);
				SetPixel(j, i, finalColor);
			}

			if (uv.x >= 0 && uv.x <= 1 && uv.y >= 0 && uv.y <= 1) {
				
			}
		}
	}
}

/*
Button::Button(int w, int h, const char* filename, int x, int y, int act) {
	this->image = Image(w, h);
	this->image.LoadPNG(filename);
	this->x = x;
	this->y = y;
	this->action = act;
    this->w = w;
    this->h = h;
}

bool Button::IsMouseInside(Vector2 mousePosition) {
	if (this->x < mousePosition.x && this->y < mousePosition.y && mousePosition.x < (this->x + this->image.width) && mousePosition.y < (this->y + this->image.height)) {
		return true;
	}
}

void ParticleSystem::Init(int w, int h) {
    
    //the center of our frame change according to the frame size
    float centerX = w * 0.5f;
    float centerY = h * 0.5f;

    for (int i = 0; i < MAX_PARTICLES; i++) {
        // to have some particles (stars) at the start of the animation
        particles[i].position.x = (float)(rand() % w);
        particles[i].position.y = (float)(rand() % h);

        // we use vectors that go from the center of the frame to the position of the particle
        float dx = particles[i].position.x - centerX;
        float dy = particles[i].position.y - centerY;
        float distance = sqrtf(dx * dx + dy * dy); //with that we can compute the full vector

        if (distance > 0.1f) {
            particles[i].velocity.x = dx / distance;
            particles[i].velocity.y = dy / distance;
        }
        else {
            particles[i].velocity.x = 1.0f;
            particles[i].velocity.y = 0.0f;
        }

        particles[i].acceleration = 100.0f + (rand() % 200);
        particles[i].ttl = (float)(rand() % 40) / 10.0f; // Random start TTL so they don't sync
        particles[i].inactive = false;
    }
}

void ParticleSystem::Update(float dt, int w, int h) {
    float centerX = w * 0.5f;
    float centerY = h * 0.5f;
    float maxLife = 4.0f;
    float fadeTime = 0.8f;

    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].inactive) {
            
            particles[i].position.x = centerX + (rand() % 10 - 5);
            particles[i].position.y = centerY + (rand() % 10 - 5);

            // recompute the velocity according to the center
            float dx = particles[i].position.x - centerX;
            float dy = particles[i].position.y - centerY;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance > 0.1f) {
                particles[i].velocity.x = dx / distance;
                particles[i].velocity.y = dy / distance;
            } else {
                particles[i].velocity.x = (rand() % 2 == 0) ? 1.0f : -1.0f;
                particles[i].velocity.y = (rand() % 2 == 0) ? 1.0f : -1.0f;
            }

            particles[i].acceleration = 100.0f;
            particles[i].ttl = maxLife;
            particles[i].inactive = false;
            continue;
        }

        // the very same classic principle of physics:
        // P_t+1 = P + V * accel * dt
        particles[i].position.x += particles[i].velocity.x * particles[i].acceleration * dt;
        particles[i].position.y += particles[i].velocity.y * particles[i].acceleration * dt;
        
        // since we are traveling across start, it has to accelerate as we pass them
        particles[i].acceleration += 500.0f * dt;
        particles[i].ttl -= dt;

        
        float age = maxLife - particles[i].ttl; // this is the "age" of a particle
        float multiplier = 1.0f;

        // to create the fade in, we check if the age is still under the fading period (hardcoded above)
        if (age < fadeTime) {
            multiplier = age / fadeTime;
        }
        // fade out check
        else if (particles[i].ttl < fadeTime) {
            multiplier = particles[i].ttl / fadeTime;
        }

        if (multiplier < 0.0f) {
            multiplier = 0.0f;
        }
        if (multiplier > 1.0f){
            multiplier = 1.0f;
        }

        particles[i].color.r = (unsigned char)(255 * multiplier);
        particles[i].color.g = (unsigned char)(255 * multiplier);
        particles[i].color.b = (unsigned char)(255 * multiplier);

        // check if out particle has "died"
        // or if it's out of margins
        if (particles[i].ttl <= 0 || particles[i].position.x < -20 || particles[i].position.x > w + 20 || particles[i].position.y < -20 || particles[i].position.y > h + 20) {
            particles[i].inactive = true;
        }
    }
}

void ParticleSystem::Render(Image* framebuffer) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].inactive) continue;

        // to create the effect of having a trail we take the previous x,y position
        // an idea very similar to the one used in painting lines in the paint program
        float trailScale = 0.02f; // we multiply it by a custom factor
        float prevX = particles[i].position.x - (particles[i].velocity.x * particles[i].acceleration * trailScale);
        float prevY = particles[i].position.y - (particles[i].velocity.y * particles[i].acceleration * trailScale);
        framebuffer->DrawLineDDA(prevX, prevY, particles[i].position.x, particles[i].position.y, particles[i].color);
        
    }
}
*/
#ifndef IGNORE_LAMBDAS

// You can apply and algorithm for two images and store the result in the first one
// ForEachPixel( img, img2, [](Color a, Color b) { return a + b; } );
template <typename F>
void ForEachPixel(Image& img, const Image& img2, F f) {
	for(unsigned int pos = 0; pos < img.width * img.height; ++pos)
		img.pixels[pos] = f( img.pixels[pos], img2.pixels[pos] );
}

#endif

FloatImage::FloatImage(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new float[width * height];
	memset(pixels, 0, width * height * sizeof(float));
}

// Copy constructor
FloatImage::FloatImage(const FloatImage& c) {
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new float[width * height];
		memcpy(pixels, c.pixels, width * height * sizeof(float));
	}
}

// Assign operator
FloatImage& FloatImage::operator = (const FloatImage& c)
{
	if (pixels) delete[] pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new float[width * height * sizeof(float)];
		memcpy(pixels, c.pixels, width * height * sizeof(float));
	}
	return *this;
}

FloatImage::~FloatImage()
{
	if (pixels)
		delete[] pixels;
}

// Change image size (the old one will remain in the top-left corner)
void FloatImage::Resize(unsigned int width, unsigned int height)
{
	float* new_pixels = new float[width * height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for (unsigned int x = 0; x < min_width; ++x)
		for (unsigned int y = 0; y < min_height; ++y)
			new_pixels[y * width + x] = GetPixel(x, y);

	delete[] pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}
