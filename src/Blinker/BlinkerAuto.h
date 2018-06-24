#if defined(BLINKER_MQTT)
#include "modules/ArduinoJson/ArduinoJson.h"
#include <Blinker/BlinkerConfig.h>
#include <utility/BlinkerDebug.h>
#include <utility/BlinkerUtility.h>

template <class API>
class BlinkerAUTO
{
    public :
        BlinkerAUTO() {
            _autoState = false;
        }

        void run(String key, float data, int32_t nowTime) {
#ifdef BLINKER_DEBUG_ALL
            BLINKER_LOG6("BlinkerAUTO run key: ", key, " data: ", data, " nowTime: ", nowTime);
#endif
            for (uint8_t _num = 0; _num < _targetNum; _num++) {
                if (!_autoState) {
                    return;
                }

                if (key != STRING_format(_targetKey[_num])) {
                    return;
                }

                if (_time1[_num] < _time2[_num]) {
                    if (!(nowTime >= _time1[_num] && nowTime <= _time2[_num])) {
#ifdef BLINKER_DEBUG_ALL
                        BLINKER_LOG2("out of time slot: ", nowTime);
#endif
                        return;
                    }
                }
                else if (_time1[_num] > _time2[_num]) {
                    if (nowTime > _time1[_num] && nowTime < _time2[_num]) {
#ifdef BLINKER_DEBUG_ALL
                        BLINKER_LOG2("out of time slot: ", nowTime);
#endif
                        return;
                    }
                }

                switch (_compareType[_num]) {
                    case BLINKER_COMPARE_LESS:
                        if (data < _targetData[_num]) {
                            if (!isTrigged[_num]) {
                                triggerCheck("less");
                            }
                        }
                        else {
                            isTrigged[_num] = false;
                            isRecord[_num] = false;
                        }
                        break;
                    case BLINKER_COMPARE_EQUAL:
                        if (data = _targetData[_num]) {
                            if (!isTrigged[_num]) {
                                triggerCheck("equal");
                            }
                        }
                        else {
                            isTrigged[_num] = false;
                            isRecord[_num] = false;
                        }
                        break;
                    case BLINKER_COMPARE_GREATER:
                        if (data > _targetData[_num]) {
                            if (!isTrigged[_num]) {
                                triggerCheck("greater");
                            }
                        }
                        else {
                            isTrigged[_num] = false;
                            isRecord[_num] = false;
                        }
                        break;
                    default:
                        break;
                }
            }
        }

        void triggerCheck(String state) {
            if (!isRecord[_num]) {
                isRecord[_num] = true;
                _treTime[_num] = millis();
            }

            if ((millis() - _treTime[_num]) / 1000 >= _duration[_num]) {
#ifdef BLINKER_DEBUG_ALL
                BLINKER_LOG2(state, " trigged");
#endif
                // if (static_cast<Proto*>(this)->autoTrigged(_linkDevice[_num], _linkType[_num], _linkData[_num]))
                // {
                    // static_cast<API*>(this)->parse();
                if (static_cast<API*>(this)->api2proto(_linkDevice[_num], _linkType[_num], _linkData[_num])) {

                    isTrigged[_num] = true;
#ifdef BLINKER_DEBUG_ALL
                    BLINKER_LOG1("trigged sucessed");
#endif
                }
                else
                {
#ifdef BLINKER_DEBUG_ALL
                    BLINKER_LOG1("trigged failed");
#endif
                }
            }
        }

