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

uint16_t holdTime = 0;
bool joystickPressed = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  Serial.println("Started");
  pinMode(CPIN, INPUT_PULLUP);
  strip.begin();
  //chase(strip.Color(255, 0, 0)); // Red
}

void loop() {
  handleJoystick();

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
      rainbowLoop(120,20);
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
      //color palette
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
//Self made derivative of H2RGB, takes a value of 0 to 1530
//Will wrap around at 65536, but shouldn't be that noticable and by the time it has done 43 loops already, but you may make your loop go from 0 to 1530 so it's a smooth transition.
//Somewhat functions the same as the code below, but vastly more efficient in both speed and memory.
//Also, you have much more fine grained color 0-1530 scale instead of 0-360 and no divisions which can have rounding issues.
static uint32_t rgbSlide(uint16_t s){
  s = s%1530;
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  /*  0   1   2   3   4   5   6   
  R 255 255   0   0   0 255 255
  G   0 255 255 255   0   0   0
  B   0   0   0 255 255 255   0
   */
  switch(s>>8){
    case 0://Red, adding green
    r = 255;
    g = s%256;
    break;
    case 1://Green, dissolving red
    r = 255-s%256;
    g = 255;
    break;
    case 2://Green, adding blue
    g = 255;
    b = s%256;
    break;
    case 3://Blue, dissolving green
    b = 255;
    g = 255 - s%256;
    break;
    case 4://Blue adding red
    b = 255;
    r = s%256;
    break;
    case 5://Red, disolving blue.
    r = 255;
    b = 255 - s%256;
    break;
  }
  //Serial.print(s,DEC);
  //Serial.print(",");
  //Serial.println(s>>8,DEC);
  return strip.Color(r,g,b);
}
/*
uint32_t HSV2RGB(uint16_t h, double s, double v) {
  //this is the algorithm to convert from RGB to HSV
  double r = 0;
  double g = 0;
  double b = 0;

  double hf = h / 60.0;

  int i = (int)floor(h / 60.0);
  double f = h / 60.0 - i;
  double pv = v * (1 - s);
  double qv = v * (1 - s * f);
  double tv = v * (1 - s * (1 - f));

  switch (i)
  {
    case 0: //rojo dominante
      r = v;
      g = tv;
      b = pv;
      break;
    case 1: //verde
      r = qv;
      g = v;
      b = pv;
      break;
    case 2:
      r = pv;
      g = v;
      b = tv;
      break;
    case 3: //azul
      r = pv;
      g = qv;
      b = v;
      break;
    case 4:
      r = tv;
      g = pv;
      b = v;
      break;
    case 5: //rojo
      r = v;
      g = pv;
      b = qv;
      break;
  }

  //set each component to a integer value between 0 and 255
  uint8_t red = constrain((int)255 * r, 0, 255);
  uint8_t green = constrain((int)255 * g, 0, 255);
  uint8_t blue = constrain((int)255 * b, 0, 255);

  return strip.Color(red, green, blue);
}
*/
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
static void diceLoop() {
  static uint16_t dice = 7;
  if (holdTime < 100) {
    dice = 7;
  }
  if (dice != 65535 && dice > 6) {
    dice ++;
    if (dice % 1000) {
      showNumber(random(1, 6));
    }
  }
  if (dice == 65535) {
    dice = random(1, 6);
    showNumber(dice);
  }
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


//////////////////////////////////////////////// HELP DRAW FUNCTIONS ////////////////////////////////////////////////
static void showNumber(uint8_t nr) {
  switch (nr) {
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}

/*
  //Serial.print(x, DEC);
  //Serial.print(",");
  //Serial.print(y, DEC);
  //Serial.print(",");
  //Serial.print(pixNr, DEC);
  //Serial.print(",");
  //Serial.println(digitalRead(CPIN), DEC);
  //drawXY(xpos,ypos,strip.Color(0, 0, 0));
  //handleJoystick();
  //drawXY(xpos,ypos,strip.Color(255, 0, 0));
*/

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
    xpos++;
    up = true;
  }
  if (x < 100) {
    xpos--;
    down = true;
  }
  if (y > 1000) {
    ypos++;
    left = true;
  }
  if (y < 100) {
    ypos--;
    right = true;
  }
  if (xpos > 12) {
    xpos = 0;
  }
  if (ypos > 12) {
    ypos = 0;
  }
}
static void drawXY(int x, int y, uint32_t c) {
  int i = 0;
  if (y & 1) { //uneven
    i = y * 12 + (11 - x);
  } else { //even (first line)
    i = y * 12 + x;
  }
  strip.setPixelColor(i, c);
}
static uint32_t randomColor() {
  switch (random(0, 3)) {
    case 0:
      return strip.Color(255, random(0, 128), random(0, 128));
      break;
    case 1:
      return strip.Color(random(0, 128), 255, random(0, 128));
      break;
    case 2:
      return strip.Color(random(0, 128), random(0, 128), 255);
      break;
  }
}
