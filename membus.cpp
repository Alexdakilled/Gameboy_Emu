#include <vector>
#include "cart.h"
#include "membus.h"
#include "core.h"
#include "cpu.h"
#include "timer.h"
#include "ppu.h"

struct MBC {
	uint16_t ROM0_bank_num = 0;
	uint16_t ROMn_bank_num = 1;
	uint16_t RAMn_bank_num = 0;
	uint8_t mode_register = 0;
	uint8_t extra_register = 0;
	bool RAM_enabled = false;


	uint8_t RAM[16 * 1024] = {};

	uint8_t NoMBC_read(uint16_t location) {
		if (location <= 0x7FFF) {
			return cartridge.full_cart_data[location];
		}

		if (cartridge.cart_type_byte != 0x00) {
			return RAM[location - 0xA000];
		}

		Corelog << "Program attempted cart RAM read despite not declaring RAM in header(" << std::hex << location << ")\n";
		return 0;
	}

	void NoMBC_write(uint16_t location, uint8_t data) {
		if (location <= 0x7FFF) {
			Corelog << "Program attempted cart ROM write despite lacking cart registers(" << std::hex << location << ")\n";
			return;
		}

		if (cartridge.cart_type_byte != 0x00) {
			RAM[location - 0xA000] = data;
			return;
		}

		Corelog << "Program attempted cart RAM write despite not declaring RAM in header(" << std::hex << location << ")\n";
	}

	uint8_t MBC1_read(uint16_t location) {
		if (location <= 0x3FFF) {
			return cartridge.full_cart_data[location + (ROM0_bank_num * 0x3FFF)];
		}

		if (location <= 0x7FFF) {
			return cartridge.full_cart_data[location + (ROMn_bank_num-1 * 0x3FFF)];
		}

		if (cartridge.cart_type_byte != 0x01) {
			if (RAM_enabled) {
				return RAM[location - 0xA000 + (RAMn_bank_num * 0x3FFF)];
			}
			Corelog << "Program attempted cart RAM read despite not enabling RAM(" << std::hex << location << ")\n";
			return 0xFF;
		}

		Corelog << "Program attempted cart RAM read despite not declaring RAM in header(" << std::hex << location << ")\n";
		return 0;
	}

	void MBC1_write(uint16_t location, uint8_t data) {
		if (location <= 0x1FFF) {
			if((data & 0x0F) == 0x0A) {RAM_enabled = true;}
			else{RAM_enabled = false;}

			return;
		}

		if (location <= 0x3FFF) {
			extra_register &= data & 0x1F;

			uint8_t temp;

			if (cartridge.ROM_size_byte <= 0x03) {
				if ((extra_register & 0x0F) == 0) {temp = extra_register + 1;}
				else{temp = extra_register;}
			}
			else {
				if ((extra_register & 0x1F) == 0) {temp = extra_register + 1;}
				else{temp = extra_register;}
			}

			if (mode_register) {
				if (cartridge.ROM_size_byte >= 0x05) {
					ROM0_bank_num = extra_register & 0x60;
					ROMn_bank_num = temp;
					return;
				}

				else {
					Corelog << "(MBC1) Mode 1 + < 1MiB ROM| Broken?\n";
					ROMn_bank_num = temp & 0x1F;
					RAMn_bank_num = (extra_register & 0x60) >> 5;
					return;
				}

			}

			else {
				if (cartridge.ROM_size_byte >= 0x05) {
					ROM0_bank_num = 0;
					ROMn_bank_num = temp;
					return;
				}

				else {
					ROMn_bank_num = temp & 0x1F;
					RAMn_bank_num = (extra_register & 0x60) >> 5;
					return;
				}
			}
		}

		if (location <= 0x5FFF) {
			extra_register &= ((data & 0x03) << 5) & 0x60;

			uint8_t temp;

			if (cartridge.ROM_size_byte <= 0x03) {
				if ((extra_register & 0x0F) == 0) {temp = extra_register + 1;}
				else{temp = extra_register;}
			}
			else {
				if ((extra_register & 0x1F) == 0) {temp = extra_register + 1;}
				else{temp = extra_register;}
			}

			if (mode_register) {
				if (cartridge.ROM_size_byte >= 0x05) {
					ROM0_bank_num = extra_register & 0x60;
					ROMn_bank_num = temp;
					return;
				}

				else {
					Corelog << "(MBC1) Mode 1 + < 1MiB ROM| Broken?\n";
					ROMn_bank_num = temp & 0x1F;
					RAMn_bank_num = (extra_register & 0x60) >> 5;
					return;
				}

			}

			else {
				if (cartridge.ROM_size_byte >= 0x05) {
					ROM0_bank_num = 0;
					ROMn_bank_num = temp;
					return;
				}

				else {
					ROMn_bank_num = temp & 0x1F;
					RAMn_bank_num = (extra_register & 0x60) >> 5;
					return;
				}
			}
		}

		if (location <= 0x7FFF) {
			mode_register = data & 0x01;

			if (mode_register) {ROM0_bank_num = extra_register & 0x60;}
			else {ROM0_bank_num = 0;}

			return;
		}


		if (cartridge.cart_type_byte != 0x01) {
			if (RAM_enabled) {
				RAM[location - 0xA000 + (RAMn_bank_num * 0x3FFF)] = data;
			}
			Corelog << "Program attempted cart RAM write despite not enabling RAM(" << std::hex << location << ")\n";
			return;
		}

		Corelog << "Program attempted cart RAM read despite not declaring RAM in header(" << std::hex << location << ")\n";
	}

