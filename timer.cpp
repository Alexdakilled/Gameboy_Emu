#include "timer.h"
#include "core.h"
#include "cpu.h"

Timer timer;

void timer_tick() {
    timer.div_count += 1;

    if (timer.div_count >= 64) {
        timer.div += 1;
        timer.div_count = 0;
    }


    if ((timer.tac & 0x04) == 0x04) {
        timer.tima_count += 1;

        int inc_cycles;

        switch (timer.tac & 0x03) {
            case 0x00:
                inc_cycles = 256;
                break;
            case 0x01:
                inc_cycles = 4;
                break;
            case 0x02:
                inc_cycles = 16;
                break;
            case 0x03:
                inc_cycles = 64;
                break;

           default:
                inc_cycles = 9999;
                Corelog << "Impossible switch/case logic in timer TIMA increment?";
        }

        if (timer.tima_count >= inc_cycles) {
            if(timer.tima == 0xFF) {
                cpu.IF |= 0x04; // Timer interrupt
                timer.tima = timer.tma;
                timer.tima_count = 0;
            }

            else {
                timer.tima += 1;
                timer.tima_count = 0;
            }
        }
    }
}



