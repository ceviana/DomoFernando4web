// TCP Client Demo - Here is a basic server which responds to TCP client HTTP GET commands (added 1/8/17):
/* FIRST ONE ID:      ESP_72F4E5      5C-CF-7F-72-F4-E5
“get relay state!”    http://11.12.14.16/get relay state!
“get io state!”       http://11.12.14.16
“get MAC!”            http://11.12.14.16

open relay!
close relay!
get relay state!
get io state!
get MAC! 

open relay!
close relay!
get relay state!
get io state!
get MAC! 


http://11.12.14.16/GET? 

*/
#include <ESP8266WiFi.h>
// Esp8266 pinouts
#define ESP8266_GPIO2    2  // Blue LED.
#define ESP8266_GPIO4    4  // Relay control. 
#define ESP8266_GPIO5    5  // Optocoupler input.
#define LED_PIN          ESP8266_GPIO2
// WiFi Definitions.
const char ssid[] = "ESP_GUEST";
const char pswd[] = "01234567890";
WiFiServer server( 80 );
volatile int relayState = 0;      // Relay state.
void setup() {
  initHardware();
  connectWiFi();
  server.begin();}
void GetClient( WiFiClient client ) {
// Read the first line of the request.
  String req = client.readStringUntil( '\r' );
  Serial.println( req );
  client.flush();
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  if ( req.indexOf( "OPTIONS" ) != -1 ) {
    s += "Allows: GET, OPTIONS";
  } else if ( req.indexOf( "GET" ) != -1 ) {
    if ( req.indexOf( "open" ) != -1 ) {
      // relay on!
      s += "relay on!";
      relayState = 1;
      digitalWrite( ESP8266_GPIO4, 1 ); // Relay control pin.
    } else if ( req.indexOf( "close" ) != -1 ) {
      // relay off!
      s += "relay off!";
      relayState = 0;
      digitalWrite( ESP8266_GPIO4, 0 ); // Relay control pin.
    } else if ( req.indexOf( "relay" ) != -1 ) {
      if ( relayState == 0 )
        // relay off!
        s += "relay off!";       else
        // relay on!
        s += "relay on!";
    } else if ( req.indexOf( "io" ) != -1 ) {
      if ( digitalRead( ESP8266_GPIO5 ) == 0 )
        s += "input io is:0!";       else
        s += "input io is:1!";
    } else if ( req.indexOf( "MAC" ) != -1 ) {
      uint8_t mac[WL_MAC_ADDR_LENGTH];
      WiFi.softAPmacAddress( mac );
      String macID = String( mac[WL_MAC_ADDR_LENGTH - 5], HEX) + String( mac[WL_MAC_ADDR_LENGTH - 4], HEX) +
                     String( mac[WL_MAC_ADDR_LENGTH - 3], HEX) + String( mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                     String( mac[WL_MAC_ADDR_LENGTH - 1], HEX) + String( mac[WL_MAC_ADDR_LENGTH], HEX);
      macID.toUpperCase();
      s += "MAC address: " + macID;
    } else       s += "Invalid Request.<br> Try: open/close/relay/io/MAC";
 } else 
    s = "HTTP/1.1 501 Not Implemented\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  client.flush();
  s += "</html>\n";
  // Send the response to the client.
  client.print( s );
  delay( 1 );
  Serial.println( "Client response sent." );}
unsigned n = 0;
void loop() {
Serial.print("Loop number: ");Serial.println(n++);
delay(2000);
// Check if a client has connected.
  WiFiClient client = server.available();
  if ( client ) 
    GetClient( client );}
void connectWiFi() {
  byte ledStatus = LOW;
  Serial.println();
  Serial.println( "Connecting to: " + String( ssid ) );
  // Set WiFi mode to station (as opposed to AP or AP_STA).
  WiFi.mode( WIFI_STA );
  // WiFI.begin([ssid], [passkey]) initiates a WiFI connection.
  // to the stated [ssid], using the [passkey] as a WPA, WPA2, or WEP passphrase.
  WiFi.begin( ssid, pswd );
  while ( WiFi.status() != WL_CONNECTED ) {
    // Blink the LED.
    digitalWrite( LED_PIN, ledStatus ); // Write LED high/low.
    ledStatus = ( ledStatus == HIGH ) ? LOW : HIGH;
    delay( 100 );  }
  Serial.println( "WiFi connected" );  
  Serial.println( "IP address: " );
  Serial.println( WiFi.localIP() );}
void initHardware() {
  Serial.begin( 115200 );
  pinMode( ESP8266_GPIO4, OUTPUT );       // Relay control pin.
  pinMode( ESP8266_GPIO5, INPUT_PULLUP ); // Input pin.
  pinMode( LED_PIN, OUTPUT );             // ESP8266 module blue LED.
  digitalWrite( ESP8266_GPIO4, 0 );       // Set relay control pin low.
  Serial.println("\n\nget relay state!”    http://11.12.14.16/get relay state!");
  Serial.println("get io state!”       http://11.12.14.16");
  Serial.println("get MAC!”            http://11.12.14.16\n\n");
}

