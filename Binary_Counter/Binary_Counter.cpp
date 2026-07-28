#include <stdio.h>
#include "pico/stdlib.h"

int main()
{
    stdio_init_all();

    int count = 0;
    int ledPin[] = {0, 1, 2, 3, 4, 5};

    // Initialize all LED pins using a loop
    for (int i = 0; i < 6; i++) {
        gpio_init(ledPin[i]);
        gpio_set_dir(ledPin[i], GPIO_OUT);
    }

    // Button pin initialization
    gpio_init(20);
    gpio_set_dir(20, GPIO_IN);
    gpio_pull_up(20);

    while (true) 
    {
        if (!gpio_get(20))
        {
            sleep_ms(50);
            
            // Increment count up to 63 (6 bits: 0 to 63)
            count = (count + 1) % 64;

            // Update all LEDs using bitwise shifting!
            for (int i = 0; i < 6; i++) 
            {
                // Checks if the i-th bit is active in 'count'
                int bitState = (count & (1 << i)) ? 1 : 0;
                gpio_put(ledPin[i], bitState);
            }
            
            while (!gpio_get(20))
            {
                sleep_ms(10);
            }
        }
    }
}
