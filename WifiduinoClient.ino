// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       WifiduinoClient.ino
    Created:	2018/9/12 13:55:56
    Author:     HOME\Rainbow
*/

// Define User Types below here or use a .h file
//

// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//

//#include <WiFiUdp.h>
//#include <WiFiServerSecure.h>
//#include <WiFiServer.h>
//#include <WiFiClientSecure.h>
//#include <WiFiClient.h>
//#include <ESP8266WiFiType.h>
//#include <ESP8266WiFiSTA.h>
//#include <ESP8266WiFiScan.h>
//#include <ESP8266WiFiMulti.h>
//#include <ESP8266WiFiGeneric.h>
//#include <ESP8266WiFiAP.h>

#include <ESP8266WiFi.h>

#define DEBUG;
//#define DEBUG_btn;
//#define DEBUG_Serialcmd;
#define DEBUG_tcp

//wifi客户端的全局对象
WiFiClient client;
String ssid_global = "";
String pwd_global = "";
String serverip_global = "";
String port_global = "";

//检查wifi和服务器连接的相关参数
long lastCheckTime = 0;
const int checkInterval = 1000;
long lastSendDataToServerTime = 0;
const int sendInterval = 1000;
int onlineMode = 1;  //设置在线或离线模式

//计时器变量
long beginTime = 0;
long stopTime = 0;


//WiFi和tcp server的相关变量
int wifistatus = 0;  //0等待连接，1连接中，2连接成功,-1连接失败
int serverstatus = 0; //0等待连接，1连接中，2连接成功,-1连接失败



//wifiduino针脚设定
//const int buttonPin = 14;     // the number of the pushbutton pin
//const int ledPin = 5;      // the number of the LED pin
//const int buzzerPin = 4;

//wemos d1针脚设定
const int buttonPin = 13;     // the number of the pushbutton pin
const int ledPin = 14;      // the number of the LED pin
const int buzzerPin = 4;


//读取按钮状态功能的相关变量
int buttonState = 0;         
int lastButtonState = LOW;
long lastDebounceTime = 0;
long debounceDelay = 50;

//命令来源的枚举变量
enum cmdSource
{
	FromScreen,FromButton,FromTCP
};

int timerStatus = 0; //计时器的状态，0未启动，10手动准备好，11手动计时中，12手动计时停止，20自动准备好，21自动计时中，22自动计时停止



