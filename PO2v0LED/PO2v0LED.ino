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
#include       <Arduino.h>
//#include     <Wire.h>             // Needed for legacy versions of Arduino
//#include     <ESP8266WiFi.h>
//#include     <WiFiClient.h>
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// PINOUTS - DEFINES - DESCRICOES
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define      LD_GRN        5         // GPIO-11  LD0 >--> LED GREEN - PO2
// ATENCAO - AQUI !!!!!!!!!! ADC NAO PODE SER USADO COM OSAIDA DIGITAL!!!
#define      LD_RED       13         // GPIO-13  LD1 >----> LED RED - PO2
#define      PC_LON       16         // GPIO-16 >--> ENT_PC_LED_ON_L
#define      PC_PWR       14         // GPIO-14 >--> SAI_PC_PWR_L
#define      PC_RST       12         // GPIO-12 >--> SAI_PC_RST_L
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
//#define      ATL7          5         // GPIO-05 + SCL >--> PULLUP INTERNO
//#define      ATL8          4         // GPIO-04 + SDA >--> PULLUP INTERNO
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define      ATL4         15         // GPIO-15 + ESTADO NORMAL DO ESP / PERMITE ROTINAS E RESTART
#define      ATL9          2         // GPIO-02 + LED NATIVO DO ESP8266 / PERMITE ROTINAS E RESTART
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define SERIAL_BAUD 115200
#define DATA_ATUAL  "08/04/2018"  // MODIFICAR p/ DATA ULTIMA ATUALIZACAO
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
unsigned times = 0;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// constants won't change. Used here to set a pin number :
//const int ledPin =  LED_BUILTIN;// the number of the LED pin
// Variables will change :
// int ledState = HIGH ;             // ledState used to set the LED
// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
// unsigned long previousMillis = 0;        // will store last time LED was updated
// constants won't change :
// const long interval = 1000;           // interval at which to blink (milliseconds)
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// FUNCTION SHOW READER ALL I/O STATES IN THIS TIME
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// FIRST OF ALL READ ALL GPIO I/O
void IoNowState(){ times++;
Serial.print("| PC_LON | PC_PWR | PC_RST | LD_GRN | LD_RED  | ");
Serial.println(times);
Serial.print("| ");
Serial.print(digitalRead(PC_LON)); Serial.print("\t | ");
Serial.print(digitalRead(PC_PWR)); Serial.print("\t  | ");
Serial.print(digitalRead(PC_RST)); Serial.print("\t   | ");
Serial.print(digitalRead(LD_GRN)); Serial.print("\t    | ");
Serial.print(digitalRead(LD_RED)); Serial.println("\t      |");}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// SETUP - ALL SETTINGS
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void setup() {
Serial.begin(SERIAL_BAUD);
Serial.println("\n| - - - - - - - - - - - - - - - - - - - - - - |");
Serial.println("| WI-FI POWER ON OFF RESET - REMOTE SYSTEM!   |");
Serial.println("| IDEALIZADO POR MARCUS!                      |");
Serial.println("| VERSAO INICIAL - ABR/2018!                  |");
Serial.println("| - - - - - - - - - - - - - - - - - - - - - - |");
Serial.print("| WPO2R - Rev.: 1 - Versao: "); 
Serial.print(DATA_ATUAL); Serial.println("        |");
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
//  ESP8266 - SPECIFICS
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  pinMode(ATL4, OUTPUT);     digitalWrite(ATL4, HIGH);   // GPIO-15 + ESTADO NORMAL DO ESP / HIGH
  pinMode(ATL9, OUTPUT);     digitalWrite(ATL9, HIGH);   // GPIO-02 + ESTADO NORMAL DO ESP / HIGH
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// GPIO I/O SETTINGS AND INICIAL STATE - LOGIC LEVEL
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
pinMode(PC_LON, INPUT);         //digitalWrite(PC_PWR, LOW);  // PC_LON OFF
//pinMode(PC_LON, INPUT_PULLUP);  digitalWrite(PC_PWR, LOW);  // PC_LON OFF
pinMode(PC_PWR, OUTPUT);        //digitalWrite(PC_PWR, LOW);  // PC_PWR OFF
pinMode(PC_RST, OUTPUT);        //digitalWrite(PC_RST, LOW);  // PC_RST OFF
pinMode(LD_GRN, OUTPUT);        //digitalWrite(LD_GRN, LOW);  // LD_GRN OFF
pinMode(LD_RED, OUTPUT);        //digitalWrite(LD_RED, LOW);  // LD_GRN OFF
// set the digital pin as output:
//pinMode(ledPin, OUTPUT);   digitalWrite(ledPin, HIGH);  // LED_BUILTIN OFF
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
}   // END SETUP
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// LOOP
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void loop() { IoNowState();
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
digitalWrite(LD_RED, LOW);  // LD_RED ON
delay(150);
digitalWrite(LD_GRN, HIGH);  // LD_GRN OFF
IoNowState(); delay(1500); 
digitalWrite(LD_RED, HIGH);  // LD_RED ON
delay(150);
digitalWrite(LD_GRN, LOW);  // LD_GRN OFF
IoNowState(); delay(1500);
// SAIDAS DIGITAIS
digitalWrite(PC_PWR, LOW);  // PC_PWR OFF
digitalWrite(PC_RST, LOW);  // PC_RST OFF
Serial.println("| - - - - - - - - - - - - - - - - - - - - - - |");
Serial.println("| PC_PWR >--> NLS [1] - - - - - - - - - - - - |");
Serial.println("| PC_RST >--> NLS [1] - - - - - - - - - - - - |");
Serial.println("| - - - - - - - - - - - - - - - - - - - - - - |");
IoNowState(); delay(1500);
digitalWrite(PC_PWR, HIGH);  // PC_PWR ON
digitalWrite(PC_RST, HIGH);  // PC_RST ON
Serial.println("| - - - - - - - - - - - - - - - - - - - - - - |");
Serial.println("| PC_PWR >--> NLI [0] - - - - - - - - - - - - |");
Serial.println("| PC_RST >--> NLI [0] - - - - - - - - - - - - |");
Serial.println("| - - - - - - - - - - - - - - - - - - - - - - |");
IoNowState(); delay(1500);
} // END MAIN LOOP!
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// END
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
