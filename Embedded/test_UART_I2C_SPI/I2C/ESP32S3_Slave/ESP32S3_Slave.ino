#include <Arduino.h>
#include <Wire.h>
#include "Audio.h"
#include "SPIFFS.h"

// I2S Connections for MAX98357A
#define I2S_DOUT 11
#define I2S_BCLK 10
#define I2S_LRC 12

// #define I2C_SDA 16
// #define I2C_SCL 19
#define I2C_SLAVE_ADDR 0x04
#define AUDIO_BUFFER_SIZE 32

Audio audio;

volatile bool receiving = false;
volatile size_t bufferIndex = 0;
uint8_t buffer[AUDIO_BUFFER_SIZE];
File audioFile;

void receiveEvent(int howMany) {
  while (Wire.available()) {
    if (!receiving) {
      audioFile = SPIFFS.open("/received_audio.mp3", FILE_WRITE);
      if (!audioFile) {
        Serial.println("Failed to open temporary audio file");
        return;
      }
      receiving = true;
      bufferIndex = 0;
    }

    buffer[bufferIndex++] = Wire.read();

    // Check if we received the end signal
    if (bufferIndex >= 3 && strncmp((char *)(buffer + bufferIndex - 3), "END", 3) == 0) {
      audioFile.write(buffer, bufferIndex - 3);  // Write remaining data without "END"
      audioFile.close();
      receiving = false;

      // Play the received audio file
      audio.connecttoFS(SPIFFS, "/received_audio.mp3");
      while (audio.isRunning()) {
        audio.loop();
      }

      SPIFFS.remove("/received_audio.mp3");  // Clean up
      break;
    }

    // Write buffer to file if full
    if (bufferIndex == AUDIO_BUFFER_SIZE) {
      audioFile.write(buffer, AUDIO_BUFFER_SIZE);
      bufferIndex = 0;
    }
  }
}


void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SLAVE_ADDR);
//   Wire.setClock(100000);  // Set I2C clock speed
  Wire.onReceive(receiveEvent);

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(10);

  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
}

void loop() {
  // Nothing to do in the loop, everything is handled in the receiveEvent
}