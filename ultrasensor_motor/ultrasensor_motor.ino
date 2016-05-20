// ---------------------------------------------------------------------------
// This example code was used to successfully communicate with 15 ultrasonic sensors. You can adjust
// the number of sensors in your project by changing SONAR_NUM and the number of NewPing objects in the
// "sonar" array. You also need to change the pins for each sensor for the NewPing objects. Each sensor
// is pinged at 33ms intervals. So, one cycle of all sensors takes 495ms (33 * 15 = 495ms). The results
// are sent to the "oneSensorCycle" function which currently just displays the distance data. Your project
// would normally process the sensor results in this function (for example, decide if a robot needs to
// turn and call the turn function). Keep in mind this example is event-driven. Your complete sketch needs
// to be written so there's no "delay" commands and the loop() cycles at faster than a 33ms rate. If other
// processes take longer than 33ms, you'll need to increase PING_INTERVAL so it doesn't get behind.
// ---------------------------------------------------------------------------
#include <NewPing.h>
#define SONAR_NUM     2 // Number or sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 50 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).
#define YELLOW 10
#define RED 11
#define SPD_FL 3
#define SPD_FR 5
#define SPD_RL 6
#define SPD_RR 9
#define EN_R 4
#define M1_1 1
#define M1_2 2
#define M2_1 3
#define M2_2 4
#define M3_1 5
#define M3_2 6
#define M4_1 7
#define M4_2 8
#define FWD 0
#define RVS 1
#define OFF 10
unsigned long previousMillis = 0;

unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int cm[SONAR_NUM];         // Where the ping distances are stored.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.
uint8_t dcycle = 0;
NewPing sonar[SONAR_NUM] = {     // Sensor object array.
  NewPing(13, 12, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(5,4,MAX_DISTANCE),
};

void setup() {
  Serial.begin(115200);
  pingTimer[0] = millis() + 75;           // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < SONAR_NUM; i++) // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
    pinMode(YELLOW, OUTPUT); pinMode(RED, OUTPUT); pinMode(M1_1, OUTPUT); pinMode(M1_2, OUTPUT); pinMode(M2_1, OUTPUT); pinMode(M2_2, OUTPUT); pinMode(M3_1, OUTPUT); pinMode(M3_2, OUTPUT);pinMode(M4_1, OUTPUT);pinMode(M4_2, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through all the sensors.
    if (millis() >= pingTimer[i]) {         // Is it this sensor's time to ping?
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
     if (i == 0 && currentSensor == SONAR_NUM - 1) oneSensorCycle(); // Sensor ping cycle complete, do something with the results.
      sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
      currentSensor = i;                          // Sensor being accessed.
      sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
    }
  }
 int a,b;
  // The rest of your code would go here.
  a= cm[0];
  b=cm[1];
if(a < 10) { digitalWrite(YELLOW, HIGH);} else { digitalWrite(YELLOW, LOW);}
if(b < 10) { digitalWrite(RED, HIGH);} else digitalWrite(RED, LOW);
if ((b>=20) && (a >=20))
{
  setdir4M(FWD); setspd4M(dcycle);
  dcycle++; 
  if (dcycle > 254) 
      { dcycle = 255;}

}
else { 
  //Object found --> slow down to 0
  setspd4M(dcycle);
  dcycle--; if (dcycle <1) dcycle =0;
}
if (dcycle ==0)
  {
    //Stopped - Reverse for 0.5seg  - Then rotate 
    if (currentMillis - previousMillis <= 500) // do this loop for up to 1000mS
    { setdir4M(1); setspd4M(100); previousMillis = currentMillis;}
    //A-sensor found object, rotate until distance is not seen --> move forward again
    if(a <10) { setdir4M(6); setspd4M(100); }
    //B-sensor found object, rotate until distance is not see --> move forward again
    if(b <10) {setdir4M(4); setspd4M(100);  }
  }
  
}
void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}
void oneSensorCycle() { // Sensor ping cycle complete, do something with the results.
  for (uint8_t i = 0; i < SONAR_NUM; i++) {
    Serial.print(i);
    Serial.print("=");
    Serial.print(cm[i]);
    Serial.print("cm ");
  }
  Serial.println();
}
void setdir4M(int r) {

  switch(r) {
   //all motors Forward
   case 0: {digitalWrite(M1_1, LOW); digitalWrite(M2_1, LOW); digitalWrite(M3_1, LOW);digitalWrite(M4_1, LOW);digitalWrite(M1_2, HIGH); digitalWrite(M2_2, HIGH);digitalWrite(M3_2, HIGH);digitalWrite(M4_2, HIGH); }
   //all motors Reverse
   case 1: {digitalWrite(M1_2, LOW); digitalWrite(M2_2, LOW);digitalWrite(M3_2, LOW);digitalWrite(M4_2, LOW); digitalWrite(M1_1, HIGH); digitalWrite(M2_1, HIGH); digitalWrite(M3_1, HIGH);digitalWrite(M4_1, HIGH);}
   //Left Motors stopped -- Right Motors Forward
   case 2: {digitalWrite(M1_1, LOW); digitalWrite(M2_1, HIGH); } //TODO
   //Right Motors Stopped -- Left Motors Forward
   case 3: {} //TODO
   //Left Motors Reverse -- Right Motors Stopped
   case 4: {} //TODO
   //Right Motors Reverse -- Left Motors Stopped
   case 5: {} //TODO
   //Left Motors Reverse -- Right Motors Forward
   case 6: {} //TODO
   //Right Motors Reverse -- Left Motors Forward
   case 7: {} //TODO
   case 10: {digitalWrite(M1_2, LOW); digitalWrite(M2_2, LOW);digitalWrite(M3_2, LOW);digitalWrite(M4_2, LOW); digitalWrite(M1_1, LOW); digitalWrite(M2_1, LOW); digitalWrite(M3_1, LOW);digitalWrite(M4_1, LOW);  }   
}
}
void setspd4M(int d) {
    analogWrite(SPD_FL,d); analogWrite(SPD_FR,d); analogWrite(SPD_RL,d); analogWrite(SPD_RR,d); 
}
void setspd2M(int side, int d){
  switch(side) {
    case 1: {analogWrite(SPD_FL,d); analogWrite(SPD_FR,d);} //Front Motors
    case 2: {analogWrite(SPD_RL,d); analogWrite(SPD_RR,d); }//Rear Motors
    case 3: {analogWrite(SPD_FL,d); analogWrite(SPD_RL,d); }//Left Motors
    case 4: {analogWrite(SPD_FR,d); analogWrite(SPD_RR,d); }//Right Motors
  }
}

