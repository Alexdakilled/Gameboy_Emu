#include "cpu.h"
#include "core.h"

CPU cpu;

namespace {
	uint16_t ld_2_bytes() {
		core_tick();
		uint16_t num = read_mem(cpu.pc++);

		core_tick();
		num |= static_cast<uint16_t>(read_mem(cpu.pc++)) << 8;

		return num;

	}

	uint8_t ld_1_byte() {
		core_tick();
		uint8_t num = read_mem(cpu.pc++);
		return num;
	}



	// Instructions
	void cpu_noop() {
		core_tick();
	}

	void cpu_halt() {
		cpu.halted = true;
	}

	void cpu_ccf() {
		core_tick();
		cpu.nf = 0;
		cpu.hf = 0;
		cpu.cf = not cpu.cf;
	}

	void cpu_cpl() {
		core_tick();
		cpu.nf = 1;
		cpu.hf = 1;
		cpu.a = ~cpu.a;
	}


	void cpu_incb() {
		cpu.inc_8(cpu.b);
	}
	void cpu_incc() {
		cpu.inc_8(cpu.c);
	}
	void cpu_incd() {
		cpu.inc_8(cpu.d);
	}
	void cpu_ince() {
		cpu.inc_8(cpu.e);
	}
	void cpu_inch() {
		cpu.inc_8(cpu.h);
	}
	void cpu_incl() {
		cpu.inc_8(cpu.l);
	}
	void cpu_inca() {
		cpu.inc_8(cpu.a);
	}
	void cpu_incHL() {
		core_tick();
		uint8_t address = read_mem(cpu.hl);
		core_tick();
		cpu.inc_8(address);
	}


	void cpu_decb() {
		cpu.dec_8(cpu.b);
	}
	void cpu_decc() {
		cpu.dec_8(cpu.c);
	}
	void cpu_decd() {
		cpu.dec_8(cpu.d);
	}
	void cpu_dece() {
		cpu.dec_8(cpu.e);
	}
	void cpu_dech() {
		cpu.dec_8(cpu.h);
	}
	void cpu_decl() {
		cpu.dec_8(cpu.l);
	}
	void cpu_deca() {
		cpu.dec_8(cpu.a);
	}
	void cpu_decHL() {
		core_tick();
		uint8_t address = read_mem(cpu.hl);
		core_tick();
		cpu.dec_8(address);
	}

	void cpu_addab() {
		cpu.add(cpu.b);
	}
	void cpu_addac() {
		cpu.add(cpu.c);
	}
	void cpu_addad() {
		cpu.add(cpu.c);
	}
	void cpu_addae() {
		cpu.add(cpu.e);
	}
	void cpu_addah() {
		cpu.add(cpu.h);
	}
	void cpu_addal() {
		cpu.add(cpu.l);
	}
	void cpu_addaHL() {
		core_tick();
		uint8_t num = read_mem(cpu.hl);
		cpu.add(num);
	}
	void cpu_addaa() {
		cpu.add(cpu.a);
	}
	void cpu_addan() {
		uint8_t num = ld_1_byte();
		cpu.add(num);
	}

	void cpu_adcab() {
		cpu.adc(cpu.b);
	}
	void cpu_adcac() {
		cpu.adc(cpu.c);
	}
	void cpu_adcad() {
		cpu.adc(cpu.c);
	}
	void cpu_adcae() {
		cpu.adc(cpu.e);
	}
	void cpu_adcah() {
		cpu.adc(cpu.h);
	}
	void cpu_adcal() {
		cpu.adc(cpu.l);
	}
	void cpu_adcaHL() {
		core_tick();
		uint8_t num = read_mem(cpu.hl);
		cpu.adc(num);
	}
	void cpu_adcaa() {
		cpu.adc(cpu.a);
	}
	void cpu_adcan() {
		uint8_t num = ld_1_byte();
		cpu.adc(num);
	}

	void cpu_subab() {
		cpu.sub(cpu.b);
	}
	void cpu_subac() {
		cpu.sub(cpu.c);
	}
	void cpu_subad() {
		cpu.sub(cpu.c);
	}
	void cpu_subae() {
		cpu.sub(cpu.e);
	}
	void cpu_subah() {
		cpu.sub(cpu.h);
	}
	void cpu_subal() {
		cpu.sub(cpu.l);
	}
	void cpu_subaHL() {
		core_tick();
		uint8_t num = read_mem(cpu.hl);
		cpu.sub(num);
	}
	void cpu_subaa() {
		cpu.sub(cpu.a);
	}
	void cpu_suban() {
		uint8_t num = ld_1_byte();
		cpu.sub(num);
	}

	void cpu_sbcab() {
		cpu.sbc(cpu.b);
	}
	void cpu_sbcac() {
		cpu.sbc(cpu.c);
	}
	void cpu_sbcad() {
		cpu.sbc(cpu.c);
	}
	void cpu_sbcae() {
		cpu.sbc(cpu.e);
	}
	void cpu_sbcah() {
		cpu.sbc(cpu.h);
	}
	void cpu_sbcal() {
		cpu.sbc(cpu.l);
	}
	void cpu_sbcaHL() {
		core_tick();
		uint8_t num = read_mem(cpu.hl);
		cpu.sbc(num);
	}
	void cpu_sbcaa() {
		cpu.sbc(cpu.a);
	}
	void cpu_sbcan() {
		uint8_t num = ld_1_byte();
		cpu.sbc(num);
	}

	void cpu_andab() {
		cpu.cpu_and(cpu.b);
	}
	void cpu_andac() {
		cpu.cpu_and(cpu.c);
	}
	void cpu_andad() {
		cpu.cpu_and(cpu.c);
	}
	void cpu_andae() {
		cpu.cpu_and(cpu.e);
	}
	void cpu_andah() {
		cpu.cpu_and(cpu.h);
	}
	void cpu_andal() {
		cpu.cpu_and(cpu.l);
	}
	void cpu_andaHL() {
		core_tick();
		uint8_t num = read_mem(cpu.hl);
		cpu.cpu_and(num);
	}
	void cpu_andaa() {
		cpu.cpu_and(cpu.a);
	}
	void cpu_andan() {
		uint8_t num = ld_1_byte();
		cpu.cpu_and(num);
	}

	void cpu_xorab() {
		cpu.cpu_xor(cpu.b);
	}
	void cpu_xorac() {
		cpu.cpu_xor(cpu.c);
	}
	void cpu_xorad() {
		cpu.cpu_xor(cpu.c);
	}
	void cpu_xorae() {
		cpu.cpu_xor(cpu.e);
	}
	void cpu_xorah() {
		cpu.cpu_xor(cpu.h);
	}
	void cpu_xoral() {
		cpu.cpu_xor(cpu.l);
	}
	void cpu_xoraHL() {
		core_tick();
		uint8_t num = read_mem(cpu.hl);
		cpu.cpu_xor(num);
	}
	void cpu_xoraa() {
		cpu.cpu_xor(cpu.a);
	}
	void cpu_xoran() {
		uint8_t num = ld_1_byte();
		cpu.cpu_xor(num);
	}

	void cpu_orab() {
		cpu.cpu_or(cpu.b);
	}
	void cpu_orac() {
		cpu.cpu_or(cpu.c);
	}
	void cpu_orad() {
		cpu.cpu_or(cpu.c);
	}
	void cpu_orae() {
		cpu.cpu_or(cpu.e);
	}
	void cpu_orah() {
		cpu.cpu_or(cpu.h);
	}
	void cpu_oral() {
		cpu.cpu_or(cpu.l);
	}
	void cpu_oraHL() {
		core_tick();
		uint8_t num = read_mem(cpu.hl);
		cpu.cpu_or(num);
	}
	void cpu_oraa() {
		cpu.cpu_or(cpu.a);
	}
	void cpu_oran() {
		uint8_t num = ld_1_byte();
		cpu.cpu_or(num);
	}

	void cpu_cpab() {
		cpu.cp(cpu.b);
	}
	void cpu_cpac() {
		cpu.cp(cpu.c);
	}
	void cpu_cpad() {
		cpu.cp(cpu.c);
	}
	void cpu_cpae() {
		cpu.cp(cpu.e);
	}
	void cpu_cpah() {
		cpu.cp(cpu.h);
	}
	void cpu_cpal() {
		cpu.cp(cpu.l);
	}
	void cpu_cpaHL() {
		core_tick();
		uint8_t num = read_mem(cpu.hl);
		cpu.cp(num);
	}
	void cpu_cpaa() {
		cpu.cp(cpu.a);
	}
	void cpu_cpan() {
		uint8_t num = ld_1_byte();
		cpu.cp(num);
	}










	void cpu_incbc() {
		core_tick();
		core_tick();
		cpu.inc_16(cpu.bc);
	}

	void cpu_incde() {
		core_tick();
		core_tick();
		cpu.inc_16(cpu.de);
	}

	void cpu_inchl() {
		core_tick();
		core_tick();
		cpu.inc_16(cpu.hl);
	}

	void cpu_incsp() {
		core_tick();
		core_tick();
		cpu.inc_16(cpu.sp);
	}

	void cpu_decbc() {
		core_tick();
		core_tick();
		cpu.dec_16(cpu.bc);
	}

	void cpu_decde() {
		core_tick();
		core_tick();
		cpu.dec_16(cpu.de);
	}

	void cpu_dechl() {
		core_tick();
		core_tick();
		cpu.dec_16(cpu.hl);
	}

	void cpu_decsp() {
		core_tick();
		core_tick();
		cpu.dec_16(cpu.sp);
	}

	void cpu_addhlbc() {
		core_tick();
		core_tick();
		cpu.nf = 0;
		if (cpu.hl + cpu.bc > 0x0FFF){cpu.hf = 1;}
		if (cpu.hl + cpu.bc > 0xFFFF){cpu.cf = 1;}
		cpu.hl += cpu.bc;
	}

	void cpu_addhlde() {
		core_tick();
		core_tick();
		cpu.nf = 0;
		if (cpu.hl + cpu.de > 0x0FFF){cpu.hf = 1;}
		if (cpu.hl + cpu.de > 0xFFFF){cpu.cf = 1;}
		cpu.hl += cpu.de;
	}

