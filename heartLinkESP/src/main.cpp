//#include "VectorHaptics.h"
//#include "ESP32Profiles.h"
#include <Arduino.h>

/*VectorHaptics vh;
VHChannels channels;
ESP32Profile myBoard;*/
#define heartPin 2
int heartVal = 0;
// JUMPER CABLE ON IO0  
void setup() {
    Serial.begin(115200);
    pinMode(heartPin, INPUT);
    /*
    channels.add(1, {25});       // Channel 1 on GPIO pin 25
    vh.init(&myBoard, {&channels});
    */
}

void loop() {
  heartVal = analogRead(heartPin);
  Serial.println(heartVal);
  delay(1000);
  /*
    vh.play({PULSE(20, 1, 1)});  // Short strong pulse
    delay(1000);                  // Wait 1 second
    */

}