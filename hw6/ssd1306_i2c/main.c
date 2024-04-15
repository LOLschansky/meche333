#include <stdio.h>
#include <string.h>
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "ssd1306.h"
#include "font.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

void drawChar(int x, int y, char letter);
void drawMessage(int x, int y, char* message);
float volt();
/*
//Device address
#define ADDR _u(0b0111100)

// Hardware registers
#define REG_IODIR _u(0x00)
#define REG_GPIO _u(0x09)
#define REG_OLAT _u(0x0A)


void chip_init() {
    // use the "handheld device dynamic" optimal setting (see datasheet)
    uint8_t buf[2];

    // set GP7 as output, rest as input
    buf[0] = REG_IODIR;
    buf[1] = 0b01111111;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}

void set(char v) {
    uint8_t buf[2];

        // send register number followed by its corresponding value
    buf[0] = REG_OLAT;
    buf[1] = v << 7;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}

int read() {
    uint8_t buf[1];
    uint8_t reg = REG_GPIO;

    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);
    i2c_read_blocking(i2c_default, ADDR, buf, 1, false);

    if (buf[0]&0b1 == 0b1){
        return 1;
    }
    else{
        return  0;
    }
}

*/



int main(){
    stdio_init_all();

    // I2C is "open drain", pull ups to keep signal high when no data is being sent
    i2c_init(i2c_default, 100 * 20000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);


    //adc setup
    adc_init(); // init the adc module
    adc_gpio_init(26); // set ADC0 pin to be adc input instead of GPIO
    adc_select_input(0); // select to read from ADC0

    //Pico heartbeat
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // initializie the chip
    //chip_init(); // gp7 is output, gp0 is input

    //Dispaly setup
    ssd1306_setup();

    char volts[50];
    char fps[50];
    int i = 0;
    //sprintf(message,"hello = %d", i);
    //sprintf(message,"hello");

    //drawChar(32,12,message[0]);
    // drawMessage(32,12, message);


    //sprintf(message,"%f V %d", volt(),i);
    //drawMessage(32,12, message);
    
    //ssd1306_update();
    //sleep_ms(250); // sleep so that data polling and register update don't collide
    while (1) {
        //Pico heartbeat
        gpio_put(LED_PIN, 1);
        //ssd1306_drawPixel(64, 16, 1);
        //ssd1306_update();

        gpio_put(LED_PIN, 0);
       //ssd1306_drawPixel(64, 16, 0);
       // ssd1306_update();

        //drawChar(64,12,message[0]);
        //ssd1306_update();

        sprintf(volts,"%.2f V %d", volt(),0);
        drawMessage(32,12, volts);
    
        ssd1306_update();

        unsigned int start = to_us_since_boot(get_absolute_time());
        sprintf(fps, "i = %d", i);
        drawMessage(1,1,fps);
        ssd1306_update();
        i++;
        
        unsigned int stop = to_us_since_boot(get_absolute_time());

        unsigned int t = stop - start;
        sprintf(fps, "FPS = %.2f", 1000000.0/t);
        drawMessage(1,24,fps);
        ssd1306_update();
        sleep_ms(50);

    }
}

void drawChar(int x, int y, char letter){
    int i, j;
    for (i = 0; i<5; i++){
        char c = ASCII[letter-32][i];
        for (j = 0; j<8; j++){
            char bit = (c>>j)&0b1;
            if (bit == 0b1){
                ssd1306_drawPixel(x+i, y+j,1);
            }
            else{
                ssd1306_drawPixel(x+i,y+j,0);
            }
        }
    }
}

void drawMessage(int x, int y, char* message){
    int i = 0;
    while (message[i]>0){
     drawChar(x+i*5,y,message[i]);
     i++;
    }
}

float volt(){
    char message[50];
    int i = 0;

    uint16_t result = adc_read();
    const float conversion_factor = 3.3f / (1 << 12);
    //sprintf(message,"%f V\n %d", result*conversion_factor, i);
    return (float) result*conversion_factor;

}