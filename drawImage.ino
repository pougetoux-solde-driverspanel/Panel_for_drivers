
#include <PxMatrix.h>
#include "DefaultImages.h"
#ifdef ESP32

#define P_LAT 22
#define P_A 19
#define P_B 23
#define P_C 18
#define P_D 5
#define P_E 15
#define P_OE 2
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

#endif

#ifdef ESP8266

#include <Ticker.h>
Ticker display_ticker;
#define P_LAT 16
#define P_A 5
#define P_B 4
#define P_C 15
#define P_D 12
#define P_E 0
#define P_OE 2

#endif
// Pins for LED MATRIX

uint8_t display_draw_time=0;

PxMATRIX display(32,32,P_LAT, P_OE,P_A,P_B,P_C,P_D);
// Some standard colors
uint16_t myRED = display.color565(255, 0, 0);
uint16_t myGREEN = display.color565(0, 255, 0);
uint16_t myBLUE = display.color565(0, 0, 255);
uint16_t myWHITE = display.color565(255, 255, 255);
uint16_t myYELLOW = display.color565(255, 255, 0);
uint16_t myCYAN = display.color565(0, 255, 255);
uint16_t myMAGENTA = display.color565(255, 0, 255);
uint16_t myBLACK = display.color565(0, 0, 0);

uint16 myCOLORS[8] = {myRED, myGREEN, myBLUE, myWHITE, myYELLOW, myCYAN, myMAGENTA, myBLACK};



// ISR for display refresh
void display_updater()
{

  display.display(70);

}

void setup() {
  Serial.begin(9600);
  display.begin(16);
  display.clearDisplay();
  Serial.print("Pixel draw latency in us: ");
  unsigned long start_timer = micros();
  display.drawPixel(1, 1, 0);
  unsigned long delta_timer = micros() - start_timer;
  Serial.println(delta_timer);

  Serial.print("Display update latency in us: ");
  start_timer = micros();
  display.display(0);
  delta_timer = micros() - start_timer;
  Serial.println(delta_timer);

  display_ticker.attach(0.002, display_updater);
  yield();
  display.clearDisplay();
  uint16_t  image1[] = { 0x4FE0, 0xe66465, 0xe66465, 0x4FE0,
                         0x4FE0, 0xe66465, 0xe66465, 0x4FE0,
                         0x4FE0, 0xe66465, 0xe66465, 0x4FE0,
                         0x4FE0, 0xe66465, 0xe66465, 0x4FE0,
                         0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,
                         0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0,
                         0xe66465, 0x4FE0, 0x4FE0, 0xe66465,
                         0x4FE0, 0xe66465, 0xe66465, 0x4FE0,
                         0x4FE0, 0x4FE0, 0x4FE0, 0x4FE0};
//  drawImage(0, 0, image1);
  drawImage(0, 0, THUMBSUP);
  delay(500);
}


void drawImage(int x, int y, uint16_t *image)
{
  
  int width = 32;
  int height = 32;
  
  for (int xx = 0; xx < height * width; xx++)
  {
    display.drawPixel(xx % width + x , xx / width + y, image[xx]);
  }
  
}


void loop() {
}
