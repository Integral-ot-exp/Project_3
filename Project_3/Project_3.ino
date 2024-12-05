#include "Adafruit_GFX.h"
#include "Adafruit_NeoMatrix.h"
#include "Adafruit_NeoPixel.h"

#define PIN 6
#define BRIGHTNESS 255

// Define matrix width and height.
#define mw 40
#define mh 11

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix( mw, mh, PIN,
  NEO_MATRIX_TOP  + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB         + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), 
  matrix.Color(0, 255, 0), 
  matrix.Color(0, 0, 255) };

int x    = matrix.width();
int pass = 0;
double in;

void setup() 
{
  matrix.begin();
  matrix.setTextWrap( false );
  matrix.setBrightness( BRIGHTNESS );
  matrix.setTextColor( colors[0] );

  matrix.fillScreen( matrix.Color(200, 200, 200) );
  matrix.show();
  delay(2000);
  pinMode(4, INPUT);
}

void loop() 
{
  matrix.fillScreen( matrix.Color(0, 0, 0) );
  matrix.setCursor(x, 2);
  matrix.print( F("||||||||||") );

  if(--x < -45) 
  {
    x = matrix.width();
    if(++pass >= 3) pass = 0;
    matrix.setTextColor( colors[pass] );
  }
  matrix.show();
  delay( 50 );
}

