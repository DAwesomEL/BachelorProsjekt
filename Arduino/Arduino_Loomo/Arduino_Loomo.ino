int PWM1 = 6;
int PWM2 = 5;
int En1 = 4;
int En2 = 7;
int pwmVal = 0;
int ledPin = 13;

void setup() {
  Serial.begin(9600); // Initialize serial communication with a baud rate of 9600
  pinMode(ledPin, OUTPUT);
  pinMode(En2, OUTPUT);
  pinMode(En1, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  digitalWrite(En1, HIGH);
  digitalWrite(En2, HIGH);

}

void loop() {

if (Serial.available() > 0) { // Check if there's data available on the serial port
    char command = Serial.read(); // Read the incoming byte
    switch (command) {
          case 'F': // Forward
            analogWrite(PWM1, 0);
            analogWrite(PWM2, 255);
            break;
          case 'B': // Backward
            analogWrite(PWM1, 255);
            analogWrite(PWM2, 0);
            break;
          case 'S': // Stop

            digitalWrite(ledPin, LOW); // Turn the LED off
            analogWrite(PWM1, 0);
            analogWrite(PWM2, 0);
            break;
          case 'G': // Turn on led
            digitalWrite(ledPin, HIGH); // Turn the LED on
            
            break;
        } 
  } 
}

  
  



