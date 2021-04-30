#include <FastLED.h>

#define LED_PIN     10
#define NUM_LEDS    61
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100


const int groundpin = 18;             // analog input pin 4 -- ground
const int powerpin = 19;              // analog input pin 5 -- voltage
const int xpin = A3;                  // x-axis of the accelerometer
const int ypin = A2;                  // y-axis of the accelerometer
const int zpin = A1;                  // z-axis of the accelerometer

CRGBPalette16 currentPalette;
TBlendType    currentBlending;
static bool starting = true;
unsigned long now = millis();
static uint8_t startIndex = 0;
static int last_value = 0;
static int threshold = 40; // Determine motion senstivity

void setup() {
  Serial.begin(9600);

    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(  BRIGHTNESS );
    
    SetupGreenAndWhitePalette();
}


void loop(){

  // Show startup sequence for 15 seconds
  while(millis() < now + 15000){
        startup();
  }

  if (moving()){
    allwhite();
    delay(3000); // Delay the next acceleration reading
  }
  else {
    allgreen();
  }
  
    
}

bool moving(){
  int x = analogRead(xpin);
  int y = analogRead(ypin);
  int z = analogRead(zpin);
  int current_value = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)); // Current Acceleration
  
  Serial.print("Last Value: ");
  Serial.print(last_value);
  Serial.print(" Current Value: ");
  Serial.print(current_value);
  Serial.print(" Calculated Value: ");
  Serial.print(abs(current_value - last_value));
  Serial.println();

  if (abs(current_value - last_value) > threshold ){
    last_value = current_value;
    return true; 
  }

  last_value = current_value;
  return false;
  
}

void startup(){

    startIndex = startIndex + 1;
    FillLEDsFromPaletteColors(startIndex);
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex){
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
        FastLED.show();
    }
}

void SetupGreenAndWhitePalette(){
  CRGB green = CHSV(HUE_GREEN, 255, 255);
  CRGB white = CRGB::White;
  CRGB black = CRGB::Black;

  currentPalette = CRGBPalette16(
                                 green, green, white, white,
                                 white, white, green, green,
                                 green, green, white, white,
                                 white, white, green, green
    );
    currentBlending = LINEARBLEND;
}

void allwhite(){
  for(int i = 0; i< NUM_LEDS; i++){
    leds[i] = CRGB::White;
  }
  FastLED.show();
}

void allgreen(){
  for(int i = 0; i< NUM_LEDS; i++){
    leds[i] = CHSV(HUE_GREEN, 255, 255);
  }
  FastLED.show();
}
