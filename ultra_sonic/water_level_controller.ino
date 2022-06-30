// System automatically starts the sketch when the board is powered on.
// The sketch is responsible for initializing the hardware and setting up the water level monitoring
// At the board startup it will turn on the pump if water level is less than 100%.
// There is a dry run protection logic which will turn of the pump if there is no water flow for more than 6 minutes.
// The dry run protection timeout is set to 90 minutes

// The scheduling logic will be implemented in the future which will be based on the time of day using the RTC.
// Author : Satyendra Singh

#define echoPin 2  // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3  // attach pin D3 Arduino to pin Trig of HC-SR04
#define relayPin 4 // attach pin D4 Arduino to pin Relay

// define variables
bool pump_running = false; // variable for the pump status
bool pump_switch = false; // variable for the pump switch status
int pump_start_level = 0; // variable for the pump start level
int dry_run_check_interval = 180; // variable for the dry run check interval value 180 for 360 seconds = 6 minutes
int dry_run_check_counter = 0; // variable for the dry run counter
bool dry_run_wait = false; // variable for the dry run flag
int dry_run_wait_counter = 0; // variable for the dry run wait counter
int dry_run_wait_interval = 2700; // variable for the dry run wait interval value 2700 for 5400 seconds = 90 minutes

// define constants
const int max_range = 300; // constant for the maximum range of the sensor
const float speed_of_sound = 0.034; // constant for the speed of sound
const int water_stop_distance = 15; // constant for the water stop distance
const int tank_height = 140; // constant for the tank height
const int water_level_low = 100; // constant for the water level low


void setup()
{
    pinMode(trigPin, OUTPUT);                         // Sets the trigPin as an OUTPUT
    pinMode(echoPin, INPUT);                          // Sets the echoPin as an INPUT
    pinMode(relayPin, OUTPUT);                        // Sets the relayPin as an OUTPUT
    Serial.begin(9600);                               // Serial Communication is starting with 9600 of baudrate speed
    Serial.println(" Ultrasonic Sensor HC-SR04"); // print some text in Serial Monitor
    Serial.println(" with Arduino UNO R3");
    // intial delay of 5 seconds before starting pump
    Serial.println(" Initializing... ");
    int i = 5;
    while (i > 0)
    {
        Serial.println(i);
        delay(1000);
        i--;
    }
    Serial.println(" Initialization complete");

    setUpWaterLevelController(); // call the function to set up the water level controller
}

void loop()
{
    waterLevelController(); // call the function to control the water level
    // Get status every two seconds
    delay(2000); // delay for 2 seconds
}

void setUpWaterLevelController()
{
    int distance = getDistance();
    int level = getWaterLevel(distance);
    pump_start_level = getWaterLevelInPercentage(level);
    startPump(pump_start_level);                                    // turn on pump
    Serial.println(" Maximum Range: " + String(max_range) + " cm"); // print maximum range in Serial Monitor
}

void startPump(int level)
{
    if (isTankFull(level))
    {
        Serial.println(" Tank is full");
        stopPump();
        return;
    }
    Serial.println(" Starting Pump");
    delay(2000); // delay for 2 seconds
    digitalWrite(relayPin, HIGH);
    pump_running = true;
    dry_run_check_counter = dry_run_check_interval;
    pump_start_level = level;
    pump_switch = true;
    Serial.println(" Pump Started");
}

void stopPump()
{
    Serial.println(" Stopping Pump");
    delay(2000); // delay for 2 seconds
    digitalWrite(relayPin, LOW);
    pump_running = false;
    // reset dry run counter
    dry_run_check_counter = 0;
    pump_switch = false;
    Serial.println(" Pump Stopped");
}

void pausePump(){
    Serial.println(" Pausing Pump");
    delay(2000); // delay for 2 seconds
    digitalWrite(relayPin, LOW);
    pump_running = false;
    // reset dry run counter
    dry_run_check_counter = 0;
    Serial.println(" Pump Paused");
}

long getDistance()
{
    digitalWrite(trigPin, LOW); // Sets the trigPin to LOW
    delayMicroseconds(2); // waits 2 micro seconds
    digitalWrite(trigPin, HIGH); // Sets the trigPin to HIGH
    delayMicroseconds(10); // waits 10 micro seconds
    digitalWrite(trigPin, LOW); // Sets the trigPin to LOW
    long duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel in microseconds
    int distance = duration * speed_of_sound; // Calculating the distance
    distance = distance / 2; // Divide by 2 to remove the sound travel time of the echo to the distance
    return distance; // returns the distance in cm
}

int getWaterLevel(int distance)
{
    int water_level = tank_height - distance;
    return water_level;
}

int getWaterLevelInPercentage(int level) {

    int per = (level + water_stop_distance) * 100 / tank_height;
    if (per <= 100)
    {
       return per;
    }
    return 100;
}

bool isTankFull(int level)
{
    if (level >= 100)
    {
        return true;
    }
    return false;
}

void printPumpStatus()
{
    if (pump_running)
    {
        Serial.println(" Pump is running ");
    }
    else
    {
        Serial.println(" Pump is not running ");
    }
}

bool isWaterFlowing(int level)
{
    if (level > pump_start_level)
    {
        return true;
    }
    return false;
}

void processDryRunProtect(int level)
{
    Serial.println(" Dry dry run protect engaged");
    if (dry_run_wait)
    {
        //Serial.println(" Dry run wait: " + String(dry_run_wait_counter));
       
        if (dry_run_wait_counter > 0)
        {
            dry_run_wait_counter--;
        }
        else
        {
            dry_run_wait = false;
            dry_run_wait_counter = 0;
            dry_run_check_counter = dry_run_check_interval;
            startPump(level);
        }
        return;
    }

    //Serial.println(" Dry run check: " + String(dry_run_check_counter));

    if (dry_run_check_counter > 0)
    {
        dry_run_check_counter--;
    }
    else
    {
        if (isWaterFlowing(level))
        {
            Serial.println(" Water is flowing ");
            dry_run_check_counter = dry_run_check_interval;
            dry_run_wait = false;
            pump_start_level = level;
        } else
        {
            Serial.println(" Water is not flowing ");
            // stop pump and enable dry run wait
            pausePump();
            dry_run_wait = true;
            dry_run_wait_counter = dry_run_wait_interval;
        }
    }
}

void waterLevelController(){
    int distance = getDistance();
    int level = getWaterLevel(distance);
    int per = getWaterLevelInPercentage(level);
    // max distance is 300 cm
    if (distance > max_range)
    {
        Serial.println(" Tank Height is more than supported range of " + String(max_range) + " cm");
        // As a precaution stop pump
        stopPump();
    }
    else if (level < 0)
    {
        Serial.println(" Sensor distance is more than the tank capacity");
        // As a precaution stop pump
        stopPump();
    }
    else
    {
        // stop at water_stop_distance in cm
        if (distance <= water_stop_distance && pump_running)
        {
            stopPump();
        }
        // start if water level goes beyond water_level_low in cm
        else if (distance > water_level_low && !pump_running && !dry_run_wait)
        {
            pump_start_level = level;
            startPump(per);
        }

        // water level in percentage
        int per = getWaterLevelInPercentage(level);
        Serial.print(" Water level is " + String(per) + "%");

        if (pump_switch)
        {
            processDryRunProtect(per);
        }

        printPumpStatus();

        // print the water distance in Serial Monitor
        // Serial.print(" Distance: " + String(distance) + " cm");
    }
}
