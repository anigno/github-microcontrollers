int pwmPin = 9;
int frequency = 15000; // Starting frequency
int dutyCycle = 128; // 50% duty cycle

void setup() {
  pinMode(pwmPin, OUTPUT);
}

void loop() {
  int period = 1000000 / frequency; // Calculate period in microseconds
  int pulseWidth = period * dutyCycle / 256; // Calculate pulse width in microseconds
  
  digitalWrite(pwmPin, HIGH); // Start the pulse
  delayMicroseconds(pulseWidth); // Pulse width determines duty cycle
  digitalWrite(pwmPin, LOW); // End the pulse
  delayMicroseconds(period - pulseWidth); // Rest of period is off
  
  frequency += 100; // Increase frequency by 100 Hz
  if (frequency > 20000) { // If frequency goes above 20 kHz, reset to 15 kHz
    frequency = 15000;
  }
}
