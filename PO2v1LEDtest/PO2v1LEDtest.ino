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
/*   CADA GPIO POSSUI UMA IDENTIFICACAO ESPECIFICA
     PORTAS UTILIZADAS NAS PLACAS DA MINI ESTACAO CLIMATICA
     ATL3     >--> GPIO-16 
     ATL4     >--> GPIO-14 
     ATL5     >--> GPIO-12 + LED1
     ATL7     >--> GPIO-05 + SCL >--> PULLUP INTERNO / SENSOR BMP-180 (PRESSAO)
     ATL8     >--> GPIO-04 + SDA >--> PULLUP INTERNO / SENSOR BMP-180 (PRESSAO)
     /RST     >--> ---[10k]--+3V3  -//- JUMPER COM 0V P/ "RESET"
                   +++[103]---CERAMIC CAPACITOR
     CH-PD    >--> ---[10k]--+3V3 + 103 CERAMIC CAPACITOR
                   +++[103]---CERAMIC CAPACITOR
     GPIO-02  >--> ---[10k]--+3V3
     GPIO-00  >--> ---[10k]--+3V3  -//- JUMPER COM 0V P/ "FLASH"
     GPIO-15  >--> ---[10k]---0V
     RX + TX    >--> CONEXOES PARA GRAVADOR EXTERNO
     PROCEDIMENTO PARA GRAVACAO COM GRAVADOR FTDI
     FTDI-TX  >--> ATL-RX
     FTDI-RX  >--> ATL-TX
     FTDI-3V3 >--> ATL-3V3
     FTDI-0V  >--> ATL-0V
     NUNCA ALIMENTAR ESTE MODULO DIRETAMENTE PELO GRAVADOR
     OU USB DO COMPUTADOR! */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Blink without Delay
 Turns on and off a light emitting diode (LED) connected to a digital
 pin, without using the delay() function.  This means that other code
 can run at the same time without being interrupted by the LED code.
 The circuit:
 * Use the onboard LED.
 * Note: Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO 
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products
 http://www.arduino.cc/en/Tutorial/BlinkWithoutDelay  */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// LIBRARIES TO WORK TOGETHER
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// PINOUTS - DEFINES - DESCRICOES
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define      ATL4         15    // GPIO-15 + ESTADO NORMAL DO ESP / PERMITE ROTINAS E RESTART
#define      ATL9          2    // GPIO-02 + LED NATIVO DO ESP8266 / PERMITE ROTINAS E RESTART
#define      PWR_PIN      14    // Pin for the PWR signal line
#define      RST_PIN      12    // Pin for the RST signal line
#define      STATUS_PIN   16    // Pin for the status LED signal line
#define      LD_GRN       13    // GPIO-11  LD0 >--> LED GREEN - PO2
#define      LD_RED        5    // GPIO-13  LD1 >----> LED RED - PO2
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// DEFINES TIMES TO TASKS
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define ESP_SERIAL        Serial
// How long the PowerOFF button should be pressed to power off PC forcefully
#define PWR_OFF_TIME      4500
// How long the button should be pressed to REBOOT, POWER ON or RESET
#define PUSH_TIME         400
// How often the tasks server should be polled, ms
#define POLL_INTERVAL     20000
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define SERIAL_BAUD 115200
#define DATA_ATUAL  "270518"  // MODIFICAR p/ DATA ULTIMA ATUALIZACAO
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
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// constants won't change. Used here to set a pin number :
//const int ledPin =  LED_BUILTIN;// the number of the LED pin
// Variables will change :
// int ledState = HIGH ;             // ledState used to set the LED
// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
// unsigned long previousMillis = 0;// will store last time LED was updated
// constants won't change :
// const long interval = 1000;// interval at which to blink (milliseconds)
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// FUNCTION TIME WITHOUT DELAY FUNCTION
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void PassTime(unsigned msecs){}
//      lastPass = millis() + 5000UL;
/* The "UL" after the number is a syntax detail that is important when 
dealing with large numbers in millis and micros, therefore it is shown
although this is pseudo code. */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
unsigned short Xled = 0;  // LED BLINK TIMES COUNTER
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// FUNCTION BLINK TOGGLE RED LED
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void TgglBlnkLed(short LedColour, short xTimes, short dlTime){
/*  LedColour   >-->   0 = RED; 1 = GREEN; 2 = RED + GREEN 
 *                     3 = TOGGLES RED; 4 TOGGLES GREEN
 *  xTimes      >-->   TIMES TO BLINK
    dlTime      >-->   NLS TIME LED  */
switch (LedColour){
case 0: { for (Xled = 0; Xled < xTimes; Xled++){
          digitalWrite(LD_RED, HIGH); 
          delay(dlTime);
          digitalWrite(LD_RED, LOW);  
          delay(dlTime / 4);
          }break;}  
case 1: { for (Xled = 0; Xled < xTimes; Xled++){
          digitalWrite(LD_GRN, HIGH); 
          delay(dlTime); //PassTime(dlTime); 
          digitalWrite(LD_GRN, LOW);  
          delay(dlTime / 4);
          } break;} 
case 2: { for (Xled = 0; Xled < xTimes; Xled++){
          digitalWrite(LD_RED, HIGH); digitalWrite(LD_GRN, HIGH); 
          delay(dlTime); //PassTime(dlTime); 
          digitalWrite(LD_RED, LOW);  digitalWrite(LD_GRN, LOW); 
          delay(dlTime / 4);
          } break;}  
case 3: { digitalWrite(LD_RED, !digitalRead(LD_RED)); break;}
case 4: { digitalWrite(LD_GRN, !digitalRead(LD_GRN)); break;}
case 5: { digitalWrite(LD_RED, LOW); digitalWrite(LD_GRN, LOW); break;}
case 6: { digitalWrite(LD_RED, HIGH); digitalWrite(LD_GRN, LOW); break;}
case 7: { digitalWrite(LD_RED, LOW); digitalWrite(LD_GRN, HIGH); break;}
case 8: { digitalWrite(LD_RED, HIGH); digitalWrite(LD_GRN, HIGH); break;}}}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// FUNCTION SHOW READ ALL I/O STATES AT THIS TIME
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// FIRST OF ALL READ ALL GPIO
void IoNowState(){ times++;
Serial.print("| PC_LON | PC_PWR | PC_RST | LD_GRN | LD_RED  | ");
Serial.println(times);
Serial.print("| ");
Serial.print(digitalRead(STATUS_PIN)); Serial.print("\t | ");
Serial.print(digitalRead(PWR_PIN)); Serial.print("\t  | ");
Serial.print(digitalRead(RST_PIN)); Serial.print("\t   | ");
Serial.print(digitalRead(LD_GRN)); Serial.print("\t    | ");
Serial.print(digitalRead(LD_RED)); Serial.println("\t      |");}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// SETUP - ALL SETTINGS
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void setup() {
//  pinMode(ATL5, OUTPUT);     digitalWrite(ATL5, HIGH);   // GPIO-16 + LED0 / INICIA HIGH E TERMINA SETUP LOW
  pinMode(ATL4, OUTPUT);     digitalWrite(ATL4, HIGH);   // GPIO-15 + ESTADO NORMAL DO ESP / HIGH
  pinMode(ATL9, OUTPUT);     digitalWrite(ATL9, HIGH);   // GPIO-02 + ESTADO NORMAL DO ESP / HIGH

Serial.begin(SERIAL_BAUD);
Serial.setDebugOutput(true);
Serial.println();
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print('.');
      delay(1000);}