	void cpu_addhlhl() {
		core_tick();
		core_tick();
		cpu.nf = 0;
		if (cpu.hl + cpu.hl > 0x0FFF){cpu.hf = 1;}
		if (cpu.hl + cpu.hl > 0xFFFF){cpu.cf = 1;}
		cpu.hl += cpu.hl;
	}

	void cpu_addhlsp() {
		core_tick();
		core_tick();
		cpu.nf = 0;
		if (cpu.hl + cpu.sp > 0x0FFF){cpu.hf = 1;}
		if (cpu.hl + cpu.sp > 0xFFFF){cpu.cf = 1;}
		cpu.hl += cpu.sp;
	}

	void cpu_addspn() {
		core_tick();
		auto num = static_cast<int8_t>(ld_1_byte());

		core_tick();
		core_tick();
		core_tick();
		cpu.zf = 0;
		cpu.nf = 0;
		if (cpu.sp + num > 0x000F){cpu.hf = 1;}
		if (cpu.sp + num > 0x00FF){cpu.cf = 1;}
		cpu.sp += num;
	}




	// LD B r8
	void cpu_ldbb() {
		core_tick();
		Corelog << "Possible breakpoint?";
		cpu.b = cpu.b;
	}

	void cpu_ldbc() {
		core_tick();
		cpu.b = cpu.c;
	}

	void cpu_ldbd() {
		core_tick();
		cpu.b = cpu.d;
	}

	void cpu_ldbe() {
		core_tick();
		cpu.b = cpu.e;
	}

	void cpu_ldbh() {
		core_tick();
		cpu.b = cpu.h;
	}

	void cpu_ldbl() {
		core_tick();
		cpu.b = cpu.l;
	}

	void cpu_ldbHL() {
		core_tick();
		core_tick();
		cpu.b = read_mem(cpu.hl);
	}

	void cpu_ldba() {
		core_tick();
		cpu.b = cpu.a;
	}

	// LD C r8
	void cpu_ldcb() {
		core_tick();
		cpu.c = cpu.b;
	}

	void cpu_ldcc() {
		core_tick();
		cpu.c = cpu.c;
	}

	void cpu_ldcd() {
		core_tick();
		cpu.c = cpu.d;
	}

	void cpu_ldce() {
		core_tick();
		cpu.c = cpu.e;
	}

	void cpu_ldch() {
		core_tick();
		cpu.c = cpu.h;
	}

	void cpu_ldcl() {
		core_tick();
		cpu.c = cpu.l;
	}

	void cpu_ldcHL() {
		core_tick();
		core_tick();
		cpu.c = read_mem(cpu.hl);
	}

	void cpu_ldca() {
		core_tick();
		cpu.c = cpu.a;
	}

	// LD D r8
	void cpu_lddb() {
		core_tick();
		cpu.d = cpu.b;
	}

	void cpu_lddc() {
		core_tick();
		cpu.d = cpu.c;
	}

	void cpu_lddd() {
		core_tick();
		Corelog << "Possible debug msg?";
		cpu.d = cpu.d;
	}

	void cpu_ldde() {
		core_tick();
		cpu.d = cpu.e;
	}

	void cpu_lddh() {
		core_tick();
		cpu.d = cpu.h;
	}

	void cpu_lddl() {
		core_tick();
		cpu.d = cpu.l;
	}

	void cpu_lddHL() {
		core_tick();
		core_tick();
		cpu.d = read_mem(cpu.hl);
	}

	void cpu_ldda() {
		core_tick();
		cpu.d = cpu.a;
	}

	// LD E r8
	void cpu_ldeb() {
		core_tick();
		cpu.e = cpu.b;
	}

	void cpu_ldec() {
		core_tick();
		cpu.e = cpu.c;
	}

	void cpu_lded() {
		core_tick();
		cpu.e = cpu.d;
	}

	void cpu_ldee() {
		core_tick();
		cpu.e = cpu.e;
	}

	void cpu_ldeh() {
		core_tick();
		cpu.e = cpu.h;
	}

	void cpu_ldel() {
		core_tick();
		cpu.c = cpu.l;
	}

	void cpu_ldeHL() {
		core_tick();
		core_tick();
		cpu.e = read_mem(cpu.hl);
	}

	void cpu_ldea() {
		core_tick();
		cpu.e = cpu.a;
	}

	// LD H r8
	void cpu_ldhb() {
		core_tick();
		cpu.h = cpu.b;
	}

	void cpu_ldhc() {
		core_tick();
		cpu.h = cpu.c;
	}

	void cpu_ldhd() {
		core_tick();
		cpu.h = cpu.d;
	}

	void cpu_ldhe() {
		core_tick();
		cpu.h = cpu.e;
	}

	void cpu_ldhh() {
		core_tick();
		cpu.h = cpu.h;
	}

	void cpu_ldhl() {
		core_tick();
		cpu.h = cpu.l;
	}

	void cpu_ldhHL() {
		core_tick();
		core_tick();
		cpu.h = read_mem(cpu.hl);
	}

	void cpu_ldha() {
		core_tick();
		cpu.h = cpu.a;
	}

	// LD L r8
	void cpu_ldlb() {
		core_tick();
		cpu.l = cpu.b;
	}

	void cpu_ldlc() {
		core_tick();
		cpu.l = cpu.c;
	}

	void cpu_ldld() {
		core_tick();
		cpu.l = cpu.d;
	}

	void cpu_ldle() {
		core_tick();
		cpu.l = cpu.e;
	}

	void cpu_ldlh() {
		core_tick();
		cpu.l = cpu.h;
	}

	void cpu_ldll() {
		core_tick();
		cpu.l = cpu.l;
	}

	void cpu_ldlHL() {
		core_tick();
		core_tick();
		cpu.l = read_mem(cpu.hl);
	}

	void cpu_ldla() {
		core_tick();
		cpu.l = cpu.a;
	}

	// LD [HL] r8
	void cpu_ldHLb() {
		core_tick();
		core_tick();
		write_mem(cpu.hl, cpu.b);
	}

	void cpu_ldHLc() {
		core_tick();
		core_tick();
		write_mem(cpu.hl, cpu.c);
	}

	void cpu_ldHLd() {
		core_tick();
		core_tick();
		write_mem(cpu.hl, cpu.d);
	}

	void cpu_ldHLe() {
		core_tick();
		core_tick();
		write_mem(cpu.hl, cpu.e);
	}

	void cpu_ldHLh() {
		core_tick();
		core_tick();
		write_mem(cpu.hl, cpu.h);
	}

	void cpu_ldHLl() {
		core_tick();
		core_tick();
		write_mem(cpu.hl, cpu.l);
	}

	void cpu_ldHLa() {
		core_tick();
		core_tick();
		write_mem(cpu.hl, cpu.a);
	}


	// LD A r8
	void cpu_ldab() {
		core_tick();
		cpu.a = cpu.b;
	}

	void cpu_ldac() {
		core_tick();
		cpu.a = cpu.c;
	}

	void cpu_ldad() {
		core_tick();
		cpu.a = cpu.d;
	}

	void cpu_ldae() {
		core_tick();
		cpu.a = cpu.e;
	}

	void cpu_ldah() {
		core_tick();
		cpu.a = cpu.h;
	}

	void cpu_ldal() {
		core_tick();
		cpu.a = cpu.l;
	}

	void cpu_ldaHL() {
		core_tick();
		core_tick();
		cpu.a = read_mem(cpu.hl);
	}

	void cpu_ldaa() {
		core_tick();
		cpu.a = cpu.a;
	}



	void cpu_ldBCa() {
		core_tick();
		core_tick();
		write_mem(cpu.bc, cpu.a);
	}

	void cpu_ldDEa() {
		core_tick();
		core_tick();
		write_mem(cpu.de, cpu.a);
	}

	void cpu_ldHLIa() {
		core_tick();
		core_tick();
		write_mem(cpu.hl++, cpu.a);
	}

	void cpu_ldHLDa() {
		core_tick();
		core_tick();
		write_mem(cpu.hl--, cpu.a);
	}


	void cpu_ldaBC() {
		core_tick();
		core_tick();
		cpu.a = read_mem(cpu.bc);
	}

	void cpu_ldaDE() {
		core_tick();
		core_tick();
		cpu.a = read_mem(cpu.de);
	}

	void cpu_ldaHLI() {
		core_tick();
		core_tick();
		cpu.a = read_mem(cpu.hl++);
	}

	void cpu_ldaHLD() {
		core_tick();
		core_tick();
		cpu.a = read_mem(cpu.hl--);
	}

	void cpu_ldNNa() {
		uint16_t address = ld_2_bytes();
		core_tick();
		core_tick();
		write_mem(address, cpu.a);
	}

	void cpu_ldaNN() {
		uint16_t address = ld_2_bytes();
		core_tick();
		core_tick();
		cpu.a = read_mem(address);
	}





	void cpu_ldbn() {
		uint8_t num = ld_1_byte();

		core_tick();
		cpu.b = num;
	}

	void cpu_lddn() {
		uint8_t num = ld_1_byte();

		core_tick();
		cpu.d = num;
	}

	void cpu_ldhn() {
		uint8_t num = ld_1_byte();

		core_tick();
		cpu.h = num;
	}

	void cpu_ldHLn() {
		uint8_t num = ld_1_byte();

		core_tick();
		core_tick();
		write_mem(cpu.hl, num);
	}

	void cpu_ldcn() {
		uint8_t num = ld_1_byte();

		core_tick();
		cpu.c = num;
	}

	void cpu_lden() {
		uint8_t num = ld_1_byte();

		core_tick();
		cpu.e = num;
	}

	void cpu_ldln() {
		uint8_t num = ld_1_byte();

		core_tick();
		cpu.l = num;
	}

	void cpu_ldan() {
		uint8_t num = ld_1_byte();

		core_tick();
		cpu.a = num;
	}


	void cpu_ldbcnn() {
		uint16_t num = ld_2_bytes();

		core_tick();
		cpu.bc = num;

	}

	void cpu_lddenn() {
		uint16_t num = ld_2_bytes();

		core_tick();
		cpu.de = num;

	}

	void cpu_ldhlnn() {
		uint16_t num = ld_2_bytes();

		core_tick();
		cpu.hl = num;

	}

	void cpu_ldspnn() {
		uint16_t num = ld_2_bytes();

		core_tick();
		cpu.sp = num;

	}

	void cpu_ldhNa() {
		uint16_t address = ld_1_byte();

		core_tick();
		address += 0xFF00;

		core_tick();
		write_mem(address, cpu.a);
	}