void setup()
{

	Serial.begin(9600);    
	pinMode(buttonPin, INPUT);
	pinMode(ledPin, OUTPUT);
	pinMode(buzzerPin, OUTPUT);
	SendCmdToScreen("rest");
	
	//delay(10);
	//Serial.println();
	//Serial.println();
	//Serial.print("Connecting to ");//写几句提示，哈哈
	//Serial.println(ssid);
	//SendCmdToScreen("wifistatus.val=0");
	//SendCmdToScreen("serverstatus.val=0");
	//ConnectServer("MyDell", "ddll1314", "", "");
	//if (wifistatus == 2)
	//{
	//	Serial.println("");
	//	Serial.println("WiFi connected");
	//	Serial.println("IP address: ");
	//	Serial.println(WiFi.localIP());//WiFi.localIP()返回8266获得的ip地址
	//}
	//else if (wifistatus == -1)
	//{
	//	Serial.println("");
	//	Serial.println("WiFi connected FAIL!");
	//}

	//WiFi.begin(ssid, password);

	// //控制屏幕中变量，达到显示wifi状态的目的
	//while (WiFi.status() != WL_CONNECTED)//WiFi.status() ，这个函数是wifi连接状态，返回wifi链接状态
	//									 //这里就不一一赘述它返回的数据了，有兴趣的到ESP8266WiFi.cpp中查看
	//{
	//	delay(500);
	//
	//	SendCmdToScreen("wifistatus.val=1");

	//}
	////连接成功
	//SendCmdToScreen("wifistatus.val=2");
	//


	//Serial.println("");
	//Serial.println("WiFi connected");
	//Serial.println("IP address: ");
	//Serial.println(WiFi.localIP());//WiFi.localIP()返回8266获得的ip地址
}
void(*resetFunc) (void) = 0;
void loop()
{
	
	//处理屏幕传过来的命令
	String cmdStringFromSerial = GetCmdFromSerial();
	if (cmdStringFromSerial != "")
	{
#ifdef DEBUG
		Serial.println(cmdStringFromSerial);
#endif // DEBUG
		String cmd = "";
		String firstpara = "";
		String secondpara = "";
		String thirdpara = "";
		String fourthpara = "";

		split(cmdStringFromSerial, &cmd, &firstpara, &secondpara, &thirdpara, &fourthpara);


#ifdef DEBUG_Serialcmd
		Serial.print("cmd:");
		Serial.println(cmd);

		Serial.print("firstpara:");
		Serial.println(firstpara);
		Serial.print("secondpara:");
		Serial.println(secondpara);
		Serial.print("thirdpara:");
		Serial.println(thirdpara);
		Serial.print("fourthpara:");
		Serial.println(fourthpara);
#endif // DEBUG_Serialcmd
		if (cmd=="set")  //比较字符串，strcmp在字符串相等时返回0
		{
			if (firstpara=="sys1")  //比较字符串，strcmp在字符串相等时返回0
			{  
				timerStatus = atoi(secondpara.c_str());  //转换字符串为数字
			}
			if (firstpara == "onlinemode")  //比较字符串，strcmp在字符串相等时返回0
			{
				onlineMode = atoi(secondpara.c_str());  //转换字符串为数字
			}
		}
		if (cmd =="connectwifi")
		{
			ssid_global = firstpara;
			pwd_global = secondpara;
			serverip_global = thirdpara;
			port_global = fourthpara;
			ConnectServer(firstpara, secondpara, thirdpara, fourthpara);
		}
		if (cmd == "timerstart")
		{
			StartTimer(FromScreen,firstpara);
		}
		if (cmd == "timerstop")
		{
			StopTimer(FromScreen, firstpara);
		}
		if (cmd == "timerreset")
		{
			ResetTimer(FromScreen,firstpara);			
		}
		if (cmd == "timerready")
		{
			ReadyTimer(FromScreen);
		}
	}
	//处理按钮事件
	if (ButtonPressed(buttonPin))
	{
#ifdef DEBUG
		Serial.println("button press");
		Serial.print("timerStatus:");
		Serial.println(timerStatus);
#endif // DEBUG
		
		if (timerStatus == 10) //10为手动准备好
		{		
			StartTimer(FromButton, "manual");
		}
		else if (timerStatus == 0)
		{
			ReadyTimer(FromButton);
		}
		else if (timerStatus == 11) //11为手动计时中,21为自动计时中，都需要用按钮停止
		{
			StopTimer(FromButton,"manual");
		}
		else if ( timerStatus == 21) //11为手动计时中,21为自动计时中，都需要用按钮停止
		{
			StopTimer(FromButton,"auto");
		}
	}

	//处理服务器发来的命令
	
	String cmdStringFromTcp = GetCmdFromTcp();


	if (cmdStringFromTcp != "")
	{
#ifdef DEBUG_tcp		
		Serial.println(cmdStringFromTcp);
#endif // DEBUG_tcp

		String cmd = "";
		String firstpara = "";
		String secondpara = "";
		String thirdpara = "";
		String fourthpara = "";
		split(cmdStringFromTcp, &cmd, &firstpara, &secondpara, &thirdpara, &fourthpara);
#ifdef DEBUG_tcp		
		Serial.print("cmd:");
		Serial.println(cmd);

#endif // DEBUG_tcp
		if (cmd== "timerstart")  //比较字符串
		{
			Serial.println("timerstart");
			StartTimer(FromTCP,"auto");
		}
		if (cmd== "timerstop")
		{
			StopTimer(FromTCP, "auto");
		}
		if (cmd == "timerreset")
		{
			ResetTimer(FromTCP, "auto");
		}
		if (cmd == "devicereset")
		{
			resetFunc();
		}

	}

	//定时检查目前的wifi和server状态
	if (onlineMode)  //仅在线模式才检查
	{
		if ((millis() - lastCheckTime) > checkInterval)  //定时执行检查任务
		{

			if (WiFi.status() != WL_CONNECTED)		//如果查询wifi没有连接，执行连接wifi的函数
			{
				if (wifistatus != 1)  //如果wifi没有在连接中，执行以下函数
				{
					SendCmdToScreen("sys0=0");   //设置屏幕中的wifi状态
					wifistatus = 0;				//设置单片机内的wifi状态
					if (onlineMode&&ssid_global != ""&&serverip_global != ""&&port_global != "")  //在线模式下并且已经记忆了wifi参数，再进行连接；参数为空，说明屏幕还未给单片机发送过连接指令
					{
						ConnectServer(ssid_global, pwd_global, serverip_global, port_global);
					}
				}
				
			}
			else              //如果查询wifi已连接，做一下判断
			{
				if (wifistatus != 2)  //如果单片机内的wifi状态不是已连接，说明之前正在连接中，现在已经连接好了，所以设置单片机的wifi状态为已连接，设置屏幕内的wifi状态为已连接
				{
					wifistatus = 2;
					SendCmdToScreen("sys0=2");
#ifdef DEBUG
					Serial.print("connect success!");
#endif // DEBUG	
				}
				// 没有else语句，因为如果查询wifi状态和单片机wifi状态都是已连接，说明工作正常，不需要采取措施
			}

			if (!client.connected())     //查询tcp client的连接状态，如果没有连接，执行以下语句
			{
				SendCmdToScreen("sys2=0");  //设置屏幕内tcp状态为未连接
				serverstatus = 0;		//设置单片机内tcp状态为未连接						
				if (onlineMode&&ssid_global != ""&&serverip_global != ""&&port_global != "")
				{   //在线模式下并且已经记忆了wifi参数，再进行连接；参数为空，说明屏幕还未给单片机发送过连接指令
					ConnectServer(ssid_global, pwd_global, serverip_global, port_global);
				}
			}
			else   //查询tcp client的连接状态，如果已经连接，执行以下语句
			{
				if (serverstatus != 2)     //如果单片机内的tcp状态不是已连接，说明之前正在连接中，现在已经连接好了，所以设置单片机的tcp状态为已连接，设置屏幕内的wifi状态为已连接，将主板ID发送给服务器
				{
					serverstatus = 2;
					SendCmdToScreen("sys2=2");
					String s = "chipid,"; //连接成功后，发送主板ID
					s.concat(ESP.getChipId());  //加入主板ID
					SendDataToTcp(s);
				}
			}
			lastCheckTime = millis();  //重置上一次检查时间
		}
	}

	
	//定时通知服务器，计时器的当前状态和时间

	if (onlineMode) //在线模式才发送数据给服务器
	{
		if ((millis() - lastSendDataToServerTime) > sendInterval)  //定时执行检查任务
		{
			if (client.connected())   //确认连接正常再发送数据
			{
				String data = "timerstatus,";
				data.concat(timerStatus);
				data.concat(",timespan,");
				data.concat(CalTimespan());
				SendDataToTcp(data);

			}
			lastSendDataToServerTime = millis();  //重置上一次发送数据时间
		}
		
	}
	/*



	while (client.available())//available()同ARDUINO，不解释了
	{
		
		char val = client.read();//read()同arduino
		if (val == 'a') {//pc端发送0和1来控制
			
			digitalWrite(ledPin, LOW);
			unsigned char data[] = { 0xC3 ,0xF0 ,0xB5 ,0xC6 ,0xC1 ,0xCB };
			ChangeValofScreenObject(data, "t0.txt");
		}
		if (val == 'b')
		{
		
			digitalWrite(ledPin, HIGH);
			unsigned char data[] = { 0xC1,0xC1, 0xB5,0xC6 };
	

			ChangeValofScreenObject(data, "t0.txt");
		}
	}
	*/
	
}

