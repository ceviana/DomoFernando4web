#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <stdlib.h> 

// WiFi information
//const char WIFI_SSID[] = "VIANA";
//const char WIFI_PSK[] = "15373420883";
const char WIFI_SSID[] = "ESP_GUEST"; // visitante 2,4GHz TPLINK - 0k
const char WIFI_PSK[] = "01234567890";

//const char WIFI_SSID[] = "Interconexion - Home Office";
//const char WIFI_PSK[] = "Polaris Consultoria";

//const char WIFI_SSID[] = "Mixceviana";   // celular
//const char WIFI_PSK[] = "0123456789";

//const char WIFI_SSID[] = "AndroidAFVap";
//const char WIFI_PSK[] = "2003311200";

String WIFI_IP = "NONE";
String WIFI_MAC = "NONE";

String SN="776543";
String VERSAO="v1.0r4";
 
// Remote site information
const char http_site[] = "www.api.app4iot.com.br";

const int http_port = 80; // https criptografia 443
 
// Pin definitions
//const int LED_PIN = 13;
const int LED_PIN = 12;     // atl board led1
//const int LED_PIN = 16;     // atl board led0
 
// Global variables
WiFiClient client;

String _buffer;

char v1[10];
char v2[10];
char v3[10];
char v4[10];
String v5;
char cnt[10];
long l1,l2,l3,l4,l5,lcnt;

double db5;
 
void setup() {
  
  // Set up serial console to read web page
  Serial.begin(115200);
  Serial.print("Thing GET Example");
  
  // Set up LED for debugging
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Connect to WiFi
  connectWiFi();

  WIFI_IP = WiFi.localIP().toString();
  WIFI_MAC = WiFi.macAddress();
  
  // Attempt to connect to website
  if ( !getPage() ) {
    Serial.println("GET request failed");
  }
}
 
void loop() {
  
  // If there are incoming bytes, print them
  if ( client.available() ) {

    String c = client.readStringUntil('\r');
    //char c = client.read();
    //Serial.print(c);
    //Serial.println(" Contando");    
    int state_pos = c.indexOf("{");
    if (state_pos > 0)
      _buffer = c;
  }
  
  // If the server has disconnected, stop the client and WiFi
  if ( !client.connected() ) {

    Serial.print("Aqui------");
    Serial.println(_buffer);
    Serial.println();

    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(_buffer);
    if ( root.success( )) {
      String _action = root["act"];
      boolean _status = root["status"];

      Serial.print("Acao : " );
      Serial.println(_action);
      Serial.print("Status: ");
      Serial.println(_status);

      if ( _action == "ON" ) {
        digitalWrite(LED_PIN, HIGH);
      } 
      
      if ( _action == "OFF") {
        digitalWrite(LED_PIN, LOW);
      }
    }

    delay(5000);

    // Attempt to connect to website
    if ( !getPage() ) {
      Serial.println("GET request failed");
    }
    
    // Close socket and wait for disconnect from WiFi
    /*
    client.stop();
    if ( WiFi.status() != WL_DISCONNECTED ) {
      WiFi.disconnect();
    }
    
    // Turn off LED
    //digitalWrite(LED_PIN, LOW);
    
    // Do nothing
    Serial.println("Finished Thing GET test");
    while(true){
      delay(1000);
    }
    */
    
  }
}
 
// Attempt to connect to WiFi
void connectWiFi() {
  
  byte led_status = 0;
  
  // Set WiFi mode to station (client)
  WiFi.mode(WIFI_STA);
  
  // Initiate connection with SSID and PSK
  WiFi.begin(WIFI_SSID, WIFI_PSK);
  
  // Blink LED while we wait for WiFi connection
  while ( WiFi.status() != WL_CONNECTED ) {
    //digitalWrite(LED_PIN, led_status);
    //led_status ^= 0x01;
    Serial.print(".");
    delay(100);
  }
  
  // Turn LED on when we are connected
  digitalWrite(LED_PIN, LOW);
}
 
// Perform an HTTP GET request to a remote page
bool getPage() {
  
  // Attempt to make a connection to the remote server
  Serial.print(" Site http: ");
  Serial.print(http_site);
  Serial.print(" Porta: ");
  Serial.println(http_port);
  
  if ( !client.connect(http_site, http_port) ) {
    return false;
  }

  l1 = random(100);
  l2 = random(100);
  l3 = random(100);
  l4 = random(100);
  db5 = random(100) * 0.334;
  lcnt = random(100);

  ltoa(l1, v1, 10);
  ltoa(l2, v2, 10);
  ltoa(l3, v3, 10);
  ltoa(l4, v4, 10);
  v5 = String(db5, 3); // decimal com 3 casas
  ltoa(lcnt, cnt, 10);
   
  String _comando;
  //_comando  = "GET /api-app4iot.php?act=GRV&tkn=iop2oi1jhu87hgn1hgqnbaiuk187jd&dis=cab4lpawluyxc30pkhb1mw410k17x9&var=wlg7tvk6wmhr91hc08q4wh2ik7mqa7";
  _comando  = "GET /api-app4iot.php?act=GRV&tkn=iop2oi1jhu87hgn1hgqnbaiuk187jd&dis=cab4lpawluyxc30pkhb1mw410k17x9&var=0";
  _comando += "&v1=";
  _comando += v1;
  _comando += "&v2=";
  _comando += v2;
  _comando += "&v3=";
  _comando += v3;
  _comando += "&v4=";
  _comando += v4;
  _comando += "&v5=";
  _comando += v5;
  _comando += "&cnt=";
  _comando += cnt;
  _comando += "&ip="; 
  _comando += WIFI_IP;
  _comando += "&mac=";
  _comando += WIFI_MAC;
  _comando += "&sid=";
  _comando += WIFI_SSID;
  _comando += "&pwd=";
  _comando += encrypty(WIFI_PSK);
  _comando += "&sn=";
  _comando += SN;
  _comando += "&ver=";
  _comando += VERSAO; 
  _comando += " HTTP/1.1";

  Serial.println(_comando);
  
  // Make an HTTP GET request
  client.println(_comando);
  client.print("Host: ");
  client.println(http_site);
  client.println("Connection: close");
  client.println();
  
  return true;
}

String encrypty(String valor) {
  String k1 = "1234567890abcdefghijklmnopqrstuvxz!@#$%*()_+-=[{]};:.>,<?";
  String k2 = "?<,>;.:}]{[=-+_)(*%$#@!zxvutsrqponmlkjihgfedcea0987654321";
  int tam = valor.length();

  String pos = "";
  int pos_k1;
  char pos_k2;
  for (int p=0; p<tam; p++ ) {
      
      pos = valor.charAt(p);

      pos_k1 = k1.indexOf(pos);

      pos_k2 = k2.charAt(pos_k1);

      valor.setCharAt( p, pos_k2);
  }
  
  return valor;
}