	void cpu_ldhaN() {
		uint16_t address = ld_1_byte();

		core_tick();
		address += 0xFF00;

		core_tick();
		cpu.a = read_mem(address);
	}

	void cpu_ldhCa() {
		core_tick();
		uint16_t address = 0xFF00 + cpu.c;

		core_tick();
		write_mem(address, cpu.a);
	}

	void cpu_ldhaC() {
		core_tick();
		uint16_t address = 0xFF00 + cpu.c;

		core_tick();
		cpu.a = read_mem(address);
	}

	void cpu_ldNNsp() {
		uint16_t address = ld_2_bytes();

		core_tick();
		write_mem(address, cpu.sp & 0xFF);

		core_tick();
		core_tick();
		write_mem(address + 1, cpu.sp >> 8);
	}

	void cpu_ldhlspn() {
		uint8_t num = ld_1_byte();

		core_tick();
		uint16_t result = cpu.sp + static_cast<int8_t>(num);
		cpu.zf = 0;
		cpu.nf = 0;
		cpu.hf = (result > 0xF);
		cpu.cf = (result > 0xFF);

		core_tick();
		cpu.hl = result;
	}

	void cpu_ldsphl() {
		core_tick();
		core_tick();

		cpu.sp = cpu.hl;
	}



	void cpu_jrn() {
		core_tick();
		uint8_t jr_address = ld_1_byte();
		cpu.jr(true, static_cast<int8_t>(jr_address));
	}

	void cpu_jrzn() {
		core_tick();
		uint8_t jr_address = ld_1_byte();
		cpu.jr((cpu.zf), static_cast<int8_t>(jr_address));
	}

	void cpu_jrcn() {
		core_tick();
		uint8_t jr_address = ld_1_byte();
		cpu.jr((cpu.cf), static_cast<int8_t>(jr_address));
	}

	void cpu_jrnzn() {
		core_tick();
		uint8_t jr_address = ld_1_byte();
		cpu.jr(!cpu.zf, static_cast<int8_t>(jr_address));
	}

	void cpu_jrncn() {
		core_tick();
		uint8_t jr_address = ld_1_byte();
		cpu.jr(!cpu.cf, static_cast<int8_t>(jr_address));
	}

	void cpu_jpnn() {
		core_tick();
		uint16_t jp_address = ld_2_bytes();
		cpu.jp(true, jp_address);
	}

	void cpu_jphl() {
		cpu.jp(true, cpu.hl);
	}

	void cpu_jpznn() {
		core_tick();
		uint16_t jp_address = ld_2_bytes();
		cpu.jp((cpu.zf), jp_address);
	}

	void cpu_jpcnn() {
		core_tick();
		uint16_t jp_address = ld_2_bytes();
		cpu.jp((cpu.cf), jp_address);
	}

	void cpu_jpnznn() {
		core_tick();
		uint16_t jp_address = ld_2_bytes();
		cpu.jp(!cpu.zf , jp_address);
	}

	void cpu_jpncnn() {
		core_tick();
		uint16_t jp_address = ld_2_bytes();
		cpu.jp(!cpu.cf, jp_address);
	}




	void cpu_popbc() {
		cpu.pop(cpu.c, cpu.b);
	}
	void cpu_popde() {
		cpu.pop(cpu.e, cpu.d);
	}
	void cpu_pophl() {
		cpu.pop(cpu.l, cpu.h);
	}
	void cpu_popaf() {
		uint8_t num;
		cpu.pop(num, cpu.a);

		cpu.cf = (num >> 4) & 1;
		cpu.hf = (num >> 5) & 1;
		cpu.nf = (num >> 6) & 1;
		cpu.zf = (num >> 7) & 1;
	}

	void cpu_ret() {
		cpu.ret(true);
	}
	void cpu_retnz() {
		core_tick();
		cpu.ret(!cpu.zf);
	}
	void cpu_retnc() {
		core_tick();
		cpu.ret(!cpu.cf);
	}
	void cpu_retz() {
		core_tick();
		cpu.ret(cpu.zf);
	}
	void cpu_retc() {
		core_tick();
		cpu.ret(cpu.cf);
	}
	void cpu_reti() {
		cpu.ret(true);
		cpu.ime_flag = true;
	}













	void cpu_pushbc() {
		cpu.push(cpu.b, cpu.c);
	}
	void cpu_pushde() {
		cpu.push(cpu.d, cpu.e);
	}
	void cpu_pushhl() {
		cpu.push(cpu.h, cpu.l);
	}
	void cpu_pushaf() {
		uint8_t num = 0;
		num += cpu.zf;
		num += cpu.nf << 1;
		num += cpu.hf << 2;
		num += cpu.cf << 3;

		cpu.push(cpu.a, num);

	}


	void cpu_callnn() {
		uint16_t jp_address = ld_2_bytes();
		cpu.call(true, jp_address);
	}

	void cpu_callznn() {
		uint16_t jp_address = ld_2_bytes();
		cpu.call((cpu.zf), jp_address);
	}

	void cpu_callcnn() {
		uint16_t jp_address = ld_2_bytes();
		cpu.call((cpu.cf), jp_address);
	}

	void cpu_callnznn() {
		uint16_t jp_address = ld_2_bytes();
		cpu.call(!cpu.zf, jp_address);
	}

	void cpu_callncnn() {
		uint16_t jp_address = ld_2_bytes();
		cpu.call(!cpu.cf, jp_address);
	}

	void cpu_rst00() {
		cpu.call(true, 0x0000);
	}
	void cpu_rst10() {
		cpu.call(true, 0x0010);
	}
	void cpu_rst20() {
		cpu.call(true, 0x0020);
	}
	void cpu_rst30() {
		cpu.call(true, 0x0030);
	}
	void cpu_rst08() {
		cpu.call(true, 0x0008);
	}
	void cpu_rst18() {
		cpu.call(true, 0x0018);
	}
	void cpu_rst28() {
		cpu.call(true, 0x0028);
	}
	void cpu_rst38() {
		cpu.call(true, 0x0038);
	}


	void cpu_di() {
		core_tick();
		cpu.ime_flag = false;
	}

	void cpu_ei() {
		core_tick();
		cpu.ei_counter = 1;
	}

	void cpu_scf() {
		core_tick();
		cpu.nf = 0;
		cpu.hf = 0;
		cpu.cf = 1;
	}

	void cpu_daa() {
		core_tick();

		if (cpu.nf) {
			uint8_t adj = 0;
			if (cpu.hf) {adj += 0x6;}
			if (cpu.cf) {adj += 0x66;}

			cpu.a -= adj;
			if (cpu.a) {cpu.zf = 0;}
		}

		else {
			uint8_t adj = 0;
			if (cpu.hf || ((cpu.a & 0xF) > 0x9)) {adj += 0x6;}
			if (cpu.cf || (cpu.a > 0x99)) {
				adj += 0x60;
				cpu.cf = 1;
			}

			cpu.a += adj;
			if (cpu.a) {cpu.zf = 0;}

		}

		cpu.hf = 0;

	}

	void cpu_rlca() {
		cpu.rlc(cpu.a);
		cpu.zf = 0;
	}

	void cpu_rla() {
		cpu.rl(cpu.a);
		cpu.zf = 0;
	}

	void cpu_rrca() {
		cpu.rrc(cpu.a);
		cpu.zf = 0;
	}

	void cpu_rra() {
		cpu.rr(cpu.a);
		cpu.zf = 0;
	}


	void cpu_prefix() {
		core_tick();
		cpu.use_extended_opcodes = true;
	}


// Instructions

// Extended
	void cpu_rlcb() {
		core_tick();
		cpu.rlc(cpu.b);
	}
	void cpu_rlcc() {
		core_tick();
		cpu.rlc(cpu.c);
	}
	void cpu_rlcd() {
		core_tick();
		cpu.rlc(cpu.d);
	}
	void cpu_rlce() {
		core_tick();
		cpu.rlc(cpu.e);
	}
	void cpu_rlch() {
		core_tick();
		cpu.rlc(cpu.h);
	}
	void cpu_rlcl() {
		core_tick();
		cpu.rlc(cpu.l);
	}
	void cpu_rlcHL() {
		core_tick();
		uint8_t num = read_mem(cpu.hl);
		cpu.rlc(num);
		write_mem(cpu.hl, num);
	}
	void cpu_rlc_a() {
		core_tick();
		cpu.rlc(cpu.a);
	}

	void cpu_rlb() {
		core_tick();
		cpu.rl(cpu.b);
	}
	void cpu_rlc() {
		core_tick();
		cpu.rl(cpu.c);
	}
	void cpu_rld() {
		core_tick();
		cpu.rl(cpu.d);
	}
	void cpu_rle() {
		core_tick();
		cpu.rl(cpu.e);
	}
	void cpu_rlh() {
		core_tick();
		cpu.rl(cpu.h);
	}
	void cpu_rll() {
		core_tick();
		cpu.rl(cpu.l);
	}
	void cpu_rlHL() {
		core_tick();
		uint8_t num = read_mem(cpu.hl);
		cpu.rl(num);
		write_mem(cpu.hl, num);
	}
	void cpu_rl_a() {
		core_tick();
		cpu.rl(cpu.a);
	}

	void cpu_rrcb() {
		core_tick();
		cpu.rrc(cpu.b);
	}
	void cpu_rrcc() {
		core_tick();
		cpu.rrc(cpu.c);
	}
	void cpu_rrcd() {
		core_tick();
		cpu.rrc(cpu.d);
	}
	void cpu_rrce() {
		core_tick();
		cpu.rrc(cpu.e);
	}
	void cpu_rrch() {
		core_tick();
		cpu.rrc(cpu.h);
	}
	void cpu_rrcl() {
		core_tick();
		cpu.rrc(cpu.l);
	}
	void cpu_rrcHL() {
		core_tick();
		uint8_t num = read_mem(cpu.hl);
		cpu.rrc(num);
		write_mem(cpu.hl, num);
	}
	void cpu_rrc_a() {
		core_tick();
		cpu.rrc(cpu.a);
	}