	uint8_t MBC2_read(uint16_t location) {
		if (location <= 0x3FFF) {
			return cartridge.full_cart_data[location + (ROM0_bank_num * 0x3FFF)];
		}

		if (location <= 0x7FFF) {
			return cartridge.full_cart_data[location + (ROMn_bank_num-1 * 0x3FFF)];
		}

		if (RAM_enabled) {
			return RAM[(location & 0x01FF) - 0xA000] & 0x0F;
		}

		Corelog << "Program attempted cart RAM read despite not enabling RAM(" << std::hex << location << ")\n";
		return 0xFF;
	}

	void MBC2_write(uint16_t location, uint8_t data) {
		if (location & 0x0100) {
			ROMn_bank_num = data & 0x0F;
			if (ROMn_bank_num == 0){RAMn_bank_num = 1;}
		}

		else {
			if((data & 0x0F) == 0x0A) {RAM_enabled = true;}
			else{RAM_enabled = false;}
		}
	}










	
	uint8_t read(uint16_t location) {
		switch (cartridge.cart_type_byte) {
			// NoMBC
			case 0x00:
			case 0x08:
			case 0x09:
				return NoMBC_read(location);


				// MBC1
			case 0x01:
			case 0x02:
			case 0x03:
				return MBC1_read(location);


			case 0x05:
			case 0x06:
				return MBC2_read(location);


			default: return 0;
		}
	}

	void write(uint16_t location, uint8_t data) {
		switch (cartridge.cart_type_byte) {
			// NoMBC
			case 0x00:
			case 0x08:
			case 0x09:
				return NoMBC_write(location, data);

				// MBC1
			case 0x01:
			case 0x02:
			case 0x03:
				return MBC1_write(location, data);

			case 0x05:
			case 0x06:
				return MBC2_write(location, data);

			default: ;
		}
	}

};


uint8_t IO_read(uint16_t location) {
	switch (location) {
		case 0xFF04:
			return timer.div;
		case 0xFF05:
			return timer.tima;
		case 0xFF06:
			return timer.tma;
		case 0xFF07:
			return timer.tac;
		case 0xFF0F:
			return cpu.IF;
		case 0xFF40:
			return ppu_lcd_registers.control;
		case 0xFF41:
			return ppu_lcd_registers.status;
		case 0xFF42:
			return ppu_lcd_registers.scy;
		case 0xFF43:
			return ppu_lcd_registers.scx;
		case 0xFF44:
			return ppu_lcd_registers.ly;
		case 0xFF45:
			return ppu_lcd_registers.lyc;
		case 0xFF46:
			return ppu_lcd_registers.oam_dma;
		case 0xFF47:
			return ppu_lcd_registers.bgp;
		case 0xFF48:
			return ppu_lcd_registers.obp0;
		case 0xFF49:
			return ppu_lcd_registers.obp1;
		case 0xFF4A:
			return ppu_lcd_registers.wy;
		case 0xFF4B:
			return ppu_lcd_registers.wx;


		case 0xFF50:
			Corelog << "Program read from Boot ROM control?(" << std::hex << location << ")\n";
			return 0x01;



		default:
			Corelog << "Program read from an unimplemented or unsupported IO register(" << std::hex << location << ")\n";
			return 0x00;
	}
}







void IO_write(uint16_t location, uint8_t data) {
	switch (location) {
		case 0xFF04:
			timer.div = 0x00;
			return;
		case 0xFF05:
			timer.tima = data;
			return;
		case 0xFF06:
			timer.tma = data;
			return;
		case 0xFF07:
			timer.tac = data;
			return;
		case 0xFF0F:
			cpu.IF = data;
			return;

		case 0xFF40:
			ppu_on_lcd_control_write(data);
			return;
		case 0xFF41:
			ppu_on_lcd_status_write(data);
			return;
		case 0xFF42:
			ppu_lcd_registers.scy = data;
			return;
		case 0xFF43:
			ppu_lcd_registers.scx = data;
			return;
		case 0xFF44:
			Corelog << "Program wrote to read only Y LCD coord?(" << std::hex << location << ")\n";
			return;
		case 0xFF45:
			ppu_on_lyc_write(data);
			return;
		case 0xFF46:
			ppu_lcd_registers.oam_dma = data;
			return;
		case 0xFF47:
			ppu_lcd_registers.bgp = data;
			ppu_on_palette_write(location);
			return;
		case 0xFF48:
			ppu_lcd_registers.obp0 = data;
			ppu_on_palette_write(location);
			return;
		case 0xFF49:
			ppu_lcd_registers.obp1 = data;
			ppu_on_palette_write(location);
			return;
		case 0xFF4A:
			ppu_lcd_registers.wy = data;
			return;
		case 0xFF4B:
			ppu_lcd_registers.wx = data;
			return;

		case 0xFF50:
			Corelog << "Program wrote to Boot ROM control?(" << std::hex << location << ")\n";
			return;



		default:
			Corelog << "Program wrote to an unimplemented or unsupported IO register(" << std::hex << location << ")\n";
	}
}





