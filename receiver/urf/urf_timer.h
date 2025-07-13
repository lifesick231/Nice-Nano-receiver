#include <stdint.h>
#include "nrf_gpio.h"

#define LED_PIN 15
static inline void led_on(void)  { nrf_gpio_pin_set(LED_PIN); }
static inline void led_off(void) { nrf_gpio_pin_clear(LED_PIN); }
static inline void led_toggle(void) {nrf_gpio_pin_toggle(LED_PIN);}

void delay(uint32_t ms);
void debug(uint32_t times);

void time_start(void);
void time_pause(void);
void time_resume(void);
void time_stop(void);
void time_adjust(int ms_shift);
uint32_t micros(void);
uint32_t micros_count(void); //based on time interrupt, default step 250 us
uint32_t millis(void);
uint32_t seconds(void);
void delay_ms(uint32_t ms);
void delay_mcs(uint32_t mcs);
void schedule_event(uint32_t steps_dt, void (*tm_event)(void), int repeated);
void schedule_subevent1(uint32_t steps_dt, void (*tm_event)(void));
void schedule_subevent2(uint32_t steps_dt, void (*tm_event)(void));
//void schedule_subevent(uint32_t steps_dt, void (*tm_event)(void));
void schedule_event_delayed(uint32_t delay, uint32_t steps_dt, void (*tm_event)(void), int repeated);
void schedule_event_adjust(int dt);
void schedule_event_stop(void);
void schedule_event_cancel_sub1(void);
void schedule_event_cancel_sub2(void);
