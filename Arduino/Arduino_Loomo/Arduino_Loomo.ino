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
          case 'W': // Start Wave function
            // Wave
            break;
          case 'H': // Go to home position
            // Home
            break;
          case 'S': // Stop wave function
            // Stop
            break;
        } 
  } 
}

  
/* 



*/