	void cpu_rrb() {
		core_tick();
		cpu.rr(cpu.b);
	}
	void cpu_rrc() {
		core_tick();
		cpu.rr(cpu.c);
	}
	void cpu_rrd() {
		core_tick();
		cpu.rr(cpu.d);
	}
	void cpu_rre() {
		core_tick();
		cpu.rr(cpu.e);
	}
	void cpu_rrh() {
		core_tick();
		cpu.rr(cpu.h);
	}
	void cpu_rrl() {
		core_tick();
		cpu.rr(cpu.l);
	}
	void cpu_rrHL() {
		core_tick();
		uint8_t num = read_mem(cpu.hl);
		cpu.rr(num);
		write_mem(cpu.hl, num);
	}
	void cpu_rr_a() {
		core_tick();
		cpu.rr(cpu.a);
	}

	void cpu_swapb() {
		cpu.swap(cpu.b);
	}
	void cpu_swapc() {
		cpu.swap(cpu.c);
	}
	void cpu_swapd() {
		cpu.swap(cpu.d);
	}
	void cpu_swape() {
		cpu.swap(cpu.e);
	}
	void cpu_swaph() {
		cpu.rr(cpu.h);
	}
	void cpu_swapl() {
		cpu.swap(cpu.l);
	}
	void cpu_swapHL() {
		uint8_t num = read_mem(cpu.hl);
		cpu.swap(num);
		write_mem(cpu.hl, num);
	}
	void cpu_swapa() {
		core_tick();
		cpu.swap(cpu.a);
	}

	void cpu_res0b() {
		cpu.res(0, cpu.b);
	}
	void cpu_res0c() {
		cpu.res(0, cpu.c);
	}
	void cpu_res0d() {
		cpu.res(0, cpu.d);
	}
	void cpu_res0e() {
		cpu.res(0, cpu.e);
	}
	void cpu_res0h() {
		cpu.res(0, cpu.h);
	}
	void cpu_res0l() {
		cpu.res(0, cpu.l);
	}
	void cpu_res0HL() {
		uint8_t num = read_mem(cpu.hl);
		cpu.res(0, num);
		write_mem(cpu.hl, num);
	}
	void cpu_res0a() {
		core_tick();
		cpu.res(0, cpu.a);
	}

	void cpu_res1b() {
		cpu.res(1, cpu.b);
	}
	void cpu_res1c() {
		cpu.res(1, cpu.c);
	}
	void cpu_res1d() {
		cpu.res(1, cpu.d);
	}
	void cpu_res1e() {
		cpu.res(1, cpu.e);
	}
	void cpu_res1h() {
		cpu.res(1, cpu.h);
	}
	void cpu_res1l() {
		cpu.res(1, cpu.l);
	}
	void cpu_res1HL() {
		uint8_t num = read_mem(cpu.hl);
		cpu.res(1, num);
		write_mem(cpu.hl, num);
	}
	void cpu_res1a() {
		core_tick();
		cpu.res(1, cpu.a);
	}

	void cpu_res2b() {
		cpu.res(2, cpu.b);
	}
	void cpu_res2c() {
		cpu.res(2, cpu.c);
	}
	void cpu_res2d() {
		cpu.res(2, cpu.d);
	}
	void cpu_res2e() {
		cpu.res(2, cpu.e);
	}
	void cpu_res2h() {
		cpu.res(2, cpu.h);
	}
	void cpu_res2l() {
		cpu.res(2, cpu.l);
	}
	void cpu_res2HL() {
		uint8_t num = read_mem(cpu.hl);
		cpu.res(2, num);
		write_mem(cpu.hl, num);
	}
	void cpu_res2a() {
		core_tick();
		cpu.res(2, cpu.a);
	}

	void cpu_res3b() {
		cpu.res(3, cpu.b);
	}
	void cpu_res3c() {
		cpu.res(3, cpu.c);
	}
	void cpu_res3d() {
		cpu.res(3, cpu.d);
	}
	void cpu_res3e() {
		cpu.res(3, cpu.e);
	}
	void cpu_res3h() {
		cpu.res(3, cpu.h);
	}
	void cpu_res3l() {
		cpu.res(3, cpu.l);
	}
	void cpu_res3HL() {
		uint8_t num = read_mem(cpu.hl);
		cpu.res(3, num);
		write_mem(cpu.hl, num);
	}
	void cpu_res3a() {
		core_tick();
		cpu.res(3, cpu.a);
	}

	void cpu_res4b() {
		cpu.res(4, cpu.b);
	}
	void cpu_res4c() {
		cpu.res(4, cpu.c);
	}
	void cpu_res4d() {
		cpu.res(4, cpu.d);
	}
	void cpu_res4e() {
		cpu.res(4, cpu.e);
	}
	void cpu_res4h() {
		cpu.res(4, cpu.h);
	}
	void cpu_res4l() {
		cpu.res(4, cpu.l);
	}
	void cpu_res4HL() {
		uint8_t num = read_mem(cpu.hl);
		cpu.res(4, num);
		write_mem(cpu.hl, num);
	}
	void cpu_res4a() {
		core_tick();
		cpu.res(4, cpu.a);
	}

	void cpu_res5b() {
		cpu.res(5, cpu.b);
	}
	void cpu_res5c() {
		cpu.res(5, cpu.c);
	}
	void cpu_res5d() {
		cpu.res(5, cpu.d);
	}
	void cpu_res5e() {
		cpu.res(5, cpu.e);
	}
	void cpu_res5h() {
		cpu.res(5, cpu.h);
	}
	void cpu_res5l() {
		cpu.res(5, cpu.l);
	}
	void cpu_res5HL() {
		uint8_t num = read_mem(cpu.hl);
		cpu.res(5, num);
		write_mem(cpu.hl, num);
	}
	void cpu_res5a() {
		core_tick();
		cpu.res(5, cpu.a);
	}

	void cpu_res6b() {
		cpu.res(6, cpu.b);
	}
	void cpu_res6c() {
		cpu.res(6, cpu.c);
	}
	void cpu_res6d() {
		cpu.res(6, cpu.d);
	}
	void cpu_res6e() {
		cpu.res(6, cpu.e);
	}
	void cpu_res6h() {
		cpu.res(6, cpu.h);
	}
	void cpu_res6l() {
		cpu.res(6, cpu.l);
	}
	void cpu_res6HL() {
		uint8_t num = read_mem(cpu.hl);
		cpu.res(6, num);
		write_mem(cpu.hl, num);
	}
	void cpu_res6a() {
		core_tick();
		cpu.res(6, cpu.a);
	}

	void cpu_res7b() {
		cpu.res(7, cpu.b);
	}
	void cpu_res7c() {
		cpu.res(7, cpu.c);
	}
	void cpu_res7d() {
		cpu.res(7, cpu.d);
	}
	void cpu_res7e() {
		cpu.res(7, cpu.e);
	}
	void cpu_res7h() {
		cpu.res(7, cpu.h);
	}
	void cpu_res7l() {
		cpu.res(7, cpu.l);
	}
	void cpu_res7HL() {
		uint8_t num = read_mem(cpu.hl);
		cpu.res(7, num);
		write_mem(cpu.hl, num);
	}
	void cpu_res7a() {
		core_tick();
		cpu.res(7, cpu.a);
	}


	void cpu_bit0b() {
		cpu.bit(0, cpu.b);
	}
	void cpu_bit0c() {
		cpu.bit(0, cpu.c);
	}
	void cpu_bit0d() {
		cpu.bit(0, cpu.d);
	}
	void cpu_bit0e() {
		cpu.bit(0, cpu.e);
	}
	void cpu_bit0h() {
		cpu.bit(0, cpu.h);
	}
	void cpu_bit0l() {
		cpu.bit(0, cpu.l);
	}
	void cpu_bit0HL() {
		uint8_t num = read_mem(cpu.hl);
		cpu.bit(0, num);
	}
	void cpu_bit0a() {
		core_tick();
		cpu.bit(0, cpu.a);
	}

	void cpu_bit1b() {
		cpu.bit(1, cpu.b);
	}
	void cpu_bit1c() {
		cpu.bit(1, cpu.c);
	}
	void cpu_bit1d() {
		cpu.bit(1, cpu.d);
	}
	void cpu_bit1e() {
		cpu.bit(1, cpu.e);
	}
	void cpu_bit1h() {
		cpu.bit(1, cpu.h);
	}
	void cpu_bit1l() {
		cpu.bit(1, cpu.l);
	}
	void cpu_bit1HL() {
		uint8_t num = read_mem(cpu.hl);
		cpu.bit(1, num);
	}
	void cpu_bit1a() {
		core_tick();
		cpu.bit(1, cpu.a);
	}

	void cpu_bit2b() {
		cpu.bit(2, cpu.b);
	}
	void cpu_bit2c() {
		cpu.bit(2, cpu.c);
	}
	void cpu_bit2d() {
		cpu.bit(2, cpu.d);
	}
	void cpu_bit2e() {
		cpu.bit(2, cpu.e);
	}
	void cpu_bit2h() {
		cpu.bit(2, cpu.h);
	}
	void cpu_bit2l() {
		cpu.bit(2, cpu.l);
	}
	void cpu_bit2HL() {
		uint8_t num = read_mem(cpu.hl);
		cpu.bit(2, num);
	}
	void cpu_bit2a() {
		core_tick();
		cpu.bit(2, cpu.a);
	}

	void cpu_bit3b() {
		cpu.bit(3, cpu.b);
	}
	void cpu_bit3c() {
		cpu.bit(3, cpu.c);
	}
	void cpu_bit3d() {
		cpu.bit(3, cpu.d);
	}
	void cpu_bit3e() {
		cpu.bit(3, cpu.e);
	}
	void cpu_bit3h() {
		cpu.bit(3, cpu.h);
	}
	void cpu_bit3l() {
		cpu.bit(3, cpu.l);
	}
	void cpu_bit3HL() {
		uint8_t num = read_mem(cpu.hl);
		cpu.bit(3, num);
	}
	void cpu_bit3a() {
		core_tick();
		cpu.bit(3, cpu.a);
	}

	void cpu_bit4b() {
		cpu.bit(4, cpu.b);
	}
	void cpu_bit4c() {
		cpu.bit(4, cpu.c);
	}
	void cpu_bit4d() {
		cpu.bit(4, cpu.d);
	}
	void cpu_bit4e() {
		cpu.bit(4, cpu.e);
	}
	void cpu_bit4h() {
		cpu.bit(4, cpu.h);
	}
	void cpu_bit4l() {
		cpu.bit(4, cpu.l);
	}
	void cpu_bit4HL() {
		uint8_t num = read_mem(cpu.hl);
		cpu.bit(4, num);
	}
	void cpu_bit4a() {
		core_tick();
		cpu.bit(4, cpu.a);
	}

