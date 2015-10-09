
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

#define MAX_BRIGHTNESS 255

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
  addPixelColor(hour, pixels.Color(0,0x33,0xFF));

  /*
   * Process minute
   */

  for(uint8_t i=0;i<NUMPIXELS;i++){
    //float brightness = getBrightness((float)i/NUMPIXELS,(minute()+second()/60)/60);
    float brightness = getBrightness((float)i/NUMPIXELS,(millis()%60000)/60000.0);
    //if (i=5) Serial.println(brightness);
    addPixelColor(i, pixels.Color(0xAA * brightness, 0xCC * brightness,0));
    addPixelColor(i, pixels.Color(8, 8, 8));
  }

  /*
   * Show Pixels
   */
  pixels.show();
}

/*
 * Adds another Color to a existent LED Color based on RGB-Channels
 */
uint32_t addPixelColor(uint8_t led, uint32_t color){

  uint32_t before = pixels.getPixelColor(led);

  uint16_t r = (uint8_t)(before >> 16) + (uint8_t)(color >> 16);
  uint16_t g = (uint8_t)(before >> 8) + (uint8_t)(color >> 8);
  uint16_t b = (uint8_t)(before) + (uint8_t)(color);

  pixels.setPixelColor(led, pixels.Color(min(r,0xFF),min(g,0xFF),min(b,0xFF)));
}

/*
 * get Brightness based on sinus
 */
float getBrightness(float led_position, float target_position) {
  
  float diff = led_position-target_position;
  if (diff > 0.5) diff = 1-diff;
  if (diff < -0.5) diff = 1+diff;
#ifdef __DEBUG
  Serial.print("Getting Brighntess for led_position ");
  Serial.println(led_position);
  
  Serial.print("Getting Brighntess for target_position ");
  Serial.print(target_position);
  Serial.print(" with diff of ");
  Serial.println(diff);
#endif

  float width = 0.12;
  if ( -width > diff || diff > width) return 0;
  
  float rad = diff/width*3.14/2;
  float br = cos(rad);
  
#ifdef __DEBUG

  
  Serial.print("Cosinus is ");
  Serial.print(br);
  Serial.print(" on rad-angle ");
  Serial.println(rad);
#endif
  
  float brightness = abs(br);
  
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


