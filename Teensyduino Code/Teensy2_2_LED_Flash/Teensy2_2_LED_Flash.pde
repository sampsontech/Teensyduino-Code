// 2 LED Blink
// A simple flashing LED example with a switch to trigger "stuff"
// by David Sampson 
// 20/7/2011


const int ledPin = 11;   // Teensy has LED on 11, Teensy++ on 6


const int led_1_pos = 21;
const int led_1_gnd = 20;
const int led_2_pos = 19;
const int led_2_gnd = 18;
const int sw_1_common = 17;   // common - used for input with pullup resister
const int sw_1_open = 16;   // open position - connected to ground

void setup() {
  // initialize the digital pin as an output and turn it off.
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  // loop for the number of times the startup LED flash while cycle
  for (int a = 40; a >0; a--)
  {
    digitalWrite(ledPin, HIGH);        // Turn the LED on.
    delay(a);                          // leave the LED on for this long
    digitalWrite(ledPin, LOW);         // Turn the LED off.
    delay(a);                          // leave the LED off for this long
  }
  delay(1000);                       // wait for a bit then let the main loop start
  
  //Set up the 2 LED pins as output and turn the LED's off
  pinMode(led_1_pos, OUTPUT);
  digitalWrite(led_1_pos, LOW);
  pinMode(led_1_gnd, OUTPUT);
  digitalWrite(led_1_gnd, LOW);
  pinMode(led_2_pos, OUTPUT);
  digitalWrite(led_2_pos, LOW);
  pinMode(led_2_gnd, OUTPUT);
  digitalWrite(led_2_gnd, LOW);

  //Set up the switch pins
  pinMode(sw_1_common, OUTPUT);
  digitalWrite(sw_1_common, LOW);    // make the common a ground low / ground pin
  pinMode(sw_1_open, INPUT_PULLUP);
}

// the loop() methor runs over and over again, as long as the board has power
void loop() {
  digitalWrite(led_1_pos, HIGH);   // turn the first LED on
  delay(100);                      // wait for a period
  digitalWrite(led_1_pos, LOW);    // turn the first LED off
  digitalWrite(led_2_pos, HIGH);   // turn the second LED on
  delay(100);                      // wait for a period
  digitalWrite(led_2_pos, LOW);    // turn the second LED off
  
  //Test if the switch is pressed
  if (digitalRead(sw_1_open) == HIGH) {
    digitalWrite(led_2_pos, HIGH);      // turn the second LED on again so that both are on together
  }  
}