	void cpu_bit5b() {
		cpu.bit(5, cpu.b);
	}
	void cpu_bit5c() {
		cpu.bit(5, cpu.c);
	}
	void cpu_bit5d() {
		cpu.bit(5, cpu.d);
	}
	void cpu_bit5e() {
		cpu.bit(5, cpu.e);
	}
	void cpu_bit5h() {
		cpu.bit(5, cpu.h);
	}
	void cpu_bit5l() {
		cpu.bit(5, cpu.l);
	}
	void cpu_bit5HL() {
		uint8_t num = read_mem(cpu.hl);
		cpu.bit(5, num);
	}
	void cpu_bit5a() {
		core_tick();
		cpu.bit(5, cpu.a);
	}

	void cpu_bit6b() {
		cpu.bit(6, cpu.b);
	}
	void cpu_bit6c() {
		cpu.bit(6, cpu.c);
	}
	void cpu_bit6d() {
		cpu.bit(6, cpu.d);
	}
	void cpu_bit6e() {
		cpu.bit(6, cpu.e);
	}
	void cpu_bit6h() {
		cpu.bit(6, cpu.h);
	}
	void cpu_bit6l() {
		cpu.bit(6, cpu.l);
	}
	void cpu_bit6HL() {
		uint8_t num = read_mem(cpu.hl);
		cpu.bit(6, num);
	}
	void cpu_bit6a() {
		core_tick();
		cpu.bit(6, cpu.a);
	}

	void cpu_bit7b() {
		cpu.bit(7, cpu.b);
	}
	void cpu_bit7c() {
		cpu.bit(7, cpu.c);
	}
	void cpu_bit7d() {
		cpu.bit(7, cpu.d);
	}
	void cpu_bit7e() {
		cpu.bit(7, cpu.e);
	}
	void cpu_bit7h() {
		cpu.bit(7, cpu.h);
	}
	void cpu_bit7l() {
		cpu.bit(7, cpu.l);
	}
	void cpu_bit7HL() {
		uint8_t num = read_mem(cpu.hl);
		cpu.bit(7, num);
	}
	void cpu_bit7a() {
		core_tick();
		cpu.bit(7, cpu.a);
	}




struct cpu_instruction {
	const char* name;
	int len;
	cpu_execute_func execute;
};

}


