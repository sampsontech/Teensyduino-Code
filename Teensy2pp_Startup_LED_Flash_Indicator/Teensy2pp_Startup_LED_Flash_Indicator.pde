/* LED Blink, Teensyduino Tutorial #1
   http://www.pjrc.com/teensy/tutorial.html
 
   This example code is in the public domain.
*/

const int ledPin = 6;   // Teensy has LED on 11, Teensy++ on 6

// the setup() method runs once, when the sketch starts

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
  delay(2000);                       // wait for a bit then let the main loop start
}

// the loop() methor runs over and over again,
// as long as the board has power

void loop() {
  digitalWrite(ledPin, HIGH);   // set the LED on
  delay(1000);                  // wait for a second
  digitalWrite(ledPin, LOW);    // set the LED off
  delay(1000);                  // wait for a second
}

