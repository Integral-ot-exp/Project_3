#include "Adafruit_GFX.h"
#include "Adafruit_NeoMatrix.h"
#include "Adafruit_NeoPixel.h"

#define PIN 6
#define PIN_POT A0

// Define matrix width and height.
#define mw 8
#define mh 8

int color(int);
void del(int);
void set(int);
void full(int);

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix( mw, mh, PIN,
  NEO_MATRIX_TOP  + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB         + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), 
  matrix.Color(0, 255, 0), 
  matrix.Color(0, 0, 255) };

void setup() 
{
  pinMode(PIN_POT, INPUT);
  matrix.begin();
  matrix.setTextWrap( false );
  
  matrix.setTextColor( colors[0] );
  matrix.show();
}

void loop() 
{
  int rot, y, x, n, r, col, j, max;
  
  rot = analogRead(PIN_POT);
  matrix.setBrightness( 255 );
  
  set(rot);
  full(rot);
  del(rot);

  matrix.show();
}

void set(int rot)
{
  int r, x, y, n;

  n = rot / 16;
  r = n / 8;
  x = n - 8 * r;
  y = n / 8;

  matrix.drawPixel(x, y, color(n));
}

void del(int rot)
{
  int x, r, y, n;

  n = rot / 16 + 1;
  r = n / 8;
  y = n / 8;
  x = n - 8 * r;
  
  matrix.drawPixel(x, y, 0);
}

void full(int rot)
{
  int r, x, y, n, max;

  n = rot / 16;

  for(max = 0; max < n; max++)
  {
    r = max / 8;
    x = max - 8 * r;
    y = max / 8;
    matrix.drawPixel(x, y, color(max));
  }

}

int color(int n)
{
  if (n >= 0 && n <= 20)
  {
    return colors[1];
  }

  if (n >= 21 && n <= 45)
  {
    return colors[1] + colors[0];
  }
  if (n >= 46 && n <= 64)
  {
    return colors[0];
  }

}
