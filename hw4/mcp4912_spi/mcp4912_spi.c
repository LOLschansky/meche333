/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"
#include "math.h"


#define READ_BIT 0x80

int32_t t_fine;

uint16_t dig_T1;
int16_t dig_T2, dig_T3;
uint16_t dig_P1;
int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
uint8_t dig_H1, dig_H3;
int8_t dig_H6;
int16_t dig_H2, dig_H4, dig_H5;



#ifdef PICO_DEFAULT_SPI_CSN_PIN
static inline void cs_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    asm volatile("nop \n nop \n nop");
}
#endif

#if defined(spi_default) && defined(PICO_DEFAULT_SPI_CSN_PIN)

static void write_register(unsigned short voltage, int channel) {
    uint8_t buf[2];
    
    //buf[0] = 0b01111111;
    channel = channel << 7;
    buf[0] = channel;
    buf[0] = buf[0] | (0b111 << 4);
    buf[0] = buf[0] | (voltage >> 6);

    buf[1] = voltage << 2;
    //buf[1] = buf[1] | 0b00000011; // last two bits can be anything

    cs_select();
    spi_write_blocking(spi_default, buf, 2);
    cs_deselect();


}
#endif


int main() {
    stdio_init_all();
#if !defined(spi_default) || !defined(PICO_DEFAULT_SPI_SCK_PIN) || !defined(PICO_DEFAULT_SPI_TX_PIN) || !defined(PICO_DEFAULT_SPI_RX_PIN) || !defined(PICO_DEFAULT_SPI_CSN_PIN)
#warning spi/bme280_spi example requires a board with SPI pins
    puts("Default SPI pins were not defined");
#else

    // Set baud to 1MHhz
    spi_init(spi_default, 1000 * 1000);
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    // Make the SPI pins available to picotool
    bi_decl(bi_3pins_with_func(PICO_DEFAULT_SPI_RX_PIN, PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI));

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    // Make the CS pin available to picotool
    bi_decl(bi_1pin_with_name(PICO_DEFAULT_SPI_CSN_PIN, "SPI CS"));

    
    int rate = 1;
    int t = 0;

    //
    int rate_t = 2;
    int t_t = 0;
    int voltage_triangle = 0;

       while (true){
        float voltage_fl_sin = sin(2.0*3.1415*2*(((float) t)/1024.0))*511.0 + 511.0;
        int voltage_sin = (int) voltage_fl_sin;

        voltage_triangle = voltage_triangle + rate_t;
        //need to multiply by 2 to get proper period


        write_register(voltage_triangle-1, 1);
        write_register(voltage_sin, 0);

        if ((t == 0) || (t == 1023)){
            rate = rate * -1;
        }
        if ((voltage_triangle  == 0) || (voltage_triangle == 1022)){
            rate_t = rate_t * -1;
        }
        sleep_ms(1);
        t = t + rate;

    }
    



#endif
}
