#include <FastLED.h>

//---------------MSGEQ7-------------------------------------------

int analogPin = 0; // MSGEQ7 OUT
int strobePin = 2; // MSGEQ7 STROBE
int resetPin = 4; // MSGEQ7 RESET
int spectrumValue[7];
 
//Filtreringsverdi
int filterValue = 70;
 
//----------------FastLED-------------------------------------------

#define NUM_LEDS 60 // Antall LED pa stripa
#define DATA_PIN 7  // Data Pin til stripa

// Define the array of leds
CRGB leds[NUM_LEDS];

 
void setup()
{
  Serial.begin(9600);
  // Read from MSGEQ7 OUT
  pinMode(analogPin, INPUT);
  // Write to MSGEQ7 STROBE and RESET
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
 
  // Set analogPin's reference voltage
  analogReference(DEFAULT); // 5V
 
  // Set startup values for pins
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);

  //Initialisering av LEDS
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}
 
void loop()
{
  
  lesAudioSignal();
  fyllLED();
  
}

void lesAudioSignal() {

  // Set reset pin low to enable strobe
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);
 
  // Get all 7 spectrum values from the MSGEQ7
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(strobePin, LOW);
    delayMicroseconds(40); // Allow output to settle
 
    spectrumValue[i] = analogRead(analogPin);
 
    // Constrain any value above 1023 or below filterValue
    spectrumValue[i] = constrain(spectrumValue[i], filterValue, 1023);
 
    // Remap the value to a number between 0 and antall_LEDS (per kolonne)
    spectrumValue[i] = map(spectrumValue[i], filterValue, 1023, 0, 12);
 
    // Remove serial stuff after debugging
    Serial.print(spectrumValue[i]);
    Serial.print(" ");
    digitalWrite(strobePin, HIGH);
  }
 
  Serial.println();

}


void fyllLED() {

  FastLED.clear();

  for(int i = 0; i <= spectrumValue[0]; i++)
  {
    leds[i] = CRGB::Green;
  }
  for(int i = 12; i <= (spectrumValue[1]+12); i++)
  {
    leds[i] = CRGB::Yellow;
  }
  for(int i = 24; i <= spectrumValue[2]+24; i++)
  {
    leds[i] = CRGB::Red;
  }
  for(int i = 36; i <= spectrumValue[3]+36; i++)
  {
    leds[i] = CRGB::Blue; 
  }
  for(int i = 48; i <= spectrumValue[4]+48; i++)
  {
    leds[i] = CRGB::Purple; 
  }
  /*for(int i = 40; i < spectrumValue[5]+40; i++)
  {
    leds[i] = CRGB::Coral;
  }
  for(int i = 48; i < spectrumValue[6]+48; i++)
  {
    leds[i] = CRGB::GreenYellow; 
  }*/ 

  FastLED.setBrightness(20);
  //FastLED.delay(20);
  FastLED.show();
  

}
