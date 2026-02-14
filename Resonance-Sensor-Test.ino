#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // ESP32 I2C pins: SDA = 21, SCL = 22

  // Trying until the sensor connects
  while (!mpu.begin()) {
    Serial.println("Waiting for MPU6050...");
    delay(1000);
  }
  
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.println("Sensor Ready!");
}

void loop() {
  // Grab fresh data from the MPU6050
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Calculate Total G-Force
  float x = a.acceleration.x/9.81, y = a.acceleration.y/9.81, z = a.acceleration.z/9.81;
  float totalG = sqrt(x*x + y*y + z*z);

  // Read the Heart Rate Sensor from Pin 34
  int pulse = analogRead(34);

  // Print clean, single-line output
  Serial.printf("G-Force: %.2f | Pulse: %d\n", totalG, pulse);

  delay(100); // Wait 1/10th of a second before the next reading
}