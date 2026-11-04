#pragma once
#include <cstdint>

extern uint8_t VRAM[0x2000];// 8000-9FFF
extern uint8_t OAM[0x00A0];// FE00-FE9F


void membus_init();
uint8_t read_mem(uint16_t location);
void write_mem(uint16_t location, uint8_t data);