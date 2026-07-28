#include <stdio.h>
#include "pico/stdlib.h"


int main()
{
    gpio_init(0);
    gpio_set_dir(0, GPIO_OUT);
    gpio_put(0, 1);

    while (true) 
    {
    }
}
