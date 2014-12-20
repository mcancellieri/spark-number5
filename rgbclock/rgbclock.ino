// This #include statement was automatically added by the Spark IDE.
#include "SparkTime/SparkTime.h"

/*

  templight.ino

  Adjusts the color of an RGB LED to changes in temperature with tempHigh (Celcius) being full red and tempLow (Celcius) being full blue.
  Also uses a push-button for on/off functionality.

  Written for the Spark Core

*/

// LED leads connected to PWM pins
const int RED_LED_PIN = A4;
const int GREEN_LED_PIN = A1;
const int BLUE_LED_PIN = A0;
int temperaturePin = A7; //temperature sensor pin
int inputPin = D1; //push-button pin
int val = 0; //variable for push-button status
int state = 1; //variable for on/off state of the LED
int tempHigh = 28; //top value in range of temperature values (mess with me!)
int tempLow = 24; //bottom value in range of temperature values (mess with me!) --make sure tempHigh stays above tempLow
// and the current temperature is within the range for optimum performance.

UDP UDPClient;
SparkTime rtc;

unsigned long currentTime;
unsigned long lastTime = 0UL;
String timeStr;


// Used to store the intensity level of the individual LEDs, intitialized to full brightness (0)
int redIntensity = 0;
int greenIntensity = 0;
int blueIntensity = 0;
double temperature= 0.0;


void setup() {


    rtc.begin(&UDPClient, "north-america.pool.ntp.org");
    rtc.setTimeZone(1); // gmt offset
    Serial.begin(9600);

    //set LED pins to outputs

    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(BLUE_LED_PIN, OUTPUT);

    //set temperature and push-button pins as inputs
    pinMode(temperaturePin, INPUT);
    pinMode(inputPin, INPUT);

    //open serial connection for debugging
    Serial.begin(9600);
    // Register a Spark variable here
    Spark.variable("temperature", &temperature, DOUBLE);
}

void loop() {

    currentTime = rtc.now();

    //val = digitalRead(inputPin); //read the state of the push-button
    temperature = (analogRead(temperaturePin) * 3.3) / 4095;  //getting the voltage reading from the temperature sensor
    temperature = (temperature - 0.5) * 100; //turn the voltage into a Celcius reading
    //Serial.println(temperature); //print temperature to serial for debugging


    //if (val == LOW) { //if push-button pressed
      //  state = !state; //reverse on/off state
    //    delay(250); //primitive button debounce

    //}

    //if (!state) { //if light-state off, set Red and Blue vals to minimum (255)
      //  adjRed = 255;
    //    adjBlue = 255;
    //}



    int hour = rtc.hour(currentTime);
	  int minute = rtc.minute(currentTime);
	  int second = rtc.second(currentTime);
    int adjRed = (int) (255 - hour*255/60 ); // turn temperature into integer adjusted red value (0 is full red, 255 is min)
    int adjGreen = (int) (255- minute*255/60);//+255)%255);
    int adjBlue = (int) (255- second+255/60); // turn temperature into integer adjusted blue value (0 is full blue, 255 is min)

    //write red and blue values to respective pins
    analogWrite(RED_LED_PIN, adjRed);
    analogWrite(GREEN_LED_PIN, adjGreen);
    analogWrite(BLUE_LED_PIN, adjBlue);


    //print red and blue adjusted values alongside light state (on/off as 1/0)
    Serial.println("hour:" + String(adjRed));
    Serial.println("minute:" + String(adjGreen));
    Serial.println("second:" + String(adjBlue));
    Serial.println("temp:" + String(temperature));

    delay(1000);


}
