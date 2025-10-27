/*!
 * @file DFRobot_RGBLCD1602.h
 * @brief DFRobot_RGBLCD1602 class infrastructure
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @maintainer [yangfeng](feng.yang@dfrobot.com)
 * @version  V1.0
 * @date  2021-09-24
 * @url https://github.com/DFRobot/DFRobot_RGBLCD1602
 */

#ifndef __DFRobot_RGBLCD1602_H__
#define __DFRobot_RGBLCD1602_H__
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <driver/i2c.h>
//#include <Arduino.h>
//#include <Wire.h>
//#include "Print.h"

/*!
 *  @brief Device I2C Arress
 */
#define LCD_ADDRESS     (0x7c>>1)

/*!
 *  @brief color define
 */ 
#define WHITE           0
#define RED             1
#define GREEN           2
#define BLUE            3

#define REG_MODE1       0x00
#define REG_MODE2       0x01
#define REG_OUTPUT      0x08

/*!
 *  @brief commands
 */
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

/*!
 *  @brief flags for display entry mode
 */
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

/*!
 *  @brief flags for display on/off control
 */
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

/*!
 *  @brief flags for display/cursor shift
 */
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

/*!
 *  @brief flags for function set
 */
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

class DFRobot_RGBLCD1602 
{

public:
  DFRobot_RGBLCD1602(uint8_t RGBAddr,uint8_t lcdCols=16,uint8_t lcdRows=2,uint8_t lcdAddr=LCD_ADDRESS);
  void init();
  void clear();
  void home();
  void noDisplay();
  void display();
  void stopBlink();
  void blink();
  void noCursor();
  void cursor();
  void scrollDisplayLeft();
  void scrollDisplayRight();
  void leftToRight();
  void rightToLeft();
  void noAutoscroll();
  void autoscroll();
  void customSymbol(uint8_t location, uint8_t charmap[]);
  void setCursor(uint8_t col, uint8_t row);
  void setRGB(uint8_t r, uint8_t g, uint8_t b);
  void print(const char* s);
  void setPWM(uint8_t color, uint8_t pwm){setReg(color, pwm);}
  void setColor(uint8_t color);
  void closeBacklight(){setRGB(0, 0, 0);}
  void setColorWhite(){setRGB(255, 255, 255);}
  size_t write(uint8_t data);
  void command(uint8_t data);
  void setBacklight(bool mode);
  //void setPWM(uint8_t color, uint8_t pwm){setReg(color, pwm); if(_RGBAddr==0x6B){setReg(0x07, pwm);}}      // set pwm
   
private:
  void begin(uint8_t rows, uint8_t charSize = LCD_5x8DOTS);
  void send(uint8_t *data, uint8_t len);
  void setReg(uint8_t addr, uint8_t data);

  uint8_t _showFunction;
  uint8_t _showControl;
  uint8_t _showMode;
  uint8_t _initialized;
  uint8_t _numLines,_currLine;
  uint8_t _lcdAddr;
  uint8_t _RGBAddr;
  uint8_t _cols;
  uint8_t _rows;
  //TwoWire *_pWire;
  i2c_port_t _i2cPort;

public:
 /* uint8_t _lcdAddr;
  uint8_t _RGBAddr;
  uint8_t _cols;
  uint8_t _rows;
  //TwoWire *_pWire;
  i2c_port_t _i2cPort;*/
  uint8_t REG_RED      =   0 ;       // pwm2
  uint8_t REG_GREEN    =   0 ;       // pwm1
  uint8_t REG_BLUE     =   0 ;       // pwm0
  uint8_t REG_ONLY     =   0 ;       // pwm0
};

#endif
