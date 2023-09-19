#include <RCSwitch.h>
#include <Adafruit_NeoPixel.h>

#define PIN 3         // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 12  // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


int laststate;
int currentstate;
int sessionminutes;
int sessionseconds;
int minutesled;
int secondsled;
int doorstatus;
unsigned long previousMillis;
unsigned long currentMillis;
int leds[12];

RCSwitch mySwitch = RCSwitch();

void tick() {
  sessionseconds = sessionseconds + 1;
  if (sessionseconds == 60) {
    sessionseconds = 0;
    sessionminutes = sessionminutes + 1;
  }
  Serial.print("Session time = ");
  Serial.print(sessionminutes);
  Serial.print(":");
  if (sessionseconds < 10) {
    Serial.print("0");
  }
  Serial.println(sessionseconds);
}

void updateclock() {
  for (int i = 0; i < 12; ++i) {
    leds[i] = 0;
  }
  pixels.clear();
  secondsled = sessionseconds / 5;
  minutesled = sessionminutes;
  for (int i = 1; i <= sessionminutes; ++i) {
    pixels.setPixelColor(i, pixels.Color(20, 0, 0));
    if (i == 12) {
      pixels.setPixelColor(0, pixels.Color(20, 0, 0));
    }
  }
  pixels.setPixelColor(secondsled, pixels.Color(40, 10, 0));
  if ((secondsled <= minutesled) && (secondsled != 0)) {
    pixels.setPixelColor(secondsled, pixels.Color(60, 0, 0));
  }
  if ((secondsled == 0) && (minutesled >= 12)) {
    pixels.setPixelColor(0, pixels.Color(60, 0, 0));
  }
  pixels.show();
}

void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(1);  //Receiver on interrupt 1
  pixels.begin();
  pixels.clear();
  pixels.show();
}

void loop() {
  if (mySwitch.available()) {
    currentstate = mySwitch.getReceivedValue();
    if (currentstate != laststate) {
      laststate = currentstate;
      if (currentstate == 3594) {  //Transmitter sends 3594 on when opening
        Serial.println("Door opened.");
        doorstatus = 0;
        Serial.print("Session ended. Time = ");
        Serial.print(sessionminutes);
        Serial.print(":");
        if (sessionseconds < 10) {
          Serial.print("0");
        }
        Serial.println(sessionseconds);
        sessionseconds = 0;
        sessionminutes = 0;
        for (int i = 0; i < 12; ++i) {
          pixels.setPixelColor(i, pixels.Color(0, 20, 0));
        }
        pixels.show();
      }
      if (currentstate == 3598) {  //Transmitter sends 3598 on when closing
        Serial.println("Door closed.");
        doorstatus = 1;
      }
    }
    mySwitch.resetAvailable();
  }
  if (doorstatus == 1) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= 1000) {
      previousMillis = currentMillis;
      tick();
      updateclock();
    }
  }
}