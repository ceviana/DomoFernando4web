/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*                    VERSAO RPi TESTE DO CODIGO - UPDATED 050118
               VERAO 3.0           DATA: 01072017
               COMPILADO NA VERSAO ARDUINO: 1.8.1
               __________________________________
                PLACA WIFI ESP8266-07 AT THINKER
                PROGRAMA: MINI ESTACAO CLIMATICA
                CONTÃ‰M SENSORES: BMP-180 E DHT22
               __________________________________
               CONFIGURACAO PLACA GRAVACAO - ESP-07
               FUNCIONA COM BIBLIOTECA ESP-07 COMMUNITY ATE VERSAO 2.3.0
               ATENCAO - VERSAO 2.4 NAO FUNCIONA PARA ESTE MODELO ESP-07
               __________________________________
               ATENCAO NAO COMPILAR ESP-07 NA VERSAO 2.4 OU SUPERIOR!!!!
               __________________________________
               PLACA:             GENERIC ESP8266 MODULE
               BIBLIOTECA         2.3.0
               FLASH MODE:        DIO
               FLASH SIZE:        1M (512K SPIFFS)
               DEBUG PORT:        DISABLED <--<
               DEBUG LEVEL:       TOUT <--<
               RESET MOTHOD:      ck
               FLASH FREQUENCY:   40 MHz
               CPU FREQUENCY:     80 MHz
               UPLOAD SPEED:      115200
               PORTA: PORTA ESP CONECTADA AO COMPUTADOR
               __________________________________
               CONFIGURACAO PLACA GRAVACAO - ESP-12E
               PLACA:             NODE MCU 1.0 (ESP-12E MODULE)
               CPU FREQUENCY:     80 MHz
               FLASH SIZE:        4M (1M SPIFFS)
               UPLOAD SPEED:      115200
               BIBLIOTECA         2.3.0 >--> +/- 
               IwIP variant:      v.1.4 preBuilt >--> compila + erro flash ao gravar!
               IwIP variant:      v.1.4 preBuilt OpenSource >--> nao compila
               IwIP variant:      v.2 preBuilt (MSS=536) >--> compila mas nao funciona e corrompe a memoria do ESP
               PORTA: PORTA ESP CONECTADA AO COMPUTADOR
               __________________________________                       */
/*  reset causes:
0 -> normal startup by power on
1 -> hardware watch dog reset
2 -> software watch dog reset (From an exception)
3 -> software watch dog reset system_restart (Possibly unfed watchdog got angry)
4 -> soft restart (Possibly with a restart command)
5 -> wake up from deep-sleep
boot device:
    0:
    1: ram
    3: flash
See more at: http://www.esp8266.com/viewtopic.php?p=2096#p2112          */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Recommended Modes - Based on Bosch BME280I2C environmental sensor data sheet.
Weather Monitoring :
   forced mode, 1 sample/minute
   pressure ×1, temperature ×1, humidity ×1, filter off
   Current Consumption =  0.16 μA
   RMS Noise = 3.3 Pa/30 cm, 0.07 %RH
   Data Output Rate 1/60 Hz
Humidity Sensing :
   forced mode, 1 sample/second
   pressure ×0, temperature ×1, humidity ×1, filter off
   Current Consumption = 2.9 μA
   RMS Noise = 0.07 %RH
   Data Output Rate =  1 Hz
Indoor Navigation :
   normal mode, standby time = 0.5ms
   pressure ×16, temperature ×2, humidity ×1, filter = x16
   Current Consumption = 633 μA
   RMS Noise = 0.2 Pa/1.7 cm
   Data Output Rate = 25Hz
   Filter Bandwidth = 0.53 Hz
   Response Time (75%) = 0.9 s
Gaming :
   normal mode, standby time = 0.5ms
   pressure ×4, temperature ×1, humidity ×0, filter = x16
   Current Consumption = 581 μA
   RMS Noise = 0.3 Pa/2.5 cm
   Data Output Rate = 83 Hz
   Filter Bandwidth = 1.75 Hz
   Response Time (75%) = 0.3 s  */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#include     <Wire.h>             // Needed for legacy versions of Arduino
