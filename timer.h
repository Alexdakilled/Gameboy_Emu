#pragma once
#include <cstdint>


struct Timer {
    uint8_t div;
    uint8_t tima;
    uint8_t tma;
    uint8_t tac;

    int div_count;
    int tima_count;
};

extern Timer timer;

void timer_init();
void timer_tick();