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
               FUNCIONA COM BIBLIOTECE ESP-07 COMMUNITY ATE VERSAO 2.3.0
               ATENCAO - VERSAO 2.4 NAO FUNCIONA PARA ESTE MODELO ESP-07
               __________________________________
               ATENCAO NAO COMPILAR ESP-07 NA VERSAO 2.4 OU SUPERIOR!!!!
               __________________________________
               PLACA:             GENERIC ESP8266 MODULE
               FLASH MODE:        DIO
               FLASH SIZE:        1M (512K SPIFFS)
               DEBUG PORT:        DISABLED <--<
               DEBUG LEVEL:       RIEN <--<
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
               PORTA: PORTA ESP CONECTADA AO COMPUTADOR
               __________________________________                       */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// LIBRARIES TO WORK TOGETHER
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// PINOUTS - DEFINES - DESCRICOES
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define ESP8266_GPIO2     2               // Blue LED.
#define RELAY             4               // Relay control. 
#define ESP8266_GPIO5     5               // Optocoupler input.
#define LED_PIN           ESP8266_GPIO2
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define SERIAL_BAUD       115200
#define DATA_ATUAL  "13/05/2018"  // MODIFICAR p/ DATA ULTIMA ATUALIZACAO
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// Wi-Fi network settings
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
//const char* ssid = "DARK2";
//const char* password = "2003311200300";
const char* ssid = "ESP_GUEST";
const char* password = "01234567890";
//const char* ssid = "VIANA";
//const char* password = "15373420883";
// URL to task providing script, that will be polled each POLL_INTERVAL ms.
// The endpoint script should return one of the following strings as plain text
// in order to execute desired actions: PWR_ON, PWR_OFF, RESET, SHUTDOWN
// const char*  // old
String endpoint = "http://ceviana.com/pc-power/endpoint.php";
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
unsigned times = 0;   //  counter times var
// How often the tasks server should be polled, ms
#define POLL_INTERVAL     15000
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// SETUP - ALL SETTINGS
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void setup() {
Serial.begin(SERIAL_BAUD);
Serial.setDebugOutput(true);
  pinMode( RELAY, OUTPUT );       // Relay control pin.
  pinMode( ESP8266_GPIO5, INPUT_PULLUP ); // Input pin.
  pinMode( LED_PIN, OUTPUT );             // ESP8266 module blue LED.
  digitalWrite( RELAY, LOW );       // Set relay control pin low.
Serial.println();
byte ledStatus = LOW;
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
    // Blink the LED.
    digitalWrite( LED_PIN, ledStatus ); // Write LED high/low.
    ledStatus = ( ledStatus == HIGH ) ? LOW : HIGH;
    delay( 200 );
    Serial.print('.');      delay(200);}
Serial.println("\n| - - - - - - - - - - - - - - - - - - - - - - |");
  Serial.println("| WI-FI RELAY ESP8266 CONTROL SYSTEM!         |");
  Serial.println("| IDEALIZADO PARA FERNANDO - DOMOTICA         |");
  Serial.println("| VERSAO INICIAL - MAIO/2018!                 |");
  Serial.println("| - - - - - - - - - - - - - - - - - - - - - - |");
    Serial.print("| RELAY - Rev.: 1 - Versao: "); 
Serial.print(DATA_ATUAL); Serial.println("        |");
// Syncronize time from NTP servers
configTime(3 * 3600, 0, "ua.pool.ntp.org", "time.nist.gov");
}   // END SETUP
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// FUNCTION TO POWER ON PC IF IT IS NOT
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void doRelayOn() {
  if (isRelayOn()) {
    Serial.println("|> RELE ESTA LIGADO. TAREFA NAO REALIZADA!");
    return;}
digitalWrite(RELAY, HIGH);
Serial.print("ESTADO RELE: "); Serial.println(digitalRead(RELAY));
Serial.println("|> COMANDO PARA RELIGAR RELE FOI ENVIADO!");
reportStatus();}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// FUNCTION POWER OFF RELAY - FORCA DESLIGAMENTO
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void doRelayOff(){Serial.print("> - - - - - - > ");  Serial.println(times);
  if (!isRelayOn()) {
    Serial.println("|> RELE JA SE ENCONTRA DESLIGADO. TAREFA NAO REALIZADA!");
return;}
digitalWrite(RELAY, LOW);
Serial.print("ESTADO RELE: "); Serial.println(digitalRead(RELAY));
Serial.println("|> COMANDO PARA DESLIGAR RELE FOI ENVIADO!");
reportStatus();}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// FUNCTION TO VERIFY RELE IS POWERED ON OR NOT
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
bool isRelayOn() {
  return digitalRead(RELAY);}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// FUNCTION TO SEND REPORT STATUS
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void reportStatus() {
  Serial.print("|> ESTADO DO RELE: power ");
  if (isRelayOn()) {Serial.println("LIGADO"); 
            } else {Serial.println("DESLIGADO");}}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// FUNCTION POOLTASKS
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void pollTasks() { times++;
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("|> Wi-Fi NAO ESTA CONECTADO. NAO E POSSIVEL VERIFICAR TAREFAS!\n");}
  HTTPClient http;
// ENVIANDO SITUACAO DE LIGADO OU DESLIGADO PARA O SERVIDOR
String _comando = endpoint;
  _comando += "?estado=";
  if(isRelayOn()){
  _comando += "ON";}
  else {_comando += "OFF";}
  http.begin(_comando);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8"); 
  char request[64];
  snprintf(request, sizeof request, "status=%d", isRelayOn());
  int httpCode = http.POST(request);
  Serial.printf("|> [HTTP] POST success. Code: %d\n", httpCode);
  Serial.printf("|> Number of resquests: %d\n", times); 
  if (httpCode != HTTP_CODE_OK) {
    Serial.printf("|> [HTTP] POST failed, error: %s\n", http.errorToString(httpCode).c_str());
    http.end();
    return;}
  String payload = http.getString();
  Serial.print("|>----->  ");
  Serial.println(payload);
  if (payload == "PWR_OFF") {
    doRelayOff();
  } else if (payload == "PWR_ON") {
    doRelayOn();
//  } else if (payload == "SHUTDOWN") {
//    do_shutdown();
//  } else if (payload == "RESET") {
//    do_reset();
    } http.end();}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// LOOP
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void loop() {
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* here is where you'd put code that needs to be running all the time.
check to see if it's time to blink the LED; that is, if the difference
between the current time and last time you blinked the LED is bigger 
than the interval at which you want to blink the LED. */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* unsigned long currentMillis = millis();
if (currentMillis - previousMillis >= interval) { // save the last time you blinked the LED
previousMillis = currentMillis; // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;} // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
    digitalWrite(LD_GRN, ledState);
    digitalWrite(LD_RED, ledState);
} //  END - ELAPSED TIME WITHOUT DELAY 
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*TgglBlnkLed(0, 5, 400);
// void TgglBlnkLed(short LedColour, short xTimes, short dlTime){
TgglBlnkLed(1, 5, 400);
TgglBlnkLed(2, 1, 1500);
TgglBlnkLed(5, 0, 0); 
//delay(500);
doResetPC(); //IoNowState();
doPoweroffPC(); //IoNowState();
//times++;*/

digitalWrite( LED_PIN, HIGH );
delay(POLL_INTERVAL);
reportStatus();
pollTasks();                                           } // END MAIN LOOP!
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// END
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*    digitalWrite(LD_RED, !digitalRead(LD_RED));  */
