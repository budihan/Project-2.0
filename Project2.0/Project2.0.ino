#include "pitches.h"
#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, PIN, NEO_GRB + NEO_KHZ800);

//Mic
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

//lightflicker
int delayTime = 250;

//lcd
LiquidCrystal lcd(12,11,5,4,3,2);
int noiseCount = 0;

void setup()
{
  strip.begin();
  strip.setBrightness(10);
  Serial.begin(9600);
  //LCD opstellen
  lcd.begin(16,2);
  lcd.print("Aantal waarschuwingen");
}

void loop()
{
  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(0); //Mic pin
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  double volts = (peakToPeak * 3.3) / 1024;  // convert to volts

  Serial.println(volts);

//Voorwaarden
//TODO: volt bijscherpen, Op LCD weergeven + count bijhouden, Speaker

  //Geel licht = kleine geluidsoverlast
  if ( volts > 1.25 && volts < 3) {
    for (int i = 0; i < 12; i++)
    {
      strip.setPixelColor(i, 255, 255, 0);
      strip.show();
    }
    //knipperend rood = geluidsoverlast
  } else if ( volts > 3) {
    bleepWarning();
    countNoise();
    for (int i = 0; i < 10; i++) {
      lightFlicker();
    }
    // groen licht = geen overlast
  } else {
    for (int i = 0; i < 12; i++) {
      strip.setPixelColor(i, 0, 255, 0);
      strip.show();
    }
  }
}



void countNoise(){
  noiseCount++;
  lcd.setCursor(0,1);
  lcd.print(noiseCount);
}

void bleepWarning(){
  int melody [] = {
    NOTE_C4, NOTE_G4, NOTE_C4
  };
  int noteDurations[] = {
    8,8,8
  };
  for (int n = 0; n < 3; n++){
    int noteDuration = 1000 / noteDurations[n];
    tone (8,melody[n],noteDuration);
    int pause = noteDuration * 1.80;
    delay(pause);
    noTone(3);
  }
}

void lightFlicker() {
  strip.setPixelColor(0, 0, 0, 0);
  strip.setPixelColor(1, 0, 0, 0);
  strip.setPixelColor(2, 0, 0, 0);
  strip.setPixelColor(3, 0, 0, 0);
  strip.setPixelColor(4, 0, 0, 0);
  strip.setPixelColor(5, 0, 0, 0);
  strip.setPixelColor(6, 0, 0, 0);
  strip.setPixelColor(7, 0, 0, 0);
  strip.setPixelColor(8, 0, 0, 0);
  strip.setPixelColor(9, 0, 0, 0);
  strip.setPixelColor(10, 0, 0, 0);
  strip.setPixelColor(11, 0, 0, 0);
  strip.show();
  delay(delayTime);
  strip.setPixelColor(0, 255, 0, 0);
  strip.setPixelColor(1, 255, 0, 0);
  strip.setPixelColor(2, 255, 0, 0);
  strip.setPixelColor(3, 255, 0, 0);
  strip.setPixelColor(4, 255, 0, 0);
  strip.setPixelColor(5, 255, 0, 0);
  strip.setPixelColor(6, 255, 0, 0);
  strip.setPixelColor(7, 255, 0, 0);
  strip.setPixelColor(8, 255, 0, 0);
  strip.setPixelColor(9, 255, 0, 0);
  strip.setPixelColor(10, 255, 0, 0);
  strip.setPixelColor(11, 255, 0, 0);
  strip.show();
  delay(delayTime);
}




