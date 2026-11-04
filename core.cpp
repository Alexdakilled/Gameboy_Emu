#include <iostream>
#include "cpu.h"
#include "cart.h"
#include "core.h"
#include "timer.h"
#include "ppu.h"
#include "gfx.h"

#include <thread>
#include <chrono>

std::ofstream Corelog("log.txt");

int core_clock_counter = 0;
bool quitting = false;
uint8_t current_opcode;
cpu_execute_func opcode_execute_pointer;

gfx_context* gfx_debug_ctx = nullptr;
gfx_context* gfx_render_ctx = nullptr;

bool bit_helper_func(uint8_t num, uint8_t bit) {
    if (!bit){if (!(num & 0x01)) {return false;}}
    else if (bit == 1){if (!(num & 0x02)) {return false;}}
    else if (bit == 2){if (!(num & 0x04)) {return false;}}
    else if (bit == 3){if (!(num & 0x08)) {return false;}}
    else if (bit == 4){if (!(num & 0x10)) {return false;}}
    else if (bit == 5){if (!(num & 0x20)) {return false;}}
    else if (bit == 6){if (!(num & 0x40)) {return false;}}
    else if (bit == 7){if (!(num & 0x80)) {return false;}}

    return true;
}

bool core_init(const std::string& filename) {
    if (!Corelog.is_open()) {
        std::cerr << "Could not create or open log file\n";
    }

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        std::cerr << "Could not initialize SDL: " << SDL_GetError() << '\n';
        return false;
    }

    const SDL_Color BLACK = { 0, 0, 0, 0 };

    gfx_debug_ctx = gfx_init(512, 512, "Debug");
    if (!gfx_debug_ctx) {
        core_shutdown();
        return false;
    }
    gfx_clear(gfx_debug_ctx, BLACK);
    gfx_update(gfx_debug_ctx);

    gfx_render_ctx = gfx_init(160 * 3, 144 * 3, "Render");
    if (!gfx_render_ctx) {
        core_shutdown();
        return false;
    }
    gfx_clear(gfx_render_ctx, BLACK);
    gfx_update(gfx_render_ctx);


    if (!cart_init(filename)) {
        core_shutdown();
        return false;
    }

    if (!cartridge.valid_logo) {
        Corelog << "Invalid logo?\n";
        core_shutdown();
        return false;
    }

    if (!cartridge.checksum_passed) {
        Corelog << "Checksum failed?\n";
        core_shutdown();
        return false;
    }


    cpu.af = 0x01B0;
    cpu.bc = 0x0013;
    cpu.de = 0x00D8;
    cpu.hl = 0x014D;
    cpu.sp = 0xFFFE;
    cpu.pc = 0x0100;
    cpu.IE = 0x00;
    cpu.IF = 0xE1;
    cpu.ime_flag = false;
    cpu.ei_counter = 0;
    cpu.halted = false;
    cpu.use_extended_opcodes = false;

    timer.div = 0xAB;
    timer.tima = 0x00;
    timer.tma = 0x00;
    timer.tac = 0xF8;
    timer.div_count = 0;
    timer.tima_count = 0;

    ppu_init();
    ppu_reset();

    return true;
}

void core_loop() {
    while (!quitting) {
        
        if (cpu.halted) {
            core_tick();
            if (cpu.IE & cpu.IF) {
                cpu.halted = false;
            }
        }


        while (!cpu.halted && !quitting) {
            cpu_handle_interrupt();
            current_opcode = cpu_fetch(opcode_execute_pointer);
            if (!cpu_execute(current_opcode, opcode_execute_pointer)) {
                return;
            }
        }
    }

}

void core_tick() {
    timer_tick();
    ppu_tick();

    std::this_thread::sleep_for(std::chrono::microseconds(1));
}

void core_shutdown() {
    gfx_destroy(gfx_render_ctx);
    gfx_render_ctx = nullptr;
    gfx_destroy(gfx_debug_ctx);
    gfx_debug_ctx = nullptr;
    SDL_Quit();
}