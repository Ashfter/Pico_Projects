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

int main()
{
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
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT);
  
    ssd1306_poweron(&disp);
    ssd1306_clear(&disp);

    // set the in and out pins
    // USS trigger
    gpio_init(0);
    gpio_set_dir(0, GPIO_OUT);

    // USS Echo
    gpio_init(1);
    gpio_set_dir(1, GPIO_IN);

    // beep
    gpio_init(2);
    gpio_set_dir(2, GPIO_OUT);

    // variable
    uint64_t startTime;
    uint64_t endTime;
    uint64_t totalTime;
    uint32_t timeout;
    float distance;
    char distanceStr[32];

    while(true)
    {
        // start the trigger
        gpio_put(0, 1);
        sleep_us(10);
        gpio_put(0, 0);

        startTime = time_us_64();
        
        // wait for the pin to go to zero
        timeout = 0;
        while(gpio_get(1) == 0)
        {
            // wait for the pin to go high, or timeout to be reached
            timeout++;
            if(timeout > 500000)
            {
                break;
            }
        }

        // while loop for waiting
        while (gpio_get(1) == 1)
        {
            // do nothing were waiting
        }

        // get the end time 
        endTime = time_us_64();

        // calculate the difference in time and initialize distance
        totalTime = endTime - startTime;

        // if total time is less than 380000 aka its timeout time
        if (totalTime < 38000)
        {
            // calculate the distance
            distance = (totalTime * 0.0343f)/2.0f;

            // determine the beeping interval
            if (distance <= 50.0f && distance > 25.0f)
            {
                // beep interval
                gpio_put(2, 1);
                sleep_ms(50);
                gpio_put(2, 0);
                sleep_ms(400);
            }
            else if (distance <= 25.0f && distance > 15.0f)
            {
                gpio_put(2, 1);
                sleep_ms(30);
                gpio_put(2, 0);
                sleep_ms(200);
            }
            else if (distance <= 15.0f && distance > 5.0f)
            {
                gpio_put(2, 1);
                sleep_ms(10);
                gpio_put(2, 0);
                sleep_ms(100);
            }
            else if (distance <= 5.0f && distance > 0.0f)
            {
                gpio_put(2, 1);
                sleep_ms(10);
                gpio_put(2, 0);
                sleep_ms(50);
            }
            
            // save string and clear display
            snprintf(distanceStr, sizeof(distanceStr), "%.2f cm", distance);
            ssd1306_clear(&disp);

            // draw string
            ssd1306_draw_string(&disp, 0, 0, 2, "Distance:");
            ssd1306_draw_string(&disp, 0, 16, 2, distanceStr);

            // display string
            ssd1306_show(&disp);
        }
    }

}
