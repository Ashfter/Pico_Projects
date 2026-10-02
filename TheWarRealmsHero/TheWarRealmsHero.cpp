#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/dma.h"
#include "hardware/interp.h"
#include "hardware/timer.h"
#include "hardware/adc.h"
#include "display.h"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_MISO 4
#define PIN_CS   5
#define PIN_SCK  2
#define PIN_MOSI 3
#define PIN_DC   6
#define PIN_RST  7

// beep pin
#define PIN_BEEP 13

// user led ping
#define PIN_LED1 16
#define PIN_LED2 17
#define PIN_RGB_LED 12


// joystick pins
#define PIN_ADC0 26
#define PIN_ADC1 27

// button pins
#define PIN_K1 14
#define PIN_K2 15

// classes
class character {
    int xPos;
    int yPos;
    int maxSpeed = 5;
    int hitPoints = 10;
    int size = 4;


public:

    float xv = 0.0f;
    float yv = 0.0f;

    int getHitpoints() {return hitPoints;}
    void damagePlayer() {hitPoints--;}

    int getXPos() {return xPos;}
    void setXPos(int newPos) {xPos = newPos;}

    int getYPos() {return yPos;}
    void setYPos(int newPos) {yPos = newPos;}
    
    int getMaxSpeed() {return maxSpeed;}
    int getSize() {return size;}
};

// helper function/function prototypes
void updatePlayer(TFT_Display *display, character *player, uint16_t x_value, uint16_t y_value);

int main()
{
    stdio_init_all();

    // display variables
    TFT_Display disp = 
    {  
        .spi_port = SPI_PORT,
        .pin_sck = PIN_SCK,
        .pin_mosi = PIN_MOSI,
        .pin_cs = PIN_CS,
        .pin_dc = PIN_DC,
        .pin_rst = PIN_RST,
        .width = 480,
        .height = 320
    };

    tft_init(&disp);
    tft_clear(&disp);

    adc_init();
    adc_gpio_init(PIN_ADC0);
    adc_gpio_init(PIN_ADC1);
    
    // variables
    // timing varaibles 
    const uint32_t target_frame_us = 66664;
    uint64_t current_time;
    uint64_t last_frame_time = time_us_64();

    // player variables
    character player;
    player.setXPos(240),
    player.setYPos(160);
    uint16_t rawX;
    uint16_t rawY;

    tft_fill_circle(&disp, player.getXPos(), player.getYPos(), player.getSize(), RED);

    while (true) {
        // get raw inputs
        adc_select_input(0);
        rawX = adc_read();

        adc_select_input(1);
        rawY = adc_read();


        // calculate current time for 60 fps updating
        current_time = time_us_64();
        if (current_time - last_frame_time >= target_frame_us)
        {
            // set last frame to current time
            last_frame_time = current_time;

            // update actions
            updatePlayer(&disp, &player, rawX, rawY);

        }
    }
}


// update player function
// takes in raw x and y data
// converts into new pos for player
// updates them on the display
void updatePlayer(TFT_Display *display, character *player, uint16_t x_value, uint16_t y_value)
{
    // convert raw data into -1 to 1 scale
    float input_x = float(x_value - 2048) / 2048.0f;
    float input_y = float(y_value - 2048) / 2048.0f * -1;



    if (input_x < 0.0f)
    {
        float temp_input_x = input_x * -1;
        if (temp_input_x < 0.05f)
        {
            input_x = 0.0f;
        }
    }
    else if (input_x < 0.05f)
    {
        input_x = 0.0f;
    }

    if (input_y < 0.0f)
    {
        float temp_input_y = input_y * -1;
        if (temp_input_y < 0.05f)
        {
            input_y = 0.0f;
        }
    }
    else if (input_y < 0.05f)
    {
        input_y = 0.0f;
    }

    // get target location
    float target_x = input_x * player->getMaxSpeed();
    float target_y = input_y * player->getMaxSpeed();
    
    player->xv += (target_x - player->xv) * 0.2f;
    player->yv += (target_y - player->yv) * 0.2f;

    tft_fill_circle(display, player->getXPos(), player->getYPos(), player->getSize(), BLACK);

    if (player->getXPos() + player->xv + (player->getSize()) > 480)
    {
        player->setXPos(480 - (player->getSize() * 2));  
        player->xv = 0.0f; 
    }
    else if (player->getXPos() + player->xv + (player->getSize() * 2) < 0)
    {
        player->setXPos(0 + (player->getSize() * 2));
        player->xv = 0.0f; 
    }
    else
    {
        player->setXPos(player->getXPos() + player->xv);
    }

    if (player->getYPos() + player->yv + (player->getSize()) > 320)
    {
        player->setYPos(320 - (player->getSize() * 2));
        player->yv = 0.0f; 
    }
    else if (player->getYPos() + player->yv + (player->getSize() * 2) < 0)
    {
        player->setYPos(0 + (player->getSize() * 2));
        player->yv = 0.0f; 
    }
    else
    {
        player->setYPos(player->getYPos() + player->yv);
    }

    tft_fill_circle(display, player->getXPos(), player->getYPos(), player->getSize(), RED);
}