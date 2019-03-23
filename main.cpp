#include <iostream>
#include "SDL2/SDL.h"
#include "chip8.h"

chip8 myChip8;

SDL_Window* setupGraphics();
void setupInput();
void drawGraphics();

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

		while(!exit)
		{
			myChip8.emulateCycle();
			/*
			if(myChip8.drawFlag)
				drawGraphics();

			myChip8.setKeys();
			*/
		}
	}

	// Clean up
	SDL_DestroyWindow(window);
    SDL_Quit();
	return 0;
}

SDL_Window* setupGraphics()
{
    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

    // Create an application window with the following settings:
    SDL_Window* window = SDL_CreateWindow(
        "Chip's Chip-8 Emulator",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        640,                               // width, in pixels
        480,                               // height, in pixels
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
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

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

void drawGraphics()
{

}
