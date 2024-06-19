#include <Arduino.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>

// SD Card Reader connections
#define SD_MISO 21
#define SD_SCK  14
#define SD_MOSI 13
#define SD_CS 12

// #define I2C_SDA 16
// #define I2C_SCL 19
#define I2C_SLAVE_ADDR 0x04
#define AUDIO_BUFFER_SIZE 128

void setup() {
  Serial.begin(115200);
  Wire.begin();

  SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, SPI, 4000000)) {
    Serial.println("Card Mount Failed");
    return;
  }

  Serial.println("SD Card Initialized");
  sendAudioFile("/birth.mp3");
}

void sendAudioFile(const char* filename) {
  File audioFile = SD.open(filename, FILE_READ);
  if (!audioFile) {
    Serial.println("Failed to open audio file");
    return;
  }

  uint8_t buffer[AUDIO_BUFFER_SIZE];
  size_t bytesRead;

  while ((bytesRead = audioFile.read(buffer, AUDIO_BUFFER_SIZE)) > 0) {
    Wire.beginTransmission(I2C_SLAVE_ADDR);
    for (size_t i = 0; i < bytesRead; ++i) {
      Wire.write(buffer, bytesRead);
    }
    Wire.endTransmission();
    delay(10);  // Adjust delay as necessary for your setup
  }

  audioFile.close();
  // Send "END" signal
  Wire.beginTransmission(I2C_SLAVE_ADDR);
  Wire.write((uint8_t*)"END", 3);
  Wire.endTransmission();
  Serial.println("Audio file sent");
}

void loop() {
  // Send the audio file when a command is received (for example)
//   if (Serial.available()) {
//     String command = Serial.readString();
//     command.trim();
//     if (command == "send") {
//       sendAudioFile("/birth.mp3");
//     }
//   }
}