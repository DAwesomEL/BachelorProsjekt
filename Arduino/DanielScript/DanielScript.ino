int PWM1 = 6;
int PWM2 = 5;
int En1 = 4;
int En2 = 7;
int pwmVal = 0;
int dir = 1;
int ENCA = 2; 
int ENCB = 3;
 
long int pos = 0;

void setup() {
  Serial.begin(9600);
  pinMode(En1, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(En2, OUTPUT);
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);
  digitalWrite(En1, HIGH);
  digitalWrite(En2, HIGH);

}

void loop(){

Serial.println(pos);


  if(dir == 1){
    analogWrite(PWM1, 255);
    analogWrite(PWM2, 0);
    }

  else{
    analogWrite(PWM1, 0);
    analogWrite(PWM2, pwmVal);
    }

  if(pos >= 22500 || pos < 0 ) {

    pwmVal = 0;
    
    dirSwap(dir);
  }
 
  pwmVal++;
  delay(10);
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

void dirSwap(int& dir)
{
  if(dir != 1){
  dir = 1;}
  else{
  dir = 0;}
}