        void manager(String data) {
            DynamicJsonBuffer jsonBuffer;
            JsonObject& root = jsonBuffer.parseObject(data);

            // String auto_state = STRING_find_string(static_cast<Proto*>(this)->dataParse(), "auto\"", ",", 1);
            bool auto_state = root[BLINKER_CMD_SET][BLINKER_CMD_AUTO];
            // if (auto_state == "") {
            //     auto_state = STRING_find_string(static_cast<Proto*>(this)->dataParse(), "auto\"", "}", 1);
            // }
#ifdef BLINKER_DEBUG_ALL
            BLINKER_LOG2("auto state: ", auto_state);
#endif
            // _autoState = (auto_state == BLINKER_CMD_TRUE) ? true : false;
            _autoState = auto_state;

            // _autoId = STRING_find_numberic_value(static_cast<Proto*>(this)->dataParse(), BLINKER_CMD_AUTOID);
            _autoId = root[BLINKER_CMD_SET][BLINKER_CMD_AUTODATA][BLINKER_CMD_AUTOID];

            // String logicType;
            String logicType = root[BLINKER_CMD_SET][BLINKER_CMD_AUTODATA][BLINKER_CMD_LOGICTYPE];
            // if (STRING_find_string_value(static_cast<Proto*>(this)->dataParse(), logicType, BLINKER_CMD_LOGICTYPE)) {
#ifdef BLINKER_DEBUG_ALL
            BLINKER_LOG2("_autoId: ", _autoId);
            BLINKER_LOG2("logicType: ", logicType);
#endif
            // String target_key;
            String target_key = root[BLINKER_CMD_SET][BLINKER_CMD_AUTODATA][BLINKER_CMD_LOGICDATA][0][BLINKER_CMD_TARGETKEY];
            // STRING_find_string_value(static_cast<Proto*>(this)->dataParse(), target_key, BLINKER_CMD_TARGETKEY);
            strcpy(_targetKey[0], target_key.c_str());
            
            if (logicType == BLINKER_CMD_STATE) {
#ifdef BLINKER_DEBUG_ALL
                BLINKER_LOG1("state!");
#endif
                _targetNum = 1;

                _logicType = BLINKER_TYPE_STATE;

                logic_type[0] = BLINKER_TYPE_STATE;
                // String target_state;
                String target_state = root[BLINKER_CMD_SET][BLINKER_CMD_AUTODATA][BLINKER_CMD_LOGICDATA][0][BLINKER_CMD_TARGETSTATE];
                // if (STRING_find_string_value(static_cast<Proto*>(this)->dataParse(), target_state, BLINKER_CMD_TARGETSTATE)) {
                    if (target_state == BLINKER_CMD_ON) {
                        _targetState[0] = true;
                    }
                    else if (target_state == BLINKER_CMD_OFF) {
                        _targetState[0] = false;
                    }
#ifdef BLINKER_DEBUG_ALL
                    BLINKER_LOG2("_logicType: ", _logicType);
                    BLINKER_LOG2("_targetKey: ", _targetKey[0]);
                    BLINKER_LOG2("_targetState: ", _targetState[0]);
#endif
                // }
            }
            else if (logicType == BLINKER_CMD_NUMBERIC) {
#ifdef BLINKER_DEBUG_ALL
                BLINKER_LOG1("numberic!");
#endif
                _targetNum = 1;
                
                _logicType = BLINKER_TYPE_NUMERIC;

                logic_type[0] = BLINKER_TYPE_NUMERIC;
                // String _type;
                String _type = root[BLINKER_CMD_SET][BLINKER_CMD_AUTODATA][BLINKER_CMD_LOGICDATA][0][BLINKER_CMD_COMPARETYPE];
                // if (STRING_find_string_value(static_cast<Proto*>(this)->dataParse(), _type, BLINKER_CMD_COMPARETYPE)) {
                    if (_type == BLINKER_CMD_LESS) {
                        _compareType[0] = BLINKER_COMPARE_LESS;
                    }
                    else if (_type == BLINKER_CMD_EQUAL) {
                        _compareType[0] = BLINKER_COMPARE_EQUAL;
                    }
                    else if (_type == BLINKER_CMD_GREATER) {
                        _compareType[0] = BLINKER_COMPARE_GREATER;
                    }

                    // _targetData = STRING_find_float_value(static_cast<Proto*>(this)->dataParse(), BLINKER_CMD_TARGETDATA);
                    _targetData[0] = root[BLINKER_CMD_SET][BLINKER_CMD_AUTODATA][BLINKER_CMD_LOGICDATA][0][BLINKER_CMD_TARGETDATA];
#ifdef BLINKER_DEBUG_ALL
                    BLINKER_LOG2("_logicType: ", _logicType);
                    BLINKER_LOG4("_type: ", _type, " _compareType: ", _compareType[0]);
                    BLINKER_LOG4("_targetKey: ", _targetKey[0], " _targetData: ", _targetData[0]);
#endif
                // }
            }

            // int32_t duValue = STRING_find_numberic_value(static_cast<Proto*>(this)->dataParse(), BLINKER_CMD_DURATION);
            _duration[0] = root[BLINKER_CMD_SET][BLINKER_CMD_AUTODATA][BLINKER_CMD_LOGICDATA][0][BLINKER_CMD_DURATION];
            _duration[0] = 60 * _duration[0];

            // if (duValue != FIND_KEY_VALUE_FAILED) {
            //     _duration = 60 * STRING_find_numberic_value(static_cast<Proto*>(this)->dataParse(), BLINKER_CMD_DURATION);
            // }
            // else {
            //     _duration = 0;
            // }
#ifdef BLINKER_DEBUG_ALL
            BLINKER_LOG2("_duration: ", _duration[0]);
#endif
            // int32_t timeValue = STRING_find_array_numberic_value(static_cast<Proto*>(this)->dataParse(), BLINKER_CMD_TIMESLOT, 0);
            int32_t timeValue = root[BLINKER_CMD_SET][BLINKER_CMD_AUTODATA][BLINKER_CMD_TIMESLOT][0];

            if (timeValue) {
                // _time1 = 60 * STRING_find_array_numberic_value(static_cast<Proto*>(this)->dataParse(), BLINKER_CMD_TIMESLOT, 0);
                _time1[0] = root[BLINKER_CMD_SET][BLINKER_CMD_AUTODATA][BLINKER_CMD_TIMESLOT][0];
                _time1[0] = 60 * _time1[0];
                // _time2 = 60 * STRING_find_array_numberic_value(static_cast<Proto*>(this)->dataParse(), BLINKER_CMD_TIMESLOT, 1);
                _time2[0] = root[BLINKER_CMD_SET][BLINKER_CMD_AUTODATA][BLINKER_CMD_TIMESLOT][1];
                _time2[0] = 60 * _time2[0];
            }
            else {
                _time1[0] = 0;
                _time2[0] = 24 * 60 * 60;
            }
#ifdef BLINKER_DEBUG_ALL
            BLINKER_LOG4("_time1: ", _time1[0], " _time2: ", _time2[0]);
#endif
            // String datas;
            // datas = STRING_find_string(static_cast<Proto*>(this)->dataParse(), BLINKER_CMD_LINKDATA, "]", 3);

            // strcpy(_linkDevice[0], STRING_find_string(datas, BLINKER_CMD_LINKDEVICE, "\"", 3).c_str());
            // strcpy(_linkType[0], STRING_find_string(datas, BLINKER_CMD_LINKTYPE, "\"", 3).c_str());
            // strcpy(_linkData[0], STRING_find_string(datas, BLINKER_CMD_DATA, "}", 3).c_str());
            // _aCount++;

            String link_device = root[BLINKER_CMD_SET][BLINKER_CMD_AUTODATA][BLINKER_CMD_LINKDATA][0][BLINKER_CMD_LINKDEVICE];
            String link_type = root[BLINKER_CMD_SET][BLINKER_CMD_AUTODATA][BLINKER_CMD_LINKDATA][0][BLINKER_CMD_LINKTYPE];
            String link_data = root[BLINKER_CMD_SET][BLINKER_CMD_AUTODATA][BLINKER_CMD_LINKDATA][0][BLINKER_CMD_DATA];
            
            strcpy(_linkDevice[0], link_device.c_str());
            strcpy(_linkType[0], link_type.c_str());
            strcpy(_linkData[0], link_data.c_str());

            const char* link_device2 = root[BLINKER_CMD_SET][BLINKER_CMD_AUTODATA][BLINKER_CMD_LINKDATA][1][BLINKER_CMD_LINKDEVICE];

            if (link_device2) {
                _linkNum = 2;

                String link_device1 = root[BLINKER_CMD_SET][BLINKER_CMD_AUTODATA][BLINKER_CMD_LINKDATA][1][BLINKER_CMD_LINKDEVICE];
                String link_type1 = root[BLINKER_CMD_SET][BLINKER_CMD_AUTODATA][BLINKER_CMD_LINKDATA][1][BLINKER_CMD_LINKTYPE];
                String link_data1 = root[BLINKER_CMD_SET][BLINKER_CMD_AUTODATA][BLINKER_CMD_LINKDATA][1][BLINKER_CMD_DATA];
                
                strcpy(_linkDevice[1], link_device1.c_str());
                strcpy(_linkType[1], link_type1.c_str());
                strcpy(_linkData[1], link_data1.c_str());

#ifdef BLINKER_DEBUG_ALL
                BLINKER_LOG2("_linkNum: ", _linkNum);
                BLINKER_LOG2("_linkDevice1: ", _linkDevice[0]);
                BLINKER_LOG2("_linkType1: ", _linkType[0]);
                BLINKER_LOG2("_linkData1: ", _linkData[0]);
                BLINKER_LOG2("_linkDevice2: ", _linkDevice[1]);
                BLINKER_LOG2("_linkType2: ", _linkType[1]);
                BLINKER_LOG2("_linkData2: ", _linkData[1]);
                BLINKER_LOG2("_targetNum: ", _targetNum);
                // BLINKER_LOG2("_aCount: ", _aCount);
#endif
            }
            else {
                _linkNum = 1;

#ifdef BLINKER_DEBUG_ALL
                BLINKER_LOG2("_linkNum: ", _linkNum);
                BLINKER_LOG2("_linkDevice1: ", _linkDevice[0]);
                BLINKER_LOG2("_linkType1: ", _linkType[0]);
                BLINKER_LOG2("_linkData1: ", _linkData[0]);
                BLINKER_LOG2("_targetNum: ", _targetNum);
                // BLINKER_LOG2("_aCount: ", _aCount);
#endif
            }

            serialization();
        }

