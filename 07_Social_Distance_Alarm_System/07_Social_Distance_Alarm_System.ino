int trigPin = D1;
int echoPin = D2;
int Buzzer = D6;
 
 
void setup() 
{
  Serial.begin(9600); 
  pinMode(Buzzer, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
 
void loop() {
  long duration, distance;
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  duration=pulseIn(echoPin, HIGH);
  distance =(duration/2)/29.1;
  delay(10);

  Serial.print(distance);
  Serial.println("CM");
  
  if(distance <= 30) 
    digitalWrite(Buzzer, HIGH);
  else
    digitalWrite(Buzzer, LOW);
}
