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
#define DEBUG_Serialcmd;
#define DEBUG_tcp

WiFiClient client;
const int tcpPort = 12345;//修改为你建立的Server服务端的端口号
const int buttonPin = 4;     // the number of the pushbutton pin
const int ledPin = 5;      // the number of the LED pin

//WiFi和tcp server的相关变量
int wifistatus = 0;  //0等待连接，1连接中，2连接成功,-1连接失败
int serverstatus = 0; //0等待连接，1连接中，2连接成功,-1连接失败

//读取按钮状态功能的相关变量
int buttonState = 0;         
int lastButtonState = LOW;
long lastDebounceTime = 0;
long debounceDelay = 50;


int timerStatus = 0; //计时器的状态，0未启动，10手动准备好，11手动计时中，12手动计时停止，20自动准备好，21自动计时中，22自动计时停止
// The setup() function runs once each time the micro-controller starts
void setup()
{

	Serial.begin(9600);    
	pinMode(buttonPin, INPUT);
	pinMode(ledPin, OUTPUT);
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

// Add the main program code into the continuous loop() function
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
		int index = -1;


		//取第1个字符串
		String s = cmdStringFromSerial;
		index = s.indexOf(",");
		String s_take = s.substring(0, index);
		String s_remain = s.substring(index + 1, s.length());
		cmd = s_take;

		//取第2个字符串
		 s = s_remain;
		index = s.indexOf(",");
		 s_take = s.substring(0, index);
		 s_remain = s.substring(index + 1, s.length());
		 firstpara = s_take;
		 //取第3个字符串
		 s = s_remain;
		 index = s.indexOf(",");
		 s_take = s.substring(0, index);
		 s_remain = s.substring(index + 1, s.length());
		 secondpara = s_take;
		 //取第4个字符串
		 s = s_remain;
		 index = s.indexOf(",");
		 s_take = s.substring(0, index);
		 s_remain = s.substring(index + 1, s.length());
		 thirdpara = s_take;
		 //取第4个字符串
		 s = s_remain;
		 index = s.indexOf(",");
		 s_take = s.substring(0, index);
		 s_remain = s.substring(index + 1, s.length());
		 fourthpara = s_take;
		
	

		//char cmdchar[] = "";
		//
		//strcpy(cmdchar, cmdStringFromSerial.c_str());  //string转char[]
		//Serial.print("cmdchar:");
		//Serial.println(cmdchar);


		//char *StrTmpIn = NULL;  //临时保存strtok_r需要的参数
		//char *cmd= strtok_r(cmdchar, ",", &StrTmpIn);  //取第一个逗号前的字符串		
		//char *firstpara = strtok_r(NULL, ",", &StrTmpIn);
		//char *secondpara = strtok_r(NULL, ",", &StrTmpIn);
		//char *thirdpara = strtok_r(NULL, ",", &StrTmpIn);
		//char *fourthpara = strtok_r(NULL, ",", &StrTmpIn);
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
		}
		if (cmd =="connectwifi")
		{
			ConnectServer(firstpara, secondpara, thirdpara, fourthpara);
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
			StartTimer();
		}
		else if (timerStatus == 11) //11为手动计时中
		{
			StopTimer();
		}
	}

	//处理服务器发来的命令
	
	String cmdStringFromTcp = GetCmdFromTcp();


	if (cmdStringFromTcp != "")
	{
#ifdef DEBUG_tcp
		Serial.print("loop if:");
		Serial.println(cmdStringFromTcp);
#endif // DEBUG_tcp

		
		char cmdchar[] = "";
		strcpy(cmdchar, cmdStringFromTcp.c_str());  //string转char[]
		char *cmd = strtok(cmdchar, ",");  //取第一个逗号前的字符串		
		if (!strcmp(cmd, "timerstart"))  //比较字符串，strcmp在字符串相等时返回0
		{
			StartTimer();
		}
		else if (!strcmp(cmd, "timerstop"))
		{
			StopTimer();
		}

	}

	//定时通知屏幕目前的wifi和serve状态


	//定时通知服务器，计时器的当前状态和时间


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

void StartTimer()
{
#ifdef DEBUG
	Serial.println("start timer");
#endif // DEBUG

	digitalWrite(ledPin, HIGH);
	SendCmdToScreen("click btn_start,0");
}

void StopTimer()
{
#ifdef DEBUG
	Serial.println("stop timer");
#endif // DEBUG
	digitalWrite(ledPin, LOW);
	SendCmdToScreen("click btn_stop,0");
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

void SendDataToTcp(const char* s)
{
	if (client.connected())
	{
		int chipid = ESP.getChipId();
		client.write(chipid);
		client.write(":");
		client.write(s);
		client.write('\n');
		
	}
	else
	{
		Serial.println("No tcp connection,can't send data");
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
	while (client.available())
	{


		strOnTcp = client.readStringUntil('#');		
#ifdef DEBUG_tcp
		Serial.print("while inside:");
		Serial.println(strOnTcp);
#endif // DEBUG_tcp
	}

	return strOnTcp;
}

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
	if (WiFi.status() != WL_CONNECTED)
	{
		WiFi.begin(ssid.c_str(), pwd.c_str());
#ifdef DEBUG
		Serial.print("connect to:");
		Serial.println(ssid);
#endif // DEBUG

		
		
		SendCmdToScreen("wifistatus.val=1");  //通知屏幕已经开始连接wifi
		wifistatus = 1;
		while ((millis() - beginConnectTime < 10000) && WiFi.status() != WL_CONNECTED)  //在10秒内并且没连接上，继续连接
		{
			delay(500);			
		}
		if (WiFi.status() == WL_CONNECTED)  //连接成功
		{
			wifistatus = 2;
			SendCmdToScreen("wifistatus.val=2");
#ifdef DEBUG
			Serial.print("connect success!");
#endif // DEBUG			
			
		}
		else //wifi没连接上，说明超时退出循环
		{
			SendCmdToScreen("wifistatus.val=-1");
			wifistatus = -1;
#ifdef DEBUG
			Serial.print("connect fail!");
#endif // DEBUG				
			
		}
		
	}
	else
	{
#ifdef DEBUG
		Serial.println("WIFI already connected.");
#endif // DEBUG		
		wifistatus = 2;
		SendCmdToScreen("wifistatus.val=2");
	}

	
	//连接TCP SERVER
	if (WiFi.status() == WL_CONNECTED)
	{ 
		if (!client.connected())
		{
			beginConnectTime = millis();  //开始计时
			SendCmdToScreen("serverstatus.val=1");  //通知屏幕连接中
			while ((millis() - beginConnectTime < 5000) && !client.connected())  //在5秒内并且没连接上，继续连接
			{
				client.connect(serverip, atoi(port.c_str()));
				delay(500);
			}
			if (client.connected())
			{
#ifdef DEBUG
				Serial.println("TCP connected success.");
#endif // DEBUG							
				serverstatus = 2;
				SendCmdToScreen("serverstatus.val=2");
			}
			else
			{
#ifdef DEBUG
				Serial.println("TCP connected fail.");
#endif // DEBUG			
				serverstatus = -1;
				SendCmdToScreen("serverstatus.val=-1");
			}
			
		}
		else
		{
#ifdef DEBUG
			Serial.println("TCP already connected.");
#endif // DEBUG			
			serverstatus = 2;
			SendCmdToScreen("serverstatus.val=2");
		}
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