        void deserialization() {
            uint8_t checkData;
            EEPROM.begin(BLINKER_EEP_SIZE);
            EEPROM.get(BLINKER_EEP_ADDR_CHECK, checkData);

            if (checkData != BLINKER_CHECK_DATA) {
                _autoState = false;
                EEPROM.commit();
                EEPROM.end();
                return;
            }

//             EEPROM.get(BLINKER_EEP_ADDR_AUTONUM, _aCount);
// #ifdef BLINKER_DEBUG_ALL
//             BLINKER_LOG2("_aCount: ", _aCount);
// #endif
            // EEPROM.get(BLINKER_EEP_ADDR_AUTOID, _autoId);
            // EEPROM.get(BLINKER_EEP_ADDR_AUTO, _autoData);
            EEPROM.get(BLINKER_EEP_ADDR_AUTO_START + _num * BLINKER_ONE_AUTO_DATA_SIZE + BLINKER_EEP_ADDR_AUTOID, _autoId);
            EEPROM.get(BLINKER_EEP_ADDR_AUTO_START + _num * BLINKER_ONE_AUTO_DATA_SIZE + BLINKER_EEP_ADDR_TYPESTATE, _typeState);

            _logicType = _typeState >> 6;// | _autoState << 4 | _linkNum;
            _autoState = _typeState >> 4 & 0x03;
            _linkNum = _typeState & 0x0F;

#ifdef BLINKER_DEBUG_ALL
            BLINKER_LOG2("_typeState: ", _typeState);
#endif

            if (_logicType == BLINKER_TYPE_STATE || _logicType == BLINKER_TYPE_NUMERIC) {
                _targetNum = 1;
            }
#ifdef BLINKER_DEBUG_ALL
            BLINKER_LOG2("_targetNum: ", _targetNum);
#endif
            if (_targetNum == 1) {
                EEPROM.get(BLINKER_EEP_ADDR_AUTO_START + _num * BLINKER_ONE_AUTO_DATA_SIZE + BLINKER_EEP_ADDR_AUTO1, _autoData[0]);

                // _linkNum = _autoId >> 30;
                // _autoId = _autoId;// & 0xFFFFFFFF;
                // _autoState = _autoData >> 31;
                logic_type[0] = _autoData[0] >> 30 & 0x03;
#ifdef BLINKER_DEBUG_ALL
                BLINKER_LOG1("===============================================");
                BLINKER_LOG2("_autoId: ", _autoId);
                // BLINKER_LOG2("_autoState: ", _autoState ? "true" : "false");
                BLINKER_LOG2("_logicType: ", _logicType ? "numberic" : "state");
#endif
                if (_logicType == BLINKER_TYPE_STATE) {
                    _targetState[0] = _autoData[0] >> 28 & 0x03;
#ifdef BLINKER_DEBUG_ALL
                    BLINKER_LOG2("_targetState: ", _targetState[0] ? "on" : "off");
#endif
                }
                else {
                    _compareType[0] = _autoData[0] >> 28 & 0x03;
                    // EEPROM.get(BLINKER_EEP_ADDR_TARGETDATA, _targetData);
                    EEPROM.get(BLINKER_EEP_ADDR_AUTO_START + _num * BLINKER_ONE_AUTO_DATA_SIZE + BLINKER_EEP_ADDR_TARGETKEY1, _targetKey[0]);
                    EEPROM.get(BLINKER_EEP_ADDR_AUTO_START + _num * BLINKER_ONE_AUTO_DATA_SIZE + BLINKER_EEP_ADDR_TARGETDATA1, _targetData[0]);
#ifdef BLINKER_DEBUG_ALL
                    BLINKER_LOG2("_compareType: ", _compareType[0] ? (_compareType[0] == BLINKER_COMPARE_GREATER ? "greater" : "equal") : "less");
                    BLINKER_LOG2("_targetKey: ", _targetKey[0]);
                    BLINKER_LOG2("_targetData: ", _targetData[0]);
#endif
                }

                _duration[0] = (_autoData[0] >> 22 & 0x3f) * 60;
                _time1[0] = (_autoData[0] >> 11 & 0x7ff) * 60;
                _time2[0] = (_autoData[0] & 0x7ff) * 60;

                // EEPROM.get(BLINKER_EEP_ADDR_LINKDEVICE1, _linkDevice[0]);
                // EEPROM.get(BLINKER_EEP_ADDR_LINKTYPE1, _linkType[0]);
                // EEPROM.get(BLINKER_EEP_ADDR_LINKDATA1, _linkData[0]);
                EEPROM.get(BLINKER_EEP_ADDR_AUTO_START + _num * BLINKER_ONE_AUTO_DATA_SIZE + BLINKER_EEP_ADDR_LINKDEVICE1, _linkDevice[0]);
                EEPROM.get(BLINKER_EEP_ADDR_AUTO_START + _num * BLINKER_ONE_AUTO_DATA_SIZE + BLINKER_EEP_ADDR_LINKTYPE1, _linkType[0]);
                EEPROM.get(BLINKER_EEP_ADDR_AUTO_START + _num * BLINKER_ONE_AUTO_DATA_SIZE + BLINKER_EEP_ADDR_LINKDATA1, _linkData[0]);

#ifdef BLINKER_DEBUG_ALL
                BLINKER_LOG2("_duration: ", _duration[0]);
                BLINKER_LOG4("_time1: ", _time1[0], " _time2: ", _time2[0]);
                BLINKER_LOG2("_linkNum: ", _linkNum);
                BLINKER_LOG2("_linkDevice: ", _linkDevice[0]);
                BLINKER_LOG2("_linkType: ", _linkType[0]);
                BLINKER_LOG2("_linkData: ", _linkData[0]);
                BLINKER_LOG2("_typeState: ", _typeState);

                BLINKER_LOG1("===============================================");
#endif
                EEPROM.commit();
                EEPROM.end();
            }
        }

