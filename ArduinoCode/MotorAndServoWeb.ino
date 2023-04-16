#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>  //Include the Servo Library

#include "PageIndex.h"; //--> Include the contents of the User Interface Web page from website, stored in the same folder as the .ino file  

#define ServoPort D1   //--> Defining Servo Port
#define LED1 D5   //-->Defining a Simple LED
#define IN1 D2 //--> DEfining Motor Driver IN1
#define IN2 D3//--> DEfining Motor Driver IN2

//Make a wifi name and password as access point

const char* ssid = "ESPServo";  // your SSID

const char* password = "12345678"; //WIFI Password


Servo myservo;  //--> create servo object to control a servo

ESP8266WebServer server(80);  //--> Server on port 80

String SERstate1 = "OFF";

//This routine is executed when you open NodeMCU ESP8266 IP Address in browser

void handleRoot() {

  String s = MAIN_page; //Read HTML contents

  server.send(200, "text/html", s); //Send web page

}

//Procedure for handling servo control

void handleServo() {
  String POS = server.arg("servoPOS");
  int pos = 180 - POS.toInt();
  myservo.write(pos);   //--> Move the servo motor according to the POS value
  delay(15);
  Serial.print("Servo Angle:");
  Serial.println(pos);
  server.send(200, "text/plane", "");
}

void controlServo() {
  String t_state = server.arg("SERstate1");
  Serial.println(t_state);
  int pos;
  if (t_state == "1") {
    analogWrite(IN1, 255);
    analogWrite(IN2, 0);
  }
  else if (t_state == "2") {
    analogWrite(IN1, 200);
    analogWrite(IN2, 0);
  }
  else if (t_state == "3") {
    analogWrite(IN2, 200);
    analogWrite(IN1, 0);
  }
  else if (t_state == "4") {
    analogWrite(IN1, LOW);
    analogWrite(IN2, LOW);
  }
  else {
    SERstate1 = "OFF";
  }
  server.send(200, "text/plane", SERstate1);
}

void statusSER() {
  server.send(200, "text/plane", SERstate1);
}
void controlLED1() {
  digitalWrite(LED1, HIGH);
  delay(200);
  digitalWrite(LED1, LOW);
  delay(200);
}


void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  pinMode(LED1, OUTPUT);
  Serial.begin(115200);
  delay(500);
  myservo.attach(ServoPort); //--> attaches the servo on D1 to the servo object
  
  
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);


  //Initialize Webserver
  server.on("/", handleRoot); //--> Routine to handle at root location. This is to display web page.
  server.on("/setPOS", handleServo); //--> Sets servo position from Web request
  server.on("/setSER1", controlServo);
  server.on("/readSER1", statusSER);
  server.begin();

  Serial.println("HTTP server started");

}


void loop() {

  server.handleClient();

}