Serial.println("\n| - - - - - - - - - - - - - - - - - - - - - - |");
Serial.println("| WI-FI POWER ON OFF RESET - REMOTE SYSTEM!   |");
Serial.println("| IDEALIZADO PARA MARCUS!                     |");
Serial.println("| VERSAO INICIAL - ABR/2018!                  |");
Serial.println("| - - - - - - - - - - - - - - - - - - - - - - |");
Serial.print("| WPO2R - Rev.: 1 - Versao: "); 
Serial.print(DATA_ATUAL); Serial.println("        |");
// Syncronize time from NTP servers
configTime(3 * 3600, 0, "ua.pool.ntp.org", "time.nist.gov");
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
//  ESP8266 - SPECIFICS
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  pinMode(ATL4, OUTPUT);     digitalWrite(ATL4, HIGH);   // GPIO-15 + ESTADO NORMAL DO ESP / HIGH
  pinMode(ATL9, OUTPUT);     digitalWrite(ATL9, HIGH);   // GPIO-02 + ESTADO NORMAL DO ESP / HIGH
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// GPIO I/O SETTINGS AND INICIAL STATE - LOGIC LEVEL
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
pinMode(STATUS_PIN, INPUT);    //digitalWrite(PC_PWR, LOW);  // PC_LON OFF
//pinMode(PC_LON, INPUT_PULLUP);
pinMode(PWR_PIN, OUTPUT);      //digitalWrite(PC_PWR, LOW);  // PC_PWR OFF
pinMode(RST_PIN, OUTPUT);      //digitalWrite(PC_RST, LOW);  // PC_RST OFF
pinMode(LD_GRN, OUTPUT);       //digitalWrite(LD_GRN, LOW);  // LD_GRN OFF
pinMode(LD_RED, OUTPUT);       //digitalWrite(LD_RED, LOW);  // LD_GRN OFF
}   // END SETUP
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// FUNCTION TO SHUTDOWN PC IF IT IS ON - TOQUE RAPIDO NO INTERRUPTOR
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void do_shutdown() {
if (!isPoweredOn()) {
Serial.println("|> PC JA SE ENCONTRA DESLIGADO. TAREFA NAO REALIZADA!");
TgglBlnkLed(0, 5, 400); 
return;}
digitalWrite(PWR_PIN, HIGH);
Serial.print("ESTADO PC_PWR: "); Serial.println(digitalRead(PWR_PIN));
delay(PUSH_TIME); digitalWrite(PWR_PIN, LOW);
Serial.print("ESTADO PC_PWR: "); Serial.println(digitalRead(PWR_PIN)); 
TgglBlnkLed(1, 5, 400);
Serial.println("|> COMANDO PARA SHUTDOWN PC FOI ENVIADO!");
reportStatus();}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// FUNCTION TO POWER ON PC IF IT IS NOT
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void do_powerOn() {
  if (isPoweredOn()) {
    Serial.println("|> PC ESTA LIGADO. TAREFA NAO REALIZADA!");
    TgglBlnkLed(0, 5, 400);
    return;}
digitalWrite(PWR_PIN, HIGH);
Serial.print("ESTADO PC_PWR: "); Serial.println(digitalRead(PWR_PIN));
delay(PUSH_TIME); digitalWrite(PWR_PIN, LOW);
Serial.print("ESTADO PC_PWR: "); Serial.println(digitalRead(PWR_PIN)); 
TgglBlnkLed(1, 5, 400);
Serial.println("|> COMANDO PARA RELIGAR PC FOI ENVIADO!");
reportStatus();}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// FUNCTION POWER OFF PC - SEGURA O INTERRUPTOR - FORCA DESLIGAMENTO
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void do_powerOff(){Serial.print("> - - - - - - > ");  Serial.println(times);
TgglBlnkLed(0, 5, 400);
  if (!isPoweredOn()) {
    Serial.println("|> PC JA SE ENCONTRA DESLIGADO. TAREFA NAO REALIZADA!");
    TgglBlnkLed(1, 5, 400); return;}
digitalWrite(PWR_PIN, HIGH);
Serial.print("ESTADO PC_PWR : "); Serial.println(digitalRead(PWR_PIN));
delay(PWR_OFF_TIME); digitalWrite(PWR_PIN, LOW);
Serial.print("ESTADO PC_PWR: "); Serial.println(digitalRead(PWR_PIN)); 
TgglBlnkLed(2, 5, 400);
Serial.println("|> COMANDO PARA DESLIGAR PC FOI ENVIADO!");
reportStatus();}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// FUNCTION TO VERIFY THAT PC IS POWERED ON OR NOT
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
bool isPoweredOn() {
  return digitalRead(STATUS_PIN);}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// FUNCTION TO SEND REPORT STATUS
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void reportStatus() {
  Serial.print("|> ESTADO DO PC: power ");
  if (isPoweredOn()) {
    Serial.println("LIGADO"); TgglBlnkLed(6, 0, 0);
  } else {
    Serial.println("DESLIGADO"); TgglBlnkLed(7, 0, 0);}}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// FUNCTION TO RESET PC IF IT IS ON
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void do_reset() {Serial.print("> - - - - - - > ");  Serial.println(times);
TgglBlnkLed(3, 0, 0); 
  if (!isPoweredOn()) {
    Serial.println("|> PC DESLIGADO. TAREFA NAO REALIZADA!");}
digitalWrite(RST_PIN, HIGH);
Serial.print("ESTADO PC_RST: "); Serial.println(digitalRead(RST_PIN)); 
delay(PUSH_TIME); digitalWrite(RST_PIN, LOW);
Serial.print("ESTADO PC_RST: "); Serial.println(digitalRead(RST_PIN)); 
TgglBlnkLed(3, 0, 0);
Serial.println("|> COMANDO DE RESET PC FOI ENVIADO!");
reportStatus();}
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
  if(isPoweredOn()){
  _comando += "ON";}
  else {_comando += "OFF";}
  http.begin(_comando);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8"); 
  char request[64];
  snprintf(request, sizeof request, "status=%d", isPoweredOn());
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
    do_powerOff();
  } else if (payload == "PWR_ON") {
    do_powerOn();
  } else if (payload == "SHUTDOWN") {
    do_shutdown();
  } else if (payload == "RESET") {
    do_reset();} http.end();}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// LOOP
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void loop() { //IoNowState();
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


delay(POLL_INTERVAL);
reportStatus();
pollTasks();


} // END MAIN LOOP!
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// END
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*    digitalWrite(LD_RED, !digitalRead(LD_RED));  */
