#include "HUSKYLENS.h"

HUSKYLENS huskylens;

const int ledPin = 7;

void setup() {
    Serial.begin(115200);
    Wire.begin();


    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW); 
    while (!huskylens.begin(Wire)) {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1. Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings >> Protocol Type >> I2C)"));
        Serial.println(F("2. Please recheck the connection."));
        delay(100);
    }
}

void loop() {
    if (!huskylens.request()) {
        Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
        return;
    }

    if (!huskylens.isLearned()) {
        Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
        return;
    }

    if (!huskylens.available()) {
        Serial.println(F("No block or arrow appears on the screen!"));
        digitalWrite(ledPin, LOW); 
        return;
    }

    Serial.println(F("Data available!"));
    bool faceDetected = false;

    while (huskylens.available()) {
        HUSKYLENSResult result = huskylens.read();
        printResult(result);

        if (result.ID == 1 && result.command == COMMAND_RETURN_BLOCK) {
            faceDetected = true;
            Serial.println(F("Face detected!"));
        }
    }

    if (faceDetected) {
        digitalWrite(ledPin, HIGH);
        Serial.println(F("LED ON"));
    } else {
        digitalWrite(ledPin, LOW);
        Serial.println(F("LED OFF"));
    }
}

void printResult(HUSKYLENSResult result) {
    if (result.command == COMMAND_RETURN_BLOCK) {
        Serial.println(String() + F("Block: xCenter=") + result.xCenter + F(", yCenter=") + result.yCenter + F(", width=") + result.width + F(", height=") + result.height + F(", ID=") + result.ID);
    } else if (result.command == COMMAND_RETURN_ARROW) {
        Serial.println(String() + F("Arrow: xOrigin=") + result.xOrigin + F(", yOrigin=") + result.yOrigin + F(", xTarget=") + result.xTarget + F(", yTarget=") + result.yTarget + F(", ID=") + result.ID);
    } else {
        Serial.println(F("Object unknown!"));
    }
}
