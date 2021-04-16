#include <avr/io.h>
#include <util/delay.h>

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

void oledInit()
{
    oledSendCmd(SSD1306_DISPLAY_OFF);
    oledSendCmd(SSD1306_SET_CLOCK_DIV_RATIO);
    oledSendCmd(0x80);
    oledSendCmd(SSD1306_SET_MULTIPLEX_RATIO);
    oledSendCmd(0x3F);
    oledSendCmd(SSD1306_SET_DISPLAY_OFFSET);
    oledSendCmd(0x0);
    oledSendCmd(SSD1306_SET_START_LINE | 0x00);
    oledSendCmd(SSD1306_CHARGE_PUMP);
    oledSendCmd(0x14);
    oledSendCmd(SSD1306_MEMORY_ADDR_MODE);
    oledSendCmd(0x00);
    oledSendCmd(SSD1306_SET_SEGMENT_REMAP | 0x01);
    oledSendCmd(SSD1306_COM_SCAN_DIR_DEC);
    oledSendCmd(SSD1306_SET_COM_PINS);
    oledSendCmd(0x12);
    oledSendCmd(SSD1306_SET_CONTRAST_CONTROL);
    oledSendCmd(0xCF);
    oledSendCmd(SSD1306_SET_PRECHARGE_PERIOD);
    oledSendCmd(0xF1);
    oledSendCmd(0xDB);
    oledSendCmd(0x40);
    oledSendCmd(SSD1306_DISPLAY_ALL_ON_RESUME);
    oledSendCmd(SSD1306_NORMAL_DISPLAY);
    oledSendCmd(SSD1306_DEACTIVATE_SCROLL);
    oledSendCmd(SSD1306_DISPLAY_ON);
}

void oledSendCmd(const unsigned char cmd)
{
    // i2c start
    TWCR = (1 << TWSTA) | (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT))); // wait until TWINT set

    //send addr
    TWDR = SSD1306_ADDRESS << 1;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT))); // wait until TWINT set

    //init for cmd
    TWDR = 0x00;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT))); // wait until TWINT set

    //send cmd
    TWDR = cmd;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT))); // wait until TWINT set

    // i2c stop
    TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
}

inline void oledSendData(const unsigned char data)
{
    // i2c start
    TWCR = (1 << TWSTA) | (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT))); // wait until TWINT set

    //send addr
    TWDR = SSD1306_ADDRESS << 1;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT))); // wait until TWINT set

    //init for cmd
    TWDR = 0xC0;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT))); // wait until TWINT set

    //send cmd
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT))); // wait until TWINT set

    // i2c stop
    TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT))); // wait until TWINT set
}

void oledWriteBytesStart(const int img_width, const int img_height)
{
    oledSendCmd(SSD1306_SET_COLUMN_ADDR);
    oledSendCmd(0);                         // start at left edge of screen
    oledSendCmd(img_width - 1);             // end at right edge of screen

    oledSendCmd(SSD1306_SET_PAGE_ADDR);
    oledSendCmd(0);                         // start at the top row
    oledSendCmd((img_height / 8) - 1);      // end at the bottom row

    // i2c start
    TWCR = ((1 << TWINT) | (1 << TWSTA) | (1 << TWEN));
    while (!(TWCR & (1 << TWINT))); // wait until TWINT set

    // send slave addr
    TWDR = SSD1306_ADDRESS << 1;
    TWCR = ((1 << TWINT) | (1 << TWEN));
    while (!(TWCR & (1 << TWINT))); // wait until TWINT set

    // send D/C_sentinal byte for data stream
    TWDR = SSD1306_DATA_CONTINUE;
    TWCR = ((1 << TWINT) | (1 << TWEN));
    while (!(TWCR & (1 << TWINT))); // wait until TWINT set
}

// call oledWriteBytesStart() before using this function
void oledWriteBytes(const unsigned char *data, const int count)
{    
    for (int i = 0; i < count;)
    {
        // i2c start
        TWCR = ((1 << TWINT) | (1 << TWSTA) | (1 << TWEN)); // send START
        while (!(TWCR & (1 << TWINT))); // delay until TWINT set

        // send slave addr
        TWDR = SSD1306_ADDRESS << 1;
        TWCR = ((1 << TWINT) | (1 << TWEN));
        while (!(TWCR & (1 << TWINT))); // delay until TWINT set

        // send D/C_sentinal byte for data stream
        TWDR = 0x40; 
        TWCR = ((1 << TWINT) | (1 << TWEN));
        while (!(TWCR & (1 << TWINT))); // delay until TWINT set

        // 16 bytes per I2C transmission__64 transmissions per screen
        for (int j = 0; j < 16 && i < count; j++)
        {
            TWDR = *(data + i);
            TWCR = ((1 << TWINT) | (1 << TWEN));
            while (!(TWCR & (1 << TWINT))); // delay until TWINT set
            i++; 
        }

        // i2c stop
        TWCR = ((1 << TWINT) | (1 << TWEN) | (1 << TWSTO));
        while (!(TWCR & (1 << TWINT))); // delay until TWINT set
    }
}

void oledClearDisp(const int img_width, const int img_height)
{
    unsigned char disp_data[1024];
    memset(disp_data, 0, sizeof(disp_data));

    oledSendCmd(SSD1306_DISPLAY_OFF);
    oledWriteBytesStart(128, 64);
    oledWriteBytes(disp_data, sizeof(disp_data))
    oledSendCmd(SSD1306_DISPLAY_ON);
}

void oledWriteFullDisp(const unsigned char *data_arr, const int data_size)
{
    unsigned char disp_data[1024];
    memset(disp_data, 0, sizeof(disp_data));

    oledSendCmd(SSD1306_DISPLAY_OFF);
    oledWriteBytesStart(128, 64);
    oledWriteBytes(data_arr, data_size)
    oledSendCmd(SSD1306_DISPLAY_ON);
}

int main()
{
    oledInit();
    oledClearDisp();

    //write your logic to display on oled

    return 0;
}