        void serialization() {
            uint8_t checkData;

            // _autoData = _autoState << 31 | _logicType << 30 ;
            _typeState = _logicType << 6 | _autoState << 4 | _linkNum;

            if (_targetNum == 1) {
                _autoData[0] = logic_type[0] << 30 ;
                if (_logicType == BLINKER_TYPE_STATE) {
                    _autoData[0] |= _targetState[0] << 28;
                }
                else {
                    _autoData[0] |= _compareType[0] << 28;
                }
                _autoData[0] |= _duration[0] / 60 << 22 | _time1[0] / 60 << 11 | _time2[0] / 60;

                EEPROM.begin(BLINKER_EEP_SIZE);

                EEPROM.get(BLINKER_EEP_ADDR_CHECK, checkData);

                if (checkData != BLINKER_CHECK_DATA) {
                    EEPROM.put(BLINKER_EEP_ADDR_CHECK, BLINKER_CHECK_DATA);
                }

                // EEPROM.put(BLINKER_EEP_ADDR_AUTONUM, _aCount);

                // _autoId = _linkNum << 30 | _autoId;
                // EEPROM.put(BLINKER_EEP_ADDR_AUTO_START + _num * BLINKER_ONE_AUTO_DATA_SIZE, _autoId);
                // EEPROM.put(BLINKER_EEP_ADDR_AUTO, _autoData);
                EEPROM.put(BLINKER_EEP_ADDR_AUTO_START + _num * BLINKER_ONE_AUTO_DATA_SIZE + BLINKER_EEP_ADDR_AUTOID, _autoId);
                EEPROM.put(BLINKER_EEP_ADDR_AUTO_START + _num * BLINKER_ONE_AUTO_DATA_SIZE + BLINKER_EEP_ADDR_TYPESTATE, _typeState);
                EEPROM.put(BLINKER_EEP_ADDR_AUTO_START + _num * BLINKER_ONE_AUTO_DATA_SIZE + BLINKER_EEP_ADDR_AUTO1, _autoData[0]);
                EEPROM.put(BLINKER_EEP_ADDR_AUTO_START + _num * BLINKER_ONE_AUTO_DATA_SIZE + BLINKER_EEP_ADDR_TARGETKEY1, _targetKey[0]);
                    
                if (_logicType == BLINKER_TYPE_NUMERIC) {
                    // EEPROM.put(BLINKER_EEP_ADDR_TARGETDATA, _targetData);
                    EEPROM.put(BLINKER_EEP_ADDR_AUTO_START + _num * BLINKER_ONE_AUTO_DATA_SIZE + BLINKER_EEP_ADDR_TARGETDATA1, _targetData[0]);
                }

                // EEPROM.put(BLINKER_EEP_ADDR_LINKDEVICE1, _linkDevice[0]);
                // EEPROM.put(BLINKER_EEP_ADDR_LINKTYPE1, _linkType[0]);
                // EEPROM.put(BLINKER_EEP_ADDR_LINKDATA1, _linkData[0]);
                EEPROM.put(BLINKER_EEP_ADDR_AUTO_START + _num * BLINKER_ONE_AUTO_DATA_SIZE + BLINKER_EEP_ADDR_LINKDEVICE1, _linkDevice[0]);
                EEPROM.put(BLINKER_EEP_ADDR_AUTO_START + _num * BLINKER_ONE_AUTO_DATA_SIZE + BLINKER_EEP_ADDR_LINKTYPE1, _linkType[0]);
                EEPROM.put(BLINKER_EEP_ADDR_AUTO_START + _num * BLINKER_ONE_AUTO_DATA_SIZE + BLINKER_EEP_ADDR_LINKDATA1, _linkData[0]);

                EEPROM.commit();
                EEPROM.end();
#ifdef BLINKER_DEBUG_ALL
                BLINKER_LOG2("serialization _typeState: ", _typeState);
                BLINKER_LOG2("serialization _autoData: ", _autoData[0]);
#endif
            }
        }

