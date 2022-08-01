unsigned int LED[6]= {D0, D1, D3, D5, D7, D8};
unsigned int Button = D6;
unsigned int pattern = 0, ButtonState = 0;
int i = 0, j=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for(i=0; i<6; i++)
  {
    pinMode(LED[i], OUTPUT);
    digitalWrite(LED[i], LOW);
  }
  //digitalWrite(Button, INPUT);
  attachInterrupt(digitalPinToInterrupt(Button), IntCallback, RISING);
}

void loop() {
  
    if(pattern == 0)
      Right_To_Left();
    else if(pattern == 1)
      Left_To_Right();
    else if(pattern == 2)
      Compress();
    else
      Decompress();
}

ICACHE_RAM_ATTR void IntCallback()
{
  Serial.println("Interrupt");
    if(pattern == 3)
      pattern = 0;
    else
      pattern++;
}

void Right_To_Left() {
  for(i=0; i<6; i++)
  {
      for(j=0; j<6; j++)
      {
        if(i==j)
        digitalWrite(LED[j], HIGH);
        else
        digitalWrite(LED[j], LOW);
      }
     delay(1000);   
  }   
}

void Left_To_Right() {
  for(i=6; i>=0; i--)
  {
      for(j=6; j>=0; j--)
      {
        if(i==j)
        digitalWrite(LED[j], HIGH);
        else
        digitalWrite(LED[j], LOW);
      }
     delay(1000);   
  }   
}

void Compress()
{
  for(i=0; i<3; i++)
  {
      for(j=0; j<6; j++)
      {
        if(j==(5 - i) || i == j)
        digitalWrite(LED[j], HIGH);
        else
        digitalWrite(LED[j], LOW);
      }
     delay(1000);   
  }
}

void Decompress()
{
  for(i=2; i>=0; i--)
  {
      for(j=0; j<6; j++)
      {
        if(j==(5 - i) || i == j)
        digitalWrite(LED[j], HIGH);
        else
        digitalWrite(LED[j], LOW);
      }
     delay(1000);   
  }
}