void StartTimer(int source, String page)
{
#ifdef DEBUG
	Serial.println("start timer");
#endif // DEBUG
	if (timerStatus == 10 ||timerStatus == 20)
	{
		if (source == FromButton)
		{

			timerStatus = 11;
			SendCmdToScreen("click btn_start,0");

		}
		else if (source == FromTCP&& timerStatus == 20)
		{

			timerStatus = 21;
			CountDownTone();
			SendCmdToScreen("click btn_start,0");
		}
		else if (source == FromScreen)
		{
			if (page == "manual")
			{
				timerStatus = 11;
			}

		}
		beginTime = millis();
		digitalWrite(ledPin, HIGH);
		
	}
	
}

void StopTimer(int source, String page)
{
#ifdef DEBUG
	Serial.println("stop timer");
#endif // DEBUG
	if (timerStatus == 11 || timerStatus == 21)
	{
		if (source == FromButton)
		{
			if (timerStatus==11)
			{
				timerStatus = 12;
			}
			else if (timerStatus ==21)
			{
				timerStatus = 22;
			}
			SendCmdToScreen("click btn_stop,0");
		}
		else if (source == FromTCP&& timerStatus == 21)
		{
			timerStatus = 22;
			SendCmdToScreen("click btn_stop,0");
		}
		else if (source == FromScreen)
		{
			if (page == "manual")
			{
				timerStatus = 12;
			}
			else if (page == "auto")
			{
				timerStatus = 22;
			}
		}
		stopTime = millis();
		digitalWrite(ledPin, LOW);
		
	}
	
}

