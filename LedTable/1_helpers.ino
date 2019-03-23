static void displayArray(const uint16_t arr[], uint32_t c){
  for (uint8_t i = 0; i < 12; i++) {
    for(uint8_t j = 0; j < 12; j++){
      //Serial.print(bitRead(arr[i], j),DEC);
      if(bitRead(arr[i], j)){
        drawXY(i, j, c);
      }
    }
    //Serial.println("");
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
