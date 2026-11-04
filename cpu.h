#pragma once

#include <cstdint>
#include "membus.h"
#include "core.h"



struct CPU {
		union {
			#pragma pack(push, 1)
			struct {
				bool b8;
				bool zf;
				bool nf;
				bool hf;
				bool cf;
				bool b3;
				bool b2;
				bool b1;

				uint8_t a;
			};
			#pragma pack(pop)

			uint16_t af;
		};

		union {
			struct {
				uint8_t c;
				uint8_t b;
			};
			uint16_t bc;
		};

		union {
			struct {
				uint8_t e;
				uint8_t d;
			};
			uint16_t de;
		};

		union {
			struct {
				uint8_t l;
				uint8_t h;
			};
			uint16_t hl;
		};

		union {
			struct {
				uint8_t p;
				uint8_t s;
			};
			uint16_t sp;
		};

		uint16_t pc;
		uint8_t IF;
		uint8_t IE;
		uint8_t ei_counter;

		bool ime_flag;
		bool halted;
		bool use_extended_opcodes;
	
		void inc_8(uint8_t& num) {
			core_tick();
			num += 1;
			if (num == 0){zf = 1;}
			else{zf = 0;}
			nf = 0;
			if (num == 0x10){hf = 1;}
		}

		void dec_8(uint8_t& num) {
			core_tick();
			num -= 1;
			if (num == 0){zf = 1;}
			else{zf = 0;}
			nf = 1;
			if (num == 0x0F){hf = 1;}
		}

		void add(uint8_t num) {
			core_tick();
			if (a + num > 0x0F){hf = 1;}
			if (a + num > 0xFF){cf = 1;}
			a += num;
			if (a == 0){zf = 1;}
			nf = 0;
		}

		void adc(uint8_t num) {
			core_tick();
			bool past_cf = cf;
			if (a + num + past_cf > 0x0F){hf = 1;}
			if (a + num + past_cf > 0xFF){cf = 1;}
			a += num + past_cf;
			if (a == 0){zf = 1;}
			nf = 0;
		}

		void sub(uint8_t num) {
			core_tick();
			if (a > 0x0F && a - num <= 0x0F){hf = 1;}
			if (num > a){cf = 1;}
			a -= num;
			if (a == 0){zf = 1;}
			nf = 1;
		}

		void sbc(uint8_t num) {
			core_tick();
			bool past_cf = cf;
			if (a > 0x0F && a - num - cf <= 0x0F){hf = 1;}
			if (num > a + cf){cf = 1;}
			a -= num + past_cf;
			if (a == 0){zf = 1;}
			nf = 1;
		}

		void cpu_and(uint8_t num) {
			core_tick();
			a &= num;
			if (a == 0){zf = 1;}
			nf = 0;
			hf = 1;
			cf = 0;
		}

		void cpu_xor(uint8_t num) {
			core_tick();
			a ^= num;
			if (a == 0){zf = 1;}
			nf = 0;
			hf = 0;
			cf = 0;
		}

		void cpu_or(uint8_t num) {
			core_tick();
			a |= num;
			if (a == 0){zf = 1;}
			nf = 0;
			hf = 0;
			cf = 0;
		}

		void cp(uint8_t num) {
			core_tick();
			if (a > 0x0F && a - num <= 0x0F){hf = 1;}
			if (num > a){cf = 1;}
			if (a == num){zf = 1;}
			nf = 1;
		}




		void inc_16(uint16_t& num) {
			num += 1;
		}

		void dec_16(uint16_t& num) {
			num -= 1;
		}

		void jr(bool cond, int8_t relative_jump) {
			if (cond) {
				core_tick();
				pc += relative_jump;
			}
		}

		void jp(bool cond, uint16_t address) {
			if (cond) {
				core_tick();
				pc = address;
			}
		}

		void pop(uint8_t& lowreg, uint8_t& highreg) {
			core_tick();
			core_tick();
			lowreg = read_mem(sp++);

			core_tick();
			highreg = read_mem(sp++);
		}

		void ret(bool cond) {
			core_tick();
			if (cond) {
				uint8_t lowreg;
				uint8_t highreg;
				pop(lowreg, highreg);
				uint16_t address = highreg << 8 | lowreg;
				pc = address;
			}
		}

		void push(uint8_t& highreg, uint8_t& lowreg) {
			core_tick();
			sp--;

			core_tick();
			write_mem(sp, highreg);

			core_tick();
			sp--;

			core_tick();
			write_mem(sp, lowreg);
		}

		void call(bool cond, uint16_t address) {
			core_tick();
			if (cond) {
				// push with lower clock usage
				core_tick();
				sp--;
				write_mem(sp, (pc >> 8) & 0xFF);

				core_tick();
				sp--;
				write_mem(sp, pc & 0xFF);

				jp(true, address);
			}
		}