void ResetTimer(int source, String page)
{
#ifdef DEBUG
	Serial.println("reset timer");
#endif // DEBUG
	if (timerStatus == 12 || timerStatus == 22 || timerStatus == 10 || timerStatus == 20 || timerStatus == 0)
	{
		if (source == FromButton)
		{
			timerStatus = 10;
			SendCmdToScreen("click btn_reset,0");
		}
		else if (source == FromTCP)
		{
			timerStatus = 0;
			SendCmdToScreen("click btn_reset,0");
		}
		else if (source == FromScreen)
		{
			if (page == "manual")
			{
				timerStatus = 10;
			}
			else if (page == "auto")
			{
				timerStatus = 0;
			}
		}

		digitalWrite(ledPin, LOW);
		beginTime = 0;
		stopTime = 0;
	}
}

void ReadyTimer(int source)
{
#ifdef DEBUG
	Serial.println("ready timer");
#endif // DEBUG
	if (timerStatus == 0)
	{
		if (source == FromButton)
		{
			timerStatus = 20;
			SendCmdToScreen("click btn_ready,0");
		}
		else if (source == FromTCP)
		{

		}
		else if (source == FromScreen)
		{
			timerStatus = 20;
		}

		digitalWrite(ledPin, LOW);
	}
}
long CalTimespan()
{
	long timespan = 0;
	switch (timerStatus)
	{
	case 11:
		timespan = millis() - beginTime;
		break;
	case 21:
		timespan = millis() - beginTime;
		break;
	case 12:
		timespan = stopTime - beginTime;
		break;
	case 22:
		timespan = stopTime - beginTime;
		break;
	default:
		timespan = 0;
		break;
	}
	return timespan;
}
void SendCmdToScreen(String s)
{
	unsigned char hexdata[3] = { 0xff,0xff,0xff };
	Serial.write(hexdata, 3);
	Serial.print(s);
	Serial.write(hexdata, 3);
	//   DLSerial.print(s);  
	//   DLSerial.write(hexdata, 3);
}


void SendDataToTcp(String s)
{
	
	if (client.connected())
	{
		//写入数据头标识
		client.write(0xf8);		client.write(0xf0);		client.write(0xf0);		client.write(0xf0);		client.write(0xf0);		client.write(0xf0);		client.write(0xf0);		client.write((byte)0x0);
		//计算字符串总长度
		unsigned int length = s.length() * 2;  //数据长度为字符串长度*2

		for (int i = 0; i < 8; i++)  //写入8位的长度数据
		{

			unsigned int a = length & 0xff;  //取最末尾的2位16进制数据
			client.write(a);
			length = length >> 8;//移位8位2进制，即2位16进制
		}

		//正式写入数据内容
		for (int i = 0; i < s.length(); i++)
		{
			client.write(s[i]);
			client.write((byte)0x0);
		}

	}
}




