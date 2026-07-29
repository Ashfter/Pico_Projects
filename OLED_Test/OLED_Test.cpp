#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

extern "C"
{
    #include "ssd1306.h"
}

#define I2C_PORT i2c0
#define SDA_PIN 4
#define SCL_PIN 5

int main() {
    stdio_init_all();

    // 1. Exact same initialization that worked for the scanner
    i2c_init(I2C_PORT, 100000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    sleep_ms(1000); // Allow display to power up fully

    // 2. Initialize display struct with zero-out to prevent garbage memory flags
    ssd1306_t disp;
    memset(&disp, 0, sizeof(ssd1306_t));

    // 3. Fire up the driver at address 0x3C
    if (!ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT)) {
        printf("SSD1306 initialization failed!\n");
    } else {
        printf("SSD1306 initialized successfully!\n");
    }

    ssd1306_poweron(&disp);
    ssd1306_clear(&disp);

    // 4. Draw something unmistakable (a bounding box and text)
    ssd1306_draw_empty_square(&disp, 0, 0, 128, 64);
    ssd1306_draw_string(&disp, 10, 24, 1, "IT WORKS!");
    ssd1306_show(&disp);

    while (true) {
        sleep_ms(1000);
    }
}