/* 範例10-3: 在OLED上顯示溫濕度資訊
 */

#include <U8g2lib.h>
#include "Adafruit_SHT31.h"
#include <Wire.h>

#define ImgWIDTH 28
#define ImgHEIGHT 28

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
Adafruit_SHT31 sht3x = Adafruit_SHT31();

// 溫度圖片
static const unsigned char PROGMEM tem_icon[] = { /* 0X20,0X01,0X1C,0X00,0X1C,0X00, */
0X00,0X3C,0X00,0X00,0X00,0XFE,0X00,0X00,0X00,0XC6,0X00,0X00,0X00,0X83,0X3C,0X00,
0X00,0X83,0X3C,0X00,0X00,0X83,0X00,0X00,0X00,0X83,0X00,0X00,0X00,0X93,0X3C,0X00,
0X00,0X93,0X00,0X00,0X00,0X93,0X00,0X00,0X00,0X93,0X3C,0X00,0X00,0X93,0X00,0X00,
0X00,0X93,0X00,0X00,0X00,0X93,0X3C,0X00,0X00,0X93,0X3C,0X00,0X00,0X93,0X00,0X00,
0X00,0X93,0X01,0X00,0X80,0X91,0X03,0X00,0XC0,0X38,0X03,0X00,0XC0,0X7C,0X06,0X00,
0X40,0XC6,0X06,0X00,0X40,0XC6,0X06,0X00,0XC0,0X46,0X06,0X00,0XC0,0X7C,0X06,0X00,
0X80,0X38,0X03,0X00,0X80,0X83,0X03,0X00,0X00,0XFF,0X01,0X00,0X00,0X7C,0X00,0X00,
};

// 濕度圖片
static const unsigned char PROGMEM hum_icon[] = { /* 0X20,0X01,0X1C,0X00,0X1C,0X00, */
0X00,0X60,0X00,0X00,0X00,0X60,0X00,0X00,0X00,0XF0,0X00,0X00,0X00,0X90,0X00,0X00,
0X00,0X98,0X01,0X00,0X00,0X08,0X01,0X00,0X00,0X0C,0X03,0X00,0X00,0X04,0X02,0X00,
0X00,0X02,0X04,0X00,0X00,0X02,0X04,0X00,0X00,0X01,0X08,0X00,0X00,0X01,0X08,0X00,
0X80,0X00,0X10,0X00,0X80,0X1C,0X16,0X00,0X40,0X26,0X23,0X00,0X60,0X26,0X61,0X00,
0X20,0XBC,0X40,0X00,0X20,0XC0,0X40,0X00,0X20,0X60,0X40,0X00,0X20,0X20,0X40,0X00,
0X20,0X90,0X43,0X00,0X20,0X58,0X46,0X00,0X60,0X4C,0X66,0X00,0X40,0XC4,0X23,0X00,
0X80,0X00,0X10,0X00,0X80,0X01,0X18,0X00,0X00,0X0E,0X07,0X00,0X00,0XF8,0X01,0X00,
};
void setup(void) {
  Serial.begin(115200);

  if (! sht3x.begin(0x44)) {  // 若找到設備，begin()將返回true
    Serial.println("Couldn't find SHT3x");
    while (1) delay(1);
  }

  u8g2.begin();
}

void loop(void) {
  float t = sht3x.readTemperature();  // 讀取目前溫度
  float h = sht3x.readHumidity();     // 讀取目前濕度

  u8g2.setFont(u8g2_font_luBS12_tr); // 設定字型 (12 pixel)
  u8g2.firstPage();
  do {
    //繪圖
    u8g2.drawXBMP(0,0, ImgWIDTH, ImgHEIGHT, tem_icon);  
    u8g2.drawXBMP(0,36, ImgWIDTH, ImgHEIGHT, hum_icon);
    // 顯示溫度
    if (! isnan(t)) {  
      u8g2.setCursor(32, 21);
      u8g2.print(t, 2);
      u8g2.drawCircle(98, 4, 2, U8G2_DRAW_ALL );
      u8g2.drawStr(100, 21, "C");
    }
    // 顯示濕度
    if (! isnan(h)) {  
      u8g2.setCursor(32, 57);
      u8g2.print(h, 2);
      u8g2.drawStr(100,57,"%");
    }
  } while (u8g2.nextPage());
  delay(2000);
}