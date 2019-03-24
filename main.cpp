#include <iostream>
#include <chrono>
#include <thread>
#include "SDL2/SDL.h"
#include "chip8.h"

chip8 myChip8;

SDL_Window* setupGraphics();
void setupInput();
void drawGraphics(SDL_Window& window, chip8& device);

int WIDTH = 64 * 10;
int HEIGHT = 32 * 10;

int main(int argc, char **argv)
{
	bool exit = false;
	SDL_Window *window;

	if (argc < 2)
	{
		std::cout << "App Usage: " << argv[0] << " <rom_name>" << std::endl;
		return 0;
	}
	else
	{
		myChip8.initialize();
		myChip8.loadGame(argv[1]);

		window = setupGraphics();
		SDL_Event event;
		while(SDL_WaitEvent(&event))
		{
			if(event.type == SDL_QUIT)
			{
				break;
			}
			else
			{
				myChip8.emulateCycle();
				 if (myChip8.drawFlag) {
					drawGraphics(*window, myChip8);
					std::this_thread::sleep_for(std::chrono::microseconds(1200));
				}
				/*
				myChip8.setKeys();
				*/	
			}   	
    	}
	}

	// Clean up
	SDL_DestroyWindow(window);
    SDL_Quit();
	return 0;
}

SDL_Window* setupGraphics()
{
	std::cout << "Setup Graphics" << std::endl;
    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

    // Create an application window with the following settings:
    SDL_Window* window = SDL_CreateWindow(
        "Chip's Chip-8 Emulator",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        WIDTH,                               // width, in pixels
        HEIGHT,                               // height, in pixels
		NULL
    );

    // Check that the window was successfully created
    if (window == nullptr) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return nullptr;
    }

	// We must call SDL_CreateRenderer in order for draw calls to affect this window.
	SDL_Renderer* renderer;
	renderer = SDL_CreateRenderer(window, -1, 0);

	// Select the color for drawing. It is set to red here.
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	// Clear the entire screen to our selected color.
	SDL_RenderClear(renderer);

	// Up until now everything was drawn behind the scenes.
	// This will show the new, red contents of the window.
	SDL_RenderPresent(renderer);

    // The window is open: could enter program loop here (see SDL_PollEvent())

    return window;
}

void setupInput()
{

}

void drawGraphics(SDL_Window& window, chip8& device)
{
	device.drawFlag = false;

	// Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(&window, -1, 0);
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

    // Create texture that stores frame buffer
    SDL_Texture* sdlTexture = SDL_CreateTexture(renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            64, 32);

	// Store pixels in temporary buffer
	uint32_t pixels[2048] = {};
	for (int i = 0; i < 2048; ++i) {
		uint8_t pixel = device.gfx[i];
		pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
	}
	// Update SDL texture
	SDL_UpdateTexture(sdlTexture, NULL, pixels, 64 * sizeof(Uint32));
	// Clear screen and render
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, sdlTexture, NULL, NULL);
	SDL_RenderPresent(renderer);
}
