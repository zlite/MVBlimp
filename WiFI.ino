/*
  WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will create a new access point (with no password).
 It will then launch a new server and print out the IP address
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 13.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on#include <SPI.h>#include <SPI.h>
#include <Adafruit_WINC1500.h>
#include <Adafruit_WINC1500.h>
 http://yourAddress/L turns it off

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * WiFi shield attached
 * LED attached to pin 13

 created 25 Nov 2012
 by Tom Igoe
 */

 
bool Init_WiFi(void) 
{

char ssid[] = "feather";      //  created AP name
char pass[] = "wing";         // (not supported yet)

int status = WL_IDLE_STATUS;

#ifdef WINC_EN
  pinMode(WINC_EN, OUTPUT);
  digitalWrite(WINC_EN, HIGH);
#endif

  while (!Serial);
  delay(1000);
  Serial.begin(9600);      // initialize serial communication

  Serial.println("Access Point Web Server");

  pinMode(LED_PIN, OUTPUT);      // set the LED pin mode
// **** WARNING ****
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  if (WiFi.beginAP(ssid) != WL_CONNECTED) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }

  Adafruit_WINC1500Client client = server.available();   // listen for incoming clients
  
  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
  server.begin();Adafruit_WINC1500Server server(80);

  // you're connected now, so print out the status
  printWifiStatus();
  if (WiFi.beginAP(ssid) == WL_CONNECTED) return true;
}


void webserver(void) {


  Adafruit_WINC1500Client client = server.available();   // listen for incoming clients 
  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> turn the LED on<br>");
            client.print("Click <a href=\"/L\">here</a> turn the LED off<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED_PIN, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED_PIN, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
<<<<<<< HEAD
}
=======
}
>>>>>>> origin/master
