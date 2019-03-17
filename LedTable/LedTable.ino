#include <Adafruit_NeoPixel.h>

#define LEDPIN A3
#define N_LEDS 144
#define XPIN  A0
#define YPIN  A1
#define CPIN 12

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, LEDPIN, NEO_GRB + NEO_KHZ800);
int x = 0;
int y = 0;
int c = 0;
int xpos, ypos;
int animSpeed = 100;
int pixNr = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  Serial.println("Started");
  pinMode(CPIN, INPUT_PULLUP);
  strip.begin();
  //chase(strip.Color(255, 0, 0)); // Red
}

void loop() {
  drawXY(xpos,ypos,strip.Color(0, 0, 0));
  handleJoystick();
  drawXY(xpos,ypos,strip.Color(255, 0, 0));
  strip.show();
  delay(animSpeed);
  //Serial.print(x, DEC);
  //Serial.print(",");
  //Serial.print(y, DEC);
  //Serial.print(",");
  //Serial.print(pixNr, DEC);
  //Serial.print(",");
  //Serial.println(digitalRead(CPIN), DEC);
}

static void handleJoystick(){
  x = analogRead(XPIN);
  y = analogRead(YPIN);
  c = !digitalRead(CPIN);
  if(x > 1000){
    xpos++;
  }
  if(x < 100){
    xpos--;
  }
  if(y > 1000){
    ypos++;
  }
  if(y < 100){
    ypos--;
  }
}
static void drawXY(int x, int y, uint32_t c){
  int i = 0;
  if(y%2){//uneven
    //DOWN
    i = y*12 + (11-x);
  }else{//even (first line)
    //UP
    i = y*12 + x;
  }
  strip.setPixelColor(i,c);
}

//chase(strip.Color(255, 0, 0)); // Red
//chase(strip.Color(0, 255, 0)); // Green
//chase(strip.Color(0, 0, 255)); // Blue
static void chase(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels()+4; i++) {
      strip.setPixelColor(i  , c); // Draw new pixel
      strip.setPixelColor(i-4, 0); // Erase pixel a few steps back
      strip.show();
      delay(25);
  }
}
