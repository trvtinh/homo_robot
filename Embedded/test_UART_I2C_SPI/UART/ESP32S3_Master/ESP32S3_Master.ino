#include <Arduino.h>
#include <SD.h>

// SD Card Reader connections
#define SD_MISO 21
#define SD_SCK  14
#define SD_MOSI 13
#define SD_CS 12

#define UART_BAUD_RATE 9600
#define UART_TX_PIN 18
#define UART_RX_PIN 17
#define AUDIO_BUFFER_SIZE 512

void setup() {
  Serial.begin(115200);
  Serial1.begin(UART_BAUD_RATE, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);
  delay(500);

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
    Serial1.write(buffer, bytesRead);
    // delay(10);  // Adjust delay as necessary for your setup
  }

  audioFile.close();
  Serial1.write("END");  // Signal the end of file transfer
  Serial.println("Audio file sent");
}

void loop() {
  // Send the audio file when a command is received (for example)
      // sendAudioFile("/output.mp3");

}

