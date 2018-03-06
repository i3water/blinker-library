#ifndef BlinkerSimplerESP32_BLE_H
#define BlinkerSimplerESP32_BLE_H

#include <Blinker/BlinkerProtocol.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// #define SERVICE_UUID        "0000ffe0-1fb5-459e-8fcc-c5c9c331914b"
// #define CHARACTERISTIC_UUID "0000ffe1-36e1-4688-b7f5-ea07361b26a8"

class BlinkerTransportESP32_BLE
    :
    public BLEServerCallbacks,
    public BLECharacteristicCallbacks
{
    public :
        BlinkerTransportESP32_BLE()
            : deviceConnected(false), isAvail(false)
        {}

        void begin()
        {
            BLEDevice::init("Blinker");
            pServer = BLEDevice::createServer();

            pService = pServer->createService(BLEUUID((uint16_t)0xffe0));//SERVICE_UUID
            pServer->setCallbacks(this);

            pCharacteristic = pService->createCharacteristic(
                                            BLEUUID((uint16_t)0xffe1),//CHARACTERISTIC_UUID,
                                            BLECharacteristic::PROPERTY_READ |
                                            BLECharacteristic::PROPERTY_NOTIFY |
                                            BLECharacteristic::PROPERTY_WRITE_NR
                                        );

            pCharacteristic->setCallbacks(this);

            pCharacteristic->setValue("Blinker");
            pService->start();

            pAdvertising = pServer->getAdvertising();

            BLEAddress otherAddress = BLEDevice::getAddress();

            esp_bd_addr_t ble_m_address;
            memcpy(ble_m_address, otherAddress.getNative(), ESP_BD_ADDR_LEN);
            char macStr[9] = { 0 };
            macStr[0] = 0x48;
            macStr[1] = 0X4d;
            for (uint8_t cpynum = 0; cpynum < 6; cpynum++) {
                macStr[cpynum+2] = (char)ble_m_address[cpynum];
            }

            pAdvertisementData.setManufacturerData(macStr);
            pAdvertising->setAdvertisementData(pAdvertisementData);
            pAdvertising->addServiceUUID(BLEUUID((uint16_t)0xffe0));
            pAdvertising->start();
        }

        bool available()
        {
            if (isAvail) {
#ifdef BLINKER_DEBUG_ALL
                BLINKER_LOG2(BLINKER_F("handleBLE: "), BLEBuf);//
#endif
                isAvail = false;

                return true;
            }
            else {
                return false;
            }
        }

        String lastRead() { return STRING_format(BLEBuf); }//

        void print(String s) {
#ifdef BLINKER_DEBUG_ALL
            BLINKER_LOG2(BLINKER_F("Response: "), s);
#endif
            if (connected()) {
#ifdef BLINKER_DEBUG_ALL
                BLINKER_LOG1(BLINKER_F("Succese..."));
#endif
                pCharacteristic->setValue(s.c_str());
            }
            else {
#ifdef BLINKER_DEBUG_ALL
                BLINKER_LOG1(BLINKER_F("Faile... Disconnected"));
#endif                
            }
        }

        bool connect() { return deviceConnected = true; }

        void disconnect() { deviceConnected = false; }

        bool connected() { return deviceConnected; }

    private :
        bool                    deviceConnected;
        char                    BLEBuf[BLINKER_BUFFER_SIZE];
        bool                    isAvail;
        String                  BLEStr;
        BLEServer               *pServer;
        BLEService              *pService;
        BLECharacteristic       *pCharacteristic;
        BLEAdvertising          *pAdvertising;
        BLEAdvertisementData    pAdvertisementData;

        void onConnect(BLEServer* pServer) {
            deviceConnected = true;
#ifdef BLINKER_DEBUG_ALL
            BLINKER_LOG1("BLE connect");
#endif
        };

        void onDisconnect(BLEServer* pServer) {
            deviceConnected = false;
#ifdef BLINKER_DEBUG_ALL
            BLINKER_LOG1("BLE disconnect");
#endif
        }

        void onWrite(BLECharacteristic *pCharacteristic) {
            std::string value = pCharacteristic->getValue();

            if (value.length() > 0) {
                // BLEStr += value;
                char *data;
                uint8_t len = value.length();
                data = (char*)malloc((len+1)*sizeof(char));
                strcpy(data, value.c_str());
                // BLINKER_LOG2("data: ", String(data));
                BLEStr += String(data);

                if (STRING_contais_string(BLEStr, BLINKER_CMD_NEWLINE)) {
                    // BLINKER_LOG2("BLEStr: ", BLEStr);
                    strcpy(BLEBuf, BLEStr.c_str());
                    BLINKER_LOG2("BLEBuf: ", BLEBuf);
                    BLEStr = "";
                    isAvail = true;
                }

                // Serial.println("*********");
                // Serial.print("New value: ");
                // for (int i = 0; i < value.length(); i++)
                //     Serial.print(value[i]);

                // Serial.println();
                // Serial.println("*********");
            }
        }
};

class BlinkerESP32_BLE
    : public BlinkerProtocol<BlinkerTransportESP32_BLE>
{
    typedef BlinkerProtocol<BlinkerTransportESP32_BLE> Base;

    public:
        BlinkerESP32_BLE(BlinkerTransportESP32_BLE & transp)
            : Base(transp)
        {}

        void begin()
        {
            Base::begin();
            conn.begin();
        }
};

static BlinkerTransportESP32_BLE _blinkerTransportBLE;
BlinkerESP32_BLE Blinker(_blinkerTransportBLE);

#endif