/* 範例11: 用電子紙顯示溫溼度 */

#include <stdio.h>
#include <Wire.h>
#include "EPD_1in9.h"
#include "Adafruit_SHT31.h"

Adafruit_SHT31 sht3x = Adafruit_SHT31();

// 初始化電子紙的流程
void InitEpdSeq () {
  EPD_1in9_lut_5S();
	EPD_1in9_Write_Screen(DSPNUM_1in9_off);
	delay(500);
	
	EPD_1in9_lut_GC();
	
	EPD_1in9_Write_Screen1(DSPNUM_1in9_on);
	delay(500);
	
	EPD_1in9_Write_Screen(DSPNUM_1in9_off);
	delay(500);
	
	EPD_1in9_lut_DU_WB();
}

void setup() {
  Serial.begin(115200);
	Wire.begin(); 

  // 確認是否找到感測器
  if (! sht3x.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT3x");
    while (1) delay(1);
  }
  
  // 顯示加熱器開關狀態
  Serial.print("Heater Enabled State: ");
  if (sht3x.isHeaterEnabled())
    Serial.println("Heater ENABLED");
  else
    Serial.println("Heater DISABLED");

	Serial.println("e-Paper init and clear");
	GPIOInit();
	EPD_1in9_init();
	
	InitEpdSeq();

}

unsigned char epd_cmd[15];

unsigned char* makeEpdStr(float temp, float humi) {
  int digit;

  memset(epd_cmd, 0x00, 15);
  
  // 解析溫度
  if (temp >= 100) {
    epd_cmd[0] = EPD_NUM[1][1];
  }
  
  if (temp >= 10) {
    digit = temp;
    digit = (digit % 100) / 10;
    memcpy(&epd_cmd[1], EPD_NUM[digit], 2);
  }
  
  if (temp >= 0) {
    digit = temp;
    digit = (digit % 10);
    memcpy(&epd_cmd[3], EPD_NUM[digit], 2);
  }
  
  digit = temp * 10;
  digit = digit % 10;
  memcpy(&epd_cmd[11], EPD_NUM[digit], 2);  

  // 解析濕度
  if (humi >= 10) {
    digit = humi;
    digit = (digit % 100) / 10;
    memcpy(&epd_cmd[5], EPD_NUM[digit], 2);
  }

  if (humi >= 0) {
    digit = humi;
    digit = (digit % 10);
    memcpy(&epd_cmd[7], EPD_NUM[digit], 2);
  } else {
    memcpy(&epd_cmd[7], EPD_NUM[0], 2);
  }

  digit = humi * 10;
  digit = digit % 10;
  memcpy(&epd_cmd[9], EPD_NUM[digit], 2);

  // 顯示符號
  epd_cmd[4] |= 0x20;   // 溫度小數點
  epd_cmd[8] |= 0x20;   // 濕度小數點
  epd_cmd[10] |= 0x20;  // 濕度符號 "%"
  epd_cmd[13] |= 0x05;  // 攝氏溫度符號

  return epd_cmd;
}

#define FLUSH_INTERVAL    180000  // 單位: ms
#define SENSE_INTERVAL    5000    // 單位: ms

void loop()
{
  static float t, h;
  static unsigned long epd_flush_timer = millis() + FLUSH_INTERVAL;
  static unsigned long sht_sense_timer = 0;

  // 偵測溫溼度數值並顯示到螢幕上
  if (sht_sense_timer <= millis()) {
    sht_sense_timer = millis() + SENSE_INTERVAL;
    t = sht3x.readTemperature();
    h = sht3x.readHumidity();

    if (! isnan(t)) {  // check if 'is not a number'
      Serial.print("Temp *C = "); Serial.print(t); Serial.print("\t\t");
    } else { 
      Serial.println("Failed to read temperature");
    }
    
    if (! isnan(h)) {  // check if 'is not a number'
      Serial.print("Hum. % = "); Serial.println(h);
    } else { 
      Serial.println("Failed to read humidity");
    }

    makeEpdStr(t, h);  
    EPD_1in9_Write_Screen(epd_cmd);
    EPD_1in9_sleep();
  }
  
  // 刷新螢幕
  if (epd_flush_timer <= millis()) {
    epd_flush_timer = millis() + FLUSH_INTERVAL;
    InitEpdSeq();
    EPD_1in9_Write_Screen(epd_cmd);
    EPD_1in9_sleep();
  }

}
