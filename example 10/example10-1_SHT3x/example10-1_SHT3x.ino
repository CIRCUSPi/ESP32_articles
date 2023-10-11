/* 範例10-1: 用SHT3x測量溫溼度 
   預設I2C地址: 0x44  */

#include <Wire.h>
#include "Adafruit_SHT31.h"

Adafruit_SHT31 sht3x = Adafruit_SHT31();  // 使用預設I2C接腳

void setup() {
  Serial.begin(115200);

  while (!Serial)
    delay(10);     // 等待直到Serial console開啟

  Serial.println("SHT3x test");
  if (! sht3x.begin(0x44)) {  // 若找到設備，begin()將返回true
    Serial.println("Couldn't find SHT3x");
    while (1) delay(1);
  }
}


void loop() {
  float t = sht3x.readTemperature();  // 讀取目前溫度
  float h = sht3x.readHumidity();     // 讀取目前濕度

  if (! isnan(t)) {  // 確認是否為數字
    Serial.print("Temp = "); Serial.print(t); Serial.println(" °C");  // 顯示溫度
  } else { 
    Serial.println("Failed to read temperature");
  }
  
  if (! isnan(h)) {  // 確認是否為數字
    Serial.print("Hum. = "); Serial.print(h); Serial.println(" %RH"); // 顯示濕度
  } else { 
    Serial.println("Failed to read humidity"); 
  }
  Serial.println();
  delay(1000);
}