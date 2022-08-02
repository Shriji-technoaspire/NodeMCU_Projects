
#include <ESP8266WiFi.h>

String  i;
WiFiServer server(80);

int DIN_PIN = D7;      // data in pin
int CS_PIN = D4;       // load (CS) pin
int CLK_PIN = D5;      // clock pin
int Buzzer = D8;
int LED = D6;

int LeftSideForward = D3;
int LeftSideBackward = D2;
int RightSideForward = D0;
int RightSideBackward = D1;

const unsigned char Left_Arrow[] = 
  {
  0b00010000,
  0b00110000,
  0b01111111,
  0b11111111,
  0b01111111,
  0b00110000,
  0b00010000,
  0b00000000
};

const unsigned char Right_Arrow[] = 
  {
  0b00001000,
  0b00001100,
  0b11111110,
  0b11111111,
  0b11111110,
  0b00001100,
  0b00001000,
  0b00000000
};

const unsigned char Forward_Arrow[] = 
  {
  0b00010000,
  0b00111000,
  0b01111100,
  0b11111110,
  0b00111000,
  0b00111000,
  0b00111000,
  0b00111000
};

const unsigned char Down_Arrow[] = 
  {
  0b00111000,
  0b00111000,
  0b00111000,
  0b00111000,
  0b11111110,
  0b01111100,
  0b00111000,
  0b00010000
};

void setup() {
  i = "";
  Serial.begin(9600);
  pinMode(RightSideForward, OUTPUT);
  pinMode(LeftSideForward, OUTPUT);
  pinMode(LeftSideBackward, OUTPUT);
  pinMode(RightSideBackward, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(LED, OUTPUT);
  WiFi.disconnect();
  Serial.println("........................................");
  Serial.println("       welcome to stembotix");
  Serial.println("........................................");
  delay(3000);
  WiFi.begin("MayuHotspot","hemu6514");                          //     <~~~~~~~ WIFI SSID PASSWORD
  Serial.println("CONNECTING TO WIFI");
  while ((!(WiFi.status() == WL_CONNECTED))){
    delay(300);
    Serial.print(".");

  }
  Serial.println("I AM CONNECTED");
  Serial.println((WiFi.localIP().toString()));
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();
  if (!client) { return; }
  while(!client.available()){  delay(1); }
  i = (client.readStringUntil('\r'));
  i.remove(0, 5);
  i.remove(i.length()-9,9);
  if (i == "forword") {
    Write_DotMatrix(Forward_Arrow);
    digitalWrite(LeftSideForward, HIGH);
    digitalWrite(LeftSideBackward, LOW);
    digitalWrite(RightSideForward, HIGH);
    digitalWrite(RightSideBackward, LOW);
  }

  if (i == "revwrse") {
    Write_DotMatrix(Down_Arrow);
    digitalWrite(LeftSideForward, LOW);
    digitalWrite(LeftSideBackward, HIGH);
    digitalWrite(RightSideForward, LOW);
    digitalWrite(RightSideBackward, HIGH);
  }

  if (i == "right") {
    Write_DotMatrix(Right_Arrow);
    digitalWrite(LeftSideForward, HIGH);
    digitalWrite(LeftSideBackward, LOW);
    digitalWrite(RightSideForward, LOW);
    digitalWrite(RightSideBackward, HIGH);
  }

  if (i == "left") {
    Write_DotMatrix(Left_Arrow);
    digitalWrite(LeftSideForward, LOW);
    digitalWrite(LeftSideBackward, HIGH);
    digitalWrite(RightSideForward, HIGH);
    digitalWrite(RightSideBackward, LOW);
  }

  if (i == "led") {
    digitalWrite(LED,HIGH);
  }
  if (i == "ledoff") {
    digitalWrite(LED,LOW);
  }
  if (i == "horn") {
    digitalWrite(Buzzer,HIGH);
  }
  if (i == "hornoff") {
    digitalWrite(Buzzer,LOW);
  }
}

void Init_Func()
{
  // MAX7219 registers
  byte MAXREG_DECODEMODE = 0x09;
  byte MAXREG_INTENSITY  = 0x0a;
  byte MAXREG_SCANLIMIT  = 0x0b;
  byte MAXREG_SHUTDOWN   = 0x0c;
  byte MAXREG_DISPTEST   = 0x0f;
  int INTENSITYMIN = 0; // minimum brightness, valid range [0,15]

  pinMode(DIN_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT);
  
  // initialization of the MAX7219
  setRegistry(MAXREG_SCANLIMIT, 0x07);
  setRegistry(MAXREG_DECODEMODE, 0x00);  // using an led matrix (not digits)
  setRegistry(MAXREG_SHUTDOWN, 0x01);    // not in shutdown mode
  setRegistry(MAXREG_DISPTEST, 0x00);    // no display test
  setRegistry(MAXREG_INTENSITY, 0x0f & INTENSITYMIN);

}

//SPI Comunication Functions
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

void Write_DotMatrix(const unsigned char *Data)
{
    setRegistry(1, Data[0]);
    setRegistry(2, Data[1]);
    setRegistry(3, Data[2]);
    setRegistry(4, Data[3]);
    setRegistry(5, Data[4]);
    setRegistry(6, Data[5]);
    setRegistry(7, Data[6]);
    setRegistry(8, Data[7]);
}
