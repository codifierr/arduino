#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
const long sound_speed = 0.034; // sound speed in cm/s
const int tank_height = 140; // tank height in cm
const int stop_distance = 25; // distance to stop in cm
const int start_distance = 90; // distance to start in cm
const int max_distance = 300; // maximum distance in cm

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
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
  distance = duration * sound_speed / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor

  if (distance > max_distance) {
    Serial.println("Out of range");
  } else {
    if (distance < stop_distance) {
      Serial.println("Stop");
    } else if (distance > start_distance) {
      Serial.println("Start");
    }
    Serial.print("Distance: ");
    Serial.println(distance);
    Serial.println(" cm");
  }
}