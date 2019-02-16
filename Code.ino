#include <Time.h>
#include <TimeLib.h>


/*
    Add something here about IDEA Hacks and authors

    Remember to change pins for LEDs on different Arduinos.
*/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);


//#define YELLOW_LED 11
//#define RED_LED 12
#define trigPin 13
#define echoPin 10
#define motor 7
#define RED_LED 5
#define BLUE_LED 3
#define GREEN_LED 6

int brightness = 255;
int gBright = 0;
int rBright = 0;
int bBright = 0;
int fadeSpeed = 10;
const int buzzer = 9; //buzzer to arduino pin 9
const int SLOW = 400;
const int FAST = 200;
const int FEWLOOPS = 3;
const int MANYLOOPS = 5;
/*
void displaySensorDetails(void)
{

  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
  }
 */

struct report {
    int hr = 0;
    int minut = 0;
    int sec = 0;
    int d = 0;
    int m = 0;
    int yr = 0; 
    double latitude = 0;
    double longitude = 0;
};

void create_report(struct report* rep){
    

  rep->hr = hour();
  rep->minut = minute();
  rep->sec = second();
  rep->d = day();
  rep->m = month();
  rep->yr = year();
  
}
void wreck(){
  sensors_event_t event;
  accel.getEvent(&event);

  
  
  while ((event.acceleration.y >= 8) || (event.acceleration.y <= -8)) {
      TurnOn();
      delay(5000);
      TurnOff();
      
      tone(buzzer, 1000); // Send 1KHz sound signal...
      delay(1000);        // ...for 1 sec
      noTone(buzzer);     // Stop sound...

      /* Delay before the next sample */
      delay(100);

  }  
}

void TurnOn() { 
   for (int i = 0; i < 256; i++) {
       analogWrite(RED_LED, rBright);
       rBright +=1;
       delay(fadeSpeed);
   }
 
   for (int i = 0; i < 256; i++) {
       analogWrite(BLUE_LED, bBright);
       bBright += 1;
       delay(fadeSpeed);
   } 

   for (int i = 0; i < 256; i++) {
       analogWrite(GREEN_LED, gBright);
       gBright +=1;
       delay(fadeSpeed);
   } 
}

void TurnOff() {
   for (int i = 0; i < 256; i++) {
       analogWrite(GREEN_LED, brightness);
       analogWrite(RED_LED, brightness);
       analogWrite(BLUE_LED, brightness);
 
       brightness -= 1;
       delay(fadeSpeed);
   }
}

void buzz(int buzzSpd, int numLoops){
  //Buzzes the handlebar
for (int i = 0; i < numLoops; i++){
  digitalWrite(motor,HIGH);
  digitalWrite(RED_LED,HIGH);
  delay(buzzSpd);
  digitalWrite(motor,LOW);
  digitalWrite(RED_LED,LOW);
  //din't delay last time thru loop
  if (i+1 < numLoops)
    delay(buzzSpd);
  }
}

void car_present (){
  //what gets run when there is a car present
  int i = 0;
  while (get_dist() <= 2){
    i++;
    //buzz fast for a close car, only set off buzzer every 4 seconds 
    if ( (get_dist() <= 1) && (i%8 == 0) ) {
      report newrep;
      create_report(&newrep);
      Serial.println(newrep.hr);
      Serial.println(newrep.minut);
      Serial.println(newrep.sec);
      Serial.println(newrep.d);
      Serial.println(newrep.m);
      Serial.println(newrep.yr);
      buzz(FAST, MANYLOOPS);
      
    }
    
      
    }
    delay(500);
    if (i%6 == 0)
      buzz(SLOW, FEWLOOPS);
}

double get_dist(){
  double dur, dist;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  dur = pulseIn(echoPin, HIGH);
  dist = (dur/2) / 2910;
  Serial.print("distance = ");
  Serial.print(dist);
  Serial.print("\n");
  return dist;
}



// the setup function runs once when you press reset or power the board
void setup(void) {
  //Initialize
  //pinMode(LED_BUILTIN, OUTPUT);
  //pinMode(YELLOW_LED, OUTPUT);
  //pinMode(RED_LED, OUTPUT);
  pinMode(trigPin, OUTPUT); //trigger ultrasonic transducer
  pinMode(echoPin, INPUT); // echo ultrasonic
  pinMode(motor, OUTPUT); //vibration module
  
  pinMode(GREEN_LED, OUTPUT); //led strip
   pinMode(RED_LED, OUTPUT);//led strip
   pinMode(BLUE_LED, OUTPUT);// led strip


   pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output

   //TurnOn();
   //delay(5000);
   //TurnOff();
/*   
#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif
  
  Serial.println("Accelerometer Test"); Serial.println("");
*/
Serial.begin(9600);
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }

  /* Display some basic information on this sensor */
  //displaySensorDetails();
  
}

// the loop function runs over and over again forever
void loop(void) {
  sensor_t sensor;
  accel.getSensor(&sensor);
  
  sensors_event_t event;
  accel.getEvent(&event);

  
    // distance in m
  if ( (get_dist()) <= 2){
    buzz(SLOW, FEWLOOPS);
    car_present();
  }
  delay(500);

  if ((event.acceleration.y >= 8) || (event.acceleration.y <= -8)) {
      wreck();
  }




//Serial.print("distance = ");
//Serial.print(distance);
//Serial.print("\n");


  /* Display the results (acceleration is measured in m/s^2) */
  //Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  //Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  //Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");

  /* Note: You can also get the raw (non unified values) for */
  /* the last data sample as follows. The .getEvent call populates */
  /* the raw values used below. */
  //Serial.print("X Raw: "); Serial.print(accel.raw.x); Serial.print("  ");
  //Serial.print("Y Raw: "); Serial.print(accel.raw.y); Serial.print("  ");
  //Serial.print("Z Raw: "); Serial.print(accel.raw.z); Serial.println("");

  

  

/*
if (distance <=1) // Checking the distance, you can change the value
{
digitalWrite(motor,HIGH);
digitalWrite(RED_LED, HIGH);
delay(500);
digitalWrite(motor,LOW);
digitalWrite(RED_LED, LOW);
delay(500);

} else
{
digitalWrite(motor,LOW);
digitalWrite(RED_LED, LOW);

}*/





}
