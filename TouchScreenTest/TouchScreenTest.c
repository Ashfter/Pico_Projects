#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define PIN_SPI_SCK   2
#define PIN_SPI_MOSI  3
#define PIN_LCD_CS    5
#define PIN_LCD_DC    6
#define PIN_LCD_RST   7

#define LCD_WIDTH      320
#define LCD_HEIGHT     480
#define BOX_SIZE       5

void lcd_send_cmd(uint8_t cmd) {
    gpio_put(PIN_LCD_DC, 0);
    gpio_put(PIN_LCD_CS, 0);
    spi_write_blocking(spi0, &cmd, 1);
    gpio_put(PIN_LCD_CS, 1);
}

void lcd_send_data(const uint8_t *data, size_t len) {
    gpio_put(PIN_LCD_DC, 1);
    gpio_put(PIN_LCD_CS, 0);
    spi_write_blocking(spi0, data, len);
    gpio_put(PIN_LCD_CS, 1);
}

void lcd_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    uint8_t data[4];
    lcd_send_cmd(0x2A);
    data[0] = (x0 >> 8) & 0xFF; data[1] = x0 & 0xFF;
    data[2] = (x1 >> 8) & 0xFF; data[3] = x1 & 0xFF;
    lcd_send_data(data, 4);

    lcd_send_cmd(0x2B);
    data[0] = (y0 >> 8) & 0xFF; data[1] = y0 & 0xFF;
    data[2] = (y1 >> 8) & 0xFF; data[3] = y1 & 0xFF;
    lcd_send_data(data, 4);
}

void lcd_clear(uint16_t color) {
    lcd_set_window(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    lcd_send_cmd(0x2C);
    uint8_t high_byte = color >> 8;
    uint8_t low_byte = color & 0xFF;
    
    gpio_put(PIN_LCD_DC, 1);
    gpio_put(PIN_LCD_CS, 0);
    for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++) {
        spi_write_blocking(spi0, &high_byte, 1);
        spi_write_blocking(spi0, &low_byte, 1);
    }
    gpio_put(PIN_LCD_CS, 1);
}

// Draw a solid color block
void lcd_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    lcd_set_window(x, y, x + w - 1, y + h - 1);
    lcd_send_cmd(0x2C);
    
    uint8_t high_byte = color >> 8;
    uint8_t low_byte = color & 0xFF;
    
    gpio_put(PIN_LCD_DC, 1);
    gpio_put(PIN_LCD_CS, 0);
    for (int i = 0; i < w * h; i++) {
        spi_write_blocking(spi0, &high_byte, 1);
        spi_write_blocking(spi0, &low_byte, 1);
    }
    gpio_put(PIN_LCD_CS, 1);
}

void lcd_init() {
    spi_init(spi0, 32 * 1000 * 1000);
    gpio_set_function(PIN_SPI_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SPI_MOSI, GPIO_FUNC_SPI);

    gpio_init(PIN_LCD_CS); gpio_set_dir(PIN_LCD_CS, GPIO_OUT);
    gpio_init(PIN_LCD_DC); gpio_set_dir(PIN_LCD_DC, GPIO_OUT);
    gpio_init(PIN_LCD_RST); gpio_set_dir(PIN_LCD_RST, GPIO_OUT);

    gpio_put(PIN_LCD_RST, 0);
    sleep_ms(50);
    gpio_put(PIN_LCD_RST, 1);
    sleep_ms(150);

    lcd_send_cmd(0x11);
    sleep_ms(120);
    
    uint8_t pixel_format = 0x55; // 16-bit color
    lcd_send_cmd(0x3A);
    lcd_send_data(&pixel_format, 1);

    lcd_send_cmd(0x29);
    
    // Clear screen to white (0xFFFF)
    lcd_clear(0xFFFF); 
}

int main() {
    stdio_init_all();
    sleep_ms(500);
    
    lcd_init();

    int x = 0;
    int y = 0;
    int step = 10; // Speed of movement

    while (true) {
        // Clear previous box spot by painting it white over a small area
        lcd_fill_rect(x, y, BOX_SIZE, BOX_SIZE, 0xFFFF);

        // Move along the perimeter: Top edge (Left to Right)
        if (y == 0 && x < LCD_WIDTH - BOX_SIZE) {
            x += step;
            if (x >= LCD_WIDTH - BOX_SIZE) { x = LCD_WIDTH - BOX_SIZE; y = 0; }
        }
        // Right edge (Top to Bottom)
        else if (x == LCD_WIDTH - BOX_SIZE && y < LCD_HEIGHT - BOX_SIZE) {
            y += step;
            if (y >= LCD_HEIGHT - BOX_SIZE) { y = LCD_HEIGHT - BOX_SIZE; x = LCD_WIDTH - BOX_SIZE; }
        }
        // Bottom edge (Right to Left)
        else if (y == LCD_HEIGHT - BOX_SIZE && x > 0) {
            x -= step;
            if (x < 0) { x = 0; y = LCD_HEIGHT - BOX_SIZE; }
        }
        // Left edge (Bottom to Top)
        else if (x == 0 && y > 0) {
            y -= step;
            if (y < 0) { y = 0; x = 0; }
        }

        // Draw red box (0xF800) at new position
        lcd_fill_rect(x, y, BOX_SIZE, BOX_SIZE, 0xF800);
        
        sleep_ms(15); // Control animation speed
    }
}