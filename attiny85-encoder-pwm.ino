/* Rotary Encoder Demo v2

   David Johnson-Davies - www.technoblogy.com - 28th October 2017
   ATtiny85 @ 1 MHz (internal oscillator; BOD disabled)
   
   CC BY 4.0
   Licensed under a Creative Commons Attribution 4.0 International license: 
   http://creativecommons.org/licenses/by/4.0/
*/

// ATtiny85 inputs/outputs
const int LED = 0;
const int EncoderA = 1;
const int EncoderB = 2;
const int EncoderSwitch = 3;

// Rotary encoder **********************************************

volatile int a0;
volatile int c0;
volatile int s0;
volatile int Brightness = 0;

// Called when encoder value changes
void ChangeValue (bool Up) {
  Brightness = max(min(Brightness + (Up ? 1 : -1), 510), 0);
  analogWrite(LED, (Brightness+1)/2);
}

// Pin change interrupt service routine
ISR (PCINT0_vect) {
  int a = PINB>>EncoderA & 1;
  int b = PINB>>EncoderB & 1;
  int s = PINB>>EncoderSwitch & 1;
  if (a != a0) {              // A changed
    a0 = a;
    if (b != c0) {
      c0 = b;
      ChangeValue(a == b);
    }
  } else if (s != s0) {
    s0 = s;
    ChangeSwitch();
  }
}

// Called when encoder switch is pressed; On is 0 (down) or 1 (up)
void ChangeSwitch () {
  Brightness = 0;
  analogWrite(LED, 0);
}
  
void setup() {
  pinMode(LED, OUTPUT);
  analogWrite(LED, 0);
  pinMode(EncoderA, INPUT_PULLUP);
  pinMode(EncoderB, INPUT_PULLUP);
  pinMode(EncoderSwitch, INPUT_PULLUP);
  // Configure pin change interrupts
  PCMSK = 1<<EncoderA | 1<<EncoderSwitch;
  GIMSK = 1<<PCIE;                       // Enable interrupt
  GIFR = 1<<PCIF;                        // Clear interrupt flag
}

// Everything done under interrupt!
void loop() {
}
