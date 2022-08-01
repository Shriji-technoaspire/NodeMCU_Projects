unsigned int LED[6] = {D0, D1, D3, D5, D7, D8}; 

int i = 0, j=0;

void setup() {
  // put your setup code here, to run once:
  for(i=0; i<6; i++)
  {
    pinMode(LED[i], OUTPUT);
    digitalWrite(LED[i], LOW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
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