static cpu_instruction cpu_instructions[256] = {
	{"NOP", 0, cpu_noop}, // 0x00
	{"LD BC nn", 2, cpu_ldbcnn}, // 0x01
	{"LD [BC] A", 0, cpu_ldBCa}, // 0x02
	{"INC BC", 0, cpu_incbc}, // 0x03
	{"INC B", 0, cpu_incb}, // 0x04
	{"DEC B", 0, cpu_decb}, // 0x05
	{"LD B n", 1, cpu_ldbn}, // 0x06
	{"RLCA", 0, cpu_rlca}, // 0x07
	{"LD [nn] SP", 2, cpu_ldNNsp}, // 0x08
	{"ADD HL BC", 0, cpu_addhlbc}, // 0x09
	{"LD A [BC]", 0, cpu_ldaBC}, // 0x0A
	{"DEC BC", 0, cpu_decbc}, // 0x0B
	{"INC C", 0, cpu_incc}, // 0x0C
	{"DEC C", 0, cpu_decc}, // 0x0D
	{"LD C n", 1, cpu_ldcn}, // 0x0E
	{"RRCA", 0, cpu_rrca}, // 0x0F
	{"STOP n?", 1, nullptr}, // 0x10
	{"LD DE nn", 2, cpu_lddenn}, // 0x11
	{"LD [DE] A", 0, cpu_ldDEa}, // 0x12
	{"INC DE", 0, cpu_incde}, // 0x13
	{"INC D", 0, cpu_incd}, // 0x14
	{"DEC D", 0, cpu_decd}, // 0x15
	{"LD D n", 1, cpu_lddn}, // 0x16
	{"RLA", 0, cpu_rla}, // 0x17
	{"JR nn", 2, cpu_jrn}, // 0x18
	{"ADD HL DE", 0, cpu_addhlde}, // 0x19
	{"LD A [DE]", 0, cpu_ldaDE}, // 0x1A
	{"DEC DE", 0, cpu_decde}, // 0x1B
	{"INC E", 0, cpu_ince}, // 0x1C
	{"DEC E", 0, cpu_dece}, // 0x1D
	{"LD E n", 1, cpu_lden}, // 0x1E
	{"RRA", 0, cpu_rra}, // 0x1F
	{"JR NZ n", 1, cpu_jrnzn}, // 0x20
	{"LD HL nn", 2, cpu_ldhlnn}, // 0x21
	{"LD [HL+] A", 0, cpu_ldHLIa}, // 0x22
	{"INC HL", 0, cpu_inchl}, // 0x23
	{"INC H", 0, cpu_inch}, // 0x24
	{"DEC H", 0, cpu_dech}, // 0x25
	{"LD H n", 1, cpu_ldhn}, // 0x26
	{"DAA", 0, cpu_daa}, // 0x27
	{"JR Z nn", 2, cpu_jrzn}, // 0x28
	{"ADD HL HL", 0, cpu_addhlhl}, // 0x29
	{"LD A [HL+]", 0, cpu_ldaHLI}, // 0x2A
	{"DEC HL", 0, cpu_dechl}, // 0x2B
	{"INC L", 0, cpu_incl}, // 0x2C
	{"DEC L", 0, cpu_decl}, // 0x2D
	{"LD L n", 1, cpu_ldln}, // 0x2E
	{"CPL", 0, cpu_cpl}, // 0x2F
	{"JR NC n", 1, cpu_jrncn}, // 0x30
	{"LD SP nn", 2, cpu_ldspnn}, // 0x31
	{"LD [HL-] A", 0, cpu_ldHLDa}, // 0x32
	{"INC SP", 0, cpu_incsp}, // 0x33
	{"INC [HL]", 0, cpu_incHL}, // 0x34
	{"DEC [HL]", 0, cpu_decHL}, // 0x35
	{"LD [HL] n", 1, cpu_ldHLn}, // 0x36
	{"SCF", 0, cpu_scf}, // 0x37
	{"JR C nn", 2, cpu_jrcn}, // 0x38
	{"ADD HL SP", 0, cpu_addhlsp}, // 0x39
	{"LD A [HL-]", 0, cpu_ldaHLD}, // 0x3A
	{"DEC SP", 0, cpu_decsp}, // 0x3B
	{"INC A", 0, cpu_inca}, // 0x3C
	{"DEC A", 0, cpu_deca}, // 0x3D
	{"LD A n", 1, cpu_ldan}, // 0x3E
	{"CCF", 0, cpu_ccf}, // 0x3F
	{"LD B B", 0, cpu_ldbb}, // 0x40
	{"LD B C", 0, cpu_ldbc}, // 0x41
	{"LD B D", 0, cpu_ldbd}, // 0x42
	{"LD B E", 0, cpu_ldbe}, // 0x43
	{"LD B H", 0, cpu_ldbh}, // 0x44
	{"LD B L", 0, cpu_ldbl}, // 0x45
	{"LD B [HL]", 0, cpu_ldbHL}, // 0x46
	{"LD B A", 0, cpu_ldba}, // 0x47
	{"LD C B", 0, cpu_ldcb}, // 0x48
	{"LD C C", 0, cpu_ldcc}, // 0x49
	{"LD C D", 0, cpu_ldcd}, // 0x4A
	{"LD C E", 0, cpu_ldce}, // 0x4B
	{"LD C H", 0, cpu_ldch}, // 0x4C
	{"LD C L", 0, cpu_ldcl}, // 0x4D
	{"LD C [HL]", 0, cpu_ldcHL}, // 0x4E
	{"LD C A", 0, cpu_ldca}, // 0x4F
	{"LD D B", 0, cpu_lddb}, // 0x50
	{"LD D C", 0, cpu_lddc}, // 0x51
	{"LD D D", 0, cpu_lddd}, // 0x52
	{"LD D E", 0, cpu_ldde}, // 0x53
	{"LD D H", 0, cpu_lddh}, // 0x54
	{"LD D L", 0, cpu_lddl}, // 0x55
	{"LD D [HL]", 0, cpu_lddHL}, // 0x56
	{"LD D A", 0, cpu_ldda}, // 0x57
	{"LD E B", 0, cpu_ldeb}, // 0x58
	{"LD E C", 0, cpu_ldec}, // 0x59
	{"LD E D", 0, cpu_lded}, // 0x5A
	{"LD E E", 0, cpu_ldee}, // 0x5B
	{"LD E H", 0, cpu_ldeh}, // 0x5C
	{"LD E L", 0, cpu_ldel}, // 0x5D
	{"LD E [HL]", 0, cpu_ldeHL}, // 0x5E
	{"LD E A", 0, cpu_ldea}, // 0x5F
	{"LD H B", 0, cpu_ldhb}, // 0x60
	{"LD H C", 0, cpu_ldhc}, // 0x61
	{"LD H D", 0, cpu_ldhd}, // 0x62
	{"LD H E", 0, cpu_ldhe}, // 0x63
	{"LD H H", 0, cpu_ldhh}, // 0x64
	{"LD H L", 0, cpu_ldhl}, // 0x65
	{"LD H [HL]", 0, cpu_ldhHL}, // 0x66
	{"LD H A", 0, cpu_ldha}, // 0x67
	{"LD L B", 0, cpu_ldlb}, // 0x68
	{"LD L C", 0, cpu_ldlc}, // 0x69
	{"LD L D", 0, cpu_ldld}, // 0x6A
	{"LD L E", 0, cpu_ldle}, // 0x6B
	{"LD L H", 0, cpu_ldlh}, // 0x6C
	{"LD L L", 0, cpu_ldll}, // 0x6D
	{"LD L [HL]", 0, cpu_ldlHL}, // 0x6E
	{"LD L A", 0, cpu_ldla}, // 0x6F
	{ "LD [HL] B", 0, cpu_ldHLb }, // 0x70
	{ "LD [HL] C", 0, cpu_ldHLc }, // 0x71
	{ "LD [HL] D", 0, cpu_ldHLd }, // 0x72
	{ "LD [HL] E", 0, cpu_ldHLe }, // 0x73
	{ "LD [HL] H", 0, cpu_ldHLh }, // 0x74
	{ "LD [HL] L", 0, cpu_ldHLl }, // 0x75
	{ "HALT", 0, cpu_halt}, // 0x76
	{ "LD [HL] A", 0, cpu_ldHLa }, // 0x77
	{ "LD A B", 0, cpu_ldab }, // 0x78
	{ "LD A C", 0, cpu_ldac }, // 0x79
	{ "LD A D", 0, cpu_ldad }, // 0x7A
	{ "LD A E", 0, cpu_ldae }, // 0x7B
	{ "LD A H", 0, cpu_ldah }, // 0x7C
	{ "LD A L", 0, cpu_ldal }, // 0x7D
	{ "LD A [HL]", 0, cpu_ldaHL }, // 0x7E
	{ "LD A A", 0, cpu_ldaa }, // 0x7F
	{ "ADD A B", 0, cpu_addab }, // 0x80
	{ "ADD A C", 0, cpu_addac }, // 0x81
	{ "ADD A D", 0, cpu_addad }, // 0x82
	{ "ADD A E", 0, cpu_addae }, // 0x83
	{ "ADD A H", 0, cpu_addah }, // 0x84
	{ "ADD A L", 0, cpu_addal }, // 0x85
	{ "ADD A [HL]", 0, cpu_addaHL }, // 0x86
	{ "ADD A A", 0, cpu_addaa }, // 0x87
	{ "ADC A B", 0, cpu_adcab }, // 0x88
	{ "ADC A C", 0, cpu_adcac }, // 0x89
	{ "ADC A D", 0, cpu_adcad }, // 0x8A
	{ "ADC A E", 0, cpu_adcae }, // 0x8B
	{ "ADC A H", 0, cpu_adcah }, // 0x8C
	{ "ADC A L", 0, cpu_adcal }, // 0x8D
	{ "ADC A [HL]", 0, cpu_adcaHL }, // 0x8E
	{ "ADC A A", 0, cpu_adcaa }, // 0x8F
	{ "SUB A B", 0, cpu_subab }, // 0x90
	{ "SUB A C", 0, cpu_subac }, // 0x91
	{ "SUB A D", 0, cpu_subad }, // 0x92
	{ "SUB A E", 0, cpu_subae }, // 0x93
	{ "SUB A H", 0, cpu_subah }, // 0x94
	{ "SUB A L", 0, cpu_subal }, // 0x95
	{ "SUB A [HL]", 0, cpu_subaHL }, // 0x96
	{ "SUB A A", 0, cpu_subaa }, // 0x97
	{ "SBC A B", 0, cpu_sbcab }, // 0x98
	{ "SBC A C", 0, cpu_sbcac }, // 0x99
	{ "SBC A D", 0, cpu_sbcad }, // 0x9A
	{ "SBC A E", 0, cpu_sbcae }, // 0x9B
	{ "SBC A H", 0, cpu_sbcah }, // 0x9C
	{ "SBC A L", 0, cpu_sbcal }, // 0x9D
	{ "SBC A [HL]", 0, cpu_sbcaHL }, // 0x9E
	{ "SBC A A", 0, cpu_sbcaa }, // 0x9F
	{ "AND A B", 0, cpu_andab }, // 0xA0
	{ "AND A C", 0, cpu_andac }, // 0xA1
	{ "AND A D", 0, cpu_andad }, // 0xA2
	{ "AND A E", 0, cpu_andae }, // 0xA3
	{ "AND A H", 0, cpu_andah }, // 0xA4
	{ "AND A L", 0, cpu_andal }, // 0xA5
	{ "AND A [HL]", 0, cpu_andaHL }, // 0xA6
	{ "AND A A", 0, cpu_andaa }, // 0xA7
	{ "XOR A B", 0, cpu_xorab }, // 0xA8
	{ "XOR A C", 0, cpu_xorac }, // 0xA9
	{ "XOR A D", 0, cpu_xorad }, // 0xAA
	{ "XOR A E", 0, cpu_xorae }, // 0xAB
	{ "XOR A H", 0, cpu_xorah }, // 0xAC
	{ "XOR A L", 0, cpu_xoral }, // 0xAD
	{ "XOR A [HL]", 0, cpu_xoraHL }, // 0xAE
	{ "XOR A A", 0, cpu_xoraa }, // 0xAF
	{ "OR A B", 0, cpu_orab }, // 0xB0
	{ "OR A C", 0, cpu_orac }, // 0xB1
	{ "OR A D", 0, cpu_orad }, // 0xB2
	{ "OR A E", 0, cpu_orae }, // 0xB3
	{ "OR A H", 0, cpu_orah }, // 0xB4
	{ "OR A L", 0, cpu_oral }, // 0xB5
	{ "OR A [HL]", 0, cpu_oraHL }, // 0xB6
	{ "OR A A", 0, cpu_oraa }, // 0xB7
	{ "CP A B", 0, cpu_cpab }, // 0xB8
	{ "CP A C", 0, cpu_cpac }, // 0xB9
	{ "CP A D", 0, cpu_cpad }, // 0xBA
	{ "CP A E", 0, cpu_cpae }, // 0xBB
	{ "CP A H", 0, cpu_cpah }, // 0xBC
	{ "CP A L", 0, cpu_cpal }, // 0xBD
	{ "CP A [HL]", 0, cpu_cpaHL }, // 0xBE
	{ "CP A A", 0, cpu_cpaa }, // 0xBF
	{ "RET NZ", 0, cpu_retnz }, // 0xC0
	{ "POP BC", 0, cpu_popbc }, // 0xC1
	{ "JP NZ nn", 2, cpu_jpnznn }, // 0xC2
	{ "JP nn", 2, cpu_jpnn }, // 0xC3
	{ "CALL NZ nn", 2, cpu_callnznn }, // 0xC4
	{ "PUSH BC", 0, cpu_pushbc }, // 0xC5
	{ "ADD A n", 1, cpu_addan }, // 0xC6
	{ "RST 00", 0, cpu_rst00 }, // 0xC7
	{ "RET Z", 0, cpu_retz }, // 0xC8
	{ "RET", 0, cpu_ret }, // 0xC9
	{ "JP Z nn", 2, cpu_jpznn }, // 0xCA
	{ "PREFIX", 0, cpu_prefix }, // 0xCB
	{ "CALL Z nn", 2, cpu_callznn }, // 0xCC
	{ "CALL nn", 2, cpu_callnn }, // 0xCD
	{ "ADC A n", 1, cpu_adcan }, // 0xCE
	{ "RST 08", 0, cpu_rst08 }, // 0xCF
	{ "RET NC", 0, cpu_retnc }, // 0xD0
	{ "POP DE", 0, cpu_popde }, // 0xD1
	{ "JP NC nn", 2, cpu_jpncnn }, // 0xD2
	{ "???", 0, nullptr }, // 0xD3
	{ "CALL NC nn", 2, cpu_callncnn }, // 0xD4
	{ "PUSH DE", 0, cpu_pushde }, // 0xD5
	{ "SUB A n", 1, cpu_suban }, // 0xD6
	{ "RST 10", 0, cpu_rst10 }, // 0xD7
	{ "RET C", 0, cpu_retc }, // 0xD8
	{ "RETI", 0, cpu_reti }, // 0xD9
	{ "JP C nn", 2, cpu_jpcnn }, // 0xDA
	{ "???", 0, nullptr }, // 0xDB
	{ "CALL C nn", 2, cpu_callcnn }, // 0xDC
	{ "???", 0, nullptr }, // 0xDD
	{ "SBC A n", 1, cpu_sbcan }, // 0xDE
	{ "RST 18", 0, cpu_rst18 }, // 0xDF
	{ "LD [n] A", 1, cpu_ldhNa }, // 0xE0
	{ "POP HL", 0, cpu_pophl }, // 0xE1
	{ "LD [C] A", 0, cpu_ldhCa }, // 0xE2
	{ "???", 0, nullptr }, // 0xE3
	{ "???", 0, nullptr }, // 0xE4
	{ "PUSH HL", 0, cpu_pushhl }, // 0xE5
	{ "AND A n", 1, cpu_andan }, // 0xE6
	{ "RST 20", 0, cpu_rst20 }, // 0xE7
	{ "ADD SP n", 1, cpu_addspn }, // 0xE8
	{ "JP HL", 0, cpu_jphl }, // 0xE9
	{ "LD [nn] A", 2, cpu_ldNNa }, // 0xEA
	{ "???", 0, nullptr }, // 0xEB
	{ "???", 0, nullptr }, // 0xEC
	{ "???", 0, nullptr }, // 0xED
	{ "XOR A n", 1, cpu_xoran }, // 0xEE
	{ "RST 28", 0, cpu_rst28 }, // 0xEF
	{ "LD A [n]", 1, cpu_ldhaN }, // 0xF0
	{ "POP AF", 0, cpu_popaf }, // 0xF1
	{ "LD A [C]", 0, cpu_ldhaC }, // 0xF2
	{ "DI", 0, cpu_di }, // 0xF3
	{ "???", 0, nullptr }, // 0xF4
	{ "PUSH AF", 0, cpu_pushaf }, // 0xF5
	{ "OR A n", 1, cpu_oran }, // 0xF6
	{ "RST 30", 0, cpu_rst30 }, // 0xF7
	{ "LD HL SP+n", 1, cpu_ldhlspn }, // 0xF8
	{ "LD SP HL", 0, cpu_ldsphl }, // 0xF9
	{ "LD A [nn]", 2, cpu_ldaNN }, // 0xFA
	{ "EI", 0, cpu_ei }, // 0xFB
	{ "???", 0, nullptr }, // 0xFC
	{ "???", 0, nullptr }, // 0xFD
	{ "CP A n", 1, cpu_cpan }, // 0xFE
	{ "RST 38", 0, cpu_rst38 }, // 0xFF
};

