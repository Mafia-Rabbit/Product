const int redPin = 4;
const int greenPin = 2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
}

void loop() {
  if (Serial.available() > 0){
    String msg = Serial.readString();

    if (msg == "ON"){
      digitalWrite(greenPin, HIGH);
    }
    else if(msg == "OFF"){
      digitalWrite(greenPin, LOW);
    }

    else {
      for (int i = 0; i < 5; i++){
        digitalWrite(redPin, HIGH);
        delay(100);
        digitalWrite(redPin, LOW);
        delay(100);
      }
    }
  }
}