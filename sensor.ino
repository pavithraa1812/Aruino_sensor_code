#include <WiFi.h>
#include <WebServer.h>
const char* ssid = "Sathish";
const char* password =  "gurusamy123";
const int trigPin = 2;
const int echoPin = 5;
const int inputPin = 3;
const int ledPin = 13;                // choose the pin for the LED

float temp_celsius = 0;
WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)
void setup()
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(inputPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);  
  Wifi_connection();
   
}
void loop()                    
{
 server.handleClient();
 temperature_sensor();
 ultrasonic_sensor();
 crash_sensor();
                                  
}

void Wifi_connection()
{
  Serial.println("Try Connecting to ");
  Serial.println(ssid);
 
  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);
 
  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial
  server.on("/", handle_root);
  server.begin();
  Serial.println("HTTP server started");
  delay(100);
} 
// Handle root url (/)
void handle_root() {
  server.send(200, "text/html", HTML());
}
void temperature_sensor(){
  
  temp_celsius = ((analogRead(A0) * 330.0) / 1023.0)/10;   // To convert analog values to Celsius We have 3.3 V on our board and we know that output voltage of LM35 varies by 10 mV to every degree Celsius rise/fall. So , (A0*3300/10)/1023 = celsius
  Serial.print("  Temperature = ");
  Serial.print(temp_celsius);
  Serial.print(" Celsius, ");
  delay(3000);   

}

void ultrasonic_sensor()
{
  long duration;
  int distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
}
void crash_sensor()
{
  int val = digitalRead(inputPin);  // read input value
  if (val == HIGH) 
  {                
    digitalWrite(ledPin, LOW);// turn LED OFF
    
  } 
  else 
  {
    Serial.println("Switch Pressed!");
    digitalWrite(ledPin, HIGH);     // turn LED ON
    
  }
  delay(50);
 
}
String HTML()
{
  // HTML & CSS contents which display on web server
  String HTML;
  HTML= "<!DOCTYPE html>\
  <html>\
  <body>\
  <h1>Sensor values</h1>\
  </body>\
  </html>";
  return HTML;  
}
