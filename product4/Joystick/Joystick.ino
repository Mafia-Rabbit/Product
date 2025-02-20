const int SW_pin = 2;
const int X_pin = A0;
const int Y_pin = A1;

void setup() {
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(9600);
}

void loop() {
  int sw = digitalRead(SW_pin);
  int x = analogRead(X_pin);
  int y = analogRead(Y_pin);

  Serial.print(x);
  Serial.print(",");
  Serial.println(y);
  delay(25);
}