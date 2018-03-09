#ifndef BlinkerProtocol_H
#define BlinkerProtocol_H

#include <Blinker/BlinkerApi.h>

template <class Transp>
class BlinkerProtocol
    : public BlinkerApi< BlinkerProtocol<Transp> >
{
    friend class BlinkerApi< BlinkerProtocol<Transp> >;

    typedef BlinkerApi< BlinkerProtocol<Transp> > BApi;

    public :
        enum BlinkerState{
            CONNECTING,
            CONNECTED,
            DISCONNECTED
        };

        BlinkerProtocol(Transp& transp)
            : BApi()
            , conn(transp)
            , state(CONNECTING)
            , isFresh(false)
            , isAvail(false)
            , canParse(false)
        {}

        bool connected() { return state == CONNECTED; }

        bool connect(uint32_t timeout = BLINKER_STREAM_TIMEOUT)
        {
            state = CONNECTING;

            uint32_t startTime = millis();
            while ( (state != CONNECTED) && (millis() - startTime) < timeout ) {
                run();
            }

            return state == CONNECTED;
        }

        void disconnect() 
        {
            conn.disconnect();
            state = DISCONNECTED;
        }

        void run();

        bool available() { return isFresh; }

        String readString()
        {
            if (isFresh) {
                isFresh = false;
                return conn.lastRead();
            }
            else {
                return "";
            }
        }
        
        void print(const String &s)     { conn.print(s + BLINKER_CMD_NEWLINE); }
        void print(const char str[])    { print(STRING_format(str)); }
        void print(char c)              { print(STRING_format(c)); }
        void print(unsigned char b)     { print(STRING_format(b)); }
        void print(int n)               { print(STRING_format(n)); }
        void print(unsigned int n)      { print(STRING_format(n)); }
        void print(long n)              { print(STRING_format(n)); }
        void print(unsigned long n)     { print(STRING_format(n)); }
        void print(double n)            { print(STRING_format(n)); }
        void print()                    { print(""); }
        void println(const String &s)   { print(s); }
        void println(const char str[])  { print(str); }
        void println(char c)            { print(c); }
        void println(unsigned char b)   { print(b); }
        void println(int n)             { print(n); }
        void println(unsigned int n)    { print(n); }
        void println(long n)            { print(n); }
        void println(unsigned long n)   { print(n); }
        void println(double n)          { print(n); }
        void println()                  { print(); }
        void print(const String &s1, const String &s2)      { print("{\"" + s1 + "\":\"" + s2 + "\"}"); }
        void print(const char str1[], const char str2[])    { print("{\"" + STRING_format(str1) + "\":\"" + STRING_format(str2) + "\"}"); }
        void print(const char str[], char c)                { print("{\"" + STRING_format(str) + "\":" + STRING_format(c) + "}"); }
        void print(const char str[], unsigned char b)       { print("{\"" + STRING_format(str) + "\":" + STRING_format(b) + "}"); }
        void print(const char str[], int n)                 { print("{\"" + STRING_format(str) + "\":" + STRING_format(n) + "}"); }
        void print(const char str[], unsigned int n)        { print("{\"" + STRING_format(str) + "\":" + STRING_format(n) + "}"); }
        void print(const char str[], long n)                { print("{\"" + STRING_format(str) + "\":" + STRING_format(n) + "}"); }
        void print(const char str[], unsigned long n)       { print("{\"" + STRING_format(str) + "\":" + STRING_format(n) + "}"); }
        void print(const char str[], double n)              { print("{\"" + STRING_format(str) + "\":" + STRING_format(n) + "}"); }
        void println(const String &s1, const String &s2)    { print(s1, s2); }
        void println(const char str1[], const char str2[])  { print(str1, str2); }
        void println(const char str[], char c)              { print(str, c); }
        void println(const char str[], unsigned char b)     { print(str, b); }
        void println(const char str[], int n)               { print(str, n); }
        void println(const char str[], unsigned int n)      { print(str, n); }
        void println(const char str[], long n)              { print(str, n); }
        void println(const char str[], unsigned long n)     { print(str, n); }
        void println(const char str[], double n)            { print(str, n); }

        void flush() {
            isFresh = false;
        }

    private :
        bool checkAvail()
        {
            isAvail = conn.available();
            if (isAvail) {
                isFresh = true;
                canParse = true;
            }
            return isAvail;
        }

        String dataParse()
        {
            if (canParse) {
                return conn.lastRead();
            }
            else {
                return "";
            }
        }

        void isParsed() { isFresh = false; canParse = false; }// BLINKER_LOG1("isParsed");

        bool parseState() { return canParse; }

    protected :
        Transp&         conn;
        BlinkerState    state;
        bool            isFresh;
        bool            isAvail;
        bool            canParse;
        
        void begin()
        {
            BLINKER_LOG1(BLINKER_F(""));
        #if defined(BLINKER_NO_LOGO)
            BLINKER_LOG2(BLINKER_F("Blinker v"), BLINKER_VERSION);
        #elif defined(BLINKER_LOGO_3D)
            BLINKER_LOG1(BLINKER_F("\n"
                " ____    ___                __                       \n"
                "/\\  _`\\ /\\_ \\    __        /\\ \\               v"BLINKER_VERSION"\n"
                "\\ \\ \\L\\ \\//\\ \\  /\\_\\    ___\\ \\ \\/'\\      __   _ __   \n"
                " \\ \\  _ <'\\ \\ \\ \\/\\ \\ /' _ `\\ \\ , <    /'__`\\/\\`'__\\ \n"
                "  \\ \\ \\L\\ \\\\_\\ \\_\\ \\ \\/\\ \\/\\ \\ \\ \\\\`\\ /\\  __/\\ \\ \\/  \n"
                "   \\ \\____//\\____\\\\ \\_\\ \\_\\ \\_\\ \\_\\ \\_\\ \\____\\\\ \\_\\  \n"
                "    \\/___/ \\/____/ \\/_/\\/_/\\/_/\\/_/\\/_/\\/____/ \\/_/  \n"));
        #else
            BLINKER_LOG1(BLINKER_F("\n"
                "   ___  ___      __    v"BLINKER_VERSION"\n"
                "  / _ )/ (_)__  / /_____ ____\n"
                " / _  / / / _ \\/  '_/ -_) __/\n"
                "/____/_/_/_//_/_/\\_\\\\__/_/   \n"));
        #endif
        }
};

template <class Transp>
void BlinkerProtocol<Transp>::run()
{
    bool conState = conn.connected();

    switch (state)
    {
        case CONNECTING :
            if (conn.connect()) {
                state = CONNECTED;
            }
            break;
        case CONNECTED :
            if (conState) {
                checkAvail();
                if (isAvail) {
                    BApi::parse();
                }
            }
            else {
                state = DISCONNECTED;
            }
            break;
        case DISCONNECTED :
            conn.disconnect();
            state = CONNECTING;
            break;
    }
}

#endif