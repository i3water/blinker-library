#ifndef BlinkerApi_H
#define BlinkerApi_H

#include <Blinker/BlinkerConfig.h>
#include <utility/BlinkerDebug.h>
#include <utility/BlinkerUtility.h>

enum JoyStickAxis{
    J_Xaxis,
    J_Yaxis
};

// enum GyroAxis{
//     G_Xaxis,
//     G_Yaxis,
//     G_Zaxis
// };

enum AhrsAttitude{
    Yaw,
    Pitch,
    Roll
};

static class BlinkerButton * _Button[BLINKER_MAX_WIDGET_SIZE];
static class BlinkerSlider * _Slider[BLINKER_MAX_WIDGET_SIZE];
// static class BlinkerJoystick * _Joystick[BLINKER_MAX_WIDGET_SIZE];

class BlinkerButton
{
    public :
        BlinkerButton()
            : buttonName(NULL), buttonState(false)
        {}
        
        void name(String name) { buttonName = name; }
        String getName() { return buttonName; }
        void freshState(bool state) { buttonState = state; }
        bool getState() { return buttonState; }
        bool checkName(String name) { return ((buttonName == name) ? true : false); }
    
    private :
        String  buttonName;
        bool    buttonState;
};

class BlinkerSlider
{
    public :
        BlinkerSlider()
            : sliderName(NULL), sliderValue(0)
        {}
        
        void name(String name) { sliderName = name; }
        String getName() { return sliderName; }
        void freshValue(uint8_t value) { sliderValue = value; }
        uint8_t getValue() { return sliderValue; }
        bool checkName(String name) { return ((sliderName == name) ? true : false); }
    
    private :
        String  sliderName;
        uint8_t sliderValue;
};

// class BlinkerJoystick
// {
//     public :
//         BlinkerJoystick()
//             : joystickName(NULL)
//         {
//             joystickValue[J_Xaxis] = BLIKER_JOYSTICK_VALUE_DEFAULT;
//             joystickValue[J_Yaxis] = BLIKER_JOYSTICK_VALUE_DEFAULT;
//         }
        
//         void name(String name) { joystickName = name; }
//         String getName() { return joystickName; }
//         void freshValue(uint8_t value, JoyStickAxis axis) { joystickValue[axis] = value; }
//         uint8_t getValue(JoyStickAxis axis) { return joystickValue[axis]; }
//         bool checkName(String name) { return ((joystickName == name) ? true : false); }
    
//     private :
//         String  joystickName;
//         uint8_t joystickValue[2];
// };

template <class T>
int8_t checkNum(String name, T * c, uint8_t count)
{
    // if(count != 0) {
        for (uint8_t cNum = 0; cNum < count; cNum++) {
            if (c[cNum]->checkName(name))
                return cNum;
        }
    // }
    // else {
    //     if (c[count]->checkName(name))
    //             return count;
    // }
    return BLINKER_OBJECT_NOT_AVAIL;
}

template <class Proto>
class BlinkerApi
{
    public :
        BlinkerApi() {
            // for (uint8_t cNum = 0; cNum < BLINKER_MAX_WIDGET_SIZE; cNum++) {
                // _Button[0] = new BlinkerButton();
                // _Slider[0] = new BlinkerSlider();
                // _Joystick[0] = new BlinkerJoystick();
            // }
            // gyroValue[G_Xaxis] = 0.00;
            // gyroValue[G_Yaxis] = 0.00;
            // gyroValue[G_Zaxis] = 0.00;
            joyValue[J_Xaxis] = BLINKER_JOYSTICK_VALUE_DEFAULT;
            joyValue[J_Yaxis] = BLINKER_JOYSTICK_VALUE_DEFAULT;
            ahrsValue[Yaw] = 0;
            ahrsValue[Roll] = 0;
            ahrsValue[Pitch] = 0;
        }

