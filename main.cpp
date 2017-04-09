#include <iostream>
#include "chip8.h"

chip8 myChip8;

void setupGraphics();
void setupInput();
void drawGraphics();

int main(int argc, char **argv)
{
	bool exit = false;
	if (argc < 2)
	{
		std::cout << "App Usage: " << argv[0] << " <rom_name>" << std::endl;
		return 0;
	}
	else
	{
		myChip8.initialize();
		myChip8.loadGame(argv[1]);

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
	return 0;
}

void setupGraphics()
{

}

void setupInput()
{

}

void drawGraphics()
{

}
