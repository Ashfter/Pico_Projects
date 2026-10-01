#include <stdio.h>
#include "pico/stdlib.h"
#include <string.h>
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include <cstdlib>
#include "pico/time.h"


extern "C"
{
    #include "ssd1306.h"
}

// I2C defines
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

class paddle {

    // position variables
    int x;
    int y;

    // paddle size variables 
    int height = 12;
    int width = 2;

public:

    int getx() {return x;}
    int gety() {return y;}

    void setx(int value) {x = value;}
    void sety(int value) {y = value;}

    int getheight() {return height;}
    int getwidth() {return width;}
};

class ball
{
public:
    int x = 64;
    int y = 32;

    int width = 3;
    int height = 3;

    int speed = 1;
    int speedCap = 3;

    int horizontalMove;
    int verticalMove;
};

void resetBall(ball *Ball)
{
    srand(time_us_64());

    int random_dy = (rand() % 3) + 1;
    int dy_sign = (rand() % 2 == 0) ? 1 : -1;
    int dy = dy_sign * random_dy;
    Ball->verticalMove = random_dy;

    dy_sign = (rand() % 2 == 0) ? 1 : -1;
    Ball->horizontalMove = dy_sign;

    Ball->x = 64;
    Ball->y = 32;
}


int main()
{
    stdio_init_all();

    // Set up analog to digital for your control knob/potentiometer
    adc_init();
    adc_gpio_init(26); 
    adc_select_input(0);

    // Initialize the I2C port
    i2c_init(I2C_PORT, 100000);
    
    // Override functions and pull up pins
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    // Sleep for a second to allow the screen to power up
    sleep_ms(1000);

    // Create the display struct
    ssd1306_t disp;
    memset(&disp, 0, sizeof(ssd1306_t));

    // Initialize the display (width, height, i2c address, i2c instance)
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT);
  
    // Start up the display and clear it
    ssd1306_poweron(&disp);
    ssd1306_clear(&disp);

    // Variables
    // score
    uint32_t pscore = 0;
    char pscore_buffer[8];

    uint32_t aiscore = 0;
    char aiscore_buffer[8];

    char menu[16];

    int count = 0;

    // player variables
    paddle playerPaddle;
    playerPaddle.setx(0);
    playerPaddle.sety(0);

    uint16_t raw_val;
    int paddle_y;

    // ai variables
    paddle aiPaddle;
    aiPaddle.setx(127);
    aiPaddle.sety(0);

    // ball variables
    ball gameBall;

    // get random left right/up down starting values
    srand(time_us_64());
    int random_dy = (rand() % 3) + 1;
    int dy_sign = (rand() % 2 == 0) ? 1 : -1;
    int dy = dy_sign * random_dy;
    gameBall.verticalMove = random_dy;

    dy_sign = (rand() % 2 == 0) ? 1 : -1;
    gameBall.horizontalMove = dy_sign;

    snprintf(pscore_buffer, sizeof(pscore_buffer), " %d", pscore);
    snprintf(aiscore_buffer, sizeof(aiscore_buffer), " %d", aiscore);

    // setup
    ssd1306_draw_square(&disp, playerPaddle.getx(), playerPaddle.gety(), playerPaddle.getwidth(), playerPaddle.getheight());
    ssd1306_draw_square(&disp, aiPaddle.getx(), aiPaddle.gety(), aiPaddle.getwidth(), aiPaddle.getheight());
    ssd1306_draw_square(&disp, gameBall.x, gameBall.y, gameBall.width, gameBall.height);
    ssd1306_draw_string(&disp, 48, 4, 1, pscore_buffer);
    ssd1306_draw_string(&disp, 68, 4, 1, aiscore_buffer);
    ssd1306_show(&disp);

    sleep_ms(1000);

    while (true)
    {
        // clear display
        ssd1306_clear(&disp);

        // player movement
        raw_val = adc_read();
        paddle_y = (raw_val * (64 - 12)) / 4095;

        playerPaddle.sety(paddle_y);

        // ai movement
        if (count == 0)
        {
            if (gameBall.y > 64 - 12)
            {
                aiPaddle.sety(52);
            }
            else if (gameBall.y - aiPaddle.gety() +  aiPaddle.getheight() > 4)
            {
                aiPaddle.sety(aiPaddle.gety() + 2);
            }
            else if (gameBall.y - aiPaddle.gety() < 4)
            {
                aiPaddle.sety(aiPaddle.gety() - 2);
            }
        }

        // ball movement
        // check for collision 
        if (gameBall.x == playerPaddle.getx() || gameBall.x == (playerPaddle.getx() + playerPaddle.getwidth() - 1))
        {
            // if the x is the same, we check the y
            if (gameBall.y >= playerPaddle.gety() && gameBall.y <= (playerPaddle.gety() + playerPaddle.getheight() - 1))
            {
                // its a collision we speed the ball up
                if (gameBall.speed > gameBall.speedCap)
                {
                    // increase speed
                    gameBall.speed++;
                }
                // flip movement
                gameBall.horizontalMove = gameBall.horizontalMove * -1;

            }
            else
            {  
                // its a score
                aiscore++;

                resetBall(&gameBall);
            }
        }
        else if (gameBall.x == aiPaddle.getx() || gameBall.x == (aiPaddle.getx() + aiPaddle.getwidth() - 1))
        {
            // if the x is the same, we check the y
            if (gameBall.y >= aiPaddle.gety() && gameBall.y <= (aiPaddle.gety() + aiPaddle.getheight() - 1))
            {
                // its a collision we speed the ball up
                if (gameBall.speed > gameBall.speedCap)
                {
                    // increase speed
                    gameBall.speed++;
                }
                // flip movement
                gameBall.horizontalMove = gameBall.horizontalMove * -1;

            }
            else
            {  
                // its a score
                pscore++;

                resetBall(&gameBall);
            }
        }
        // siding colision
        if (gameBall.y > 61 || gameBall.y < 1)
        {
            gameBall.verticalMove = gameBall.verticalMove * -1;
        }
        
        // ball movement
        // if ball goes past 0 on the left
        if (gameBall.x + (gameBall.horizontalMove * gameBall.speed) < 0)
        {
            // set to zero for a turn
            gameBall.x = 0;
        }
        // else if ball goes past 128 on the right
        else if (gameBall.x + (gameBall.horizontalMove * gameBall.speed) > 128)
        {
            // set ball to 128
            gameBall.x = 128;
        }
        // else move
        else
        {
            gameBall.x = gameBall.x + (gameBall.horizontalMove * gameBall.speed);
        }

        // if ball higher than 64 on bottom
        if (gameBall.y + (gameBall.verticalMove * gameBall.speed) > 64)
        {
            // set ball to 64
            gameBall.y = 63;
        }
        // else if lower than 0 on the top
        else if (gameBall.y + (gameBall.verticalMove * gameBall.speed) < 0)
        {
            // set ball to 0
            gameBall.y = 0;
        }
        // else move
        else
        {
            gameBall.y = gameBall.y + (gameBall.verticalMove * gameBall.speed);
        }

        // update display
        snprintf(pscore_buffer, sizeof(pscore_buffer), " %d", pscore);
        snprintf(aiscore_buffer, sizeof(aiscore_buffer), " %d", aiscore);

        ssd1306_draw_square(&disp, playerPaddle.getx(), playerPaddle.gety(), playerPaddle.getwidth(), playerPaddle.getheight());
        ssd1306_draw_square(&disp, aiPaddle.getx(), aiPaddle.gety(), aiPaddle.getwidth(), aiPaddle.getheight());
        ssd1306_draw_square(&disp, gameBall.x, gameBall.y, gameBall.width, gameBall.height);
        ssd1306_draw_string(&disp, 48, 4, 1, pscore_buffer);
        ssd1306_draw_string(&disp, 68, 4, 1, aiscore_buffer);
        ssd1306_show(&disp);


        if (pscore > 7)
        {
            snprintf(menu, sizeof(menu), "Player Wins");

            ssd1306_clear(&disp);
            ssd1306_draw_string(&disp, 16, 16, 2, menu);
            ssd1306_show(&disp);

        }
        else if (aiscore > 7)
        {
            snprintf(menu, sizeof(menu), "ai Wins");
            ssd1306_draw_string(&disp, 16, 16, 2, menu);
            ssd1306_show(&disp);
        }

        
        sleep_ms(1);
    }
}

