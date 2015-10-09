
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

//#define __DEBUG

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
}

void setPixels() {
  pixels.clear();
  uint8_t hour = hourFormat12();
  if (hour == 12) hour = 0;

  
  /*
   * Process Hour
   */
  //addPixelColor(hour, pixels.Color(0,0,0xCC));
  pixels.setPixelColor(hour, pixels.Color(0x11,0x11,0xAA));

  /*
   * Process minute
   */

  for(uint8_t i=0;i<NUMPIXELS;i++){
    uint8_t brightness = getBrightness(i,(millis()%60000)/60000.0);
    //if (i=5) Serial.println(brightness);
    pixels.setPixelColor(i, pixels.Color(0, 0xCC & brightness,0));
    //addPixelColor(i, pixels.Color(0, 0xCC & brightness,0));
  }

  /*
   * Show Pixels
   */
  pixels.show();
}

uint32_t addPixelColor(uint8_t led, uint32_t color){

#ifdef __TRACE
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
float getBrightness(float led, float target_position) {
  float led_position = led/NUMPIXELS;

  float diff = led_position-target_position;
  float rad = diff*2*3.14;
  float br = cos(rad);
  
#ifdef __DEBUG
  Serial.print("Getting Brighntess for Led ");
  Serial.print(led);
  Serial.print(" resulting led_position ");
  Serial.println(led_position);
  
  Serial.print("Getting Brighntess for target_position ");
  Serial.print(target_position);
  Serial.print(" with diff of ");
  Serial.println(diff);
  
  Serial.print("Cosinus is ");
  Serial.print(br);
  Serial.print(" on rad-angle ");
  Serial.println(rad);
#endif

  if (br > 0.3 || br > -0.3) return 0;
  
  float brightness = abs(br);
  
#ifdef __DEBUG
  Serial.print("Brighntess is ");
  Serial.println(brightness);
#endif

  return brightness * 0xFF;
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


