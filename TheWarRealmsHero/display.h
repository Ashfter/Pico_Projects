#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"


typedef struct {
    spi_inst_t *spi_port;
    uint pin_sck;
    uint pin_mosi;
    uint pin_cs;
    uint pin_dc;
    uint pin_rst;
    uint16_t width;
    uint16_t height;
} TFT_Display;

typedef enum {
    BLACK  = 0x0000,
    WHITE  = 0xFFFF,
    RED    = 0xF800,
    ORANGE = 0xFD20,
    YELLOW = 0xFFE0,
    GREEN  = 0x07E0,
    BLUE   = 0x001F,
    PURPLE = 0x8010
} TFT_Color;

// initialization functions
void tft_init(TFT_Display *disp);
void tft_write_command(const TFT_Display *disp, uint8_t cmd);
void tft_write_data(const TFT_Display *disp, const uint8_t *data, size_t length);
void tft_clear(const TFT_Display *disp);
void tft_set_window(const TFT_Display *disp, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

// Strings
void tft_draw_char(const TFT_Display *disp, int16_t x, int16_t y, char c, uint16_t color, uint16_t bg_color, uint8_t size);
void tft_draw_string(const TFT_Display *disp, int16_t x, int16_t y, const char *str, uint16_t color, uint16_t bg_color, uint8_t size);


// Rectangles & Squares
void tft_draw_rect(const TFT_Display *disp, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void tft_fill_rect(const TFT_Display *disp, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

// Circles
void tft_draw_circle(const TFT_Display *disp, int16_t x0, int16_t y0, int16_t r, uint16_t color);
void tft_fill_circle(const TFT_Display *disp, int16_t x0, int16_t y0, int16_t r, uint16_t color);

// heart
void tft_draw_heart(const TFT_Display *disp, int16_t x, int16_t y, uint8_t size, uint16_t color);


#endif // DISPLAY_H