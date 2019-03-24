#include "chip8.h"

unsigned char fontset[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

void chip8::initialize()
{
	pc 			= 0x200;
	opcode 		= 0;
	I 			= 0;
	sp 			= 0;

	// TODO:
	// Clear stack
	// Clear registers V0-VF
	// Load fontset
	for (int i = 0; i < 80; i++)
		this->memory[i] = fontset[i];
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
					this->sp--;	
					this->pc = this->stack[this->sp];
					std::cout << "Stack: " << this->stack[sp] << std::endl;
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
		{
			this->drawFlag = true;

			unsigned short number_of_lines = (this->opcode & 0x000F);
			unsigned short vx = (this->opcode & 0x0F00) >> 8;
			unsigned short vy = (this->opcode & 0x00F0) >> 4;
			unsigned short pixel;

			std::cout << "vx: " << vx << std::endl;
			std::cout << "vy: " << vy << std::endl;

			std::cout << "Add sprites to gfx" << std::endl;
			V[0xF] = 0;
            for (int yline = 0; yline < number_of_lines; yline++)
            {
                pixel = memory[I + yline];
                for(int xline = 0; xline < 8; xline++)
                {
                    if((pixel & (0x80 >> xline)) != 0)
                    {
                        if(this->gfx[(vx + xline + ((vy + yline) * 64))] == 1)
                        {
                            V[0xF] = 1;
                        }
                        this->gfx[vx + xline + ((vy + yline) * 64)] ^= 1;
						std::cout << this->gfx[vx + xline + ((vy + yline) * 64)];
                    }
					std::cout << std::endl;
                }
            }
			std::cout << "Finish adding sprites to gfx" << std::endl;
			this->pc += 2;
			
			break;
		}
		case 0xF000:
			switch(opcode & 0x0FF)
			{
				case 0x029:	// LD F, Vx
					// TODO: Implement this.
					I = V[(opcode & 0x0F00) >> 8] * 0x5;
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
