#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

extern "C"
{
    #include "ssd1306.h"
}

#define I2C_PORT i2c1          
#define I2C_SDA_PIN 4          
#define I2C_SCL_PIN 3

void setup_display_hardware() {
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SCL_PIN);
    gpio_pull_up(I2C_SDA_PIN);
}

int main()
{
    stdio_init_all();
    setup_display_hardware();

    ssd1306_t disp;
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT);
    ssd1306_poweron(&disp);

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
    float distance;
    char distanceStr[32];
    uint32_t last_display_update = 0;

    while(true)
    {
        // start the trigger
        gpio_put(0, 1);
        sleep_us(10);
        gpio_put(0, 0);

        // start the time 
        startTime = time_us_64();

        // while loop for waiting
        while (gpio_get(1) == 1)
        {
            // do nothing were waiting
        }

        // get the end time 
        endTime = time_us_64();

        // calculate the difference in time
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

            // Only update the display every 200ms so I2C doesn't choke the loop timing
            uint32_t current_time = to_ms_since_boot(get_absolute_time());
            if (current_time - last_display_update > 200) {
                snprintf(distanceStr, sizeof(distanceStr), "%.2f cm", distance);
                ssd1306_clear(&disp);
                ssd1306_draw_string(&disp, 0, 0, 1, "Distance:");
                ssd1306_draw_string(&disp, 0, 16, 1, distanceStr);
                ssd1306_show(&disp);
                last_display_update = current_time;
            }
        }
    }
}