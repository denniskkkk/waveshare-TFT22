#include <SPI.h>
#include "lcdlib.h"
#include "lcdfont.h"
/*
   Waveshare SPI LCD22 TFT module
   BD663474 hitachi 320x240pixel 256K colour
   pin 1 GND 0V
   pin 3 3.3V
   pin 35 SPISCLK -> PA5 SCLK
   pin 33 SPIDATAIN -> PA7 MOSI
   pin 31 RES -> RESET PC13
   pin 37 CS -> PA4
   pin 39 RS -> PA3
*/
/* SPT2046 touch panel controller
   pin 19 TIRQ
   pin 21 TBUSY
   pin 23 TCS
   pin 25 TCLK
   pin 27 DIN
   pin 29 TOUT

*/

void LCD_WRITE_REG(unsigned int index)
{
  unsigned int value_index;
  digitalWrite (PA3, LOW);
  digitalWrite (PA4, LOW);
  value_index = index;
  value_index = value_index >> 8;
  SPI.transfer (value_index);    //00000000 000000000
  value_index = index;
  value_index &= 0x00ff;
  SPI.transfer (index);
  digitalWrite (PA3, HIGH);
  digitalWrite (PA4, HIGH);
}

void LCD_SEND_COMMAND(unsigned int index, unsigned int data)
{
  //select command register
  digitalWrite (PA3, LOW);
  digitalWrite (PA4, LOW);
  SPI.transfer (index >> 8);  //00000000 000000000
  SPI.transfer (index);
  digitalWrite (PA4, HIGH);
  //send data
  digitalWrite (PA3, HIGH);
  digitalWrite (PA4, LOW);
  SPI.transfer(data >> 8);  //00000000 000000000
  SPI.transfer(data);
  digitalWrite (PA4, HIGH);
}

void LCD_WRITE_COMMAND(unsigned int index, unsigned int data)
{
  //select command register
  digitalWrite (PA3, LOW);
  digitalWrite (PA4, LOW);
  SPI.transfer (index >> 8);  //00000000 000000000
  SPI.transfer (index);
  digitalWrite (PA4, HIGH);
  //send data
  digitalWrite (PA3, HIGH);
  digitalWrite (PA4, LOW);
  SPI.transfer (data >> 8);  //00000000 000000000
  SPI.transfer (data);
  digitalWrite (PA4, HIGH);
}

void LCD_WRITE_DATA(unsigned int data)
{
  SPI.transfer(data >> 8);  //00000000 000000000
  SPI.transfer(data);
}