void ChangeValofScreenObject(String val, String obj)
{
	unsigned char hexdata[3] = { 0xff,0xff,0xff };	
	Serial.print(obj+"=");
	Serial.write(0x22);  //双引号
	Serial.print(val);
	Serial.write(0x22); //双引号
	Serial.write(hexdata, 3);
}
void ChangeValofScreenObject(unsigned char val[], String obj)
{
	unsigned char hexdata[3] = { 0xff,0xff,0xff };
	int size = strlen((const char*)val);
	Serial.print(obj + "=");
	Serial.write(0x22);  //双引号
	Serial.write(val, size);
	Serial.write(0x22); //双引号
	Serial.write(hexdata, 3);
}

String GetCmdFromSerial()
{
	String strOnSerial = "";
	while (Serial.available())
	{

		strOnSerial = Serial.readStringUntil('#');

	}
	return strOnSerial;
}

String GetCmdFromTcp()
{
	String strOnTcp = "";
	char c[8];
	while (client.available())
	{
		unsigned char hexdata[7] = { 0xf8,	0xf0,	0xf0,	0xf0,	0xf0,	0xf0,	0xf0 };

		//Serial.write(hexdata,8);

		if (client.find(hexdata, 7)) //找到信息的头部，8个字节，仅匹配前7个，加上最后一个0x0时，程序上总出错
		{
			client.read();//越过0x0
			int readsize = client.readBytes(c, 8);

			int length = 0;
			for (int i = 0; i < readsize; i++) // 读取下8个字节，转换为字符串长度
			{
				int n = (int)c[i];  //16进制转为10进制			
				length += n << (i * 8);  //16进制移位，1位16进制对应4位的2进制，2位16进制即移位8位							
			}

			char *content = NULL;

			content = (char*)malloc(sizeof(char) * length);  //动态创建字符串数组

			readsize = client.readBytes(content, length);
#ifdef DEBUG_tcp
			Serial.print("length:");
			Serial.println(length);
			Serial.print("readsize:");
			Serial.println(readsize);
			Serial.print("content:");
#endif // DEBUG_tcp

			for (int i = 0; i < readsize; i++)
			{
				Serial.print(content[i]);
				strOnTcp += content[i];
			}
#ifdef DEBUG_tcp
			Serial.println();
			Serial.print("string:");
			Serial.println(strOnTcp);
#endif // DEBUG_tcp



		}

	}

	return strOnTcp;
}

