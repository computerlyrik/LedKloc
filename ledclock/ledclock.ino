
/*
 * Timer/Clock Config
 */
#include <Time.h>  

#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 
#define DEFAULT_TIME 1357041600 // Jan 1 2013

/*
 * Pixel/Ring Config
 */

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            11
#define NUMPIXELS      12

#define MAX_BRIGHTNESS 250

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint8_t delay_ms = 1000;
double deg_rad_fac = 1000/57296;

#define __DEBUG

void setup()  {
  //for Use with micro Attiny Board
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif

  setTime(DEFAULT_TIME);
  
  pixels.begin();
  pixels.setBrightness(MAX_BRIGHTNESS);
  
  Serial.begin(9600);
  setSyncProvider( requestSync);  //set function to call when sync required
}

void loop(){    
  if (Serial.available() > 0) {
    processSyncMessage();
  }
  if (timeStatus()!= timeNotSet) {
    setPixels();
  }
 delay(delay_ms);
}

void setPixels() {
  pixels.clear();
  uint8_t hour = hourFormat12();
  if (hour == 12) hour = 0;

  pixels.setPixelColor(hour, pixels.Color(0xA,0xA,0xAA));
  /*
   * Process Hour
   */
  //addPixelColor(hour, pixels.Color(0,0,0xCC));

  /*
   * Process minute
   */

  for(uint8_t i=0;i<NUMPIXELS;i++){
    uint8_t brightness = getBrightness(i,second());
    addPixelColor(i, pixels.Color(0, 0xCC & brightness,0));
  }

  /*
   * Show Pixels
   */
  pixels.show();
}

uint32_t addPixelColor(uint8_t led, uint32_t color){

#ifdef __DEBUG
  Serial.print("Adding Color ");
  Serial.println(color);
  Serial.print("To Color ");
  Serial.println(pixels.getPixelColor(led));
  Serial.print("New Color ");
  Serial.println(pixels.getPixelColor(led) | color);
#endif

  pixels.setPixelColor(led, pixels.getPixelColor(led) | color);
}

/*
 * get Brightness based on sinus
 */
float getBrightness(uint8_t for_led, uint8_t minute) {
  uint16_t led_position = for_led*360/NUMPIXELS;
  uint8_t minute_position = minute*6;

#ifdef __DEBUG
  Serial.print("Getting Brighntess for Led on angle ");
  Serial.println(led_position);
#endif

#ifdef __DEBUG
  Serial.print("Getting Brighntess for Minute ");
  Serial.println(minute);
#endif

  if (abs(led_position-minute_position) > 90) return 0;
  
  float brightness = cos((led_position-minute_position)*deg_rad_fac)*0xFF;
  
#ifdef __DEBUG
  Serial.print("Brighntess is ");
  Serial.println(brightness);
#endif

  return brightness;
}
void processSyncMessage() {
  unsigned long pctime;
  setTime(DEFAULT_TIME);
}

time_t requestSync()
{
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}


