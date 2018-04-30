/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define SERIAL_BAUD 115200
#define LED LED_BUILTIN            // Led in NodeMCU at pin GPIO16
// LED_BUILTIN IS INVERTED LOGIC - ATTENTION HERE!!!!!!!!!!!!!!!!!!
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
unsigned times = 0;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void setup() {
Serial.begin(SERIAL_BAUD);
Serial.println("\nLED blinks 1 time fos second!");
pinMode(LED_BUILTIN, OUTPUT);    // LED pin as output.
digitalWrite(LED_BUILTIN, HIGH);   // LED STARTS OFF - INVERTED LOGIC
}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void loop() {
digitalWrite(LED_BUILTIN, LOW); // turn the LED on.
Serial.print("... 1 COMUTANDO ESTADO DO LED ... \t");
Serial.println(times);
delay(2000); // wait for 1 second.
digitalWrite(LED_BUILTIN, HIGH);// turn the LED off.(Note that LOW is the voltage level but actually 
                        //the LED is on; this is because it is acive low on the ESP8266.
Serial.println("... 0 COMUTANDO ESTADO DO LED ...");
delay(1000);            // wait for 1 second.
times++; }
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