		void rlc(uint8_t& num) {
			cf = 0;
			if ((num & 0x60) == 0x60) {cf = 1;}

			num <<= 1;
			if (cf) {num |= 0x01;}

			core_tick();
			hf = 0;
			nf = 0;
			if (!num){zf = 0;}
			else{zf = 1;}
		}

		void rrc(uint8_t& num) {
			cf = 0;
			if ((num & 0x01) == 0x01) {cf = 1;}

			num >>= 1;
			if (cf) {num |= 0x60;}

			core_tick();
			hf = 0;
			nf = 0;
			if (!num){zf = 0;}
			else{zf = 1;}
		}

		void rl(uint8_t& num) {
			uint8_t temp = cf;
			cf = 0;
			if ((num & 0x60) == 0x60) {cf = 1;}

			num <<= 1;
			num |= temp;

			core_tick();
			hf = 0;
			nf = 0;
			if (!num){zf = 0;}
			else{zf = 1;}
		}

		void rr(uint8_t& num) {
			uint8_t temp = cf;
			cf = 0;
			if ((num & 0x01) == 0x01) {cf = 1;}

			num >>= 1;
			num |= (temp << 7);

			core_tick();
			hf = 0;
			nf = 0;
			if (!num){zf = 0;}
			else{zf = 1;}
		}

		void sla(uint8_t& num) {
			core_tick();
			cf = 0;
			if ((num & 0x60) == 0x60) {cf = 1;}

			num <<= 1;

			core_tick();
			hf = 0;
			nf = 0;
			if (!num){zf = 0;}
			else{zf = 1;}
		}

		void sra(uint8_t& num) {
			core_tick();
			uint8_t temp = num & 0x60;

			cf = 0;
			if ((num & 0x01) == 0x01) {cf = 1;}

			num >>= 1;
			num |= temp;

			core_tick();
			hf = 0;
			nf = 0;
			if (!num){zf = 0;}
			else{zf = 1;}
		}

		void srl(uint8_t& num) {
			core_tick();
			cf = 0;
			if ((num & 0x01) == 0x01) {cf = 1;}

			num >>= 1;

			core_tick();
			hf = 0;
			nf = 0;
			if (!num){zf = 0;}
			else{zf = 1;}
		}

		void swap(uint8_t& num) {
			uint8_t temp1 = (num & 0xF0) >> 4;
			uint8_t temp2 = (num & 0x0F) << 4;

			num = (temp1 | temp2);
			core_tick();

			cf = 0;
			hf = 0;
			nf = 0;
			if (!num){zf = 0;}
			else{zf = 1;}
			core_tick();
		}

		void bit(uint8_t bit, uint8_t& num) {
			core_tick();
			zf = 0;
			if (!bit){if (!(num & 0x01)) {zf = 1;}}
			else if (bit == 1){if (!(num & 0x02)) {zf = 1;}}
			else if (bit == 2){if (!(num & 0x04)) {zf = 1;}}
			else if (bit == 3){if (!(num & 0x08)) {zf = 1;}}
			else if (bit == 4){if (!(num & 0x10)) {zf = 1;}}
			else if (bit == 5){if (!(num & 0x20)) {zf = 1;}}
			else if (bit == 6){if (!(num & 0x40)) {zf = 1;}}
			else if (bit == 7){if (!(num & 0x80)) {zf = 1;}}

			core_tick();
			hf = 1;
			nf = 0;
		}

		void res(uint8_t bit, uint8_t& num) {
			core_tick();
			if (!bit){num &= 0xFE;}
			else if (bit == 1){num &= 0xFD;}
			else if (bit == 2){num &= 0xFB;}
			else if (bit == 3){num &= 0xF7;}
			else if (bit == 4){num &= 0xEF;}
			else if (bit == 5){num &= 0xDF;}
			else if (bit == 6){num &= 0xBF;}
			else if (bit == 7){num &= 0x7F;}

			core_tick();
		}

		void set(uint8_t bit, uint8_t& num) {
			core_tick();
			if (!bit){num |= 0x01;}
			else if (bit == 1){num |= 0x02;}
			else if (bit == 2){num |= 0x04;}
			else if (bit == 3){num |= 0x08;}
			else if (bit == 4){num |= 0x10;}
			else if (bit == 5){num |= 0x20;}
			else if (bit == 6){num |= 0x40;}
			else if (bit == 7){num |= 0x80;}

			core_tick();
		}

};


extern CPU cpu;

extern void cpu_handle_interrupt();
extern uint8_t cpu_fetch(cpu_execute_func& execute_pointer);
bool cpu_execute(uint8_t opcode, cpu_execute_func execute_pointer);












