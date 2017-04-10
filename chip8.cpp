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
	this->opcode = this->memory[pc] << 8 | this->memory[pc + 1];
		
	std::cout << std::hex << this->opcode << std::endl;
	std::cout << "Command: " << (this->opcode & 0xF000) << " Parameter: " << (this->opcode & 0x0FFF) << std::endl;

	switch(this->opcode & 0xF000)
	{
		case 0x0000: 
			switch(this->opcode & 0x00FF)
			{
				case 0x00EE:	// RET
					this->pc = this->stack[this->sp - 1];
					this->sp--;	
					std::cout << "Stack: " << this->stack[sp-1] << std::endl;
					std::cout << "SP: " << this->sp << std::endl;
					break;
			}
			break;
		case 0x2000: // 2nnn - CALL addr
			this->stack[sp] = this->pc;
			this->sp++;
			this->pc = (opcode & 0x0FFF);
			std::cout << "Stack: " << this->stack[sp-1] << std::endl;
			std::cout << "SP: " << this->sp << std::endl;

			break;

		case 0x6000: // 6xkk - LD Vx, byte - Sets Vx = kk.
			this->V[(this->opcode & 0x0F00) >> 8] = (this->opcode & 0x00FF);
			this->pc += 2;
			break;

		case 0x7000:	// ADD Vx, byte
			this->V[(this->opcode & 0x0F00) >> 8] += this->opcode & 0x00FF;
			this->pc += 2;
			break;

		case 0xA000: // Annn - LD I, addr - Set I = nnn.
			this->I = (this->opcode & 0x0FFF);
			this->pc += 2;
			break;

		case 0xD000: // Dxyn - DRW Vx, Vy, nibble  Draws n-by sprite from I to (Vx, Vy) ser VF = collision.
			// TODO: Implement this.
			
			this->pc += 2;
			break;

		case 0xF000:
			switch(opcode & 0x0FF)
			{
				case 0x029:	// LD F, Vx
					// TODO: Implement this.
					
					this->pc += 2;
					break;
				case 0x033:	// LD B, Vx 
					this->memory[this->I] = (V[(opcode & 0xF000) >> 8] / 100);
					this->memory[this->I + 1] = (V[(opcode & 0xF000) >> 8] / 10) % 10;
					this->memory[this->I + 2] = (V[(opcode & 0xF000) >> 8] % 100) % 10;
					this->pc += 2;
					break;

				case 0x065:	// LD Vx, [I]
					for(int i = 0; i < ((opcode & 0x0F00) >> 8); i++)
					{
						this->V[i] = this->memory[this->I + i];
					}
					this->pc += 2;
					break;
			}
	}

	std::cin.ignore();
}

void chip8::loadGame(const char* gameName)
{
		FILE *file = std::fopen(gameName, "rb");
		
		if(file == NULL)
		{	
			std::cout << "Could not read the file." << std::endl;
			exit(1);
		}

		fseek (file, 0L, SEEK_END);
		int fsize = ftell(file);
		fseek(file, 0L, SEEK_SET);

		uint8_t *buffer = &this->memory[pc];
	
		fread(buffer, fsize, 1, file);		

		fclose(file);
}

void chip8::setKeys()
{

}
