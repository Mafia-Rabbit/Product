#include <Arduino.h>
#include <ctype.h>

const int LED = 10;

int meter = 0;
int seconds = 0;
int mode = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
}

void extractNumbers(const char* str, int* numbers, int* count) {
    *count = 0;
    while (*str) {
        if (isdigit(*str)) {
            numbers[(*count)++] = strtol(str, (char**)&str, 10);
        } else {
            str++;
        }
    }
}

void loop()
{
  if (Serial.available() > 0){
    String msg = Serial.readString();
    const char* input = msg.c_str();
    int numbers[10];
    int count;

    extractNumbers(input, numbers, &count);
    if (count >= 2) {
      meter = numbers[0];
      seconds = 1000 * numbers[1];
      mode = 1;
    }
  } 

  if (mode == 1){
    delay(seconds);

      for(int i = 0; i <= 255; i++){
        analogWrite(LED, i);
        delay(10);
      }for(int i = 255; i >= 0; i--){
        analogWrite(LED, i);
        delay(10);
      }    
    mode = 0;
  }
}