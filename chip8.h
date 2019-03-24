#include <string>
#include <iostream>
#include <cstdio>

struct chip8
{
	uint16_t opcode;
	uint8_t memory[4096] = {};

	uint8_t V[16] = {};

	uint16_t I;
	uint16_t pc;

	uint8_t gfx[2048] = {};

	uint8_t delay_timer;
	uint8_t sound_timer;

	uint16_t stack[16] = {};
	uint16_t sp;

	uint8_t key[16];

	bool drawFlag;

	void initialize();
	void emulateCycle();
	void loadGame(const char* gameName);
	void setKeys();

};
