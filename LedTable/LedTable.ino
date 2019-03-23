#include <Adafruit_NeoPixel.h>

#define LEDPIN A3
#define N_LEDS 144
#define XPIN  A0
#define YPIN  A1
#define CPIN 12

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, LEDPIN, NEO_GRB + NEO_KHZ800);

uint8_t state = 0;
uint32_t drawColor = 255;
uint8_t lastState = 9;
uint8_t xpos = 1;
uint8_t ypos = 1;
uint8_t animSpeed = 1;
int pixNr = 0;
bool up, down, left, right;
int incomingByte = 0;   // for incoming serial data
uint16_t holdTime = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  Serial.println("Started");
  pinMode(CPIN, INPUT_PULLUP);
  strip.begin();
}

void loop() {
  handleJoystick();
  handleSerial();
  handleCursor();

  if (holdTime > 0) {
    Serial.print("Hold button ms: ");
    Serial.println(holdTime);
    if (holdTime > 1000) {
      state++;
      strip.clear();
      animSpeed = 10;
      drawXY(state, state, strip.Color(255, 0, 0));
      if (state > lastState) {
        state = 0;
      }
      Serial.print("State changed to: ");
      Serial.println(state);
    }
  }

  switch (state) {
    case 0:
      strip.clear();
      break;
    case 1:
      cursorLoop();
      break;
    case 2:
      chaseLoop(randomColor());
      break;
    case 3:
      randomColorsLoop();
      break;
    case 4:
      rainbowLoop(120,20);
      break;
    case 5:
      //funky twinkle/random rainbow?
      //Cyclon?
      break;
    case 6:
      snakeLoop();
      break;
    case 7:
      tetrisLoop();
      break;
    case 8:
      pongLoop();
      break;
    case 9:
      diceLoop();
      break;
  }
  strip.show();
  holdTime = 0;

  if (down) {
    animSpeed -= 10;
  }
  if (up) {
    animSpeed += 10;
  }
  if (animSpeed > 200) {
    animSpeed = 1;
  }
  delay(animSpeed);
}
//stretch:
// 120 is one bar of each color.                  (Red is a single bar)
// 12 is 1/10th of a color on screen at the time. (10 red-ish bars will pass)
//colorspeed:
// 20 is about 1 second for the bar to pass (but may depend on stretch and animspeed).
void rainbowLoop(uint8_t stretch, uint8_t colorspeed) {
  static uint16_t color = 0;
  color+=colorspeed;
  if (color > (255*6)) {
    color = 0;
  }
  for (uint8_t x = 0; x < 12; x++) {
    for (uint8_t y = 0; y < 12; y++) {
      //drawXY(x, y, HSV2RGB((color + x + y * phaseShift / 2) % 360, 1, 1));
      drawXY(x, y, rgbSlide((color + (x) + (y) * stretch)));
    }
  }
  strip.show();
}

static void randomColorsLoop() {
  strip.setPixelColor(random(0, 144), randomColor());
}
static void snakeLoop() {

}
static void cursorLoop() {
  animSpeed = 100;
  drawXY(xpos, ypos, drawColor);
  if (holdTime > 0) {
    drawColor = randomColor();
  }
}
static void tetrisLoop() {

}
static void pongLoop() {

}

static void chaseLoop(uint32_t c) {
  static uint8_t i = 0;
  static bool goingUp = true;
  if (goingUp) {
    i++;
    strip.setPixelColor(i  , c); // Draw new pixel
    strip.setPixelColor(i - 4, 0); // Erase pixel a few steps back
  } else {
    i--;
    strip.setPixelColor(i  , c); // Draw new pixel
    strip.setPixelColor(i + 4, 0); // Erase pixel a few steps back
  }
  strip.show();
  if (i > strip.numPixels() + 4) {
    i = strip.numPixels();
    goingUp = false;
  }
  if (0 == i) {
    goingUp = true;
  }
}


//////////////////////////////////////////////// HANDLER FUNCTIONS ////////////////////////////////////////////////
static void handleCursor(){
  if (up){
    xpos++;
  }
  if (down){
    xpos--;
  }
  if (left){
    ypos++;
  }
  if (right){
    ypos--;
  }
  if (xpos > 12) {
    xpos = 0;
  }
  if (ypos > 12) {
    ypos = 0;
  }
}
static void handleSerial(){
  // send data only when you receive data:
  if (Serial.available() > 0) {
      // read the incoming byte:
      incomingByte = Serial.read();

      // say what you got:
      Serial.print("I received: ");
      Serial.println(incomingByte, DEC);
      if(incomingByte > 48 && incomingByte < 58){
        state = incomingByte-48;
        Serial.print("State changed to: ");
        Serial.println(state);
        strip.clear();
      }
      switch (incomingByte){
        case 45://+
        animSpeed += 10;
        break;
        case 43://-
        animSpeed -= 10;
        break;
        case 119://w
        up = true;
        break;
        case 97://a
        left = true;
        break;
        case 115://s
        down = true;
        break;
        case 100://d
        right = true;
        break;
        case 32://space
        holdTime = 100;
        break;
      }
  }
}

static void handleJoystick() {
  up = false;
  down = false;
  right = false;
  left = false;
  while (!digitalRead(CPIN)) {
    delay(1);
    holdTime++;
  }
  uint16_t x = analogRead(XPIN);
  uint16_t y = analogRead(YPIN);
  if (x > 1000) {
    up = true;
  }
  if (x < 100) {
    down = true;
  }
  if (y > 1000) {
    left = true;
  }
  if (y < 100) {
    right = true;
  }
}