void lcd_init(void)
{
  LCD_WRITE_COMMAND( 0x000, 0x0001 ); /* oschilliation start */
  delay (1);
  /* Power settings */
  LCD_WRITE_COMMAND( 0x100, 0x0000 ); /*power supply setup*/
  LCD_WRITE_COMMAND( 0x101, 0x0000 );
  LCD_WRITE_COMMAND( 0x102, 0x3110 );
  LCD_WRITE_COMMAND( 0x103, 0xe200 );
  LCD_WRITE_COMMAND( 0x110, 0x009d );
  LCD_WRITE_COMMAND( 0x111, 0x0022 );
  LCD_WRITE_COMMAND( 0x100, 0x0120 );
  delay( 2 );
  LCD_WRITE_COMMAND( 0x100, 0x3120 );
  delay( 8 );
  /* Display control */
  LCD_WRITE_COMMAND( 0x001, 0x0100 );
  LCD_WRITE_COMMAND( 0x002, 0x0000 );
  LCD_WRITE_COMMAND( 0x003, 0x1230 );
  LCD_WRITE_COMMAND( 0x006, 0x0000 );
  LCD_WRITE_COMMAND( 0x007, 0x0101 );
  LCD_WRITE_COMMAND( 0x008, 0x0808 );
  LCD_WRITE_COMMAND( 0x009, 0x0000 );
  LCD_WRITE_COMMAND( 0x00b, 0x0000 );
  LCD_WRITE_COMMAND( 0x00c, 0x0000 );
  LCD_WRITE_COMMAND( 0x00d, 0x0018 );
  /* LTPS control settings */
  LCD_WRITE_COMMAND( 0x012, 0x0000 );
  LCD_WRITE_COMMAND( 0x013, 0x0000 );
  LCD_WRITE_COMMAND( 0x018, 0x0000 );
  LCD_WRITE_COMMAND( 0x019, 0x0000 );

  LCD_WRITE_COMMAND( 0x203, 0x0000 );
  LCD_WRITE_COMMAND( 0x204, 0x0000 );

  LCD_WRITE_COMMAND( 0x210, 0x0000 );
  LCD_WRITE_COMMAND( 0x211, 0x00ef );
  LCD_WRITE_COMMAND( 0x212, 0x0000 );
  LCD_WRITE_COMMAND( 0x213, 0x013f );
  LCD_WRITE_COMMAND( 0x214, 0x0000 );
  LCD_WRITE_COMMAND( 0x215, 0x0000 );
  LCD_WRITE_COMMAND( 0x216, 0x0000 );
  LCD_WRITE_COMMAND( 0x217, 0x0000 );

  // Gray scale settings
  LCD_WRITE_COMMAND( 0x300, 0x5343);
  LCD_WRITE_COMMAND( 0x301, 0x1021);
  LCD_WRITE_COMMAND( 0x302, 0x0003);
  LCD_WRITE_COMMAND( 0x303, 0x0011);
  LCD_WRITE_COMMAND( 0x304, 0x050a);
  LCD_WRITE_COMMAND( 0x305, 0x4342);
  LCD_WRITE_COMMAND( 0x306, 0x1100);
  LCD_WRITE_COMMAND( 0x307, 0x0003);
  LCD_WRITE_COMMAND( 0x308, 0x1201);
  LCD_WRITE_COMMAND( 0x309, 0x050a);

  /* RAM access settings */
  LCD_WRITE_COMMAND( 0x400, 0x4027 );
  LCD_WRITE_COMMAND( 0x401, 0x0000 );
  LCD_WRITE_COMMAND( 0x402, 0x0000 ); /* First screen drive position (1) */
  LCD_WRITE_COMMAND( 0x403, 0x013f ); /* First screen drive position (2) */
  LCD_WRITE_COMMAND( 0x404, 0x0000 );

  LCD_WRITE_COMMAND( 0x200, 0x0000 );
  LCD_WRITE_COMMAND( 0x201, 0x0000 );

  LCD_WRITE_COMMAND( 0x100, 0x7120 );
  LCD_WRITE_COMMAND( 0x007, 0x0103 );
  delay(1);
  LCD_WRITE_COMMAND( 0x007, 0x0113 );
  delay(1);

}

void lcd_clear_screen(unsigned int color_background)
{
  unsigned int i, j;
  LCD_WRITE_COMMAND(0x210, 0x00);
  LCD_WRITE_COMMAND(0x212, 0x0000);
  LCD_WRITE_COMMAND(0x211, 0xEF);
  LCD_WRITE_COMMAND(0x213, 0x013F);
  LCD_WRITE_COMMAND(0x200, 0x0000);
  LCD_WRITE_COMMAND(0x201, 0x0000);
  digitalWrite (PA3, LOW);
  LCD_WRITE_REG(0x202); //RAM Write index
  digitalWrite (PA4, LOW);
  for (i = 0; i < 320; i++)
  {
    for (j = 0; j < 240; j++)
    {
      LCD_WRITE_DATA( color_background );
    }
  }
  digitalWrite (PA3, LOW);
  digitalWrite (PA4, HIGH);
}

void lcd_set_cursor(unsigned char x, unsigned int y)
{
  if ( (x > 320) || (y > 240) )
  {
    return;
  }
  LCD_WRITE_COMMAND( 0x200, x );
  LCD_WRITE_COMMAND( 0x201, y );
}

