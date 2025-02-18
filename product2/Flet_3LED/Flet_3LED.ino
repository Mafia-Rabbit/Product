#define BEEP 2
#define BLUE 3
#define GREEN 4
#define RED 5
#define delayTime 10

void setup()
{
  Serial.begin(9600);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(BEEP, OUTPUT);
}

void loop()
{
  if (Serial.available() > 0){
    String msg = Serial.readString();
    if (msg == "BLUE"){
      digitalWrite(BLUE, HIGH);
      digitalWrite(GREEN, LOW);
      digitalWrite(RED, LOW);
    }
    else if(msg == "GREEN"){
      digitalWrite(BLUE, LOW);
      digitalWrite(GREEN, HIGH);
      digitalWrite(RED, LOW);
    }
    else if(msg == "RED"){
      digitalWrite(BLUE, LOW);
      digitalWrite(GREEN, LOW);
      digitalWrite(RED, HIGH);
    }
    else if(msg == "OFF" || msg == "exit"){
      digitalWrite(BLUE, LOW);
      digitalWrite(GREEN, LOW);
      digitalWrite(RED, LOW);
    }
    else {
      for (int i = 0; i < 5; i++){
        digitalWrite(BLUE, HIGH);
        digitalWrite(GREEN, HIGH);
        digitalWrite(RED, HIGH);
        tone(BEEP,330,100);
        delay(100);
        digitalWrite(BLUE, LOW);
        digitalWrite(GREEN, LOW);
        digitalWrite(RED, LOW);
        delay(100);
      }
    }
  }
}