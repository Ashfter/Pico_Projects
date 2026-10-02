#include "display.h"


static const uint8_t font5x7[][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // 32: Space
    {0x00, 0x00, 0x5F, 0x00, 0x00}, // 33: !
    {0x00, 0x07, 0x00, 0x07, 0x00}, // 34: "
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, // 35: #
    {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // 36: $
    {0x23, 0x13, 0x08, 0x64, 0x62}, // 37: %
    {0x36, 0x49, 0x55, 0x22, 0x50}, // 38: &
    {0x00, 0x05, 0x03, 0x00, 0x00}, // 39: '
    {0x00, 0x1C, 0x22, 0x41, 0x00}, // 40: (
    {0x00, 0x41, 0x22, 0x1C, 0x00}, // 41: )
    {0x14, 0x08, 0x3E, 0x08, 0x14}, // 42: *
    {0x08, 0x08, 0x3E, 0x08, 0x08}, // 43: +
    {0x00, 0x50, 0x30, 0x00, 0x00}, // 44: ,
    {0x08, 0x08, 0x08, 0x08, 0x08}, // 45: -
    {0x00, 0x60, 0x60, 0x00, 0x00}, // 46: .
    {0x20, 0x10, 0x08, 0x04, 0x02}, // 47: /
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 48: 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 49: 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 50: 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 51: 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 52: 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 53: 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 54: 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 55: 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 56: 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 57: 9
    {0x00, 0x36, 0x36, 0x00, 0x00}, // 58: :
    {0x00, 0x56, 0x36, 0x00, 0x00}, // 59: ;
    {0x08, 0x14, 0x22, 0x41, 0x00}, // 60: <
    {0x14, 0x14, 0x14, 0x14, 0x14}, // 61: =
    {0x00, 0x41, 0x22, 0x14, 0x08}, // 62: >
    {0x02, 0x01, 0x51, 0x09, 0x06}, // 63: ?
    {0x32, 0x49, 0x79, 0x41, 0x3E}, // 64: @
    {0x7C, 0x12, 0x11, 0x12, 0x7C}, // 65: A
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // 66: B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // 67: C
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // 68: D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // 69: E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // 70: F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // 71: G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // 72: H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // 73: I
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // 74: J
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // 75: K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // 76: L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // 77: M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // 78: N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // 79: O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // 80: P
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // 81: Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // 82: R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // 83: S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // 84: T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // 85: U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // 86: V
    {0x3F, 0x40, 0x38, 0x40, 0x3F}, // 87: W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // 88: X
    {0x07, 0x08, 0x70, 0x08, 0x07}, // 89: Y
    {0x61, 0x51, 0x49, 0x45, 0x43}, // 90: Z
    {0x00, 0x7F, 0x41, 0x41, 0x00}, // 91: [
    {0x02, 0x04, 0x08, 0x10, 0x20}, // 92: \ (backslash)
    {0x00, 0x41, 0x41, 0x7F, 0x00}, // 93: ]
    {0x04, 0x02, 0x01, 0x02, 0x04}, // 94: ^
    {0x40, 0x40, 0x40, 0x40, 0x40}, // 95: _
    {0x00, 0x01, 0x02, 0x04, 0x00}, // 96: `
    {0x20, 0x54, 0x54, 0x54, 0x78}, // 97: a
    {0x7F, 0x48, 0x44, 0x44, 0x38}, // 98: b
    {0x38, 0x44, 0x44, 0x44, 0x20}, // 99: c
    {0x38, 0x44, 0x44, 0x48, 0x7F}, // 100: d
    {0x38, 0x54, 0x54, 0x54, 0x18}, // 101: e
    {0x08, 0x7E, 0x09, 0x01, 0x02}, // 102: f
    {0x0C, 0x52, 0x52, 0x52, 0x3E}, // 103: g
    {0x7F, 0x08, 0x04, 0x04, 0x78}, // 104: h
    {0x00, 0x44, 0x7D, 0x40, 0x00}, // 105: i
    {0x20, 0x40, 0x44, 0x3D, 0x00}, // 106: j
    {0x7F, 0x10, 0x28, 0x44, 0x00}, // 107: k
    {0x00, 0x41, 0x7F, 0x40, 0x00}, // 108: l
    {0x7C, 0x04, 0x78, 0x04, 0x78}, // 109: m
    {0x7C, 0x08, 0x04, 0x04, 0x78}, // 110: n
    {0x38, 0x44, 0x44, 0x44, 0x38}, // 111: o
    {0x7C, 0x14, 0x14, 0x14, 0x08}, // 112: p
    {0x08, 0x14, 0x14, 0x18, 0x7C}, // 113: q
    {0x7C, 0x08, 0x04, 0x04, 0x08}, // 114: r
    {0x48, 0x54, 0x54, 0x54, 0x20}, // 115: s
    {0x04, 0x3F, 0x44, 0x40, 0x20}, // 116: t
    {0x3C, 0x40, 0x40, 0x20, 0x7C}, // 117: u
    {0x1C, 0x20, 0x40, 0x20, 0x1C}, // 118: v
    {0x3C, 0x40, 0x30, 0x40, 0x3C}, // 119: w
    {0x44, 0x28, 0x10, 0x28, 0x44}, // 120: x
    {0x0C, 0x50, 0x50, 0x50, 0x3C}, // 121: y
    {0x44, 0x64, 0x54, 0x4C, 0x44}, // 122: z
    {0x00, 0x08, 0x36, 0x41, 0x00}, // 123: {
    {0x00, 0x00, 0x7F, 0x00, 0x00}, // 124: |
    {0x00, 0x41, 0x36, 0x08, 0x00}, // 125: }
    {0x10, 0x08, 0x08, 0x10, 0x08}  // 126: ~
};


void tft_write_command(const TFT_Display *disp, uint8_t cmd) 
{
    gpio_put(disp->pin_dc, 0); // DC low for command
    gpio_put(disp->pin_cs, 0); // Select display
    spi_write_blocking(disp->spi_port, &cmd, 1);
    gpio_put(disp->pin_cs, 1); // Deselect
}

void tft_write_data(const TFT_Display *disp, const uint8_t *data, size_t length) 
{
    gpio_put(disp->pin_dc, 1); // DC high for data
    gpio_put(disp->pin_cs, 0); // Select display
    spi_write_blocking(disp->spi_port, data, length);
    gpio_put(disp->pin_cs, 1); // Deselect
}

void tft_init(TFT_Display *disp) 
{
    spi_init(disp->spi_port, 40000000); // 40 MHz SPI clock[cite: 1]
    
    // Use the pins passed from your main file struct
    gpio_set_function(disp->pin_sck, GPIO_FUNC_SPI); 
    gpio_set_function(disp->pin_mosi, GPIO_FUNC_SPI); 

    gpio_init(disp->pin_cs);
    gpio_set_dir(disp->pin_cs, GPIO_OUT);
    gpio_put(disp->pin_cs, 1);

    gpio_init(disp->pin_dc);
    gpio_set_dir(disp->pin_dc, GPIO_OUT);
    gpio_put(disp->pin_dc, 0);

    gpio_init(disp->pin_rst);
    gpio_set_dir(disp->pin_rst, GPIO_OUT);
    gpio_put(disp->pin_rst, 1);

    // Hardware Reset Pulse
    gpio_put(disp->pin_rst, 0);
    sleep_ms(50);
    gpio_put(disp->pin_rst, 1);
    sleep_ms(150);

    // Startup commands
    tft_write_command(disp, 0x01); // Software Reset
    sleep_ms(120);

    tft_write_command(disp, 0x11); // Sleep Out
    sleep_ms(120);

    tft_write_command(disp, 0x36);
    uint8_t madctl = 0x28; 
    tft_write_data(disp, &madctl, 1);

    tft_write_command(disp, 0x3A);
    uint8_t pixel_format = 0x55; // 16-bit RGB565
    tft_write_data(disp, &pixel_format, 1);

    tft_write_command(disp, 0x21);

    tft_write_command(disp, 0x29); // Display On
    sleep_ms(20);
}

void tft_set_window(const TFT_Display *disp, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    // Column Address Set (0x2A)
    tft_write_command(disp, 0x2A);
    uint8_t col_data[4] = {
        (uint8_t)(x0 >> 8), (uint8_t)(x0 & 0xFF),
        (uint8_t)(x1 >> 8), (uint8_t)(x1 & 0xFF)
    };
    tft_write_data(disp, col_data, 4);

    // Row Address Set (0x2B)
    tft_write_command(disp, 0x2B);
    uint8_t row_data[4] = {
        (uint8_t)(y0 >> 8), (uint8_t)(y0 & 0xFF),
        (uint8_t)(y1 >> 8), (uint8_t)(y1 & 0xFF)
    };
    tft_write_data(disp, row_data, 4);
}

void tft_clear(const TFT_Display *disp)
{
    // 1. Target the entire screen window
    tft_set_window(disp, 0, 0, disp->width - 1, disp->height - 1);

    // 2. Send RAM Write command
    tft_write_command(disp, 0x2C);

    // 3. Create a full-width line buffer based on disp->width (480 pixels * 2 bytes)
    uint16_t line_buffer[disp->width];
    for (uint16_t i = 0; i < disp->width; i++) {
        line_buffer[i] = BLACK; // Or whatever background color you want
    }

    gpio_put(disp->pin_dc, 1); // Data mode
    gpio_put(disp->pin_cs, 0); // Select display

    // 4. Stream the row buffer down for every vertical row (320 times)
    for (uint16_t y = 0; y < disp->height; y++) {
        spi_write_blocking(disp->spi_port, (uint8_t*)line_buffer, sizeof(line_buffer));
    }

    gpio_put(disp->pin_cs, 1); // Deselect display
}

// Draw a single character (handling full ASCII 32 to 126)
void tft_draw_char(const TFT_Display *disp, int16_t x, int16_t y, char c, uint16_t color, uint16_t bg_color, uint8_t size) 
{
    uint8_t char_index = (c >= 32 && c <= 126) ? (c - 32) : 0;

    uint16_t char_width = 5 * size;
    uint16_t char_height = 7 * size;
    
    tft_set_window(disp, x, y, x + char_width - 1, y + char_height - 1);
    tft_write_command(disp, 0x2C); // RAM Write

    gpio_put(disp->pin_dc, 1);
    gpio_put(disp->pin_cs, 0);

    for (int8_t row = 0; row < 7; row++) {
        for (uint8_t sy = 0; sy < size; sy++) {
            for (int8_t col = 0; col < 5; col++) {
                uint8_t line_data = font5x7[char_index][col];
                bool pixel_on = (line_data & (1 << row)) != 0;
                uint16_t pixel_color = pixel_on ? color : bg_color;

                uint8_t color_bytes[2] = { (uint8_t)(pixel_color >> 8), (uint8_t)(pixel_color & 0xFF) };
                
                for (uint8_t sx = 0; sx < size; sx++) {
                    spi_write_blocking(disp->spi_port, color_bytes, 2);
                }
            }
        }
    }

    gpio_put(disp->pin_cs, 1);
}

// Draw a full string of text
void tft_draw_string(const TFT_Display *disp, int16_t x, int16_t y, const char *str, uint16_t color, uint16_t bg_color, uint8_t size)
{
    int16_t cursor_x = x;
    while (*str) {
        tft_draw_char(disp, cursor_x, y, *str, color, bg_color, size);
        cursor_x += (6 * size); 
        str++;
    }
}

// Helper to draw a single pixel (useful for fine shape outlines)
static void tft_draw_pixel(const TFT_Display *disp, int16_t x, int16_t y, uint16_t color) {
    if (x < 0 || x >= disp->width || y < 0 || y >= disp->height) return;
    tft_set_window(disp, x, y, x, y);
    tft_write_command(disp, 0x2C);
    uint8_t color_bytes[2] = { (uint8_t)(color >> 8), (uint8_t)(color & 0xFF) };
    tft_write_data(disp, color_bytes, 2);
}

// Draw a filled rectangle (or square if w == h)
void tft_fill_rect(const TFT_Display *disp, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (w <= 0 || h <= 0) return;
    
    // Clip to screen boundaries if needed, or let set_window handle it
    tft_set_window(disp, x, y, x + w - 1, y + h - 1);
    tft_write_command(disp, 0x2C);

    uint8_t color_bytes[2] = { (uint8_t)(color >> 8), (uint8_t)(color & 0xFF) };
    
    gpio_put(disp->pin_dc, 1);
    gpio_put(disp->pin_cs, 0);

    // Stream pixels in bulk for maximum speed
    uint32_t total_pixels = (uint32_t)w * (uint32_t)h;
    for (uint32_t i = 0; i < total_pixels; i++) {
        spi_write_blocking(disp->spi_port, color_bytes, 2);
    }

    gpio_put(disp->pin_cs, 1);
}

// Draw an outlined rectangle/square
void tft_draw_rect(const TFT_Display *disp, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    tft_fill_rect(disp, x, y, w, 1, color);         // Top edge
    tft_fill_rect(disp, x, y + h - 1, w, 1, color);   // Bottom edge
    tft_fill_rect(disp, x, y, 1, h, color);         // Left edge
    tft_fill_rect(disp, x + w - 1, y, 1, h, color);   // Right edge
}

// Draw a circle outline using Bresenham's midpoint algorithm
void tft_draw_circle(const TFT_Display *disp, int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    tft_draw_pixel(disp, x0, y0 + r, color);
    tft_draw_pixel(disp, x0, y0 - r, color);
    tft_draw_pixel(disp, x0 + r, y0, color);
    tft_draw_pixel(disp, x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        tft_draw_pixel(disp, x0 + x, y0 + y, color);
        tft_draw_pixel(disp, x0 - x, y0 + y, color);
        tft_draw_pixel(disp, x0 + x, y0 - y, color);
        tft_draw_pixel(disp, x0 - x, y0 - y, color);
        tft_draw_pixel(disp, x0 + y, y0 + x, color);
        tft_draw_pixel(disp, x0 - y, y0 + x, color);
        tft_draw_pixel(disp, x0 + y, y0 - x, color);
        tft_draw_pixel(disp, x0 - y, y0 - x, color);
    }
}

// Draw a filled circle using fast horizontal span lines
void tft_fill_circle(const TFT_Display *disp, int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    // Draw the center vertical span initially
    tft_fill_rect(disp, x0 - r, y0, 2 * r + 1, 1, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        // Draw symmetrical horizontal lines across the circle diameter rows
        tft_fill_rect(disp, x0 - x, y0 + y, 2 * x + 1, 1, color);
        tft_fill_rect(disp, x0 - x, y0 - y, 2 * x + 1, 1, color);
        tft_fill_rect(disp, x0 - y, y0 + x, 2 * y + 1, 1, color);
        tft_fill_rect(disp, x0 - y, y0 - x, 2 * y + 1, 1, color);
    }
}

void tft_draw_heart(const TFT_Display *disp, int16_t x, int16_t y, uint8_t size, uint16_t color) {
    int total_rows = 7 * size;
    
    for (int r = 0; r < total_rows; r++) {
        int logical_row = r / size;
        
        if (logical_row == 0) {
            tft_fill_rect(disp, x + (1 * size), y + r, 2 * size, 1, color);
            tft_fill_rect(disp, x + (5 * size), y + r, 2 * size, 1, color);
            continue;
        }
        
        int16_t span_x = x;
        int16_t span_w = 0;
        
        switch (logical_row) {
            case 1:
            case 2:
                span_x = x;
                span_w = 8 * size;
                break;
            case 3:
                span_x = x + (1 * size);
                span_w = 6 * size;
                break;
            case 4:
                span_x = x + (2 * size);
                span_w = 4 * size;
                break;
            case 5:
                span_x = x + (3 * size);
                span_w = 2 * size;
                break;
            case 6: // Sharp bottom tip tapering to 1 unit wide
                span_x = x + (3.5 * size);
                span_w = 1 * size;
                break;
        }
        
        if (span_w > 0) {
            tft_fill_rect(disp, span_x, y + r, span_w, 1, color);
        }
    }
}