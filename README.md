# 目前支持的硬件
* Arduino boards
* 使用[esp8266/arduino](https://github.com/esp8266/arduino)的ESP8266  
  
---  

# 准备工作
开始使用前你需要做好如下准备:
* [Arduino IDE 1.6.12](https://www.arduino.cc/en/Main/Software)
* 使用Arduino IDE的开发板管理器安装 [esp8266/arduino](https://github.com/esp8266/arduino)
* 使用Arduino IDE的库管理器安装 [WebSockets](https://github.com/Links2004/arduinoWebSockets)  
  
---

# Blinker接口函数
## 设备配置
### Blinker.begin()
使用 **Blinker.begin()** 来配置 Blinker:
```
Blinker.begin(...);
```
根据你使用的连接方式选择不同的参数用于配置Blinker  
  

蓝牙透传模块:
```
SerialBLE.begin(9600);  
Blinker.begin(SerialBLE);
```
网络模块:
```
Blinker.begin(ssid, pswd);
```
**begin()** 主要完成以下配置:  
1.初始化硬件设置;  
2.连接网络并广播设备信息等待app连接;
## 连接管理
### Blinker.connect()
建立 **Blinker** 设备间连接并返回连接状态, 默认超时时间为10秒
```
bool result = Blinker.connect();  
  

uint32_t timeout = 30000;//ms  
bool result = Blinker.connect(timeout);
```
### Blinker.disconnect()
断开 **Blinker** 设备间连接
```
Blinker.disconnect();
```
### Blinker.connected()
返回 **Blinker** 设备间连接状态
```
bool result = Blinker.connected();
```
### Blinker.run()
需要在频繁调用以保持设备间连接及处理收到的数据, 建议放在 **loop()** 函数中
```
void loop() {
    Blinker.run();
}
```
## 数据管理
### Blinker.available()
检测是否有接收到数据
```
bool result = Blinker.available();
```
### Blinker.readString()
读取接收到的数据
```
String data = Blinker.readString();
```
### Blinker.print()
发送数据
```
Blinker.print(data);
```
发送一个Json数据, 如 {text1:data}
```
Blinker.print(text1, data);
```  
## App Widgets
### Blinker.button() 
读取开关/按键数据
```
bool result = Blinker.button("Button1");
```
### Blinker.slider()
读取滑动条数据
```
uint8_t result = Blinker.slider("Slider1");
```
### Blinker.joystick()
读取摇杆数据
```
uint8_t result_X = Blinker.joystick("Joystick1", J_Xaxis);
uint8_t result_Y = Blinker.joystick("Joystick1", J_Yaxis);
```
### Blinker.ahrs()
开启手机 **AHRS** 功能
```
Blinker.attachAhrs();
```
读取 **AHRS** 数据
```
int16_t result_Yaw = Blinker.ahrs(Yaw);
int16_t result_Roll = Blinker.ahrs(Roll);
int16_t result_Pitch = Blinker.ahrs(Pitch);
```
关闭手机 **AHRS** 功能
```
Blinker.dettachAhrs();
```
### Blinker.vibrate()
发送手机振动指令, 震动时间, 单位ms 毫秒, 数值范围0-1000, 默认为500
```
Blinker.vibrate();
Blinker.vibrate(255);  
```
## 设备延时
### Blinker.delay()
延时函数, 在延时过程中仍保持设备间连接及数据接收处理
```
Blinker.delay(500);
```
>*为了连接设备成功, 需要延时时务必使用该函数;  
>使用此函数可以在延时期间连接设备及接收数据并处理数据, 延时完成后才能执行后面的程序;  

  
