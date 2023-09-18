#include <RCSwitch.h>
#include <Adafruit_NeoPixel.h>

#define PIN 3         // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 12  // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


int laststate;
int currentstate;
long milliclosed;
long sessiontime;
int sessionminutes;
int sessionseconds;

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(1);  //Receiver on interrupt 1
  pixels.begin();
}

void loop() {
  if (mySwitch.available()) {
    currentstate = mySwitch.getReceivedValue();
    if (currentstate != laststate) {
      laststate = currentstate;
      if (currentstate == 3594) {  //Transmitter sends 3594 on when opening
        sessiontime = (millis() - milliclosed) / 1000;
        sessionminutes = sessiontime / 60;
        sessionseconds = sessiontime - (sessionminutes * 60);
        Serial.print("Door Opened. Session time = ");
        Serial.print(sessionminutes);
        Serial.print("m");
        Serial.print(sessionseconds);
        Serial.println("s");
        for (int i = 0; i < NUMPIXELS; i++) {  // For each pixel...
          pixels.setPixelColor(i, pixels.Color(0, 50, 0));
        }
        pixels.show();  // Send the updated pixel colors to the hardware.
      }
      if (currentstate == 3598) {  //Transmitter sends 3598 on when closing
        Serial.println("Door Closed.");
        milliclosed = millis();
        for (int i = 0; i < NUMPIXELS; i++) {  // For each pixel...
          pixels.setPixelColor(i, pixels.Color(150, 0, 0));
        }
        pixels.show();  // Send the updated pixel colors to the hardware.
      }
    }
    mySwitch.resetAvailable();
  }
}