//void ConnectServer(String ssid, String pwd, String serverip, String port)   //连接wifi和tcp server，连接wifi成功后再连接tcp server
//{
//#ifdef DEBUG
//	Serial.print("WiFi.status():");
//	Serial.println(WiFi.status());
//	Serial.print("wifistatus.val");
//	Serial.println(wifistatus);
//#endif // DEBUG
//
//
//	long beginConnectTime = millis();
//
//	// 连接WIFI
//	if (WiFi.status() != WL_CONNECTED)
//	{
//		WiFi.begin(ssid.c_str(), pwd.c_str());
//#ifdef DEBUG
//		Serial.print("connect to:");
//		Serial.println(ssid);
//#endif // DEBUG
//
//		
//		
//		SendCmdToScreen("wifistatus.val=1");  //通知屏幕已经开始连接wifi
//		wifistatus = 1;
//		while ((millis() - beginConnectTime < 10000) && WiFi.status() != WL_CONNECTED)  //在10秒内并且没连接上，继续连接
//		{
//			delay(500);			
//		}
//		if (WiFi.status() == WL_CONNECTED)  //连接成功
//		{
//			wifistatus = 2;
//			SendCmdToScreen("wifistatus.val=2");
//#ifdef DEBUG
//			Serial.print("connect success!");
//#endif // DEBUG			
//			
//		}
//		else //wifi没连接上，说明超时退出循环
//		{
//			SendCmdToScreen("wifistatus.val=-1");
//			wifistatus = -1;
//#ifdef DEBUG
//			Serial.print("connect fail!");
//#endif // DEBUG				
//			
//		}
//		
//	}
//	else
//	{
//#ifdef DEBUG
//		Serial.println("WIFI already connected.");
//#endif // DEBUG		
//		wifistatus = 2;
//		SendCmdToScreen("wifistatus.val=2");
//	}
//
//	
//	//连接TCP SERVER
//	if (WiFi.status() == WL_CONNECTED)
//	{ 
//		if (!client.connected())
//		{
//			beginConnectTime = millis();  //开始计时
//			SendCmdToScreen("serverstatus.val=1");  //通知屏幕连接中
//			while ((millis() - beginConnectTime < 5000) && !client.connected())  //在5秒内并且没连接上，继续连接
//			{
//				client.connect(serverip, atoi(port.c_str()));
//				delay(500);
//			}
//			if (client.connected())
//			{
//#ifdef DEBUG
//				Serial.println("TCP connected success.");
//#endif // DEBUG							
//				serverstatus = 2;
//				SendCmdToScreen("serverstatus.val=2");
//			}
//			else
//			{
//#ifdef DEBUG
//				Serial.println("TCP connected fail.");
//#endif // DEBUG			
//				serverstatus = -1;
//				SendCmdToScreen("serverstatus.val=-1");
//			}
//			
//		}
//		else
//		{
//#ifdef DEBUG
//			Serial.println("TCP already connected.");
//#endif // DEBUG			
//			serverstatus = 2;
//			SendCmdToScreen("serverstatus.val=2");
//		}
//	}
//
//	
//}

//仅连接，不检查状态

void ConnectServer(String ssid, String pwd, String serverip, String port)   //连接wifi和tcp server，连接wifi成功后再连接tcp server
{
#ifdef DEBUG
	Serial.print("WiFi.status():");
	Serial.println(WiFi.status());
	Serial.print("wifistatus.val");
	Serial.println(wifistatus);
#endif // DEBUG


	long beginConnectTime = millis();

	// 连接WIFI
	//判断是离线模式还是在线模式
	if (onlineMode == 1)   //在线模式
	{
		if (WiFi.status() != WL_CONNECTED)
		{
			if (wifistatus!=1)
			{
				WiFi.begin(ssid.c_str(), pwd.c_str());				
				SendCmdToScreen("sys0=1");  //通知屏幕已经开始连接wifi
				wifistatus = 1;
#ifdef DEBUG
				Serial.print("connect to:");
				Serial.println(ssid.c_str());	
				Serial.print("PWD:");
				Serial.println(pwd.c_str());
#endif // DEBUG
			}


			
			//把判断连接成功的语句放到主循环里，不在连接wifi函数中检查
//			while (WiFi.status() != WL_CONNECTED)  //一直循环连接
//			{
//				delay(500);
//			}
//
//			wifistatus = 2;
//			SendCmdToScreen("sys0=2");
//#ifdef DEBUG
//			Serial.print("connect success!");
//#endif // DEBUG			



		}
		else
		{
#ifdef DEBUG
			Serial.println("WIFI already connected.");
#endif // DEBUG		
			wifistatus = 2;
			SendCmdToScreen("sys0=2");
		}


		//连接TCP SERVER
		if (WiFi.status() == WL_CONNECTED)
		{
			if (!client.connected())
			{
				if (serverstatus != 1)
				{
					SendCmdToScreen("sys2=1");  //通知屏幕连接中
					client.connect(serverip, atoi(port.c_str()));
				}
				
				//while (!client.connected())  //在5秒内并且没连接上，继续连接
				//{
				//
				//	delay(500);
				//}


				//serverstatus = 2;
				//SendCmdToScreen("sys2=2");

			}
			else
			{
#ifdef DEBUG
				Serial.println("TCP already connected.");
#endif // DEBUG			
				serverstatus = 2;
				SendCmdToScreen("sys2=2");
				String s = "chipid,"; //连接成功后，发送主板ID
				s.concat(ESP.getChipId());  //加入主板ID
				SendDataToTcp(s);
			}
		}

	}
	else  //离线模式
	{
#ifdef DEBUG
		Serial.println("offline mode.");
#endif // DEBUG		
		wifistatus = 0;
		SendCmdToScreen("sys0=0");
		serverstatus = 0;
		SendCmdToScreen("sys2=0");

	}


}

