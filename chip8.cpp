#include "chip8.h"

void chip8::initialize()
{
	pc 			= 0x200;
	opcode 	= 0;
	I 			= 0;
	sp 			= 0;

	// TODO:
	// Clear display
	// Clear stack
	// Clear registers V0-VF
	// Clear memory
	// Load fontset
	// Reset timers
}

void chip8::emulateCycle()
{
	opcode = memory[pc] << 8 | memory[pc + 1];
		
	for(int i = 0; i < 4096; i++)
	{
		if (memory[i] != 0)
			std::cout << std::hex << *(memory + i);
	}


	std::cout << std::hex << opcode << std::endl;
	std::cout << "Command: " << (opcode & 0xF000) << " Parameter: " << (opcode & 0x0FFF) << std::endl;

	switch(opcode & 0xF000)
	{
		case 0x6000: // 6xkk - LD Vx, byte - Sets Vx = kk.
			std::cout << "0x6000" << std::endl;
			std::cout << ((opcode & 0x0F00) >> 8) << " = " << (opcode & 0x00FF) << std::endl;
			this->V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
			pc += 2;
			break;

		case 0xA000: // Annn - LD I, addr - Set I = nnn.
			std::cout << "0xA000" << std::endl;
			this->I = opcode & 0x0FFF;
			pc += 2;
			break;
	}

	std::cin.ignore();
}

void chip8::loadGame(const char* gameName)
{
		FILE *file = std::fopen(gameName, "rb");
		
		unsigned char *buffer = &this->memory[pc];

		fread(buffer, sizeof file, 1, file);		

		fclose(file);
}

void chip8::setKeys()
{

}
