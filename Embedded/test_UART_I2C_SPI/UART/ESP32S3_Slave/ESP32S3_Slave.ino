#include <Arduino.h>
#include "Audio.h"
#include "SPIFFS.h"

// I2S Connections for MAX98357A
#define I2S_DOUT 11
#define I2S_BCLK 10
#define I2S_LRC 9

#define UART_BAUD_RATE 9600
#define UART_TX_PIN 18
#define UART_RX_PIN 17
#define AUDIO_BUFFER_SIZE 512

Audio audio;

void setup() {
  Serial.begin(115200);
  Serial1.begin(UART_BAUD_RATE, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(10);

  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
}

void loop() {
  static File audioFile;
  static bool receiving = false;
  static uint8_t buffer[AUDIO_BUFFER_SIZE];
  static size_t bufferIndex = 0;

  if (Serial1.available()) {
    if (!receiving) {
      audioFile = SPIFFS.open("/received_audio.mp3", FILE_WRITE);
      if (!audioFile) {
        Serial.println("Failed to open temporary audio file");
        return;
      }
      receiving = true;
      bufferIndex = 0;
    }

    int bytesRead = Serial1.readBytes(buffer + bufferIndex, AUDIO_BUFFER_SIZE - bufferIndex);
    bufferIndex += bytesRead;

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