#include     <ESP8266WiFi.h>
#include     <WiFiClient.h>
#include     <ArduinoJson.h>
#include     <stdlib.h>
#include     "E:\GIT\DomoFernando\INTERNETv10BME\BME280I2C.h"
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define SERIAL_BAUD 115200
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// WiFi information
const char WIFI_SSID[] = "ESP_GUEST"; // visitante 2,4GHz TPLINK - 0k
const char WIFI_PSK[] = "01234567890";
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
String WIFI_IP = "NONE";
String WIFI_MAC = "NONE";
String SN="776543";
String VERSAO="v1.0r4";
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// Remote site information
const char http_site[] = "www.api.app4iot.com.br";
const int http_port = 80; // https criptografia 443
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// Pin definitions
//const int LED_PIN = 13;     // nodeMCU
const int LED_PIN = 12;     // atl board led1
//const int LED_PIN = 16;     // atl board led0
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// CONSTANTS - virtual delay
unsigned long       tempoPrevio = 0;        // VARIAVEL DE CONTROLE DE TEMPO
unsigned long       intervalo = 20000;      // VARIAVEL PARA CONTROLE DE SUBIDA DOS DADOS (1a. SUBIDA = 45 SEGUNDOS)
bool                b;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// Global variables
unsigned long       counterLocal = 0;
unsigned short      measCounter = 0;    // local measurements counter
// Variables will change:
int ledState = LOW;             // ledState used to set the LED
WiFiClient client;
String _buffer;
String v1;
String v2;
String v3;
//char v1[10];
//char v2[10];
//char v3[10];
char v4[10];
char v5[10];
char cnt[10];
long l1,l2,l3,l4,l5,lcnt;
float temp = 0, hum = 0, pres = 0;
float P_bme = 0, U_bme = 0, T_bme = 0;   // VARIAVEIS SENSOR BME280
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
BME280I2C::Settings settings(
   BME280::OSR_X1,
   BME280::OSR_X1,
   BME280::OSR_X1,
   BME280::Mode_Forced,
   BME280::StandbyTime_1000ms,
   BME280::Filter_Off,
   BME280::SpiEnable_False,
   0x76 // I2C address. I2C specific.
);
BME280I2C bme(settings);
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void setup() { measCounter = 0;
// Set up serial console to read web page
  Serial.begin(SERIAL_BAUD);
  Serial.print("\nThing GET Example");
// Set up LED for debugging
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
//Serial.println("... 0 COMUTANDO ESTADO DO LED ...");
// Connect to WiFi
  connectWiFi();
WIFI_IP = WiFi.localIP().toString();
WIFI_MAC = WiFi.macAddress();
  Serial.print("\nIP LAN: ");
  Serial.println(WiFi.localIP());
  Serial.println("MAC: " + WiFi.macAddress());
// Attempt to connect to website
if ( !getPage() ) {Serial.println("GET request failed");}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
Wire.begin();
  while(!bme.begin()){
    Serial.println("Could not find BME280I2C sensor!");
    /*delay(1000);*/}
  switch(bme.chipModel()){
     case BME280::ChipModel_BME280:
       Serial.println("Found BME280 sensor! Success.");
       break;
     case BME280::ChipModel_BMP280:
       Serial.println("Found BMP280 sensor! No Humidity available.");
       measCounter = 0;
       break;
     default:
       Serial.println("Found UNKNOWN sensor! Error!");}
// Change some settings before using.
   settings.tempOSR = BME280::OSR_X4;
   bme.setSettings(settings);} // ReadBME280Data();}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