static cpu_instruction cpu_instructions_extended [256] = {
	{ "RLC B", 0, cpu_rlcb }, // 0x00
	{ "RLC C", 0, cpu_rlcc }, // 0x01
	{ "RLC D", 0, cpu_rlcd }, // 0x02
	{ "RLC E", 0, cpu_rlce }, // 0x03
	{ "RLC H", 0, cpu_rlch }, // 0x04
	{ "RLC L", 0, cpu_rlcl }, // 0x05
	{ "RLC [HL]", 0, cpu_rlcHL }, // 0x06
	{ "RLC A", 0, cpu_rlc_a }, // 0x07
	{ "RRC B", 0, cpu_rrcb }, // 0x08
	{ "RRC C", 0, cpu_rrcc }, // 0x09
	{ "RRC D", 0, cpu_rrcd }, // 0x0A
	{ "RRC E", 0, cpu_rrce }, // 0x0B
	{ "RRC H", 0, cpu_rrch }, // 0x0C
	{ "RRC L", 0, cpu_rrcl }, // 0x0D
	{ "RRC [HL]", 0, cpu_rrcHL }, // 0x0E
	{ "RRC A", 0, cpu_rrc_a }, // 0x0F
	{ "RL B", 0, cpu_rlb }, // 0x10
	{ "RL C", 0, cpu_rlc }, // 0x11
	{ "RL D", 0, cpu_rld }, // 0x12
	{ "RL E", 0, cpu_rle }, // 0x13
	{ "RL H", 0, cpu_rlh }, // 0x14
	{ "RL L", 0, cpu_rll }, // 0x15
	{ "RL [HL]", 0, cpu_rlHL }, // 0x16
	{ "RL A", 0, cpu_rl_a }, // 0x17
	{ "RR B", 0, cpu_rrb }, // 0x18
	{ "RR C", 0, cpu_rrc }, // 0x19
	{ "RR D", 0, cpu_rrd }, // 0x1A
	{ "RR E", 0, cpu_rre }, // 0x1B
	{ "RR H", 0, cpu_rrh }, // 0x1C
	{ "RR L", 0, cpu_rrl }, // 0x1D
	{ "RR [HL]", 0, cpu_rrHL }, // 0x1E
	{ "RR A", 0, cpu_rr_a }, // 0x1F
	{ "SLA B", 0, nullptr }, // 0x20
	{ "SLA C", 0, nullptr }, // 0x21
	{ "SLA D", 0, nullptr }, // 0x22
	{ "SLA E", 0, nullptr }, // 0x23
	{ "SLA H", 0, nullptr }, // 0x24
	{ "SLA L", 0, nullptr }, // 0x25
	{ "SLA [HL]", 0, nullptr }, // 0x26
	{ "SLA A", 0, nullptr }, // 0x27
	{ "SRA B", 0, nullptr }, // 0x28
	{ "SRA C", 0, nullptr }, // 0x29
	{ "SRA D", 0, nullptr }, // 0x2A
	{ "SRA E", 0, nullptr }, // 0x2B
	{ "SRA H", 0, nullptr }, // 0x2C
	{ "SRA L", 0, nullptr }, // 0x2D
	{ "SRA [HL]", 0, nullptr }, // 0x2E
	{ "SRA A", 0, nullptr }, // 0x2F
	{ "SWAP B", 0, cpu_swapb }, // 0x30
	{ "SWAP C", 0, cpu_swapc }, // 0x31
	{ "SWAP D", 0, cpu_swapd }, // 0x32
	{ "SWAP E", 0, cpu_swape }, // 0x33
	{ "SWAP H", 0, cpu_swaph }, // 0x34
	{ "SWAP L", 0, cpu_swapl }, // 0x35
	{ "SWAP [HL]", 0, cpu_swapHL }, // 0x36
	{ "SWAP A", 0, cpu_swapa }, // 0x37
	{ "SRL B", 0, nullptr }, // 0x38
	{ "SRL C", 0, nullptr }, // 0x39
	{ "SRL D", 0, nullptr }, // 0x3A
	{ "SRL E", 0, nullptr }, // 0x3B
	{ "SRL H", 0, nullptr }, // 0x3C
	{ "SRL L", 0, nullptr }, // 0x3D
	{ "SRL [HL]", 0, nullptr }, // 0x3E
	{ "SRL A", 0, nullptr }, // 0x3F
	{ "BIT 0 B", 0, cpu_bit0b}, // 0x80
	{ "BIT 0 C", 0, cpu_bit0c }, // 0x81
	{ "BIT 0 D", 0, cpu_bit0d }, // 0x82
	{ "BIT 0 E", 0, cpu_bit0e }, // 0x83
	{ "BIT 0 H", 0, cpu_bit0h }, // 0x84
	{ "BIT 0 L", 0, cpu_bit0l }, // 0x85
	{ "BIT 0 [HL]", 0, cpu_bit0HL }, // 0x86
	{ "BIT 0 A", 0, cpu_bit0a }, // 0x87
	{ "BIT 1 B", 0, cpu_bit1b }, // 0x88
	{ "BIT 1 C", 0, cpu_bit1c }, // 0x89
	{ "BIT 1 D", 0, cpu_bit1d }, // 0x8A
	{ "BIT 1 E", 0, cpu_bit1e }, // 0x8B
	{ "BIT 1 H", 0, cpu_bit1h }, // 0x8C
	{ "BIT 1 L", 0, cpu_bit1l }, // 0x8D
	{ "BIT 1 [HL]", 0, cpu_bit1HL }, // 0x8E
	{ "BIT 1 A", 0, cpu_bit1a }, // 0x8F
	{ "BIT 2 B", 0, cpu_bit2b }, // 0x90
	{ "BIT 2 C", 0, cpu_bit2c }, // 0x91
	{ "BIT 2 D", 0, cpu_bit2d }, // 0x92
	{ "BIT 2 E", 0, cpu_bit2e }, // 0x93
	{ "BIT 2 H", 0, cpu_bit2h }, // 0x94
	{ "BIT 2 L", 0, cpu_bit2l }, // 0x95
	{ "BIT 2 [HL]", 0, cpu_bit2HL }, // 0x96
	{ "BIT 2 A", 0, cpu_bit2a }, // 0x97
	{ "BIT 3 B", 0, cpu_bit3b }, // 0x98
	{ "BIT 3 C", 0, cpu_bit3c }, // 0x99
	{ "BIT 3 D", 0, cpu_bit3d }, // 0x9A
	{ "BIT 3 E", 0, cpu_bit3e }, // 0x9B
	{ "BIT 3 H", 0, cpu_bit3h }, // 0x9C
	{ "BIT 3 L", 0, cpu_bit3l }, // 0x9D
	{ "BIT 3 [HL]", 0, cpu_bit3HL }, // 0x9E
	{ "BIT 3 A", 0, cpu_bit3a }, // 0x9F
	{ "BIT 4 B", 0, cpu_bit4b }, // 0xA0
	{ "BIT 4 C", 0, cpu_bit4c }, // 0xA1
	{ "BIT 4 D", 0, cpu_bit4d }, // 0xA2
	{ "BIT 4 E", 0, cpu_bit4e }, // 0xA3
	{ "BIT 4 H", 0, cpu_bit4h }, // 0xA4
	{ "BIT 4 L", 0, cpu_bit4l }, // 0xA5
	{ "BIT 4 [HL]", 0, cpu_bit4HL }, // 0xA6
	{ "BIT 4 A", 0, cpu_bit4a }, // 0xA7
	{ "BIT 5 B", 0, cpu_bit5b }, // 0xA8
	{ "BIT 5 C", 0, cpu_bit5c }, // 0xA9
	{ "BIT 5 D", 0, cpu_bit5d }, // 0xAA
	{ "BIT 5 E", 0, cpu_bit5e }, // 0xAB
	{ "BIT 5 H", 0, cpu_bit5h }, // 0xAC
	{ "BIT 5 L", 0, cpu_bit5l }, // 0xAD
	{ "BIT 5 [HL]", 0, cpu_bit5HL }, // 0xAE
	{ "BIT 5 A", 0, cpu_bit5a }, // 0xAF
	{ "BIT 6 B", 0, cpu_bit6b }, // 0xB0
	{ "BIT 6 C", 0, cpu_bit6c }, // 0xB1
	{ "BIT 6 D", 0, cpu_bit6d }, // 0xB2
	{ "BIT 6 E", 0, cpu_bit6e }, // 0xB3
	{ "BIT 6 H", 0, cpu_bit6h }, // 0xB4
	{ "BIT 6 L", 0, cpu_bit6l }, // 0xB5
	{ "BIT 6 [HL]", 0, cpu_bit6HL }, // 0xB6
	{ "BIT 6 A", 0, cpu_bit6a }, // 0xB7
	{ "BIT 7 B", 0, cpu_bit7b }, // 0xB8
	{ "BIT 7 C", 0, cpu_bit7c }, // 0xB9
	{ "BIT 7 D", 0, cpu_bit7d }, // 0xBA
	{ "BIT 7 E", 0, cpu_bit7e }, // 0xBB
	{ "BIT 7 H", 0, cpu_bit7h }, // 0xBC
	{ "BIT 7 L", 0, cpu_bit7l }, // 0xBD
	{ "BIT 7 [HL]", 0, cpu_bit7HL }, // 0xBE
	{ "BIT 7 A", 0, cpu_bit7a }, // 0xBF
	{ "RES 0 B", 0, cpu_res0b}, // 0x80
	{ "RES 0 C", 0, cpu_res0c }, // 0x81
	{ "RES 0 D", 0, cpu_res0d }, // 0x82
	{ "RES 0 E", 0, cpu_res0e }, // 0x83
	{ "RES 0 H", 0, cpu_res0h }, // 0x84
	{ "RES 0 L", 0, cpu_res0l }, // 0x85
	{ "RES 0 [HL]", 0, cpu_res0HL }, // 0x86
	{ "RES 0 A", 0, cpu_res0a }, // 0x87
	{ "RES 1 B", 0, cpu_res1b }, // 0x88
	{ "RES 1 C", 0, cpu_res1c }, // 0x89
	{ "RES 1 D", 0, cpu_res1d }, // 0x8A
	{ "RES 1 E", 0, cpu_res1e }, // 0x8B
	{ "RES 1 H", 0, cpu_res1h }, // 0x8C
	{ "RES 1 L", 0, cpu_res1l }, // 0x8D
	{ "RES 1 [HL]", 0, cpu_res1HL }, // 0x8E
	{ "RES 1 A", 0, cpu_res1a }, // 0x8F
	{ "RES 2 B", 0, cpu_res2b }, // 0x90
	{ "RES 2 C", 0, cpu_res2c }, // 0x91
	{ "RES 2 D", 0, cpu_res2d }, // 0x92
	{ "RES 2 E", 0, cpu_res2e }, // 0x93
	{ "RES 2 H", 0, cpu_res2h }, // 0x94
	{ "RES 2 L", 0, cpu_res2l }, // 0x95
	{ "RES 2 [HL]", 0, cpu_res2HL }, // 0x96
	{ "RES 2 A", 0, cpu_res2a }, // 0x97
	{ "RES 3 B", 0, cpu_res3b }, // 0x98
	{ "RES 3 C", 0, cpu_res3c }, // 0x99
	{ "RES 3 D", 0, cpu_res3d }, // 0x9A
	{ "RES 3 E", 0, cpu_res3e }, // 0x9B
	{ "RES 3 H", 0, cpu_res3h }, // 0x9C
	{ "RES 3 L", 0, cpu_res3l }, // 0x9D
	{ "RES 3 [HL]", 0, cpu_res3HL }, // 0x9E
	{ "RES 3 A", 0, cpu_res3a }, // 0x9F
	{ "RES 4 B", 0, cpu_res4b }, // 0xA0
	{ "RES 4 C", 0, cpu_res4c }, // 0xA1
	{ "RES 4 D", 0, cpu_res4d }, // 0xA2
	{ "RES 4 E", 0, cpu_res4e }, // 0xA3
	{ "RES 4 H", 0, cpu_res4h }, // 0xA4
	{ "RES 4 L", 0, cpu_res4l }, // 0xA5
	{ "RES 4 [HL]", 0, cpu_res4HL }, // 0xA6
	{ "RES 4 A", 0, cpu_res4a }, // 0xA7
	{ "RES 5 B", 0, cpu_res5b }, // 0xA8
	{ "RES 5 C", 0, cpu_res5c }, // 0xA9
	{ "RES 5 D", 0, cpu_res5d }, // 0xAA
	{ "RES 5 E", 0, cpu_res5e }, // 0xAB
	{ "RES 5 H", 0, cpu_res5h }, // 0xAC
	{ "RES 5 L", 0, cpu_res5l }, // 0xAD
	{ "RES 5 [HL]", 0, cpu_res5HL }, // 0xAE
	{ "RES 5 A", 0, cpu_res5a }, // 0xAF
	{ "RES 6 B", 0, cpu_res6b }, // 0xB0
	{ "RES 6 C", 0, cpu_res6c }, // 0xB1
	{ "RES 6 D", 0, cpu_res6d }, // 0xB2
	{ "RES 6 E", 0, cpu_res6e }, // 0xB3
	{ "RES 6 H", 0, cpu_res6h }, // 0xB4
	{ "RES 6 L", 0, cpu_res6l }, // 0xB5
	{ "RES 6 [HL]", 0, cpu_res6HL }, // 0xB6
	{ "RES 6 A", 0, cpu_res6a }, // 0xB7
	{ "RES 7 B", 0, cpu_res7b }, // 0xB8
	{ "RES 7 C", 0, cpu_res7c }, // 0xB9
	{ "RES 7 D", 0, cpu_res7d }, // 0xBA
	{ "RES 7 E", 0, cpu_res7e }, // 0xBB
	{ "RES 7 H", 0, cpu_res7h }, // 0xBC
	{ "RES 7 L", 0, cpu_res7l }, // 0xBD
	{ "RES 7 [HL]", 0, cpu_res7HL }, // 0xBE
	{ "RES 7 A", 0, cpu_res7a }, // 0xBF
	{ "SET 0 B", 0, nullptr }, // 0xC0
	{ "SET 0 C", 0, nullptr }, // 0xC1
	{ "SET 0 D", 0, nullptr }, // 0xC2
	{ "SET 0 E", 0, nullptr }, // 0xC3
	{ "SET 0 H", 0, nullptr }, // 0xC4
	{ "SET 0 L", 0, nullptr }, // 0xC5
	{ "SET 0 [HL]", 0, nullptr }, // 0xC6
	{ "SET 0 A", 0, nullptr }, // 0xC7
	{ "SET 1 B", 0, nullptr }, // 0xC8
	{ "SET 1 C", 0, nullptr }, // 0xC9
	{ "SET 1 D", 0, nullptr }, // 0xCA
	{ "SET 1 E", 0, nullptr }, // 0xCB
	{ "SET 1 H", 0, nullptr }, // 0xCC
	{ "SET 1 L", 0, nullptr }, // 0xCD
	{ "SET 1 [HL]", 0, nullptr }, // 0xCE
	{ "SET 1 A", 0, nullptr }, // 0xCF
	{ "SET 2 B", 0, nullptr }, // 0xD0
	{ "SET 2 C", 0, nullptr }, // 0xD1
	{ "SET 2 D", 0, nullptr }, // 0xD2
	{ "SET 2 E", 0, nullptr }, // 0xD3
	{ "SET 2 H", 0, nullptr }, // 0xD4
	{ "SET 2 L", 0, nullptr }, // 0xD5
	{ "SET 2 [HL]", 0, nullptr }, // 0xD6
	{ "SET 2 A", 0, nullptr }, // 0xD7
	{ "SET 3 B", 0, nullptr }, // 0xD8
	{ "SET 3 C", 0, nullptr }, // 0xD9
	{ "SET 3 D", 0, nullptr }, // 0xDA
	{ "SET 3 E", 0, nullptr }, // 0xDB
	{ "SET 3 H", 0, nullptr }, // 0xDC
	{ "SET 3 L", 0, nullptr }, // 0xDD
	{ "SET 3 [HL]", 0, nullptr }, // 0xDE
	{ "SET 3 A", 0, nullptr }, // 0xDF
	{ "SET 4 B", 0, nullptr }, // 0xE0
	{ "SET 4 C", 0, nullptr }, // 0xE1
	{ "SET 4 D", 0, nullptr }, // 0xE2
	{ "SET 4 E", 0, nullptr }, // 0xE3
	{ "SET 4 H", 0, nullptr }, // 0xE4
	{ "SET 4 L", 0, nullptr }, // 0xE5
	{ "SET 4 [HL]", 0, nullptr }, // 0xE6
	{ "SET 4 A", 0, nullptr }, // 0xE7
	{ "SET 5 B", 0, nullptr }, // 0xE8
	{ "SET 5 C", 0, nullptr }, // 0xE9
	{ "SET 5 D", 0, nullptr }, // 0xEA
	{ "SET 5 E", 0, nullptr }, // 0xEB
	{ "SET 5 H", 0, nullptr }, // 0xEC
	{ "SET 5 L", 0, nullptr }, // 0xED
	{ "SET 5 [HL]", 0, nullptr }, // 0xEE
	{ "SET 5 A", 0, nullptr }, // 0xEF
	{ "SET 6 B", 0, nullptr }, // 0xF0
	{ "SET 6 C", 0, nullptr }, // 0xF1
	{ "SET 6 D", 0, nullptr }, // 0xF2
	{ "SET 6 E", 0, nullptr }, // 0xF3
	{ "SET 6 H", 0, nullptr }, // 0xF4
	{ "SET 6 L", 0, nullptr }, // 0xF5
	{ "SET 6 [HL]", 0, nullptr }, // 0xF6
	{ "SET 6 A", 0, nullptr }, // 0xF7
	{ "SET 7 B", 0, nullptr }, // 0xF8
	{ "SET 7 C", 0, nullptr }, // 0xF9
	{ "SET 7 D", 0, nullptr }, // 0xFA
	{ "SET 7 E", 0, nullptr }, // 0xFB
	{ "SET 7 H", 0, nullptr }, // 0xFC
	{ "SET 7 L", 0, nullptr }, // 0xFD
	{ "SET 7 [HL]", 0, nullptr }, // 0xFE
	{ "SET 7 A", 0, nullptr }, // 0xFF
};




