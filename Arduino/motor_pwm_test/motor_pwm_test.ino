int PWM = ;
int INA = 7;
int INB = 8;
int EN = A0;
int speed = 0; 
int pwmIncrement = 5; 


void setup() {

  Serial.begin(9600);
  pinMode(PWM,OUTPUT);
  pinMode(INA,OUTPUT);
  pinMode(INB,OUTPUT);
  pinMode(EN,OUTPUT);

}

void loop() {

  digitalWrite(INA,HIGH); 
  digitalWrite(INB,LOW);
  digitalWrite(EN,HIGH);

  analogWrite(PWM, speed);
   
  speed = speed + pwmIncrement;

   if (speed == 0 || speed == 255) {
      pwmIncrement = -pwmIncrement ;
   }
  
  Serial.print(speed);

  

  delay(300);

}