void loop() {                                  //printBME280Data(&Serial);
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
unsigned long currentMillis = millis();
  if (currentMillis - tempoPrevio >= intervalo) {     // SOBE OS PRIMEIROS DADOS NO PRIMEIRO MINUTO
    tempoPrevio = currentMillis;
//    intervalo = 25000;                  // X MINUTO (TEMPO DE SUBIDA)
//    intervalo = 1000;                  // 17 SEGUNDOS (TEMPO DE SUBIDA)
//    intervalo = 3600;                  // ~50 SEGUNDOS (TEMPO DE SUBIDA)
//    intervalo = 18000;                  // ~3m54s = 234s (TEMPO DE SUBIDA)
      intervalo = 23100;                  // ~298+- 4s = 4m58s (TEMPO DE SUBIDA)
    digitalWrite(LED_PIN, b=!b); // blink the onboard LED 400ms, 400ms off
Serial.print(measCounter);
Serial.println("  OlIVODelay COMUTANDO ESTADO DO LED ... "); // FIM SINGLEdELAY }
measCounter++;
//if (measCounter > 14) measCounter = 0;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// If there are incoming bytes, print them
  if ( client.available() ) {
    String c = client.readStringUntil('\r');
//char c = client.read();
//Serial.print(c);
//Serial.println(" Contando");    
    int state_pos = c.indexOf("{");
    if (state_pos > 0)
      _buffer = c;}
// If the server has disconnected, stop the client and WiFi
  if ( !client.connected() ) {
    Serial.print("Aqui------");
    Serial.println(_buffer);
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// CONTADOR DE EVENTOS LOCAL - NAO SOBE PARA O SERVIDOR
Serial.print("Medida(s) realizada(s): [ "); Serial.print(counterLocal);
Serial.println(" ]"); counterLocal++;









/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
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
        Serial.println("... 2 COMUTANDO ESTADO DO LED HIGH ...");
      } 
      if ( _action == "OFF") {
        digitalWrite(LED_PIN, LOW);
        Serial.println("... 3 COMUTANDO ESTADO DO LED LOW ...");
      }
    }
//delay(5000);
Serial.println("... NAO AGUARDOU DELAY DE 5000 ...");
// Attempt to connect to website
    if ( !getPage() ) {
      Serial.println("GET request failed");
    }
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
}} yield();}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
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
    delay(100);}
// Turn LED on when we are connected
//  digitalWrite(LED_PIN, LOW);
//  Serial.println("... 4 COMUTANDO ESTADO DO LED ...");
  }
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
// Perform an HTTP GET request to a remote page
bool getPage() {
// Attempt to make a connection to the remote server
  Serial.print(" Site http: ");
  Serial.print(http_site);
  Serial.print(" Porta: ");
  Serial.println(http_port);
  if ( !client.connect(http_site, http_port) ) {
    return false;}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// PRESSURE / TEMPERATURE / HUMIDITY MEASUREMENTS
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
ReadBME280Data();         
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
measCounter = 0;  // restart local counter
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
Serial.println("\n- - - - - - - - - - - - - - - - -");
Serial.print("Pressao atmosferica.....: ");
Serial.print(P_bme, 2);    Serial.println(" hPa");
Serial.print("Umidade relativa do ar..: ");
Serial.print(U_bme, 2);    Serial.println(" %UR");
Serial.print("Temperatura ambiente....: ");
Serial.print(T_bme, 2);    Serial.print(" *C");
Serial.println("\n- - - - - - - - - - - - - - - - -");
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  l4 = random(100);
  l5 = random(100);
  lcnt = random(100);
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  v1 = String(P_bme, 2);
  v2 = String(U_bme, 2);
  v3 = String(T_bme, 2);
  ltoa(l4, v4, 10);
  ltoa(l5, v5, 10);
  ltoa(lcnt, cnt, 10);
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
String _comando;
  _comando  = "GET /api-app4iot.php?act=GRV&tkn=iop2oi1jhu87hgn1hgqnbaiuk187jd&dis=cab4lpawluyxc30pkhb1mw410k17x9&var=wlg7tvk6wmhr91hc08q4wh2ik7mqa7";
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
Serial.println(_comando);  // faz o print na serial da pagina web inteira...
// Make an HTTP GET request
  client.println(_comando);
  client.print("Host: ");
  client.println(http_site);
  client.println("Connection: close");
  client.println();
return true;}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
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
      valor.setCharAt( p, pos_k2);}
return valor;}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void printBME280Data(Stream* client){
   float temp(NAN), hum(NAN), pres(NAN);
   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_Pa);
   bme.read(pres, temp, hum, tempUnit, presUnit);
   client->print("Temp: ");
   client->print(temp);
   client->print("°"+ String(tempUnit == BME280::TempUnit_Celsius ? 'C' :'F'));
   client->print("\t\tHumidity: ");
   client->print(hum);
   client->print("% RH");
   client->print("\t\tPressure: ");
   client->print(pres);
   client->println(" Pa");
//  delay(1000);
}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
float ReadBME280Data(){
//double P_bme = 0, U_bme = 0, T_bme = 0;
//   float temp(NAN), hum(NAN), pres(NAN);
//   float temp, hum, pres;   // TO GLOBAL VARS...
   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_hPa);
   bme.read(pres, temp, hum, tempUnit, presUnit);
P_bme = pres;
U_bme = hum;
T_bme = temp;
return (P_bme, U_bme, T_bme);}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
