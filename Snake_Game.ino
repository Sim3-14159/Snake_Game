#include <Wire.h>
#include <U8g2lib.h>
#include <EEPROM.h>

// Initialize the display
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(
  U8G2_R0,
  /* reset=*/ U8X8_PIN_NONE
);

#define uButton 2
#define dButton 3
#define lButton 4
#define rButton 5
#define R_LED 7
#define G_LED 6
#define BUZZER A3
#define aButton A1

#define snakeSize 10

byte snakeX[75], snakeY[75];
byte snakeLength;
byte foodX, foodY;
byte direction = 0;
byte changeDirection = 0;
bool gameOver = false;
bool hasStarted = false;

void setup() {
  /* uncomment the following line the first time you upload to reset EEPROM. Then, once uploaded, comment the line again and re-upload, to prevent the high score
  * being reset every setup()
   */
  //EEPROM.write(0, 0);  // Reset high score once
  randomSeed(analogRead(A0));
  u8g2.begin();

  pinMode(uButton, INPUT_PULLUP);
  pinMode(dButton, INPUT_PULLUP);
  pinMode(lButton, INPUT_PULLUP);
  pinMode(rButton, INPUT_PULLUP);
  pinMode(R_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(aButton, INPUT_PULLUP);

  digitalWrite(G_LED, HIGH);
  digitalWrite(R_LED, HIGH);
}

void loop() {
  static bool wasGameOver = false;
  static unsigned long int k = 0;

  // ----------- START SCREEN ------------
  if (!hasStarted) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_logisoso16_tr);

    u8g2.drawStr(27,17,  "S");
    u8g2.drawStr(37,20,  "N");
    u8g2.drawStr(47,17,  "A");
    u8g2.drawStr(57,20,  "K");
    u8g2.drawStr(67,17,  "E");
    u8g2.drawStr(77,20, "!");
    u8g2.drawStr(84,20, "!");
    u8g2.drawStr(91,17, "!");
    
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.setCursor(15, 50);
    u8g2.print(F("Press <A> to start"));
    u8g2.sendBuffer();

    if (!digitalRead(aButton)) {
      hasStarted = true;
      resetGame();
      delay(300); // debounce
    }
    return;
  }

  if (gameOver) {
    // Static local variables inside gameOver block
    static int buzzerTimer = 0;
    static bool highScoreChecked = false;
    static bool showHighScore = false;
    static byte highScore = 0;

    // Detect rising edge of gameOver state
    if (!wasGameOver) {
      // Just entered gameOver state, reset statics
      buzzerTimer = 0;
      highScoreChecked = false;
      showHighScore = false;
      highScore = 0;
    }

    if (!highScoreChecked) {
      byte storedScore = EEPROM.read(0);
      byte currentScore = snakeLength - 2;
      if (currentScore > storedScore) {
        EEPROM.write(0, currentScore);
        highScore = currentScore;
        showHighScore = true;
      } else {
        showHighScore = false;
      }
      highScoreChecked = true;
    }

    if (!digitalRead(aButton)) {
      gameOver = false;
      resetGame();
      hasStarted = true;
      noTone(BUZZER);
      delay(300);
      // Also update wasGameOver so next frame it knows we left gameOver state
      wasGameOver = false;
      return; // immediately exit so loop continues fresh next time
    }

    if (buzzerTimer < 15) {  // about 1 second at 34ms delay + calculation time
      tone(BUZZER, 400);
      buzzerTimer++;
    } else {
      noTone(BUZZER);
    }

    digitalWrite(R_LED, LOW);

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_logisoso16_tr);
    u8g2.setCursor(18, 20);
    u8g2.print(F("Game Over!"));

    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.setCursor(15, 40);
    u8g2.print(F("Press <A> to restart"));

    if (showHighScore) {
      u8g2.setCursor(12, 60);
      u8g2.print(F("New High Score: "));
      u8g2.print(highScore);
    }

    u8g2.sendBuffer();

    wasGameOver = true;  // Remember state for next frame
    return;
  } else {
    wasGameOver = false; // Update if we are not in game over state
  }
  

  // ----------- MAIN GAME LOOP ------------
  readJoystick();

  if (!(k % 3)) {
    digitalWrite(G_LED, HIGH);
    noTone(BUZZER);
    digitalWrite(R_LED, HIGH);

    // Update direction safely
    if (changeDirection == 0 && direction != 2) direction = 0;
    else if (changeDirection == 2 && direction != 0) direction = 2;
    else if (changeDirection == 3 && direction != 1) direction = 3;
    else if (changeDirection == 1 && direction != 3) direction = 1;

    moveSnake();
    checkCollision();
    draw();
  }

  delay(34);
  k++;
}

void readJoystick() {
  if (!digitalRead(rButton)) 
  {changeDirection = 0;}
  else if (!digitalRead(lButton)) 
  {changeDirection = 2;}
  else if (!digitalRead(uButton)) 
  {changeDirection = 3;}
  else if (!digitalRead(dButton)) 
  {changeDirection = 1;}
}

void moveSnake() {
  byte newX = snakeX[0];
  byte newY = snakeY[0];

  switch (direction) {
    case 0: newX += snakeSize; break;
    case 1: newY += snakeSize; break;
    case 2: newX -= snakeSize; break;
    case 3: newY -= snakeSize; break;
  }

  for (byte i = snakeLength; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  snakeX[0] = newX;
  snakeY[0] = newY;
}

void checkCollision() {
  if (((snakeX[0] <= foodX + 5) && (snakeX[0] >= foodX - 5)) &&
      ((snakeY[0] >= foodY - 5) && (snakeY[0] <= foodY + 5))) {
    snakeLength++;
    spawnFood();
    digitalWrite(G_LED, LOW);
    tone(BUZZER, 600);
  }

  if (snakeX[0] < 0 || snakeX[0] >= 128 || snakeY[0] < 0 || snakeY[0] >= 64) {
    gameOver = true;
  }

  for (byte i = 1; i < snakeLength; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      gameOver = true;
    }
  }
}

void draw() {
  u8g2.firstPage();
  do {
    u8g2.setDrawColor(1);
    u8g2.drawFrame(0, 0, 128, 64);

    // Draw the snake
    for (byte i = 0; i < snakeLength; i++) {
      u8g2.drawBox(snakeX[i], snakeY[i], snakeSize, snakeSize);
    }

    // Draw round food
    u8g2.drawDisc(2 + foodX + (snakeSize / 2), foodY + snakeSize / 2, snakeSize / 2);

    // Display the score
    u8g2.setFont(u8g2_font_5x8_tr); // Small readable font
    u8g2.setCursor(3, 8);
    u8g2.print(F("Score: "));
    u8g2.print(snakeLength - 2); // Score starts at 0
  } while (u8g2.nextPage());
}

void spawnFood() {
  foodX = random(0, 128 / snakeSize) * snakeSize;
  foodY = random(0, 64 / snakeSize) * snakeSize;
}

void resetGame() {
  randomSeed(analogRead(A0));
  snakeLength = 2;
  snakeX[0] = 64;
  snakeY[0] = 32;
  snakeX[1] = snakeX[0] - snakeSize;
  snakeY[1] = snakeY[0];
  direction = 0;
  changeDirection = 0;
  gameOver = false;
  spawnFood();
}