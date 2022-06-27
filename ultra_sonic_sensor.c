#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04
#define relayPin 4 //attach pin D4 Arduino to pin Relay

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
// always start pump at initialization
int pump_status; // variable for the pump status

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(relayPin, OUTPUT); // Sets the relayPin as an OUTPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
  pump_status = 1; // set pump status to 1 (ON)
  digitalWrite(relayPin,  HIGH); // turn on pump
}
void loop() {
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
// max distance is 300 cm
  if (distance > 300) {
    Serial.println("Out of range");
  } else {
    // stop at 25 cm
    if (distance < 25 && pump_status == 1) {
      Serial.println("Stop Pump");
      digitalWrite(relayPin, LOW); // turn off pump
      pump_status = 0;
      
      // start if water level goes beyond 90 cm
    } else if (distance > 90 && pump_status == 0) {
      Serial.println("Start Pump");
      digitalWrite(relayPin,  HIGH); // turn on pump
      pump_status = 1;
    }
    if (pump_status == 1) {
      Serial.println("Pump is ON");
    } else {
      Serial.println("Pump is OFF");
    }
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print("--");
  }
  delay(2000); // delay for 2 seconds
}