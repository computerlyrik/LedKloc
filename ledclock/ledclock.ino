
/*
 * Timer/Clock Config
 */
#include <Time.h>  

#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 


/*
 * Pixel/Ring Config
 */

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            11
#define NUMPIXELS      12

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN);

/*
 * Color config
 */

Color cl_hour = Color(0,0,0xCC);
Color cl_minute = Color(0,0xCC,0);
Color cl_second = Color(0xAA,0xF, 0xF);

/*
 * other config
 */
int delay_ms = 500;
double deg_rad = 1000 / 57296

void setup()  {
  //for Use with micro Attiny Board
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  
  pixels.begin();
  
  Serial.begin(9600);
  while (!Serial) ; // Needed for Leonardo only
  pinMode(13, OUTPUT);
  setSyncProvider( requestSync);  //set function to call when sync required
  Serial.println("Waiting for sync message");
}

void loop(){    
  if (Serial.available()) {
    processSyncMessage();
  }
  if (timeStatus()!= timeNotSet) {
    digitalClockDisplay();  
  }

 setPixels();
 
 delay(delay_ms);
}

void setPixels() {
  pixels.clear();

  /*
   * Process Hour
   */
  addPixelColor(hourFormat12(), cl_hour);

  /*
   * Process minute
   */

  int minute = minute();
  for(int i=0;i<NUMPIXELS;i++){

    addPixelColor(i, Color(0,
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    addPixelColor(i, Color(0,0xCC,0,getBrightness(i,minute));
  }

  /*
   * Show Pixels
   */
  pixels.show();
}

uint32_t addPixelColor(int led, Color c){
  pixels.setPixelColor(led, pixels.getPixelColor(led) && c);
}

/*
 * get Brightness based on sinus
 */
int getBrightness(int for_led, int minute) {
  int led_position = for_led*30;
  int minute_position = minute()*6;
  return cos(led_position-minute_position)*0xFF;
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
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
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
     }
  }
}

time_t requestSync()
{
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}


void loop() {

}



