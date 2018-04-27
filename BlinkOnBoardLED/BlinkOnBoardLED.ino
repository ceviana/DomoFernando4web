#define LED LED_BUILTIN            // Led in NodeMCU at pin GPIO16
// LED_BUILTIN IS INVERTED LOGIC - ATTENTION HERE!!!!!!!!!!!!!!!!!!
void setup() {
pinMode(LED_BUILTIN, OUTPUT);    // LED pin as output.
}
void loop() {
digitalWrite(LED_BUILTIN, LOW); // turn the LED on.
delay(1000); // wait for 1 second.
digitalWrite(LED_BUILTIN, HIGH);// turn the LED off.(Note that LOW is the voltage level but actually 
                        //the LED is on; this is because it is acive low on the ESP8266.
delay(1000);            // wait for 1 second.
}
