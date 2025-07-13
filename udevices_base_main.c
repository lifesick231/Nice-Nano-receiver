#include <stdbool.h>
#include <stdint.h>
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "urf_radio.h"
#include "urf_star_protocol.h"
#include "urf_timer.h"

uint8_t data_packet[260];
uint8_t cycle_packet[260];

// Debug LED pin
#define LED_PIN 15

void fast_clock_start(void)
{
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART = 1;
    while (!NRF_CLOCK->EVENTS_HFCLKSTARTED) {}
}

int main(void)
{
    // Initialize LED GPIO
    nrf_gpio_cfg_output(LED_PIN);
    nrf_gpio_pin_clear(LED_PIN); // LED off
    nrf_delay_ms(500);

    // Start high-frequency clock and timer
    fast_clock_start();
    time_start();       // from urf_timer

    // Initialize Star protocol as central (is_central = 1)
    star_init(21, 1000, 3200, 1);

    // Main loop
    while (1)
    {
        // Progress Star protocol (send polls, handle responses)
        star_loop_step();

        // If a packet was received this cycle
        if (star_has_packet())
        {
            int len = star_get_packet(data_packet, sizeof(data_packet));
            uint8_t payload_len = data_packet[1];
            uint8_t rssi = NRF_RADIO->RSSISAMPLE;

            // Build cycle packet (79,213,rssi, payload...)
            cycle_packet[0] = 79;
            cycle_packet[1] = 213;
            cycle_packet[2] = rssi;
            for (int i = 0; i < payload_len; i++)
            {
                cycle_packet[3 + i] = data_packet[i];
            }

            debug(1);
        }
        else debug(2);
    }
}
