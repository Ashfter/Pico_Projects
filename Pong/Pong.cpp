#include <stdio.h>
#include "pico/stdlib.h"
#include <string.h>
#include "hardware/spi.h"
#include "hardware/i2c.h"

#include "ssd1306.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

int main()
{
    stdio_init_all();

    // set up analog to digital
    adc_init();
    adc_gpio_init(26); 
    adc_select_input(0);

    // intialize the port
    i2c_init(I2C_PORT, 100000);
    
    // override the functions and pull up
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    // sleep for a second to allow the screen to power up
    sleep_ms(1000);

    // create the display struct
    ssd1306_t disp;
    // intialize it to zeros
    memset(&disp, 0, sizeof(ssd1306_t));

    // intialize the display
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT);
  
    // start up the display
    ssd1306_poweron(&disp);
    ssd1306_clear(&disp);

    // variables
    uint16_t position;

    while (true) {
        position = (adc_read() * 100)/ 4095;

        ssd1306_draw_string(&disp, 16, 16, 2, position);
        sleep_ms(100);
    }
}
