/* Comment this out to disable prints and save space */
//#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SoftwareSerial.h>
#include <Servo.h>

Servo myservo;  

const int heater = 10;
const int fan = 9;
const int servo = 11;
const int poten1 = A3;
const int poten2 = A5;
int arrs[2];
int poten1value;
int prev1value;
int poten2value;
int prev2value;
bool blynk_in_control = false;

char auth[] = "auth token";
char ssid[] = "ssid";
char pass[] = "password";

SoftwareSerial EspSerial(3,4); // RX, TX

#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

BLYNK_WRITE(V0)
{
  int hvalue = param.asInt(); // assigning incoming value from pin V0 to a variable
  analogWrite(heater, hvalue);
  blynk_in_control = true;
}
BLYNK_WRITE(V1)
{
  int fvalue = param.asInt(); // assigning incoming value from pin V1 to a variable
  analogWrite(fan, fvalue);
  blynk_in_control = true;
}


void setup()
{
  // Debug console
  Serial.begin(9600);
  EspSerial.begin(ESP8266_BAUD);
  Blynk.begin(auth, wifi, ssid, pass);  
  // You can also specify server:
  // Blynk.begin(auth, wifi, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, wifi, ssid, pass, IPAddress(192,168,1,100), 8080);
  check_potentio(poten1, poten2);
  prev1value, prev2value = arrs[0], arrs[1];
}

void loop()
{
  check_potentio(poten1, poten2);
  poten1value, poten2value = arrs[0], arrs[1];
  if(abs(poten1value-prev1value) > 5){
    blynk_in_control = false;
  }
  if(!blynk_in_control){
    analogWrite(heater, poten1value);
    analogWrite(fan, poten2value);
  }
  Blynk.run();
  prev1value, prev2value = arrs[0], arrs[1];
  
}

void check_potentio(const int analog_input, const int analog_input2){
  arrs[0] = analogRead(analog_input);
  arrs[1] = analogRead(analog_input2); 
}
