#include <Wire.h>
#include <Adafruit_NeoPixel.h>

// MPU-6050 I2Cアドレス
#define MPU6050_ADDR 0x68

// RGB LED設定
const int PIN_SIG = 2;
const int NUM_PIXELS = 8;
Adafruit_NeoPixel pixels(NUM_PIXELS, PIN_SIG, NEO_GRB + NEO_KHZ800);

// センサデータを格納する変数
int16_t ax, ay, az, gx, gy, gz, Temp;
int16_t prevGy = 0;
const int threshold = -200; // -200度の変化を検出する閾値

void setup() {
  Serial.begin(115200);
  Serial.println("--- Started ---");
  
  Wire.begin();
  
  // MPU6050の初期化
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6B); // MPU6050_PWR_MGMT_1レジスタ
  Wire.write(0x00); // センサを起動するために0を設定
  Wire.endTransmission();
  
  pixels.begin();
  pixels.setBrightness(20);
}

void loop() {
  // センサデータの読み取り
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 14, true);
  
  ax = Wire.read() << 8 | Wire.read();
  ay = Wire.read() << 8 | Wire.read();
  az = Wire.read() << 8 | Wire.read();
  Temp = Wire.read() << 8 | Wire.read();
  gx = Wire.read() << 8 | Wire.read();
  gy = Wire.read() << 8 | Wire.read();
  gz = Wire.read() << 8 | Wire.read();
  
  // センサデータの変換と表示
  Serial.print(ax / 16384.0); Serial.print(" g, ");
  Serial.print(ay / 16384.0); Serial.print(" g, ");
  Serial.print(az / 16384.0); Serial.print(" g, ");
  Serial.print(gx / 131.0); Serial.print(" deg/s, ");
  Serial.print(gy / 131.0); Serial.print(" deg/s, ");
  Serial.print(gz / 131.0); Serial.println(" deg/s, ");
  
  // y軸角速度が前回よりも200度以上低下した場合にLEDを点灯
  if (gy - prevGy < threshold * 131) {
    lightUpLEDs();
  }

  // 前回のgy値を更新
  prevGy = gy;

  delay(20);
}

void lightUpLEDs() {
  for (int j = 0; j < 2; j++) { // 明るくするのと暗くするのを2回繰り返す
    // LEDを徐々に明るくする
    for (int brightness = 0; brightness <= 255; brightness += 5) {
      pixels.clear();
      for (int i = 0; i < NUM_PIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(brightness, 0, 0));
      }
      pixels.show();
      delay(20);
    }

    // LEDを徐々に暗くする
    for (int brightness = 255; brightness >= 0; brightness -= 5) {
      pixels.clear();
      for (int i = 0; i < NUM_PIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(brightness, 0, 0));
      }
      pixels.show();
      delay(20);
    }
  }
}
