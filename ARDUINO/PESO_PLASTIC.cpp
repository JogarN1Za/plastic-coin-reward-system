#include <Servo.h>

// Pin assignments
const int IR_SENSOR_PIN = 2; // Pin connected to IR OUT
const int SERVO_PIN = 9;     // Pin connected to Servo signal

// Servo angles for coin dispensation mechanism
const int REST_ANGLE = 0;    // Retracted angle (waiting)
const int PUSH_ANGLE = 90;   // Extended angle (dispenses 1 peso)

Servo coinServo;
int coinCount = 0; // Tracks total 1-peso coins dispensed

void setup() {
  // Initialize Serial Terminal communication at 9600 baud rate
  Serial.begin(9600);
  
  // Wait brief moment for Serial port to establish
  delay(1000);

  // Terminal welcome & setup logs
  Serial.println("==================================================");
  Serial.println("   PLASTIC-TO-COIN REWARD SYSTEM INITIALIZING...  ");
  Serial.println("==================================================");

  pinMode(IR_SENSOR_PIN, INPUT);
  
  coinServo.attach(SERVO_PIN);
  coinServo.write(REST_ANGLE);

  Serial.println("[SYSTEM STATUS] Servo attached to Pin 9 -> Position: 0 DEG");
  Serial.println("[SYSTEM STATUS] IR Sensor initialized on Pin 2");
  Serial.println("[SYSTEM STATUS] System ready. Waiting for plastic insertion...\n");
}

void loop() {
  // Read state from IR sensor module (LOW = Object Detected, HIGH = Empty)
  int sensorState = digitalRead(IR_SENSOR_PIN);

  if (sensorState == LOW) {
    // 1. Detection Event Logging
    Serial.println("--------------------------------------------------");
    Serial.println("[SENSOR DETECT] Plastic item detected in chute!");
    
    // 2. Servo Push Operation
    Serial.println("[ACTION] Rotating servo to 90 DEG (Pushing 1-Peso Coin)...");
    coinServo.write(PUSH_ANGLE);
    delay(600); // Wait for physical motion to complete
    
    // 3. Servo Retract Operation
    Serial.println("[ACTION] Returning servo to 0 DEG (Retracting arm)...");
    coinServo.write(REST_ANGLE);
    delay(500); 

    // 4. Update and log total payout counter
    coinCount++;
    Serial.print("[SUCCESS] Coin Dispensed! Total 1-Peso coins paid out: ");
    Serial.println(coinCount);
    
    // 5. Cooldown to prevent multiple payouts for a single item
    Serial.println("[WAIT] Cooldown active (2s delay to prevent duplicate trigger)...");
    delay(2000); 
    
    Serial.println("--------------------------------------------------");
    Serial.println("[SYSTEM STATUS] Ready for next plastic item...\n");
  }

  delay(50); // Small stability delay for loop execution
}