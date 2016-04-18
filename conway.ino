#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define WORLDMAXX 19
#define WORLDMAXY 15
byte world[WORLDMAXX+1][WORLDMAXY+1];
void initworld();
void isalive(int x, int y);
int countneighbor(int x, int y);
void generatechars();
void printworld();
void updateWorld();
int moveRight();
int moveLeft();
int moveUp();
int moveDown();

unsigned int isRunning = 0;
unsigned int currentX = 0;
unsigned int currentY = 0;
int startPin = 8;
int controlEditPin = 6, moveLeftPin = 6;
int moveRightPin = 7;
unsigned int currentStartButtonState = 0, currentControlEditButtonState = 0, previousControlEditButtonState = 0;
const int refreshInterval = 300;
const int buttonReadInterval = 1000;
unsigned long previousRefreshMillis = 0;
unsigned long previousButtonReadMillis = 0;


void setup() {
  lcd.begin(8, 2);
  randomSeed(analogRead(0));
  pinMode(startPin, INPUT);
  pinMode(controlEditPin, INPUT);
  pinMode(moveRightPin, INPUT);
  Serial.begin(9600);
  initworld();
}

void loop() {

  unsigned long currentMillis = millis();

  if(currentMillis - previousRefreshMillis >= refreshInterval){
    if(isRunning == 1){
      //PLAY MODE
      updateWorld();
      lcd.setCursor(4, 0);
      lcd.print(" Ivo");
      lcd.setCursor(4, 1);
      lcd.print("TUES");
    }else{
      //EDIT MODE
      lcd.setCursor(4, 0);
      lcd.print("X:");
      lcd.print(currentX);
      lcd.setCursor(4, 1);
      lcd.print("Y:");
      lcd.print(currentY);
      if(moveRight())
        currentX++;
      if(moveLeft())
        currentX--;
      if(moveUp())
        currentY--;
      if(moveDown())
        currentY++;
      
    }
    previousRefreshMillis = currentMillis;
  }

  if(currentMillis - previousButtonReadMillis >= buttonReadInterval){
  
    currentStartButtonState = digitalRead(startPin);
    currentControlEditButtonState = digitalRead(controlEditPin);
    
    if(currentStartButtonState == 1 && currentControlEditButtonState == 1){
        if(isRunning == 1)
          isRunning = 0;
         else
          isRunning = 1;
    }

    previousButtonReadMillis = currentMillis;
  }
}

int moveRight(){
  if(currentX >= 20) return 0;

  return digitalRead(moveRightPin);
}

int moveLeft(){
  if(currentX == 0) return 0;

  return digitalRead(moveLeftPin);
}

int moveDown(){
  if(currentY >= 16) return 0;

  return 0;
}

int moveUp(){
  if(currentY == 0) return 0;

  return 0;
}

void updateWorld(){
  int i, j;
  generatechars();
  printworld();
  for (i = 0; i<= WORLDMAXX; i++) {
    for (j = 0; j<= WORLDMAXY; j++) {
      isalive(i,j);
    }
  }
  for (i = 0; i<= WORLDMAXX; i++) {
    for (j = 0; j<= WORLDMAXY; j++) {
      world[i][j] = world[i][j] >> 1;
    }
  }
}

void initworld() {
  int i, j;
  for (i=0; i<=WORLDMAXX; i++) {
    for (j=0; j<=WORLDMAXY; j++) {
      world[i][j] = !random(4);
    }
  }
  return;
}

void isalive(int x, int y) {
  int count = countneighbor(x, y);
  if (count == 2) { 
    world[x][y] += world[x][y] << 1;
  }
  if (count == 3) {
    world[x][y] += 2;
  }

  return;
}
int countneighbor(int x, int y) {
  int count = 0;
  if (x > 0) {
    count += world[x-1][y] & 1;
    if (y > 0) {
      count += world[x-1][y-1] & 1;
    }
    if (y < WORLDMAXY) {
      count += world[x-1][y+1] & 1;
    }
  }
  if (x < WORLDMAXX) {
    count += world[x+1][y] & 1;
    if (y > 0) {
      count += world[x+1][y-1] & 1;
    }
    if (y < WORLDMAXY) {
      count += world[x+1][y+1] & 1;
    }
  }
  if (y > 0) {
    count += world[x][y-1] & 1;
  }
  if (y < WORLDMAXY) {
    count += world[x][y+1] & 1;
  }
  return count;
}

void generatechars() {
  byte tempchar[8];
  int i, j, k, l;
  for (i = 0; i<2; i+=1) { // y poss / 8
    for (j = 0; j<4; j+=1) { // x poss / 5
      for (k = 0; k < 8; k+=1) {
        tempchar[k] = 0;
        for (l = 0; l<5; l+=1) {
          tempchar[k] += (world[j*5 + l][i * 8 + k] & 1) << (4 - l);
        }
      } // done building this char
      lcd.createChar(i*4 + j, tempchar);
    }
  }
  return;
}

void printworld() {
  int i, j;
  for (i=0; i<2; i+=1) {
    lcd.setCursor(0, i);
    for (j=0; j<4; j+=1) {
      lcd.write(i*4 + j);
    }
  }
  return;
}
