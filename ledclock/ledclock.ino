
/*
 * Timer/Clock Config
 */
#include <Time.h>  

#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 
#define DEFAULT_TIME 1357041600 // Jan 1 2013


#include <DCF77.h>

#define DCF_PIN 2                // Connection pin to DCF 77 device
#define DCF_INTERRUPT 2          // Interrupt number associated with pin

time_t time;
// Non-inverted input on pin DCF_PIN
DCF77 DCF = DCF77(DCF_PIN,DCF_INTERRUPT, true);

/*
 * Pixel/Ring Config
 */

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            4
#define NUMPIXELS      12

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delay_ms = 100;
double deg_rad_fac = 1000/57296;

void setup()  {
  //for Use with micro Attiny Board
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif

  setTime(DEFAULT_TIME);

  DCF.Start();
  Serial.println("Waiting for DCF77 time ... ");
  Serial.println("It will take at least 2 minutes before a first time update.");
  
  pixels.begin();
  
  Serial.begin(9600);
  pinMode(13, OUTPUT);
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

  /*
   * Process Hour
   */
  addPixelColor(hourFormat12(), pixels.Color(0,0,0xCC));

  /*
   * Process minute
   */

  for(int i=0;i<NUMPIXELS;i++){
    addPixelColor(i, pixels.Color(0,0xCC,0,getBrightness(i,minute())));
  }

  /*
   * Show Pixels
   */
  pixels.show();
}

uint32_t addPixelColor(int led, uint32_t color){
  pixels.setPixelColor(led, pixels.getPixelColor(led) && color);
}

/*
 * get Brightness based on sinus
 */
int getBrightness(int for_led, int minute) {
  int led_position = for_led*30;
  int minute_position = minute*6;
  
  if (abs(led_position-minute_position) > 90) return 0;
  
  return cos((led_position-minute_position)*deg_rad_fac)*0xFF;
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
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


