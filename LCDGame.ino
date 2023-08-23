#include <LiquidCrystal.h>

#define boton A15
#define LDR A0
#define 
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

const long game = 10;
long mapUpdate = 1000;

unsigned long previousTime1 = 0;
unsigned long previousTime2 = 0;

byte spikeUP[8] = {
  B00100,
  B00100,
  B00100,
  B01110,
  B01110,
  B11111,
  B11111,
  B11111,
};

byte spikeDOWN[8] = {
  B11111,
  B11111,
  B11111,
  B01110,
  B01110,
  B00100,
  B00100,
  B00100,
};

byte man[8] = {
  B01110,
  B01110,
  B01110,
  B00100,
  B11111,
  B00100,
  B01110,
  B10001,
};

// 0 = Espacio vacío
// 1 = Pico
// 2 = Jugador
int board[] = {
  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1,
  2, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0
};

int lose = 0;
int lastSpike = 1;

void setup() {
  
  Serial.begin(9600);
  Serial.println("Program started");

  pinMode(boton, INPUT);
  pinMode(time, INPUT);
  pinMode(led, OUTPUT);

  lcd.createChar(0, spikeUP);
  lcd.createChar(1, spikeDOWN);
  lcd.createChar(2, man);

  lcd.begin(16, 2);

  randomSeed(analogRead(A0));
}

void loop() {
  unsigned long currentTime = millis();

  if ( currentTime - previousTime1 >= game) {
    mapUpdate = (log10(analogRead(time) + 1) + 1) * 200;
    if (lose == 0) {
      // Display board
      for (int i = 0; i < 32; i++) {
        if (i < 16) {
          lcd.setCursor(i, 0);
          switch (board[i]) {
            case 0:
              lcd.write(" ");
              break;
            case 1:
              lcd.write(byte(1));
              break;
            case 2:
              lcd.write(byte(2));
              break;
          }
        } else {
          lcd.setCursor(i - 16, 1);
          switch (board[i]) {
            case 0:
              lcd.write(" ");
              break;
            case 1:
              lcd.write(byte(0));
              break;
            case 2:
              lcd.write(byte(2));
              break;
          }
        }
      }

      // Man movement
      int statusBoton = digitalRead(boton);
      if (statusBoton == 1) {
        board[0] = 2;
        board[16] = 0;
        // turning on led
        digitalWrite(led, HIGH);
      } else {
        board[0] = 0;
        board[16] = 2; 
        // turning off led
        digitalWrite(led, LOW);
      }
    } else {
      lcd.setCursor(0, 0);
      lcd.write("Has perdido...  ");
      lcd.setCursor(0, 1);
      lcd.write("Reinicia arduino");
    }
    previousTime1 = currentTime;
  }

  if (currentTime - previousTime2 >= mapUpdate) {
    // @Deprecated Changed to user-input mapUpdate values with a variable resistance
    // // Make it faster for every loop till 500 speed
    // if (mapUpdate > 500) {
    //   mapUpdate -= 10;
    // }
    // check if game status
    if (board[0] == 2) {
      if (board[1] == 1) {
        lose = 1;
      }
    } else {
      if (board[17] == 1) {
        lose = 1;
      }
    }


    Serial.println(mapUpdate);
    Serial.println("Updating map");
    int row1[16];
    int row2[16];

    for (int i = 0; i < 32; i++) {
      if (i < 16) {
        row1[i] = board[i];
      } else {
        row2[i - 16] = board[i];
      }
    }


    // lastSpike
    // 0 = Blank space
    // 1 = Upper
    // 2 = Down
    if (random(0, 2) == 0) { // Upper
      if (lastSpike == 1 || lastSpike == 0) {
        board[15] = 1;
        board[31] = 0;
        lastSpike = 1;
      } else {
        board[15] = 0;
        board[31] = 0;
        lastSpike = 0;
      }
    } else { // down
      if (lastSpike == 2 || lastSpike == 0) {
        board[15] = 0;
        board[31] = 1;
        lastSpike = 2;
      } else {
        board[15] = 0;
        board[31] = 0;
        lastSpike = 0;
      }
    }

    board[1] = row1[2];
    board[2] = row1[3];
    board[3] = row1[4];
    board[4] = row1[5];
    board[5] = row1[6];
    board[6] = row1[7];
    board[7] = row1[8];
    board[8] = row1[9];
    board[9] = row1[10];
    board[10] = row1[11];
    board[11] = row1[12];
    board[12] = row1[13];
    board[13] = row1[14];
    board[14] = row1[15];
    board[17] = row2[2];
    board[18] = row2[3];
    board[19] = row2[4];
    board[20] = row2[5];
    board[21] = row2[6];
    board[22] = row2[7];
    board[23] = row2[8];
    board[24] = row2[9];
    board[25] = row2[10];
    board[26] = row2[11];
    board[27] = row2[12];
    board[28] = row2[13];
    board[29] = row2[14];
    board[30] = row2[15];

    previousTime2 = currentTime;
  }
}