static MBC mbc;

uint8_t VRAM[0x2000];// 8000-9FFF
uint8_t OAM[0x00A0];// FE00-FE9F
uint8_t WRAM[0x2000];// C000-DFFF
uint8_t HRAM[0x007F];// FF80-FFFE

uint8_t read_mem(uint16_t location) {
	switch (location & 0xF000) {
		case 0x0000:
		case 0x1000:
		case 0x2000:
		case 0x3000:
		case 0x4000:
		case 0x5000:
		case 0x6000:
		case 0x7000:
			return mbc.read(location);

		case 0x8000:
		case 0x9000:
			if (LCDS_PPU_MODE == DRAWING) {
				Corelog << "Program read from VRAM while PPU is drawing(" << std::hex << location << ")\n";
				return 0xFF;
			}


			return VRAM[location - 0x8000];

		case 0xA000:
		case 0xB000:
			return mbc.read(location);

		case 0xC000:
		case 0xD000:
			return WRAM[location - 0xC000];

		case 0xE000:
			Corelog << "Program accessed Echo RAM?(" << std::hex << location << ")\n";
			return WRAM[location - 0xE000];

		case 0xF000:
			if (location <= 0xFDFF) {
				Corelog << "Program accessed Echo RAM?(" << std::hex << location << ")\n";
				return WRAM[location - 0xE000];
			}
			if (location <= 0xFE9F) {
				if (LCDS_PPU_MODE == DRAWING || LCDS_PPU_MODE == OAM_SEARCH) {
					Corelog << "Program read from OAM while PPU is drawing/searching(" << std::hex << location << ")\n";
					return 0xFF;
				}

				return OAM[location - 0xFE00];
			}
			if (location <= 0xFEFF) {
				Corelog << "Program accessed unusable OAM extra?(" << std::hex << location << ")\n";
				if (LCDS_PPU_MODE == DRAWING || LCDS_PPU_MODE == OAM_SEARCH) {
					Corelog << "Program read from OAM while PPU is drawing/searching(" << std::hex << location << ")\n";
					return 0xFF;
				}
				return 0x00;
			}
			if (location <= 0xFF7F) {
				return IO_read(location);
			}
			if (location <= 0xFFFE) {
				return HRAM[location - 0xFF80];
			}

			return cpu.IE;

		default:
			Corelog << "Impossible switch/case logic in read_mem?(" << std::hex << location << ")\n";
			return 0xFF;
	}
}


void write_mem(uint16_t location, uint8_t data) {
	switch (location & 0xF000) {
		case 0x0000:
		case 0x1000:
		case 0x2000:
		case 0x3000:
		case 0x4000:
		case 0x5000:
		case 0x6000:
		case 0x7000:
			return mbc.write(location, data);

		case 0x8000:
		case 0x9000:
			if (LCDS_PPU_MODE == DRAWING) {
				Corelog << "Program wrote to VRAM while PPU is drawing(" << std::hex << location << ")\n";
				return;
			}

			VRAM[location - 0x8000] = data;
			return;

		case 0xA000:
		case 0xB000:
			return mbc.write(location, data);

		case 0xC000:
		case 0xD000:
			WRAM[location - 0xC000] = data;
			return;

		case 0xE000:
			Corelog << "Program wrote to Echo RAM?(" << std::hex << location << ")\n";
			WRAM[location - 0xE000] = data;
			return;

		case 0xF000:
			if (location <= 0xFDFF) {
				Corelog << "Program wrote to Echo RAM?(" << std::hex << location << ")\n";
				WRAM[location - 0xE000] = data;
				return;
			}
			if (location <= 0xFE9F) {
				if (LCDS_PPU_MODE == DRAWING || LCDS_PPU_MODE == OAM_SEARCH) {
					Corelog << "Program wrote to OAM while PPU is drawing/searching(" << std::hex << location << ")\n";
					return;
				}

				OAM[location - 0xFE00] = data;
				return;
			}
			if (location <= 0xFEFF) {
				Corelog << "Program wrote to unusable OAM extra?(" << std::hex << location << ")\n";
				if (LCDS_PPU_MODE == DRAWING || LCDS_PPU_MODE == OAM_SEARCH) {
					Corelog << "Program wrote to OAM while PPU is drawing/searching(" << std::hex << location << ")\n";
				}
				return;
			}
			if (location <= 0xFF7F) {
				return IO_write(location, data);
			}
			if (location <= 0xFFFE) {
				HRAM[location - 0xFF80] = data;
				return;
			}

			cpu.IE = data;
			return;

		default:
			Corelog << "Impossible switch/case logic in write_mem?(" << std::hex << location << ")\n";
	}
}