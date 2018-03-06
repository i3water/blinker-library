#define	BLINKER_PRINT	Serial

#define	BLINKER_PIN		21
#define	BLINKER_ON		LOW
#define	BLINKER_OFF		HIGH

#include <BlinkerSimpleESP32_BLE.h>

void setup()
{
	Serial.begin(115200);

	pinMode(BLINKER_PIN, OUTPUT);
	digitalWrite(BLINKER_PIN, BLINKER_OFF);

	Blinker.begin();
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

	if (Blinker.button("Button1")) {
		digitalWrite(BLINKER_PIN, BLINKER_ON);
	}
	else {
		digitalWrite(BLINKER_PIN, BLINKER_OFF);
	}
}
