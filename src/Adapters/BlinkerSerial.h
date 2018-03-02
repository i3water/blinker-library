#ifndef BlinkerSerial_H
#define BlinkerSerial_H

#include "BlinkerProtocol.h"

class BlinkerTransportStream
{
    public :
        BlinkerTransportStream()
            : stream(NULL), isConnect(false)
        {}

        bool available(char *data)
        {
            if (stream->available()) {
                strcpy(data, (stream->readString()).c_str());
#ifdef BLINKER_DEBUG_ALL
                BLINKER_LOG2(BLINKER_F("handleSerial: "), data);
#endif
                return true;
            }
            else {
                return false;
            }
        }

        void begin(Stream& s)
        {
            stream = &s;
            stream->setTimeout(BLINKER_STREAM_TIMEOUT);
        }

        void print(String s)
        {
#ifdef BLINKER_DEBUG_ALL
            BLINKER_LOG2(BLINKER_F("Response: "), s);
#endif
            if(connected()) {
#ifdef BLINKER_DEBUG_ALL
                BLINKER_LOG1(BLINKER_F("Succese..."));
#endif
                stream->print(s);
            }
            else {
#ifdef BLINKER_DEBUG_ALL
                BLINKER_LOG1(BLINKER_F("Faile... Disconnected"));
#endif
            }
        }

        bool connect()
        {
            isConnect = true;
            return connected();
        }

        bool connected() { return isConnect; }

        void disconnect() { isConnect = false; }
    
    protected :
        Stream* stream;
        bool    isConnect;
};

class BlinkerSerail
    : public BlinkerProtocol<BlinkerTransportStream>
{
    typedef BlinkerProtocol<BlinkerTransportStream> Base;

    public :
        BlinkerSerail(BlinkerTransportStream& transp)
            : Base(transp)
        {}

        void begin(Stream& stream)
        {
            Base::begin();
            this->conn.begin(stream);
            BLINKER_LOG1("SerialBLE Initialled...")
        }
};

#endif