        void parse()
        {
            // BLINKER_LOG2("Parse: ", static_cast<Proto*>(this)->dataParse());
            if (static_cast<Proto*>(this)->parseState() ) {
                _fresh = false;

                for (uint8_t bNum = 0; bNum < _bCount; bNum++) {
                    button(_Button[bNum]->getName());
                }
                for (uint8_t sNum = 0; sNum < _sCount; sNum++) {
                    slider(_Slider[sNum]->getName());
                }
                // for (uint8_t jNum = 0; jNum < _jCount; jNum++) {
                //     joystick(_Joystick[jNum]->getName(), J_Xaxis);
                // }
                joystick(J_Xaxis);
                // gyro(G_Xaxis);
                ahrs(Yaw);

                // static_cast<Proto*>(this)->print("test!!");
                if (_fresh) {
                    static_cast<Proto*>(this)->isParsed();
                }
                // BLINKER_LOG6("_bCount: ", _bCount, " _sCount: ", _sCount, " _jCount: ", _jCount);
            }
        }

        bool button(const String & _bName)
        {
            // BLINKER_LOG2("bname: ", _bName);
            int8_t num = checkNum(_bName, _Button, _bCount);
            String state = STRING_find_string_value(static_cast<Proto*>(this)->dataParse(), _bName);

            if (state == BLINKER_CMD_BUTTON_PRESSED) {
                if( num == BLINKER_OBJECT_NOT_AVAIL ) {
                    if ( _bCount < BLINKER_MAX_WIDGET_SIZE ) {
                        _Button[_bCount] = new BlinkerButton();
                        _Button[_bCount]->name(_bName);
                        _Button[_bCount]->freshState(true);
                        _bCount++;
                    }
                }
                else {
                    _Button[num]->freshState(true);
                }

                // static_cast<Proto*>(this)->isParsed();
                _fresh = true;
                // BLINKER_LOG1("button isParsed");
                return true;
            }
            else if (state == BLINKER_CMD_BUTTON_RELEASED) {
                if( num == BLINKER_OBJECT_NOT_AVAIL ) {
                    if ( _bCount < BLINKER_MAX_WIDGET_SIZE ) {
                        _Button[_bCount] = new BlinkerButton();
                        _Button[_bCount]->name(_bName);
                        _Button[_bCount]->freshState(false);
                        _bCount++;
                    }
                }
                else {
                    _Button[num]->freshState(false);
                }

                // static_cast<Proto*>(this)->isParsed();
                _fresh = true;
                // BLINKER_LOG1("button isParsed");
                return false;
            }
            else {
                if( num == BLINKER_OBJECT_NOT_AVAIL ) {
                    if ( _bCount < BLINKER_MAX_WIDGET_SIZE ) {
                        _Button[_bCount] = new BlinkerButton();
                        _Button[_bCount]->name(_bName);
                        _bCount++;
                    }
                    return false;
                }

                return _Button[num]->getState();;
            }
        }

        uint8_t slider(const String & _sName)
        {
            // BLINKER_LOG2("bname: ", _sName);
            int8_t num = checkNum(_sName, _Slider, _sCount);
            int16_t value = STRING_find_numberic_value(static_cast<Proto*>(this)->dataParse(), _sName);

            if (value != FIND_KEY_VALUE_FAILED) {
                if( num == BLINKER_OBJECT_NOT_AVAIL ) {
                    if ( _sCount < BLINKER_MAX_WIDGET_SIZE ) {
                        _Slider[_sCount] = new BlinkerSlider();
                        _Slider[_sCount]->name(_sName);
                        _Slider[_sCount]->freshValue(value);
                        _sCount++;
                    }
                }
                else {
                    _Slider[num]->freshValue(value);
                }

                // static_cast<Proto*>(this)->isParsed();
                _fresh = true;
                // BLINKER_LOG1("slider isParsed");
                return value;
            }
            else {
                if( num == BLINKER_OBJECT_NOT_AVAIL ) {
                    if ( _sCount < BLINKER_MAX_WIDGET_SIZE ) {
                        _Slider[_sCount] = new BlinkerSlider();
                        _Slider[_sCount]->name(_sName);
                        _sCount++;
                    }
                    return 0;
                }
                
                return _Slider[num]->getValue();
            }
        }

