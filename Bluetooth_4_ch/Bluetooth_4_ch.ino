#include <SoftwareSerial.h>
SoftwareSerial mySerial(9, 8); // TX, RX

#define relay1 7 // define relay1 Interface
#define relay2 4 // define relay2 Interface
#define relay3 3 // define relay3 Interface
#define relay4 2 // define relay4 Interface

#define stat_led 10 //Status LED

void setup ()
{
  mySerial.begin(9600);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(stat_led, OUTPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(stat_led, HIGH);
}
void loop ()
{
  digitalWrite(stat_led, HIGH);
  while (mySerial.available() > 0 )
  {
    delay(10);
    char ch = mySerial.read();
    mySerial.println(ch);
    switch (ch)
    {
      case 'A' :
        digitalWrite(relay1, LOW); // 'X' to turn on Relay
        break;
      case 'a' :
        digitalWrite(relay1, HIGH); // 'x' to turn off Relay
        break;
      case 'B' :
        digitalWrite(relay2, LOW);
        break;
      case 'b' :
        digitalWrite(relay2, HIGH) ;
        break;
      case 'C' :
        digitalWrite(relay3, LOW);
        break;
      case 'c' :
        digitalWrite(relay3, HIGH) ;
        break;
      case 'D' :
        digitalWrite(relay4, LOW);
        break;
      case 'd' :
        digitalWrite(relay4, HIGH) ;
        break;
      case '#' :
        digitalWrite(relay1, HIGH);
        digitalWrite(relay2, HIGH);
        digitalWrite(relay3, HIGH);
        digitalWrite(relay4, HIGH);
        break;
      case '@' :
        digitalWrite(relay1, LOW);
        digitalWrite(relay2, LOW);
        digitalWrite(relay3, LOW);
        digitalWrite(relay4, LOW);
        break;
    }
  }
}