        void setNum(uint8_t num) {
            _num = num;
        }

        uint32_t id() { return _autoId; }

        uint8_t type() { return _logicType; }

    private :
        uint8_t     _num;
        // - - - - - - - -  - - - - - - - -  - - - - - - - -  - - - - - - - -
        // | | | | |            | _time1 0-1440min 11  | _time2 0-1440min 11                   
        // | | | | | _duration 0-60min 6
        // | | | | _targetState|_compareType on/off|less/equal/greater 2
        // | | | _targetState|_compareType on/off|less/equal/greater
        // |  
        // | logic_type state/numberic 2
        // autoData

        // - - - - - - - -
        // |   |   |
        // |   |   | _linkNum
        // |   | _autoState true/false 1  
        // | _logicType state/numberic/and/or 2
        // typestate
        bool        _autoState;
        uint32_t    _autoId = 0;
        uint8_t     _logicType;
        uint8_t     _typeState;
        uint32_t    _autoData[2];

        uint8_t     _targetNum = 0;

        uint8_t     logic_type[2];
        char        _targetKey[2][12];
        float       _targetData[2];
        uint8_t     _compareType[2];
        bool        _targetState[2];
        uint32_t    _time1[2];
        uint32_t    _time2[2];
        uint32_t    _duration[2];
        uint32_t    _treTime[2];
        bool        isRecord[2];
        bool        isTrigged[2];
        uint8_t     _linkNum;
        char        _linkDevice[2][BLINKER_LINKDEVICE_SIZE];
        char        _linkType[2][BLINKER_LINKTYPE_SIZE];
        char        _linkData[2][BLINKER_LINKDATA_SIZE];
};
#endif