        // uint8_t joystick(const String & _jName, JoyStickAxis axis)
        // {
        //     // BLINKER_LOG2("bname: ", _jName);
        //     int8_t num = checkNum(_jName, _Joystick, _jCount);
        //     int16_t jAxisValue = STRING_find_array_numberic_value(static_cast<Proto*>(this)->dataParse(), _jName, axis);

        //     if (jAxisValue != FIND_KEY_VALUE_FAILED) {
        //         if( num == BLINKER_OBJECT_NOT_AVAIL ) {
        //             if ( _jCount < BLINKER_MAX_WIDGET_SIZE ) {
        //                 _Joystick[_jCount] = new BlinkerJoystick();
        //                 _Joystick[_jCount]->name(_jName);
        //                 _Joystick[_jCount]->freshValue(STRING_find_array_numberic_value(static_cast<Proto*>(this)->dataParse(), _jName, J_Xaxis), J_Xaxis);
        //                 _Joystick[_jCount]->freshValue(STRING_find_array_numberic_value(static_cast<Proto*>(this)->dataParse(), _jName, J_Yaxis), J_Yaxis);
        //                 _jCount++;
        //             }
        //         }
        //         else {
        //             _Joystick[num]->freshValue(STRING_find_array_numberic_value(static_cast<Proto*>(this)->dataParse(), _jName, J_Xaxis), J_Xaxis);
        //             _Joystick[num]->freshValue(STRING_find_array_numberic_value(static_cast<Proto*>(this)->dataParse(), _jName, J_Yaxis), J_Yaxis);
        //         }

        //         // static_cast<Proto*>(this)->isParsed();
        //         _fresh = true;
        //         // BLINKER_LOG1("slider isParsed");
        //         return jAxisValue;
        //     }
        //     else {
        //         if( num == BLINKER_OBJECT_NOT_AVAIL ) {
        //             if ( _jCount < BLINKER_MAX_WIDGET_SIZE ) {
        //                 _Joystick[_jCount] = new BlinkerJoystick();
        //                 _Joystick[_jCount]->name(_jName);
        //                 _jCount++;
        //             }
        //             return BLIKER_JOYSTICK_VALUE_DEFAULT;
        //         }

        //         return _Joystick[num]->getValue(axis);
        //     }
        // }

        uint8_t joystick(JoyStickAxis axis)
        {
            // BLINKER_LOG2("bname: ", _jName);
            // int8_t num = checkNum(_jName, _Joystick, _jCount);
            int16_t jAxisValue = STRING_find_array_numberic_value(static_cast<Proto*>(this)->dataParse(), BLINKER_CMD_JOYSTICK, axis);

            if (jAxisValue != FIND_KEY_VALUE_FAILED) {
                joyValue[J_Xaxis] = STRING_find_array_numberic_value(static_cast<Proto*>(this)->dataParse(), BLINKER_CMD_JOYSTICK, J_Xaxis);
                joyValue[J_Yaxis] = STRING_find_array_numberic_value(static_cast<Proto*>(this)->dataParse(), BLINKER_CMD_JOYSTICK, J_Yaxis);

                // static_cast<Proto*>(this)->isParsed();
                _fresh = true;
                // BLINKER_LOG1("slider isParsed");
                return jAxisValue;
            }
            else {
                return joyValue[axis];
            }
        }

        // float gyro(GyroAxis axis)
        // {
        //     float gAxisValue = STRING_find_array_float_value(static_cast<Proto*>(this)->dataParse(), BLINKER_CMD_GYRO, axis);

