// Adafruit IO Temperature & Humidity Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-temperature-and-humidity
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016-2017 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"
#include "Freenove_WS2812_Lib_for_ESP32.h"
#define PIN_ANALOG_IN 35
#define AUDIO_PIN_IN 36
#define LEDS_COUNT 8 // The number of led
#define LEDS_PIN 15 // define the pin connected to the Freenove 8 led strip
#define CHANNEL 0 //
#define LIGHT_MIN 372
#define LIGHT_MAX 2048
Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);

u8 color_correct[5][3] = { {255, 255, 255}, {0, 255, 0}, {0, 0, 255}, {255, 255, 255}, {0, 0, 0} };
u8 color_env_error[5][3] = { {255, 0, 0}, {0, 255, 0}, {255, 0, 0}, {0, 255, 0}, {255, 0, 0} };

int delayval = 100;
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
bool env_error = false;



/************************ Example Starts Here *******************************/
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// pin connected to DH22 data line
#define DATA_PIN 13


// create DHT22 instance
DHT_Unified dht(DATA_PIN, DHT11);

AdafruitIO_Feed *temperature = io.feed("temperature");
AdafruitIO_Feed *humidity = io.feed("humidity");
AdafruitIO_Feed *ambient_light = io.feed("ambient_light");
AdafruitIO_Feed *audio = io.feed("audio");
//AdafruitIO_Feed *alert = io.feed("alert");

void setup() {
  strip.begin();
  strip.setBrightness(10);

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  // initialize dht22
  dht.begin();

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {
  

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  
  Serial.println(env_error);
  
  

  for (int j = 0; j < 5; j++) 
  {
    for (int i = 0; i < LEDS_COUNT; i++) 
    {
      if (!env_error) {
        strip.setLedColorData(i, color_correct[j][0], color_correct[j][1], color_correct[j][2]);
      } else {
        strip.setLedColorData(i, color_env_error[j][0], color_env_error[j][1], color_env_error[j][2]);
      }
      
      strip.show();
      delay(delayval);
    }
    delay(500);
  }

  float  sound = (float) audioCheck();

  sensors_event_t event;
  dht.temperature().getEvent(&event);
  

  float celsius = event.temperature;
  float fahrenheit = (celsius * 1.8) + 32;


  Serial.print("celsius: ");
  Serial.print(celsius);
  Serial.println(" C");

  Serial.print("fahrenheit: ");
  Serial.print(fahrenheit);
  Serial.println(" F");

  
  

  // save fahrenheit (or celsius) to Adafruit IO
  temperature->save(fahrenheit);

//  if (fahrenheit < 68.0) {
//    alert->save("Temperature Issue: The room is a bit cold");
//  } else if (fahrenheit > 76.0) {
//    alert->save("Temperature Issue: The room is a bit warm");
//  }

  dht.humidity().getEvent(&event);

  Serial.print("humidity: ");
  Serial.print(event.relative_humidity);
  Serial.println(" %");

  // save humidity to Adafruit IO
  humidity->save(event.relative_humidity);

//  if (event.relative_humidity < 30.0) {
//    alert->save("Humidity Issue: The room is a bit dry. This could lead to dry skin and mouth.");
//  } else if (event.relative_humidity > 50.0) {
//    alert->save("Humidity Issue: The room is too humid. This could encourage the growth of fungus and mold.");
//  }

    lightCheck();
    
  

  env_error = env_check(fahrenheit,event.relative_humidity,sound);
  Serial.println(env_error);
  

  // wait 5 seconds (5000 milliseconds == 5 seconds)
  delay(9000);

}


bool env_check(float temp,float humd,float sound) {
  if ((temp < 68.0) | (temp > 76.0) ) {
    return true;
  }

  if (humd < 30.0 | humd > 50.0 ) {
    return true;
  }

  if ( sound > 38.0 ) {
    return true;
  }

  return false;

  


  
}


void lightCheck() {
  int adcVal = analogRead(PIN_ANALOG_IN);
  Serial.print("Light value: ");
  Serial.println(adcVal);
  ambient_light->save(adcVal);
}

double audioCheck() {
  unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(AUDIO_PIN_IN);
      if (sample < 20000)  // toss out spurious readings
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
   peakToPeak = (signalMax - signalMin);  // max - min = peak-peak amplitude
   double volts = ((peakToPeak * 3.3) / 4096) * 0.707;
   double first = log10(volts/0.00631)*20;
   double second = first + 94 - 44 - 60;
   Serial.print("Audio: ");
   Serial.println(first);
   audio->save(first);
   return first;
}
