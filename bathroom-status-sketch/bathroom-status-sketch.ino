#include <RCSwitch.h>

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
      }
      if (currentstate == 3598) {  //Transmitter sends 3598 on when closing
        Serial.println("Door Closed.");
        milliclosed = millis();
      }
    }
    mySwitch.resetAvailable();
  }
}