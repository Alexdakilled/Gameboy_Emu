#include <cstdint>
#include <fstream>
#include <vector>
#include "cart.h"
#include "core.h"

const unsigned char logo_dump[] = {0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83,
0x00, 0x0C, 0x00,0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9,0x99, 0xBB, 0xBB, 0x67, 0x63,
0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E };

Cartridge cartridge;

bool cart_init(const std::string& filename) {
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if (!file.is_open()) {
		Corelog << "Failed to open the game\n";
		return false;
	}

	file.seekg(0, std::ios::beg);

	std::vector<uint8_t> buffer(static_cast<size_t>(1024 * 1024));
	if (!file.read(reinterpret_cast<char*>(buffer.data()), 1024 * 1024)) {
		// allow partial reads but fail if nothing was read
		std::streamsize got = file.gcount();
		if (got <= 0) {
			Corelog << "Error reading data from file\n";
			return false;
		}
		buffer.resize(static_cast<size_t>(got));
	}

	cartridge.full_cart_data = buffer;


	// Nintendo Logo
	cartridge.valid_logo = true;
	for (int i = 0; i <= 0x2F; ++i) {
		if (buffer[i + 0x0104] != logo_dump[i]) {
			cartridge.valid_logo = false;
		}
	}

	// Title
	for (int i = 0; i <= 0x0F; ++i) {
		cartridge.title[i] = buffer[i + 0x0134];
	}

	// Manufacturer Code
	for (int i = 0; i <= 3; ++i) {
		cartridge.manu_code[i] = buffer[i + 0x013F];
	}

	// CGB Flag
	cartridge.CGBonly_flag_exist = false;
	cartridge.CGBonly_flag = false;

	if (buffer[0x0143] == 0x0180) {
		cartridge.CGBonly_flag_exist = true;
		cartridge.CGBonly_flag = false;
	}
	else if (buffer[0x0143] == 0x01C0) {
		cartridge.CGBonly_flag_exist = true;
		cartridge.CGBonly_flag = true;
	}

	// Licensee
	for (int i = 0; i <= 1; ++i) {
		cartridge.new_licensee_bytes[i] = buffer[i + 0x0144];
	}

	cartridge.old_licensee_byte = buffer[0x0133];


	// SGB Flag
	cartridge.SGB_flag = (buffer[0x0146] == 0x03);

	// Cartridge Type
	cartridge.cart_type_byte = buffer[0x0147];

	// ROM Size
	cartridge.ROM_size_byte = buffer[0x0148];

	// RAM Size
	cartridge.RAM_size_byte = buffer[0x0149];

	// Destination Code
	cartridge.dest_code_byte = buffer[0x014A];

	// Mask ROM Version Number
	cartridge.ROM_version = buffer[0x014C];

	// Header Checksum
	cartridge.checksum = buffer[0x014D];

	unsigned char checksum = 0;
	for (int i = 0; i <= 24; i++) {
		checksum = checksum - buffer[i + 0x0134] - 1;
	}

	cartridge.checksum_passed = (buffer[0x014D] == checksum);


	return true;
}