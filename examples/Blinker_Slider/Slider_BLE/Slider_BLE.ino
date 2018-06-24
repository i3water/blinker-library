#define BLINKER_PRINT Serial
#define BLINKER_BLE

#include <Blinker.h>

#define SLIDER_1 "SliderKey"

void setup()
{
    Serial.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    
    Blinker.begin();
    Blinker.wInit(SLIDER_1, W_SLIDER);
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

    BLINKER_LOG2("Slider read: ", Blinker.slider(SLIDER_1));

    Blinker.delay(2000);
}