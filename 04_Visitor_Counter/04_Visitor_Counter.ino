#define inSensor D7
#define outSensor D5
 
int peopleInRoom = 0;
boolean flag1 = false;
boolean flag2 = false;

int DIN_PIN = D4;      // data in pin
int CS_PIN = D3;       // load (CS) pin
int CLK_PIN = D2;      // clock pin

const uint64_t DIGIT[] = {
  0x3c66666666663c00,
  0x7e18181838181800,
  0x7e60300c06663c00,
  0x3c66061c06663c00,
  0x0c0c7e4c2c1c0c00,
  0x3c6606067c607e00,
  0x3c66667c60663c00,
  0x1818180c0c667e00,
  0x3c66663c66663c00,
  0x3c66063e66663c00
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(inSensor, INPUT);
  pinMode(outSensor, INPUT);
  init_MAX7219();
}

void loop() {

  putWord(DIGIT[peopleInRoom]);
  
  if(digitalRead (inSensor) == 0 && flag1==0)
  {
    if(peopleInRoom<9)
    {
      flag1=1;
      if(flag2==1)
      {
        peopleInRoom++;
      }
    }
  }

  if(digitalRead (outSensor) == 0 && flag2==0)
  {
    flag2=1;
    if(flag1==1)
    {
      peopleInRoom--;
    }
  }

  if(flag1==1 && flag2==1){
    delay(1000);
    flag1=0, flag2=0;
  }
  delay(1);
}

void init_MAX7219()
{
  pinMode(DIN_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT);
    // MAX7219 registers
  byte MAXREG_DECODEMODE = 0x09;
  byte MAXREG_INTENSITY  = 0x0a;
  byte MAXREG_SCANLIMIT  = 0x0b;
  byte MAXREG_SHUTDOWN   = 0x0c;
  byte MAXREG_DISPTEST   = 0x0f;
  
  // initialization of the MAX7219
  setRegistry(MAXREG_SCANLIMIT, 0x07);
  setRegistry(MAXREG_DECODEMODE, 0x00);  // using an led matrix (not digits)
  setRegistry(MAXREG_SHUTDOWN, 0x01);    // not in shutdown mode
  setRegistry(MAXREG_DISPTEST, 0x00);    // no display test
  setRegistry(MAXREG_INTENSITY, 0x0f & 0x00);
}
void putWord(uint64_t data)
{
  byte reg = 0;

  for(reg = 0; reg < 8; reg++)
  {
    setRegistry(reg+1, data & 0xFF);
    data = data >> 8;
  }
}

void setRegistry(byte reg, byte value)
{
  digitalWrite(CS_PIN, LOW);

  putByte(reg);   // specify register
  putByte(value); // send data

  digitalWrite(CS_PIN, LOW);
  digitalWrite(CS_PIN, HIGH);
}

void putByte(byte data)
{
  byte i = 8;
  byte mask;
  while (i > 0)
  {
    mask = 0x01 << (i - 1);        // get bitmask
    digitalWrite( CLK_PIN, LOW);   // tick
    if (data & mask)               // choose bit
      digitalWrite(DIN_PIN, HIGH); // send 1
    else
      digitalWrite(DIN_PIN, LOW);  // send 0
    digitalWrite(CLK_PIN, HIGH);   // tock
    --i;                           // move to lesser bit
  }
}