void lcd_display_char(  unsigned char ch_asc,
                        unsigned int color_front,
                        unsigned int color_background,
                        unsigned char postion_x,
                        unsigned char postion_y)
{
  unsigned char i, j, b;
  const unsigned char *p = 0;
  LCD_WRITE_COMMAND(0x210, postion_x * 8); //x start point
  LCD_WRITE_COMMAND(0x212, postion_y * 16); //y start point
  LCD_WRITE_COMMAND(0x211, postion_x * 8 + 7); //x end point
  LCD_WRITE_COMMAND(0x213, postion_y * 16 + 15); //y end point
  LCD_WRITE_COMMAND(0x200, postion_x * 8);
  LCD_WRITE_COMMAND(0x201, postion_y * 16);
  digitalWrite (PA3, LOW);
  LCD_WRITE_REG(0x202);
  digitalWrite (PA4, LOW);
  p = ascii;
  p += ch_asc * 16;
  for (j = 0; j < 16; j++)
  {
    b = *(p + j);
    for (i = 0; i < 8; i++)
    {
      if ( b & 0x80 )
      {
        LCD_WRITE_DATA(color_front);
      }
      else
      {
        LCD_WRITE_DATA(color_background);
      }
      b = b << 1;
    }
  }
  digitalWrite (PA4, HIGH);
}

void lcd_display_string(unsigned char *str, unsigned int color_front,
                        unsigned int color_background, unsigned char x, unsigned char y )
{
  while (*str)
  {
    lcd_display_char( *str, color_front, color_background, x, y);
    if (++x >= 30)
    {
      x = 0;
      if (++y >= 20)
      {
        y = 0;
      }
    }
    str ++;
  }
}
//
// test colours
const unsigned int color[] =
{
  // 0 ~ 262143, 0x00000 ~ 0x3FFFF
  0xf800, //red
  0x07e0, //green
  0x001f, //blue
  0xffe0, //yellow
  0x0000, //black
  0xffff, //white
  0x07ff, //light blue
  0xf81f  //purple
};

void lcd_display_test(void)
{
  unsigned int temp, num;
  unsigned int i;
  unsigned char n;

  lcd_clear_screen(RED);
  //delay(6);

  LCD_WRITE_COMMAND(0x210, 0x00);
  LCD_WRITE_COMMAND(0x212, 0x0000);
  LCD_WRITE_COMMAND(0x211, 0xEF);
  LCD_WRITE_COMMAND(0x213, 0x013F);

  LCD_WRITE_COMMAND(0x200, 0x0000);
  LCD_WRITE_COMMAND(0x201, 0x0000);

  digitalWrite (PA3, LOW);
  LCD_WRITE_REG(0x202); //RAM Write index
  digitalWrite (PA4, LOW);
  //dispaly color bar
  for (n = 0; n < 8; n++)
  {
    digitalWrite (PA4, LOW);
    digitalWrite (PA3, HIGH);
    temp = color[n];
    for (num = 40 * 240; num > 0; num--)
    {
      LCD_WRITE_DATA(temp);
    }
  }
  //display pure color
  for (n = 0; n < 8; n++)
  {
    temp = color[n];
    for (i = 0; i < 240; i++)
    {
      for (num = 0; num < 320; num++)
      {
        LCD_WRITE_DATA(temp);
      }
    }
    // delay(6);
  }
  lcd_clear_screen(GREEN);
  digitalWrite (PA4, HIGH);
  digitalWrite (PA3, HIGH);

}

// test text
unsigned char txt[80] = "A quick brown fox ABCDEFG12345";
unsigned char txt1[80] = "******************************";
unsigned char txt2[80] = "!@#$%^&*()ABCDEFGHIJKLMNOP<>_+";

void testtext () {
  int c;
  for (c = 0; c < 18; c++) {
    lcd_display_string(txt, WHITE, color [c / 4 ], 0, c );
  }
  lcd_display_string(txt1, WHITE, BLUE, 0, 18 );
  lcd_display_string(txt2, WHITE, VIOLET, 0, 19 );
}

void setup() {
  SerialUSB.end();
  SerialUSB.begin(115200);
  pinMode (PA4, OUTPUT); // #CS
  pinMode (PA3, OUTPUT); // RS= command
  pinMode (PC13, OUTPUT);
  digitalWrite (PA4, HIGH);
  digitalWrite (PA3, HIGH);
  digitalWrite (PC13, LOW);
  delay (500);
  digitalWrite (PC13, HIGH);
  SPI.begin();
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  //SPI.setClockDivider (2);
  //SPI.endTransaction();
  lcd_init();
  lcd_clear_screen(BLUE);
  SerialUSB.println ("*****lcd test*********");
  delay (1000);
}

void loop() {
  lcd_init();
  delay (10);
  testtext();
  delay (1000);
  lcd_display_test();
  //delay (1000);

}
