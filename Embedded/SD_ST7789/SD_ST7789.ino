#include <SPI.h>
#include <SD.h> 
#include <TFT_eSPI.h> // Hardware-specific library

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library
File file;

#define TFT_CS   14 // Replace with your TFT CS pin
#define SD_CS    38
#define SD_MOSI  47
#define SD_MISO  48
#define SD_SCK   21

int i;
static int a = 0;
int image_width = 240;
int image_height = 200;
uint16_t * haha1;
uint16_t * haha2;

void setup() {
  Serial.begin(115200);
  
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  
  SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  
  digitalWrite(SD_CS, LOW);
  if(!SD.begin(SD_CS)){
    Serial.println("Card Mount Failed");
    return;
  }
  digitalWrite(SD_CS, HIGH);
  
  digitalWrite(TFT_CS, LOW);
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_WHITE);
  digitalWrite(TFT_CS, HIGH);

  haha1 = (uint16_t *) ps_malloc (image_height * image_width * sizeof (uint16_t));
  haha2 = (uint16_t *) ps_malloc (image_height * image_width * sizeof (uint16_t));
  if(psramInit()){
    Serial.println("\nPSRAM is correctly initialized");
  }else{
    Serial.println("PSRAM not available");
  }
  
  digitalWrite(SD_CS, LOW);
  file = SD.open("/haha1.h", FILE_READ);
  if(!file){
    Serial.println("Failed to open file for reading:haha1.h");
  } else {
    Serial.println("Open file for reading:haha1.h");
  }

  i = 0;
  while(file.available() ) {
    String hexString = file.readStringUntil(' ');
    haha1[i] = (uint16_t) strtol(hexString.c_str(), NULL, 16);    
    i++;
  }
  file.close();

    file = SD.open("/haha2.h", FILE_READ);
  if(!file){
    Serial.println("Failed to open file for reading:haha2.h");
  } else {
    Serial.println("Open file for reading:haha2.h");
  }

  i = 0;
  while(file.available() ) {
    String hexString = file.readStringUntil(' ');
    haha2[i] = (uint16_t) strtol(hexString.c_str(), NULL, 16);    
    i++;
  }
  file.close();

  digitalWrite(SD_CS, HIGH);
  
  digitalWrite(TFT_CS, LOW);
  while(a < 9)
  {
    if(a % 2 == 0){
    // for(int y = image_height - 1; y >= 0; y--) {
    // for(int x = 0; x < image_width; x++) {
    //   tft.drawPixel(x + 40, y + 25, haha1[(image_height - y - 1) * image_width + x]);
    //   }
    // }
    tft.pushImage(40,25, image_width, image_height, haha1);
  }

    if(a % 2 != 0){
      // for(int y = 0; y < image_height; y++) {
      // for(int x = 0; x < image_width; x++) {
      //   tft.drawPixel(x + 40, y + 25, haha2[y * image_width + x]);
      //   }
      // }
      tft.pushImage(40,25, image_width, image_height, haha2);
    }
    a++;
    delay(700);
  }
  digitalWrite(TFT_CS, HIGH);
  free(haha1);
  free(haha2);
}

void loop(){

}