void cpu_handle_interrupt() {
	if (!cpu.ime_flag){return;}
	uint8_t interrupts = (cpu.IF & 0x1F) & (cpu.IE & 0x1F);
	if (!interrupts){return;}
	uint16_t address;
	if (interrupts & 0x01) {
		cpu.IF ^= 0x01;
		address = 0x0040;
		Corelog << "VBLANK INTERRUPT!\n";
	}
	else if (interrupts & 0x02) {
		cpu.IF ^= 0x02;
		address = 0x0048;
		Corelog << "STAT INTERRUPT!\n";
	}
	else if (interrupts & 0x04) {
		cpu.IF ^= 0x04;
		address = 0x0050;
		Corelog << "TIMER INTERRUPT!\n";
	}
	else if (interrupts & 0x08) {
		cpu.IF ^= 0x08;
		address = 0x0058;
		Corelog << "SERIAL INTERRUPT!\n";
	}
	else if (interrupts & 0x10) {
		cpu.IF ^= 0x10;
		address = 0x0060;
		Corelog << "JOYPAD INTERRUPT!\n";
	}
	else {
		Corelog << "Broken interrupt handling, impossible interrupt?";
		return;
	}

	cpu.ime_flag = false;

	cpu_noop();
	cpu_noop();
	cpu.call(true, address);
}

uint8_t cpu_fetch(cpu_execute_func& execute_pointer) {
	uint8_t opcode = read_mem(cpu.pc++);

	if (cpu.use_extended_opcodes) {
		Corelog << std::hex << cpu.pc - 1 << ": " << cpu_instructions_extended[opcode].name << "\n";
		execute_pointer = cpu_instructions_extended[opcode].execute;
		cpu.use_extended_opcodes = false;
	}

	else {
		Corelog << std::hex << cpu.pc - 1 << ": " << cpu_instructions[opcode].name << "\n";
		execute_pointer = cpu_instructions[opcode].execute;
	}

	return opcode;
}

bool cpu_execute(uint8_t opcode, cpu_execute_func execute_pointer) {
	if (!execute_pointer) {
		Corelog << "Unknown Instruction: " << std::hex << +opcode << std::endl;
		return false;
	}
	
	(execute_pointer)();

	if(cpu.ei_counter == 1){cpu.ei_counter += 1;}
	else if (cpu.ei_counter >= 2) {
		cpu.ime_flag = true;
		cpu.ei_counter = 0;
	}
	
	return true;
}
