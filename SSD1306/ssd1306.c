void SSD1306_init()
{
    SSD1306_sendCmd(SSD1306_DISPLAY_OFF);
    SSD1306_sendCmd(SSD1306_SET_CLOCK_DIV_RATIO);
    SSD1306_sendCmd(0x80);
    SSD1306_sendCmd(SSD1306_SET_MULTIPLEX_RATIO);
    SSD1306_sendCmd(0x3F);
    SSD1306_sendCmd(SSD1306_SET_DISPLAY_OFFSET);
    SSD1306_sendCmd(0x0);
    SSD1306_sendCmd(SSD1306_SET_START_LINE | 0x00);
    SSD1306_sendCmd(SSD1306_CHARGE_PUMP);
    SSD1306_sendCmd(0x14);
    SSD1306_sendCmd(SSD1306_MEMORY_ADDR_MODE);
    SSD1306_sendCmd(0x00);
    SSD1306_sendCmd(SSD1306_SET_SEGMENT_REMAP | 0x01);
    SSD1306_sendCmd(SSD1306_COM_SCAN_DIR_DEC);
    SSD1306_sendCmd(SSD1306_SET_COM_PINS);
    SSD1306_sendCmd(0x12);
    SSD1306_sendCmd(SSD1306_SET_CONTRAST_CONTROL);
    SSD1306_sendCmd(0xCF);
    SSD1306_sendCmd(SSD1306_SET_PRECHARGE_PERIOD);
    SSD1306_sendCmd(0xF1);
    SSD1306_sendCmd(0xDB);
    SSD1306_sendCmd(0x40);
    SSD1306_sendCmd(SSD1306_DISPLAY_ALL_ON_RESUME);
    SSD1306_sendCmd(SSD1306_NORMAL_DISPLAY);
    SSD1306_sendCmd(SSD1306_DEACTIVATE_SCROLL);
    SSD1306_sendCmd(SSD1306_DISPLAY_ON);
}

void SSD1306_sendCmd(const unsigned char cmd)
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

inline void SSD1306_sendData(const unsigned char data)
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

void SSD1306_writeStart(const int img_width, const int img_height)
{
    SSD1306_sendCmd(SSD1306_SET_COLUMN_ADDR);
    SSD1306_sendCmd(0);                         // start at left edge of screen
    SSD1306_sendCmd(img_width - 1);             // end at right edge of screen

    SSD1306_sendCmd(SSD1306_SET_PAGE_ADDR);
    SSD1306_sendCmd(0);                         // start at the top row
    SSD1306_sendCmd((img_height / 8) - 1);      // end at the bottom row

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

void SSD1306_writeBytes(const unsigned char *data, const int size)
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

void SSD1306_clearDisp(const int img_width, const int img_height)
{
    unsigned char disp_data[1024];
    memset(disp_data, 0, sizeof(disp_data));

    SSD1306_sendCmd(SSD1306_DISPLAY_OFF);
    SSD1306_writeStart(128, 64);
    SSD1306_writeBytes(disp_data, sizeof(disp_data))
    SSD1306_sendCmd(SSD1306_DISPLAY_ON);
}

void SSD1306_writeFullDisp(const unsigned char *data_arr, const int data_size)
{
    unsigned char disp_data[1024];
    memset(disp_data, 0, sizeof(disp_data));

    SSD1306_sendCmd(SSD1306_DISPLAY_OFF);
    SSD1306_writeStart(128, 64);
    SSD1306_writeBytes(data_arr, data_size)
    SSD1306_sendCmd(SSD1306_DISPLAY_ON);
}