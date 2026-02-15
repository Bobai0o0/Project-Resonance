#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_GC9A01A.h"

// PIN DEFINITIONS (Titan Board to Round LCD)
#define TFT_SCLK 22  // Screen 'SCL' 
#define TFT_MOSI 21  // Screen 'SDA' 
#define TFT_CS   5   // Screen 'CS' 
#define TFT_DC   4   // Screen 'DC' 
#define TFT_RST  -1  // Screen 'RES' is wired to Titan's hardware RESET pin

// Tell the ESP32 to route the graphics data to these specific pins
Adafruit_GC9A01A tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// TACTICAL COLOR PALETTE
// (These screens use 16-bit color hex codes)
#define COLOR_BLACK  0x0000
#define COLOR_GREEN  0x07E0
#define COLOR_RED    0xF800
#define COLOR_WHITE  0xFFFF

void setup() {
  Serial.begin(115200);
  Serial.println("Booting Resonance Titan Display...");

  // Wake up the screen
  tft.begin();
  
  // Set rotation (0, 1, 2, or 3) to make sure top is actually top
  tft.setRotation(0); 
  
  // Start with a clean black background
  tft.fillScreen(COLOR_BLACK);
}

void loop() {
  // STATE 1: SAFE (Green Ring Demo)
  tft.fillScreen(COLOR_BLACK);
  
  // Draw a tactical 5-pixel thick Green ring around the edge
  for(int i = 0; i < 5; i++) {
    tft.drawCircle(120, 120, 115 - i, COLOR_GREEN); 
  }
  
  // Print SAFE text in the middle
  tft.setTextColor(COLOR_GREEN);  
  tft.setTextSize(4); 
  tft.setCursor(70, 105); 
  tft.print("SAFE");
  
  delay(3000); // Hold the safe screen for 3 seconds

  // STATE 2: IMPACT (Red Flash Demo)
  // Flash the entire background solid red
  tft.fillScreen(COLOR_RED); 
  
  // Print WARNING text
  tft.setTextColor(COLOR_WHITE);  
  tft.setTextSize(4); 
  tft.setCursor(40, 80); 
  tft.print("IMPACT!");
  
  tft.setTextSize(2);
  tft.setCursor(85, 130);
  tft.print("SOS...");
  
  delay(3000); // Hold the warning screen for 3 seconds
}