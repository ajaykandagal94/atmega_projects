#ifndef SSD1306_H
#define SSD1306_H

#include <avr/io.h>

#define SSD1306_LASTLINE 0x07
#define SSD1306_COMMAND 0x00
#define SSD1306_DATA 0xC0
#define SSD1306_DATA_CONTINUE 0x40
#define SSD1306_SET_CONTRAST_CONTROL 0x81
#define SSD1306_DISPLAY_ALL_ON_RESUME 0xA4
#define SSD1306_DISPLAY_ALL_ON 0xA5
#define SSD1306_NORMAL_DISPLAY 0xA6
#define SSD1306_INVERT_DISPLAY 0xA7
#define SSD1306_DISPLAY_OFF 0xAE
#define SSD1306_DISPLAY_ON 0xAF
#define SSD1306_NOP 0xE3
#define SSD1306_HORIZONTAL_SCROLL_RIGHT 0x26
#define SSD1306_HORIZONTAL_SCROLL_LEFT 0x27
#define SSD1306_SCROLL_VERT_AND_RIGHT 0x29
#define SSD1306_SCROLL_VERT_AND_LEFT 0x2A
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_SET_VERT_SCROLL_AREA 0xA3
#define SSD1306_SET_LOWER_COLUMN 0x00
#define SSD1306_SET_HIGHER_COLUMN 0x10
#define SSD1306_MEMORY_ADDR_MODE 0x20
#define SSD1306_SET_COLUMN_ADDR 0x21
#define SSD1306_SET_PAGE_ADDR 0x22
#define SSD1306_SET_START_LINE 0x40
#define SSD1306_SET_SEGMENT_REMAP 0xA0
#define SSD1306_SET_MULTIPLEX_RATIO 0xA8
#define SSD1306_COM_SCAN_DIR_INC 0xC0
#define SSD1306_COM_SCAN_DIR_DEC 0xC8
#define SSD1306_SET_DISPLAY_OFFSET 0xD3
#define SSD1306_SET_COM_PINS 0xDA
#define SSD1306_CHARGE_PUMP 0x8D
#define SSD1306_SET_CLOCK_DIV_RATIO 0xD5
#define SSD1306_SET_PRECHARGE_PERIOD 0xD9
#define SSD1306_SET_VCOM_DESELECT 0xDB
#define SSD1306_ADDRESS 0x3C

//this function must be called before using any other functions
void SSD1306_init();

void SSD1306_sendCmd(const unsigned char cmd);

void SSD1306_sendData(const unsigned char data);

void SSD1306_writeStart(const int img_width, const int img_height);

// call SSD1306_writeStart() before using this function
void SSD1306_writeBytes(const unsigned char *data, const int size);

void SSD1306_clearDisp(const int img_width, const int img_height);

void SSD1306_writeFullDisp(const unsigned char *data_arr, const int data_size);

#endif // SSD1306_H