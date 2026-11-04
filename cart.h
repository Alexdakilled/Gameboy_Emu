#pragma once

#include <string>
#include <cstdint>
#include <vector>

extern const uint8_t logo_dump[];

struct Cartridge
{
	std::vector<uint8_t> full_cart_data;

	bool valid_logo;
	uint8_t title[16];
	uint8_t manu_code[4];
	bool CGBonly_flag_exist;
	bool CGBonly_flag;
	uint8_t new_licensee_bytes[2];
	uint8_t old_licensee_byte;
	bool SGB_flag;
	uint8_t cart_type_byte;
	uint8_t ROM_size_byte;
	uint8_t RAM_size_byte;
	uint8_t dest_code_byte;
	uint8_t ROM_version;
	uint8_t checksum;
	bool checksum_passed;

	};


extern Cartridge cartridge;

bool cart_init(const std::string& filename);