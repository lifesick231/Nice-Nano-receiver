/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "bsp/board_api.h"
#include "tusb.h"
#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_timer.h"

#include "urf_timer.h"
#include "urf_star_protocol.h"
#include "urf_radio.h"

void cdc_task(void);

extern volatile uint32_t count;
extern volatile bool print_flag;

uint8_t data_packet[256];

// Constants
#define RADIO_CHANNEL     21      // Channel 21 (2420 MHz)
#define RADIO_SPEED       1000    // 1 Mbps
#define PHASE_LENGTH_US   3200    // 3.2ms per node phase
#define IS_CENTRAL        1       // This device acts as central

/*------------- MAIN -------------*/
int main(void) {

  NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
  NRF_CLOCK->TASKS_HFCLKSTART = 1;
  while (!NRF_CLOCK->EVENTS_HFCLKSTARTED);

  board_init();

  // init device stack on configured roothub port
  tusb_rhport_init_t dev_init = {
    .role = TUSB_ROLE_DEVICE,
    .speed = TUSB_SPEED_AUTO
  };

  tusb_init(BOARD_TUD_RHPORT, &dev_init);
  time_start();

  NRF_WDT->CRV = 32767;
  NRF_WDT->RR[0] = 0x6E524635;
  NRF_WDT->CONFIG = 1;
  NRF_WDT->TASKS_START = 1;

  star_init(RADIO_CHANNEL, RADIO_SPEED, PHASE_LENGTH_US, IS_CENTRAL);
  star_set_id(0x561659); // hoặc ID bạn muốn

  if (board_init_after_tusb) {
    board_init_after_tusb();
  }

  while (1) {
    NRF_WDT->RR[0] = 0x6E524635;

    tud_task(); // tinyusb device task

    star_loop_step();

    if (star_has_packet())
    {
      int len = star_get_packet(data_packet, sizeof(data_packet));

      if (tud_cdc_connected() && (tud_cdc_get_line_state() & 0x01))
      {
        tud_cdc_write_str("Packet received:\r\n");
        tud_cdc_write(data_packet, len);
        tud_cdc_write_str("\r\n");
        tud_cdc_write_flush();
      }
    }

    cdc_task();

  } 
}

//--------------------------------------------------------------------+
// USB CDC
//--------------------------------------------------------------------+
void cdc_task(void) {
  if (print_flag && tud_cdc_connected() && (tud_cdc_get_line_state() & 0x01)) {
    print_flag = false;
    char msg[64];
    snprintf(msg, sizeof(msg), "IRQ count: %lu\r\n", count);
    tud_cdc_write_str(msg);
    tud_cdc_write_flush();
  }
}

// Invoked when cdc when line state changed e.g connected/disconnected
void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts) {
  (void) itf;
  (void) rts;

  // TODO set some indicator
  if (dtr) {
    // Terminal connected
  } else {
    // Terminal disconnected
  }
}

// Invoked when CDC interface received data from host
void tud_cdc_rx_cb(uint8_t itf) {
  (void) itf;
}
