#pragma once

#include <string>
#include <fstream>
#include <gfx.h>


typedef void (*cpu_execute_func)();

extern gfx_context* gfx_debug_ctx;
extern gfx_context* gfx_render_ctx;



extern std::ofstream Corelog;

extern int core_clock_counter;
extern bool quitting;
extern uint8_t current_opcode;
extern cpu_execute_func opcode_execute_pointer;

extern bool bit_helper_func(uint8_t num, uint8_t bit);

extern bool core_init(const std::string& filename);

extern void core_loop();

extern void core_tick();

extern void core_shutdown();




