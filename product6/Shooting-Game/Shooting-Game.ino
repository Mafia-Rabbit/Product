#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int RED_BUTTON = 4;
int BLUE_BUTTON = 3;
int SHOT_BUTTON = 2;

struct Ball { int x; int y; bool active; };
struct Enemy { int x; int y; bool active; };

#define MAX_BALLS 5
#define MAX_ENEMIES 3

Ball balls[MAX_BALLS];
Enemy enemies[MAX_ENEMIES];

unsigned long lastEnemySpawnTime = 0;
unsigned long enemySpawnInterval = 2000; // 敵の生成間隔 (ミリ秒)
int enemySpeed = 3;
int score = 0;
int lives = 3; // プレイヤーのライフ
bool gameOver = false;
bool gameClear = false;
bool gameStarted = false;
int difficulty = 0; // 0: Easy, 1: Normal, 2: Hard

void setup() {
    Serial.begin(115200);
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 disconnection"));
        for (;;);
    }
    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(BLUE_BUTTON, INPUT_PULLUP);
    pinMode(SHOT_BUTTON, INPUT_PULLUP);
    display.clearDisplay(); //初期化
}

static int x = SCREEN_WIDTH / 2;
static int y = 64;

void loop() {
    if (!gameStarted) {
        displayMenu();
        return;
    }
    if (gameOver) {
        drawGameOver();
        return;
    }
    if (gameClear) {
        drawGameClear();
        return;
    }
    if (digitalRead(RED_BUTTON) == HIGH) // ボタンが押されたとき
        x = x - 5;
    else if (digitalRead(BLUE_BUTTON) == HIGH)
        x = x + 5;
    if (digitalRead(SHOT_BUTTON) == HIGH) {
        for (int i = 0; i < MAX_BALLS; i++) {
            if (!balls[i].active) {
                balls[i] = {x, y - 10, true}; // ボールを生成
                break;
            }
        }
    }
    if (millis() - lastEnemySpawnTime > enemySpawnInterval) {
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (!enemies[i].active) {
                enemies[i] = {random(0, SCREEN_WIDTH), 0, true}; // 敵を生成
                lastEnemySpawnTime = millis();
                break;
            }
        }
    }
    moveBalls();
    moveEnemies();
    checkCollisions();
    draw();
}

void moveBalls() {
    for (int i = 0; i < MAX_BALLS; i++) {
        if (balls[i].active) {
            balls[i].y -= 5; // ボールを上に移動
            if (balls[i].y < 0) {
                balls[i].active = false; // 画面外に出たら非アクティブにする
            }
        }
    }
}

void moveEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            enemies[i].y += enemySpeed; // 敵を下に移動
            if (enemies[i].y > SCREEN_HEIGHT) {
                enemies[i].active = false; // 画面外に出たら非アクティブにする
                lives--; // ライフを減らす
                if (lives <= 0) {
                    gameOver = true;
                }
            }
        }
    }
}

void checkCollisions() {
    for (int i = 0; i < MAX_BALLS; i++) {
        if (balls[i].active) {
            for (int j = 0; j < MAX_ENEMIES; j++) {
                if (enemies[j].active) {
                    // 球と敵の当たり判定
                    int dx = balls[i].x - enemies[j].x;
                    int dy = balls[i].y - enemies[j].y;
                    int distance = sqrt(dx * dx + dy * dy);
                    if (distance < 10) { // 判定のための距離
                        balls[i].active = false; // 球を非アクティブにする
                        enemies[j].active = false; // 敵を非アクティブにする
                        score += 10; // スコアに10ポイント追加
                        if (score >= 300) {
                            gameClear = true;
                            gameOver = false; // ゲームオーバーのフラグをリセット
                        }
                    }
                }
            }
        }
    }
}

void displayMenu() {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print("Select Difficulty:");
    display.setCursor(0, 10);
    display.print("RED: Easy");
    display.setCursor(0, 20);
    display.print("BLUE: Normal");
    display.setCursor(0, 30);
    display.print("SHOT: Hard");
    display.display();

    if (digitalRead(RED_BUTTON) == HIGH) {
        difficulty = 0;
        enemySpeed = 3;
        lives = 3;
        gameStarted = true;
    } else if (digitalRead(BLUE_BUTTON) == HIGH) {
        difficulty = 1;
        enemySpeed = 10;
        lives = 3;
        gameStarted = true;
    } else if (digitalRead(SHOT_BUTTON) == HIGH) {
        difficulty = 2;
        enemySpeed = 10;
        lives = 1;
        gameStarted = true;
    }
}

void draw() {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print("Score: ");
    display.print(score); // スコアを表示
    display.setCursor(SCREEN_WIDTH/2, 0);
    display.print(" LIFE: ");
    display.print(lives); // ライフを表示
    display.drawCircle(x, y, 10, WHITE); // プレイヤーを描画
    for (int i = 0; i < MAX_BALLS; i++) {
        if (balls[i].active) {
            display.fillCircle(balls[i].x, balls[i].y, 2, WHITE); // ボールを描画
        }
    }
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            display.fillRect(enemies[i].x, enemies[i].y, 8, 8, WHITE); // 敵を描画
        }
    }
    display.display();
    delay(100);
}

void drawGameOver() {
    display.clearDisplay();
    display.setCursor(0, SCREEN_HEIGHT / 2);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.print("Game Over"); // ゲームオーバーを表示
    display.display();
}

void drawGameClear() {
    display.clearDisplay();
    display.setCursor(0, SCREEN_HEIGHT / 2);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.print("Game Clear"); // ゲームクリアを表示
    display.display();
}
