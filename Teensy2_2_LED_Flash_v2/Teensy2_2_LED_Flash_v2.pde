// 2 LED Blink - Version 2
// A simple flashing LED example with a switch to trigger "stuff"
// by David Sampson 
// 9/08/2011


const int ledPin = 11;   // Teensy has LED on 11, Teensy++ on 6


const int led_1_pos = 21;
const int led_1_gnd = 20;
const int led_2_pos = 19;
const int led_2_gnd = 18;
const int led_3_pos = 17;
const int led_3_gnd = 16;
const int led_4_pos = 15;
const int led_4_gnd = 14;

const int sw_1_common = 4;   // common - connected to ground
const int sw_1_open = 5;   // open position - used for input with pullup resistor

volatile int led_blink_delay = 100;  //LED blink rate

void setup() {
  Serial.println("Starting Setup...");

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
  delay(500);                       // wait for a bit then let the main loop start
  
  //Set up the LED pins as output and turn the LED's off
  pinMode(led_1_pos, OUTPUT);
  digitalWrite(led_1_pos, LOW);
  pinMode(led_1_gnd, OUTPUT);
  digitalWrite(led_1_gnd, LOW);
  
  pinMode(led_2_pos, OUTPUT);
  digitalWrite(led_2_pos, LOW);
  pinMode(led_2_gnd, OUTPUT);
  digitalWrite(led_2_gnd, LOW);

  pinMode(led_3_pos, OUTPUT);
  digitalWrite(led_3_pos, LOW);
  pinMode(led_3_gnd, OUTPUT);
  digitalWrite(led_3_gnd, LOW);

  pinMode(led_4_pos, OUTPUT);
  digitalWrite(led_4_pos, LOW);
  pinMode(led_4_gnd, OUTPUT);
  digitalWrite(led_4_gnd, LOW);

  //Set up the switch pins
  pinMode(sw_1_common, OUTPUT);
  digitalWrite(sw_1_common, LOW);    // make the common a ground low / ground pin
  pinMode(sw_1_open, INPUT_PULLUP);

  //Set up the interrupt call for the switch
  // *** Note that this also triggers the interrupt! ***
  attachInterrupt(0, switch_interrupt, CHANGE);
  
  // Ensure interrupts are turned on
  interrupts();

  Serial.println("Ending Setup..."); 
}

// the loop() methor runs over and over again, as long as the board has power
void loop() {
  
  digitalWrite(led_1_pos, HIGH);   // turn the LED on
  delay(led_blink_delay);          // wait for a period
  digitalWrite(led_1_pos, LOW);    // turn the LED off
  digitalWrite(led_2_pos, HIGH);   // turn the LED on
  delay(led_blink_delay);          // wait for a period
  digitalWrite(led_2_pos, LOW);    // turn the LED off
  digitalWrite(led_3_pos, HIGH);   // turn the LED on
  delay(led_blink_delay);          // wait for a period
  digitalWrite(led_3_pos, LOW);    // turn the LED off
  digitalWrite(led_4_pos, HIGH);   // turn the LED on
  delay(led_blink_delay);          // wait for a period
  digitalWrite(led_4_pos, LOW);    // turn the LED off
}

void switch_interrupt()
{
  static unsigned long debounce;
  
  // test if the switch is bouncing (within 2000 microseconds of last valid trigger)
  if (micros() > (debounce + 000)) {
    // the switch is not bouncing - consider it a true switch trigger
    debounce = micros();
        
    // Display the microsecond timer value
    Serial.print(micros());
 
    // test of the switch is closed or open
    if (digitalRead(sw_1_open) == HIGH) {
      led_blink_delay = 50;      // shorten the LED blink delay so that they blink faster
      Serial.println(" - Switch Interrupt - Switch Pressed...");
    }  
    else {
      led_blink_delay = 100;    // set the LED blink delay to normal speed
      Serial.println(" - Switch Interrupt - Switch Released...");
    }
  } 
}

