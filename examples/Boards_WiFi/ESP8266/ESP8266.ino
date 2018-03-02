#define	BLINKER_PRINT	Serial
#define	BLINKER_DEBUG_ALL

#include "BlinkerSimpleESP8266_WS.h"
// #include <SoftwareSerial.h>

// SoftwareSerial SerialBLE(2, 3);

// #include "BlinkerSimpleSerialBLE.h"

// #include "PMS/PMS.h"

char ssid[] = "mostfun";
char pswd[] = "18038083873";

// #define PMS_RX_PIN				8
// #define PMS_TX_PIN				9
// Pmsx003 pms(PMS_RX_PIN, PMS_TX_PIN);

// void pmsInit()
// {
//     pms.begin();
// 	pms.waitForData(Pmsx003::wakeupTime);
// 	pms.write(Pmsx003::cmdModePassive);
// }

// void readPMS()
// {
// 	pms.write(Pmsx003::cmdReadData);
// 	const Pmsx003::pmsIdx n = Pmsx003::nValues_PmsDataNames;
// 	Pmsx003::pmsData data[n];

// 	Pmsx003::PmsStatus status = pms.read(data, n);

// 	switch (status)
// 	{
// 		case Pmsx003::OK:
// 		{
// 			// for (Pmsx003::pmsIdx i = 0; i < n; ++i)
// 			// {
// 			// 	BLINKER_LOG3(data[i], "\t" + String(Pmsx003::getDataNames(i)), " [" + String(Pmsx003::getMetrics(i)) + "]");
// 			// }

// 			Blinker.print("PM1.0", data[Pmsx003::PM1dot0]);
// 			Blinker.print("PM2.5", data[Pmsx003::PM2dot5]);
// 			Blinker.print("PM10", data[Pmsx003::PM10dot0]);
// 			Blinker.print("HCHO", data[Pmsx003::Form]/1000.0);
// 			Blinker.print("temp", data[Pmsx003::Temp]/10.0);
// 			Blinker.print("humi", data[Pmsx003::Humi]/10);

// 			break;
// 		}
// 		case Pmsx003::noData:
// 			// BLINKER_LOG1("______noData______");
// 			break;
// 		default:
// 			BLINKER_LOG1("_________________");
// 			BLINKER_LOG1(Pmsx003::errorMsg[status]);
// 	}
// }



// #include "DHT/DHT.h"

// #define DHTPIN	4
// #define DHTTYPE DHT22
// DHT dht(DHTPIN, DHTTYPE);

#if defined(ESP8266)
#define	BLINKER_PIN			BUILTIN_LED
#define	BLINKER_ON			LOW
#define	BLINKER_OFF			HIGH
#else
#define	BLINKER_PIN			13
#define	BLINKER_ON			HIGH
#define	BLINKER_OFF			LOW
#endif

// void readDHT() {
// 	float h = dht.readHumidity();
// 	float t = dht.readTemperature();
// 	float f = dht.readTemperature(true);

// 	if (isnan(h) || isnan(t) || isnan(f)) {
// 		BLINKER_LOG1("Failed to read from DHT sensor!");
// 		return;
// 	}

// 	float hif = dht.computeHeatIndex(f, h);
// 	float hic = dht.computeHeatIndex(t, h, false);

// 	BLINKER_LOG3("Humidity: ", h, " %");
// 	BLINKER_LOG3("Temperature: ", t, " *C");
// 	BLINKER_LOG3("Temperature: ", f, " *F");
// 	BLINKER_LOG3("Heat index: ", hic, " *C");
// 	BLINKER_LOG3("Heat index: ", hif, " *F");

// 	Blinker.print("Humidity", h);
// 	Blinker.print("Temperature", t);
// 	Blinker.print("Temperature", f);
// 	Blinker.print("Heat index", hic);
// 	Blinker.print("Heat index", hif);
// }

void setup()
{
	Serial.begin(115200);

	pinMode(BLINKER_PIN, OUTPUT);
	digitalWrite(BLINKER_PIN, BLINKER_OFF);

	// SerialBLE.begin(9600);
	// Blinker.begin(SerialBLE);

	Blinker.begin(ssid, pswd);
	// Blinker.attachAhrs();
	// pmsInit();
	// dht.begin();
}

void loop()
{
	Blinker.run();

	if (Blinker.available())
	{
		BLINKER_LOG2("Blinker.readString(): ", Blinker.readString());
	// 	Blinker.print(millis());

	// // 	// uint16_t sliderget = map(Blinker.slider("Slider1"), 0, 255, 0, 1023);
	// // 	// analogWrite(BLINKER_PIN, sliderget);
		
	// // 	// uint16_t joystickget = map(Blinker.joystick("Joystick1", Xaxis), 0, 255, 0, 1023);
	// // 	// analogWrite(BLINKER_PIN, joystickget);
		Blinker.vibrate();
		
		Blinker.print("text1",millis());
		// Blinker.print("test",millis());

		// Blinker.button("Button1");
	}

	// readDHT();
	// readPMS();
	// Blinker.delay(1000);

	if (Blinker.button("Button1")) {
		digitalWrite(BLINKER_PIN, BLINKER_ON);
		// Blinker.delay(1000);
	}
	else {
		digitalWrite(BLINKER_PIN, BLINKER_OFF);
		// Blinker.delay(1000);
	}
	
	// Blinker.button("Button2");

	// BLINKER_LOG2("Slider read: ", Blinker.slider("Slider1"));
	// Blinker.slider("Slider1");
	// BLINKER_LOG2("Joystick X read: ", Blinker.joystick("Joystick1", J_Xaxis));
	// BLINKER_LOG2("Joystick Y read: ", Blinker.joystick("Joystick1", J_Yaxis));
	// Blinker.joystick("Joystick1", J_Xaxis);
	// BLINKER_LOG2("Ahrs Yaw read: ", Blinker.ahrs(Yaw));
	// BLINKER_LOG2("Ahrs Roll read: ", Blinker.ahrs(Roll));
	// BLINKER_LOG2("Ahrs Pitch read: ", Blinker.ahrs(Pitch));

	Blinker.delay(5000);
}
