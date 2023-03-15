int motor = 9;
int motor2 = 10;
int INA = 7;
int INB = 8;
int speed = 0; 
int pwmIncrement = 5; 
int testDir = 2;





void setup() {

   pinMode(motor, OUTPUT);
   pinMode(motor2, OUTPUT);
   pinMode(testDir, OUTPUT);
}



void loop() {

   analogWrite(motor, 255);
   
   speed = speed + pwmIncrement;

   if (speed == 0 || speed == 255) {
      pwmIncrement = -pwmIncrement ;
   }

    digitalWrite(testDir,HIGH);

   delay(300);
}