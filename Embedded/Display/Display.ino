// #include "haha.h"
#include "demo.h"

#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
TFT_eSPI tft = TFT_eSPI();

void setup() {
  // put your setup code here, to run once:
  tft.init();
  tft.setRotation(1);

  tft.setSwapBytes(true);
  // tft.fillScreen(TFT_BLACK);
  tft.fillScreen(TFT_WHITE);
}

void loop() {
  // put your main code here, to run repeatedly:
for(int i=0;i<frames;i++)
  {
  delay(1000);
  tft.pushImage(22, 2,animation_width  , animation_height, demo[i]);
  }
}
