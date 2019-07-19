#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "Servo.h"
#include "HCSR04.h"
#include "index.h" // HTML webpage contents with javascripts
  
const int trigPin = 5;
const int echPin = 4;

  

UltraSonicDistanceSensor distanceSensor(13, 12);// Initialize sensor that uses digital pins 13 and 12.
Servo myservo;

//SSID and Password of your WiFi router
const char* ssid = "Realme";
const char* password = "12345678";
 
ESP8266WebServer server(80); //Server on port 80
//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

double distanceCm;

void handleADC() {
 int a = analogRead(A0);

 String data = "{\"ADC\":\""+String(a)+"\", \"distanceCm\":\""+ String(distanceCm) +"\"}";
 
 server.send(200, "text/plane", data); //Send ADC value, temperature and humidity JSON to client ajax request

 //Get Humidity temperatue data after request is complete
 //Give enough time to handle client to avoid problems
 
  delay(distanceSensor.measureDistanceCm());

  distanceCm = distanceSensor.measureDistanceCm();

  Serial.print(distanceCm, 1);
}
int pos = 0;
//==============================================================
//                  SETUP
//==============================================================
void setup()
{
  Serial.begin(115200);
  Serial.println();

  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
 
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
 
  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/readADC", handleADC); //This page is called by java Script AJAX
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");


myservo.attach(0);     // attaches the servo on GIO2 to the servo object

}

void loop()
{
  server.handleClient();          //Handle client requests

 
 // establish variables for duration of the ping,
 
 // and the distance result in inches and centimeters:
 
 long duration, inches, cm;
 
 
 
 // The sensor is triggered by a HIGH pulse of 10 or 
 // more microseconds.
 
 // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
 
 pinMode(trigPin, OUTPUT);
 
 digitalWrite(trigPin, LOW);
 
 delayMicroseconds(2);
 
 digitalWrite(trigPin, HIGH);
 
 delayMicroseconds(10);
 
 digitalWrite(trigPin, LOW);
 
 
 
 // Read the signal from the sensor: a HIGH pulse whose
 
 // duration is the time (in microseconds) from the sending
 
 // of the ping to the reception of its echo off of an object.
 
 pinMode(echPin, INPUT);
 
 duration = pulseIn(echPin, HIGH);
 
 
 
 // convert the time into a distance
 
 inches = microsecondsToInches(duration);
 
 cm = microsecondsToCentimeters(duration);
 
 
 
 //Tell the Arduino to print the measurement in the serial console
 
 Serial.print(inches);
 
 Serial.print("in, ");
 
 Serial.print(cm);
 
 Serial.print("cm");
 
 Serial.println();
 
 
 
 // This if-else statement tells the Arduino at what distance
 
 // it should trigger the servo, what the servo should do,
 
 // and what it should do if the distance is too far away.
 
 if (inches <= 20) {sweep(1);
 
 
 }
 
 else if (inches >= 20) {myservo.write(pos);
 
 }
 
 
 
 // Tell the Arduino to wait 0.10 seconds before pinging the
 
 // Ultrasonic Sensor again.
 
 delay(100);
 
}
 
// Converts the microseconds reading to Inches
 
long microsecondsToInches(long microseconds)
 
{
 
 // According to Parallax's datasheet for the PING))), there are
 
 // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
 
 // second). This gives the distance travelled by the ping, outbound
 
 // and return, so we divide by 2 to get the distance of the obstacle.
 
 // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
 
 return microseconds / 74 / 2;
 
}
 
//Converts the Microseconds Reading to Centimeters
 
long microsecondsToCentimeters(long microseconds)
 
{
 
 // The speed of sound is 340 m/s or 29 microseconds per centimeter.
 
 // The ping travels out and back, so to find the distance of the
 
 // object we take half of the distance travelled.
 
 return microseconds / 29 / 2;
 
}
 
//This sub-routein is what dictates the movement of the servo
 
void sweep(int NUM_OF_CYCLES)
 
 
 
{
 
// Tells the Arduino to start this loop at 0 and incriment it by 1
 
// each time the loop completes. This is how the Arduino knows to
 
// stop the loop when a specific number of the 
// Sweep routein has been ran.
 
 for (int j=0; j<NUM_OF_CYCLES; j++)
// goes from 0 degrees to 180 degrees
 for(pos = 0; pos < 110; pos += 1) 
 {                 // in steps of 1 degree
// tell servo to go to position in variable 'pos'
myservo.write(pos);
}delay(6500);// waits 15ms for the servo to reach the position delay(10); } 
// goes from 180 degrees to 0 degrees 
for(pos = 110; pos>=1; pos-=1) {
// tell servo to go to position in variable 'pos' 
myservo.write(pos); 
// waits 15ms for the servo to reach the position 
delay(10); } 

}
