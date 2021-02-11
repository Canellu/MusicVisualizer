#include <FastLED.h>

//---------------MSGEQ7-------------------------------------------

int analogPin = 0; // MSGEQ7 OUT
int strobePin = 2; // MSGEQ7 STROBE
int resetPin = 4; // MSGEQ7 RESET
int spektrumVerdi[7]; //Array til frekvensene
 
//Filtreringsverdi
int filterVerdi = 70;
 
//----------------FastLED-------------------------------------------

#define ANTALL_LEDS 60 // Antall LED pa stripa
#define DATA_PIN 3  // Data Pin til stripa

// Define the array of leds
CRGB leds[ANTALL_LEDS];

 
void setup()
{
  Serial.begin(9600);
  // Read from MSGEQ7 OUT
  pinMode(analogPin, INPUT);
  // Write to MSGEQ7 STROBE and RESET
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  
  // Initialiserer RESET og STROBE
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);

  //Initialisering av LEDS
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, ANTALL_LEDS);
}
 
void loop()
{
  lesMSGEQ7(); // Kaller på innlesing av verdier

  //Fjern når den er klar
  //Serial.println();

  visualiser(); // Kaller på fremvisning av verdier
  
}

void lesMSGEQ7(){
  
  // Setter RESET til lav for å aktivere STROBE
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);
  delayMicroseconds(72);
 
  // lagrer 7 verdier fra MSGEQ7
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(strobePin, LOW);
    delayMicroseconds(40); //Gir output tid til å stabilisere
 
    spektrumVerdi[i] = analogRead(analogPin);
 
    // Avgrenser verdiene til å være mellom filterValue og 1023
    spektrumVerdi[i] = constrain(spektrumVerdi[i], filterVerdi, 1023);
 
 
    // Re-mapper verdiene til tall mellom 0 og antall_LEDS (per kolonne)
    spektrumVerdi[i] = map(spektrumVerdi[i], filterVerdi, 1023, 0, 12);
 
    digitalWrite(strobePin, HIGH);
    delayMicroseconds(40); //Nødvendig for å oppnå minimum Strobe til Strobe tid.
  }
}

void visualiser(){
  
  FastLED.clear();

  for(int i = 0; i <= spektrumVerdi[0]; i++){
    leds[i] = CRGB::Green;
  }
  for(int i = 12; i <= spektrumVerdi[1]+12; i++){
    leds[i] = CRGB::Yellow;
  }
  for(int i = 24; i <= spektrumVerdi[2]+24; i++){
    leds[i] = CRGB::Red;
  }
  for(int i = 36; i <= spektrumVerdi[3]+36; i++){
    leds[i] = CRGB::Blue; 
  }
  for(int i = 48; i <= spektrumVerdi[4]+48; i++){
    leds[i] = CRGB::Purple; 
  }
  /*for(int i = 40; i < spektrumVerdi[5]+40; i++){
    leds[i] = CRGB::Coral;
  }
  for(int i = 48; i < spektrumVerdi[6]+48; i++){
    leds[i] = CRGB::GreenYellow; 
  }*/ 

  FastLED.setBrightness(255);
  FastLED.show();
  

}
