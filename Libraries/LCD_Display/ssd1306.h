
#include "stm32f0xx_hal.h"
#include "fonts.h"

//	This library was written and adapted by Olivier Van den Eede 2016 for use
//	with Stm32 microcontrollers and uses the HAL-i2c libraries.
//
//	This library is made to use a ssd1306 controlled oled display.
//	Before use, the following definitions must be correctly set.
//	Like the i2c port used and the size of the screen.
//
//	The library uses 2 files (fonts.c / h) This describes 3 fonts.
//	These fonts can be used in this way: 	- Font_7x10
//											- Font_11x18
//											- Font_16x26

#ifndef ssd1306
#define ssd1306

// I2c port name in main program generated by Cube
#define SSD1306_I2C_PORT		hi2c1
// I2C address 
#define SSD1306_I2C_ADDR        0x78
// SSD1306 width in pixels
#define SSD1306_WIDTH           128
// SSD1306 LCD height in pixels
#define SSD1306_HEIGHT          64


//
//	Enum for the colors of the Black and White screen
//
typedef enum {
	Black = 0x00, /*!< Black color, no pixel */
	White = 0x01  /*!< Pixel is set. Color depends on LCD */
} SSD1306_COLOR;

//
//	Structure for keeping changes
//
typedef struct {
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Inverted;
	uint8_t Initialized;
} SSD1306_t;

//	I2C port
extern I2C_HandleTypeDef SSD1306_I2C_PORT;

//
//	The functions definitions
//
uint8_t ssd1306_Init(void);
void ssd1306_Fill(SSD1306_COLOR color);
void ssd1306_UpdateScreen(void);
void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color);
char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color);
char ssd1306_WriteString(char* str, FontDef Font, SSD1306_COLOR color);
void ssd1306_SetCursor(uint8_t x, uint8_t y);

static void ssd1306_WriteCommand(uint8_t command);

#endif
