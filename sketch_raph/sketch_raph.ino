
#include <Wire.h>
#define PCF8591 (0x90 >> 1)  // I2C address of the PCF8591

#define MOSFET_GATE D4 // Change to the actual pin used

void setup() {
  Serial.begin(115200); // Initialize serial communication at 115200 bits per second
  
  pinMode(MOSFET_GATE, OUTPUT); // Set the MOSFET gate pin as output
  digitalWrite(MOSFET_GATE, LOW); // Ensure it's off initially

  //Wire.begin(D2, D1); // Initialize I2C as master with SDA on D2 and SCL on D1
  Wire.begin(); // Initialize I2C

}

 // Define thresholds based on your observations and joystick behavior
int lowerThreshold = 100; // Adjust based on your joystick's behavior for left/forward
int upperThreshold = 240; // Adjust based on your joystick's behavior for right/backward
// int deadZone = 10; // Dead zone around the neutral to prevent jitter


void loop() {
  // int value0 = readPCF8591Channel(0); // Read from channel 0, we'll consider X axis
  // int value1 = readPCF8591Channel(1); // Read from channel 1, we'll consider Y axis
  int xValue = readPCF8591Channel(0); // Assuming channel 0 for X-axis
  int yValue = readPCF8591Channel(1); // Assuming channel 1 for Y-axis

  // Now you can distinguish the readings
  // Serial.print("Channel 0: ");
  // Serial.println(value0);
  // Serial.print("Channel 1: ");
  // Serial.println(value1);

  // Check X-axis
  if(xValue > upperThreshold) {
    Serial.println("Left");
  } else if(xValue < lowerThreshold) {
    Serial.println("Right");
  } else {
    Serial.println("X Neutral");
  }

  if(yValue < lowerThreshold) {
    Serial.println("Forward");
    digitalWrite(MOSFET_GATE, HIGH); // Turn on the MOSFET, driving the car forward
  } else if(yValue > upperThreshold) {
    Serial.println("Backward");
    digitalWrite(MOSFET_GATE, LOW); // Turn off the MOSFET, stopping the car
  } else {
    Serial.println("Y Neutral");
    digitalWrite(MOSFET_GATE, LOW); // Turn off the MOSFET, stopping the car
  }

  delay(1000); // Wait a second before the next read
  // Wire.beginTransmission(PCF8591);
  // Wire.write(0x04); // Control byte to read from channel 0
  // Wire.endTransmission();
  // Wire.requestFrom(PCF8591, 1); // Request 1 byte from PCF8591
  // while(Wire.available()) {
  //   int value = Wire.read(); // Read analog value
  //   Serial.println(value);
  // }
  // delay(1000); // Wait a second before the next read
}

int readPCF8591Channel(byte channel) {
  Wire.beginTransmission(PCF8591);
  Wire.write(0x40 | channel); // Control byte to read from the specified channel
  Wire.endTransmission();

  Wire.requestFrom(PCF8591, 2); // Request 2 bytes; the first is a "dummy" reading
  
  Wire.read(); // discard the first reading
  int value = Wire.read(); // Read the actual analog value
  
  return value;
}
