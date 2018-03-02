#define BLINKER_PRINT	Serial

#if defined(ESP8266)
#define	BLINKER_PIN		BUILTIN_LED
#define	BLINKER_ON		LOW
#define	BLINKER_OFF		HIGH
#else
#define	BLINKER_PIN		13
#define	BLINKER_ON		HIGH
#define	BLINKER_OFF		LOW
#endif

#include <SoftwareSerial.h>

#define SBLE_RX_PIN		2
#define SBLE_TX_PIN		3
SoftwareSerial SerialBLE(SBLE_RX_PIN, SBLE_TX_PIN);

#include <BlinkerSimpleSerialBLE.h>

void setup()
{
	Serial.begin(115200);

	pinMode(BLINKER_PIN, OUTPUT);
	digitalWrite(BLINKER_PIN, BLINKER_OFF);

	SerialBLE.begin(9600);
	Blinker.begin(SerialBLE);
}

void loop()
{
	Blinker.run();

	if (Blinker.available()) {
		BLINKER_LOG2("Blinker.readString(): ", Blinker.readString());

		Blinker.vibrate();
		Blinker.print(millis());
	}

	if (Blinker.Button("Button1")) {
		digitalWrite(BLINKER_PIN, BLINKER_ON);
	}
	else {
		digitalWrite(BLINKER_PIN, BLINKER_OFF);
	}
}