int ButtonPressed(int buttonPin)  //获取按钮的触发状态，未按下返回0，按下返回1
{
	int reading = digitalRead(buttonPin);


	if (reading != lastButtonState)
	{
		lastDebounceTime = millis();
	}
	if ((millis() - lastDebounceTime) > debounceDelay)
	{
		if (reading != buttonState)
		{
			buttonState = reading;
			if (buttonState == HIGH)
			{
				return 1;
			}
		}
	}
	lastButtonState = reading;
	return 0;
}
void split(String s, String *cmd, String *firstpara, String *secondpara, String *thirdpara, String *fourthpara)
{

	int index = -1;
	String s_take = "";
	String s_remain = "";
	//取第1个字符串

	index = s.indexOf(",");
	s_take = s.substring(0, index);
	s_remain = s.substring(index + 1, s.length());
	*cmd = s_take;

	//取第2个字符串
	s = s_remain;
	index = s.indexOf(",");
	s_take = s.substring(0, index);
	s_remain = s.substring(index + 1, s.length());
	*firstpara = s_take;
	//取第3个字符串
	s = s_remain;
	index = s.indexOf(",");
	s_take = s.substring(0, index);
	s_remain = s.substring(index + 1, s.length());
	*secondpara = s_take;
	//取第4个字符串
	s = s_remain;
	index = s.indexOf(",");
	s_take = s.substring(0, index);
	s_remain = s.substring(index + 1, s.length());
	*thirdpara = s_take;
	//取第4个字符串
	s = s_remain;
	index = s.indexOf(",");
	s_take = s.substring(0, index);
	s_remain = s.substring(index + 1, s.length());
	*fourthpara = s_take;
}


//蜂鸣器功能代码

//启动计时的设置
int length = 5; 
char notes[] = "CCCCC";
int beats[] = { 1,1, 1, 1, 2 };
int tempo = 500;

void CountDownTone()
{
	SendCmdToScreen("vis p0,1");
	for (int i = 0; i < length; i++) {
		digitalWrite(ledPin, HIGH);
		switch (i)
		{case 0:
			SendCmdToScreen("p0.pic=9");
			break;
		case 1:
			SendCmdToScreen("p0.pic=10");
			break;
		case 2:
			SendCmdToScreen("p0.pic=11");
			break;
		case 3:
			SendCmdToScreen("p0.pic=12");
			break;
		case 4:
			SendCmdToScreen("p0.pic=13");
			break;
		default:
			break;
		}
		
		if (notes[i] == ' ') {
			delay(beats[i] * tempo); // rest
		}
		else {
			playNote(notes[i], beats[i] * tempo);
		}
		digitalWrite(ledPin, LOW);
		// pause between notes
		delay(tempo);
	}
	SendCmdToScreen("vis p0,0");

}


void playTone(int tone, int duration) {
	for (long i = 0; i < duration * 1000L; i += tone * 2) {
		digitalWrite(buzzerPin, HIGH);
		delayMicroseconds(tone);
		digitalWrite(buzzerPin, LOW);
		delayMicroseconds(tone);
	}
}

void playNote(char note, int duration) {
	char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
	int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 135 };

	// play the tone corresponding to the note name
	for (int i = 0; i < 8; i++) {
		if (names[i] == note) {
			playTone(tones[i], duration);
		}
	}
}


void SendWifiStatusToScreen()
{

		int result = WiFi.status();		
		String cmd = "sys0=" ;
		cmd.concat(result);
		SendCmdToScreen(cmd);
		
}

void SendServerStatusToScreen()
{

	int result = client.connected();
	String cmd = "sys2=";
	cmd.concat(result);
	SendCmdToScreen(cmd);

}