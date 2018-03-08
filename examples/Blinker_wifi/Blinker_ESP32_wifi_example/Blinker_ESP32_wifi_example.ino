#define	BLINKER_PRINT	Serial

#define	BLINKER_PIN		BUILTIN_LED
#define	BLINKER_ON		HIGH
#define	BLINKER_OFF		LOW

#include <BlinkerSimpleESP32_WS.h>

char ssid[] = "<Your WiFi network SSID or name>";
char pswd[] = "<Your WiFi network WPA password or WEP key>";

void setup()
{
	Serial.begin(115200);

	pinMode(BLINKER_PIN, OUTPUT);
	digitalWrite(BLINKER_PIN, BLINKER_OFF);

	Blinker.begin(ssid, pswd);
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