        //     if (gAxisValue != (float)FIND_KEY_VALUE_FAILED) {
        //         gyroValue[G_Xaxis] = STRING_find_array_float_value(static_cast<Proto*>(this)->dataParse(), BLINKER_CMD_GYRO, G_Xaxis);
        //         gyroValue[G_Yaxis] = STRING_find_array_float_value(static_cast<Proto*>(this)->dataParse(), BLINKER_CMD_GYRO, G_Yaxis);
        //         gyroValue[G_Zaxis] = STRING_find_array_float_value(static_cast<Proto*>(this)->dataParse(), BLINKER_CMD_GYRO, G_Zaxis);

        //         // static_cast<Proto*>(this)->isParsed();
        //         _fresh = true;
        //         // BLINKER_LOG1("slider isParsed");
        //         return gAxisValue;
        //     }
        //     else {
        //         return gyroValue[axis];
        //     }
        // }

        int16_t ahrs(AhrsAttitude attitude)
        {
            int16_t aAttiValue = STRING_find_array_numberic_value(static_cast<Proto*>(this)->dataParse(), BLINKER_CMD_AHRS, attitude);

            if (aAttiValue != FIND_KEY_VALUE_FAILED) {
                ahrsValue[Yaw] = STRING_find_array_numberic_value(static_cast<Proto*>(this)->dataParse(), BLINKER_CMD_AHRS, Yaw);
                ahrsValue[Roll] = STRING_find_array_numberic_value(static_cast<Proto*>(this)->dataParse(), BLINKER_CMD_AHRS, Roll);
                ahrsValue[Pitch] = STRING_find_array_numberic_value(static_cast<Proto*>(this)->dataParse(), BLINKER_CMD_AHRS, Pitch);

                _fresh = true;

                return aAttiValue;
            }
            else {
                return ahrsValue[attitude];
            }
        }
        
        void attachAhrs()
        {
            bool state = false;
            uint32_t startTime = millis();
            static_cast<Proto*>(this)->print(BLINKER_CMD_AHRS, BLINKER_CMD_ON);
            while (!state) {
                while (!static_cast<Proto*>(this)->connected()) {
                    static_cast<Proto*>(this)->run();
                    if (static_cast<Proto*>(this)->connect()) {
                        static_cast<Proto*>(this)->print(BLINKER_CMD_AHRS, BLINKER_CMD_ON);
                        break;
                    }
                }
                
                ::delay(100);

                if (static_cast<Proto*>(this)->checkAvail()) {
                    BLINKER_LOG2("GET: ", static_cast<Proto*>(this)->dataParse());
                    if (STRING_contais_string(static_cast<Proto*>(this)->dataParse(), BLINKER_CMD_AHRS)) {
                        BLINKER_LOG1("AHRS attach sucessed...");
                        parse();
                        state = true;
                        break;
                    }
                    else {
                        if (millis() - startTime > BLINKER_CONNECT_TIMEOUT_MS) {
                            BLINKER_LOG1("AHRS attach failed...Try again");
                            startTime = millis();
                            static_cast<Proto*>(this)->print(BLINKER_CMD_AHRS, BLINKER_CMD_ON);
                        }
                    }
                }
            }
        }

        void detachAhrs()
        {
            static_cast<Proto*>(this)->print(BLINKER_CMD_AHRS, BLINKER_CMD_OFF);
            ahrsValue[Yaw] = 0;
            ahrsValue[Roll] = 0;
            ahrsValue[Pitch] = 0;
        }

        void vibrate(uint16_t ms = 500)
        {
            if (ms > 1000) {
                ms = 1000;
            }

            static_cast<Proto*>(this)->print(BLINKER_CMD_VIBRATE, ms);
        }

        void delay(unsigned long ms)
        {
            uint32_t start = micros();
            while (ms > 0) {
                static_cast<Proto*>(this)->run();

                if ((micros() - start) >= 1000) {
                    ms--;
                    start = micros();
                }
            }
        }
    
    private :
        uint8_t _bCount = 0;
        uint8_t _sCount = 0;
        // uint8_t _jCount = 0;
        // float   gyroValue[3];
        uint8_t joyValue[2];
        int16_t ahrsValue[3];
        bool    _fresh = false;
};

#endif