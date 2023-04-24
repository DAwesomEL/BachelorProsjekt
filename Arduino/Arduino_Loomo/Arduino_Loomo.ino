int PWM1 = 6;
int PWM2 = 5;
int En1 = 4;
int En2 = 7;
int pwmVal = 0;
int dir = 1;
int ENCA = 2; 
int ENCB = 3;
 
long int pos = 0;


int ledPin = 13;

void setup() {
  
  Serial.begin(9600); // Initialize serial communication with a baud rate of 9600
  
  pinMode(ledPin, OUTPUT);
  pinMode(En1, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(En2, OUTPUT);
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);
  digitalWrite(En1, HIGH);
  digitalWrite(En2, HIGH);

}

void loop() {

  //Serial.println("Hello, Loomo!");
  
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

            //speed = 128;
            break;
        } 
  } 
  //delay(5000);
}

  
  
void readEncoder(){
int b = digitalRead(ENCB);
if(b>0){
  pos++;
}
else{
  pos--;
}
}

  
  






