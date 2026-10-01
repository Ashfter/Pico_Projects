#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "dht11-pico.h"  // Match your header file name

extern "C"
{
    #include "ssd1306.h"
}

// I2C defines
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9
#define DHT_PIN 0

int main()
{
    stdio_init_all();

    Dht11 dht(DHT_PIN);

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_t disp;
    memset(&disp, 0, sizeof(ssd1306_t));

    // Initialize the display (width, height, i2c address, i2c instance)
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT);
  
    // Start up the display and clear it
    ssd1306_poweron(&disp);
    ssd1306_clear(&disp);

    // Variables (the library returns doubles)
    double temp = 0.0;
    double humidity = 0.0;
    char tempStr[32];
    char humidStr[32];

    while (true) 
    {
        ssd1306_clear(&disp);

        // Fetch temperature and humidity using the library's pointer function
        dht.readRHT(&temp, &humidity);

        // Format strings (fixed missing semicolon and format specifier typo)
        snprintf(tempStr, sizeof(tempStr), "Temp: %.1f C", temp);
        snprintf(humidStr, sizeof(humidStr), "Humid: %.1f %%", humidity);

        // Adjusted Y coordinates so the text rows don't overlap on the 128x64 display
        ssd1306_draw_string(&disp, 0, 0, 1, tempStr);
        ssd1306_draw_string(&disp, 0, 16, 1, humidStr);

        ssd1306_show(&disp);

        // DHT11 requires at least 1-2 seconds between reads
        sleep_ms(2000);
    }
}