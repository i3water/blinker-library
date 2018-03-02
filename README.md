#ESP8266(Arduino兼容)接入到Blinker中  
###学习内容
- 如何通过Blinker接入WiFi设备如ESP8266  
- 如何通过Blinker向WiFi设备发送数据  
- 如果通过Blinker接收WiFi设备发送的数据  

###所需条件
- ESP8266开发板  
- USB数据线  
还需要为开发环境做好以下准备:
- 一个有效的Blinker账号  
- 运行Windows或Ubuntu的Mac或PC  
- ESP8266要连接到的无线网络  
- 建立Internet连接, 以便下载配置工具  
- Arduino IDE 1.6.8版或更高版本  

##ESP8266设备上线  
####从Github/论坛获取示例
下载示例程序后:  
1.将ESP8266通过USB连接到电脑。
2.打开储存示例的文件夹。  
3.在Arduino IDE中打开Blinker_ESP8266_wifi_example.ino文件。  
![](0.png)  

4.在Arduino IDE中,单击"File"(文件)->"Preference"(首选项)。  
5.在"Preference"(首选项)对话框中,单击"附加开发板管理器网址"框旁边的图标。  
![](1.png)  

6.在弹出窗口中输入以下 URL，并单击"OK"(好)。  
```
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```
![](2.png)  

7.在"Preference"(首选项)对话框中,单击"OK"(好)。  
8.单击"Tools"(工具)->"Board"(开发板)->"Boards Manager"(开发板管理器),并搜索ESP8266。选择最新版并单击安装。
![](3.png)  

9.单击"Tools"(工具)->"Board"(开发板)->"NodeMCU 1.0(ESP12E-Module)"。  
####配置开发板设置
1.单击"Tools"(工具)->"CPU Frequency"->"80 MHz"  
2.单击"Tools"(工具)->"Upload Speed"->"115200"  
3.单击"Tools"(工具)->"Port"(端口)->"COMx"  
>若不清楚端口号可在设备管理器的端口列表中查询。  
>未提及的配置信息按默认设置即可。  

####安装所需的库
1.在Arduino IDE中,单击"Sketch"(项目)->"Include Library"(加载库)->"Manage Libraries"(管理库)。  
2.搜索 **webSocket** ,找到并单击安装。  

![](4.png)  

####编辑示例
打开esp8266_wifi_example.ino文件,设置ESP8266要连接的无线网络名及密码并保持。
```
char ssid[] = "<Your WiFi network SSID or name>";
char pswd[] = "<Your WiFi network WPA password or WEP key>";
```
####编译下载并运行示例  
1.单击Arduino IDE中的 **上传** 按钮  
2.Arduino IDE显示上传成功后单击并打开 **串口监视器** ,串口监视器显示如下信息则代表ESP8266成功连接无线网络。  
![](5.png)  
  
##Blinker添加设备  
1.打开并登陆Blinker  
2.单击“我的设备”进入“控制中心”页面  
3.单击右上角的 “+” 进入“添加设备”页面  
![](6.png)  

4.单击“Arduino”图标进入“Arduino接入向导”页面  
![](7.png)  

5.单击“WiFi接入”等待搜索设备完成  
![](8.png)  

6.搜索设备完成后App页面会显示发现的设备列表  
![](9.png)  

7.在列表中单击与串口监视器中显示的设备名相同的设备进行设备注册  
8.设备添加成功后控制中心会显示已添加成功的设备  
![](10.png)  

9.单击设备图标进入设备控制页面  
10.单击右上角?释放自定义控件
11.单击右下角 ＋ 选择第一个对话框控件,设备连接成功将在对话框中看到 **Connectd**  
![](11.png)  

12.单击右上角?固定自定义控件,对话框中输入 hello world 并点击 Send  

![](12.png)    

13.Arduino IDE的串口监视器中将显示收到的数据并发送系统运行总时间到App  
![](14.png)  

14.App的对话框中将显示ESP8266返回的时间数据  
![](13.png)





---
#通信格式
##基本通信格式
###websocket
<!-- ```
{
	"fromDevice":"fromDeviceName",
    "data": { }
}
``` -->
开关/按键
```
{"Button1":"Pressed"}  
*Button1为app中按键的名称, 默认app自动增加编号, 用户可以自己修改  
*Pressde按下, Released松开
```
滑动条
```
{"Slider1":123}  
*Slider1名称同Button  
*数值范围0-255
```
摇杆
```
{"Joystick1":[0,123]}  
*Joystick1名称同Button  
*数值范围0-255, 依次为X/Y 即左右/上下
```
陀螺仪
```
{"Gyro":[128.10,90.85,10.11]}  
*数值类型float, 依次为X/Y/Z 轴
```  
AHRS
```
{"ahrs":"on"}  
*on/off 开启关闭ahrs功能  
{"ahrs":[-120,32,85]}
*依次为yaw/pitch/roll
```
手机振动
```
{"Rumble":123}  
数值范围0-255, 表示震动强度或者时间
```

---
#第三方用户代码接口  
<!-- ###设备初始配置
1.用户初始调用iotDevice.begin(auth, ssid, pswd)设置设备注册获取的authkey及wifi接入ssid/pswd;  
>*iotDevice仅为暂定测试用对象名  
   -->
###设备通信接口
####局域网webSocket  
设备Server端  
```
host: 192.168.x.xxx 设备对应ip  
port: 81
```

---

#Blinker接口函数
##设备配置
###Blinker.begin()
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
##连接管理
###Blinker.connect()
建立 **Blinker** 设备间连接并返回连接状态, 默认超时时间为10秒
```
bool result = Blinker.connect();  
  

uint32_t timeout = 30000;//ms  
bool result = Blinker.connect(timeout);
```
###Blinker.disconnect()
断开 **Blinker** 设备间连接
```
Blinker.disconnect();
```
###Blinker.connected()
返回 **Blinker** 设备间连接状态
```
bool result = Blinker.connected();
```
###Blinker.run()
需要在频繁调用以保持设备间连接及处理收到的数据, 建议放在 **loop()** 函数中
```
void loop() {
    Blinker.run();
}
```
##数据管理
###Blinker.available()
检测是否有接收到数据
```
bool result = Blinker.available();
```
###Blinker.readString()
读取接收到的数据
```
String data = Blinker.readString();
```
###Blinker.print()
发送数据
```
Blinker.print(data);
```
发送一个Json数据, 如 {text1:data}
```
Blinker.print(text1, data);
```  
##App Widgets
###Blinker.button() 
读取开关/按键数据
```
bool result = Blinker.button("Button1");
```
###Blinker.slider()
读取滑动条数据
```
uint8_t result = Blinker.slider("Slider1");
```
###Blinker.joystick()
读取摇杆数据
```
uint8_t result_x = Blinker.joystick("Joystick1", J_Xaxis);
```
###Blinker.ahrs()
开启手机 **AHRS** 功能
```
Blinker.attachAhrs();
```
读取 **AHRS** 数据
```
int16_t result_y = Blinker.ahrs(Yaw);
```
关闭手机 **AHRS** 功能
```
Blinker.dettachAhrs();
```
###Blinker.vibrate()
发送手机振动指令, 震动时间, 单位ms 毫秒, 数值范围0-1000, 默认为500
```
Blinker.vibrate();
Blinker.vibrate(255);  
```
##设备延时
###Blinker.delay()
延时函数, 在延时过程中仍保持设备间连接及数据接收处理
```
Blinker.delay(500);
```
>*为了连接设备成功, 需要延时时务必使用该函数;  
>使用此函数可以在延时期间连接设备及接收数据并处理数据, 延时完成后才能执行后面的程序;  

  
