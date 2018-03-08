#define BLINKER_PRINT	Serial

#include <SoftwareSerial.h>

#define SBLE_RX_PIN		2
#define SBLE_TX_PIN		3
SoftwareSerial SerialBLE(SBLE_RX_PIN, SBLE_TX_PIN);

#include <BlinkerSimpleSerialBLE.h>

void setup()
{
	Serial.begin(115200);

	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);

	SerialBLE.begin(9600);
	Blinker.begin(SerialBLE);
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
		digitalWrite(LED_BUILTIN, HIGH);
	}
	else {
		digitalWrite(LED_BUILTIN, LOW);
	}
}
