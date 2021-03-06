#define BLINKER_PRINT Serial
#define BLINKER_BLE

#include <Blinker.h>

#define RGB1 "RGBKEY"

void setup()
{
    Serial.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    
    Blinker.begin();
    Blinker.wInit(RGB1, W_RGB);
}

void loop()
{
    Blinker.run();

    if (Blinker.available()) {
        BLINKER_LOG2("Blinker.readString(): ", Blinker.readString());

        Blinker.vibrate();
        
        uint32_t BlinkerTime = millis();
        Blinker.print(BlinkerTime);
        Blinker.print("millis", BlinkerTime);
    }

    BLINKER_LOG2("Red color: ", Blinker.rgb(RGB1,R));
    BLINKER_LOG2("Green color: ", Blinker.rgb(RGB1,G));
    BLINKER_LOG2("Blue color: ", Blinker.rgb(RGB1,B));

    Blinker.delay(2000);
}