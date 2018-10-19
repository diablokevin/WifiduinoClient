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
const int tcpPort = 12345;//�޸�Ϊ�㽨����Server����˵Ķ˿ں�
const int buttonPin = 4;     // the number of the pushbutton pin
const int ledPin = 5;      // the number of the LED pin

//WiFi��tcp server����ر���
int wifistatus = 0;  //0�ȴ����ӣ�1�����У�2���ӳɹ�,-1����ʧ��
int serverstatus = 0; //0�ȴ����ӣ�1�����У�2���ӳɹ�,-1����ʧ��

//��ȡ��ť״̬���ܵ���ر���
int buttonState = 0;         
int lastButtonState = LOW;
long lastDebounceTime = 0;
long debounceDelay = 50;


int timerStatus = 0; //��ʱ����״̬��0δ������10�ֶ�׼���ã�11�ֶ���ʱ�У�12�ֶ���ʱֹͣ��20�Զ�׼���ã�21�Զ���ʱ�У�22�Զ���ʱֹͣ
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
	//Serial.print("Connecting to ");//д������ʾ������
	//Serial.println(ssid);
	//SendCmdToScreen("wifistatus.val=0");
	//SendCmdToScreen("serverstatus.val=0");
	//ConnectServer("MyDell", "ddll1314", "", "");
	//if (wifistatus == 2)
	//{
	//	Serial.println("");
	//	Serial.println("WiFi connected");
	//	Serial.println("IP address: ");
	//	Serial.println(WiFi.localIP());//WiFi.localIP()����8266��õ�ip��ַ
	//}
	//else if (wifistatus == -1)
	//{
	//	Serial.println("");
	//	Serial.println("WiFi connected FAIL!");
	//}

	//WiFi.begin(ssid, password);

	// //������Ļ�б������ﵽ��ʾwifi״̬��Ŀ��
	//while (WiFi.status() != WL_CONNECTED)//WiFi.status() �����������wifi����״̬������wifi����״̬
	//									 //����Ͳ�һһ׸�������ص������ˣ�����Ȥ�ĵ�ESP8266WiFi.cpp�в鿴
	//{
	//	delay(500);
	//
	//	SendCmdToScreen("wifistatus.val=1");

	//}
	////���ӳɹ�
	//SendCmdToScreen("wifistatus.val=2");
	//


	//Serial.println("");
	//Serial.println("WiFi connected");
	//Serial.println("IP address: ");
	//Serial.println(WiFi.localIP());//WiFi.localIP()����8266��õ�ip��ַ
}

// Add the main program code into the continuous loop() function
void loop()
{
	//������Ļ������������
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


		//ȡ��1���ַ���
		String s = cmdStringFromSerial;
		index = s.indexOf(",");
		String s_take = s.substring(0, index);
		String s_remain = s.substring(index + 1, s.length());
		cmd = s_take;

		//ȡ��2���ַ���
		 s = s_remain;
		index = s.indexOf(",");
		 s_take = s.substring(0, index);
		 s_remain = s.substring(index + 1, s.length());
		 firstpara = s_take;
		 //ȡ��3���ַ���
		 s = s_remain;
		 index = s.indexOf(",");
		 s_take = s.substring(0, index);
		 s_remain = s.substring(index + 1, s.length());
		 secondpara = s_take;
		 //ȡ��4���ַ���
		 s = s_remain;
		 index = s.indexOf(",");
		 s_take = s.substring(0, index);
		 s_remain = s.substring(index + 1, s.length());
		 thirdpara = s_take;
		 //ȡ��4���ַ���
		 s = s_remain;
		 index = s.indexOf(",");
		 s_take = s.substring(0, index);
		 s_remain = s.substring(index + 1, s.length());
		 fourthpara = s_take;
		
	

		//char cmdchar[] = "";
		//
		//strcpy(cmdchar, cmdStringFromSerial.c_str());  //stringתchar[]
		//Serial.print("cmdchar:");
		//Serial.println(cmdchar);


		//char *StrTmpIn = NULL;  //��ʱ����strtok_r��Ҫ�Ĳ���
		//char *cmd= strtok_r(cmdchar, ",", &StrTmpIn);  //ȡ��һ������ǰ���ַ���		
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
		if (cmd=="set")  //�Ƚ��ַ�����strcmp���ַ������ʱ����0
		{


			if (firstpara=="sys1")  //�Ƚ��ַ�����strcmp���ַ������ʱ����0
			{  
				timerStatus = atoi(secondpara.c_str());  //ת���ַ���Ϊ����
			}
		}
		if (cmd =="connectwifi")
		{
			ConnectServer(firstpara, secondpara, thirdpara, fourthpara);
		}
	}
	//����ť�¼�
	if (ButtonPressed(buttonPin))
	{
#ifdef DEBUG
		Serial.println("button press");
		Serial.print("timerStatus:");
		Serial.println(timerStatus);
#endif // DEBUG
		
		if (timerStatus == 10) //10Ϊ�ֶ�׼����
		{		
			StartTimer();
		}
		else if (timerStatus == 11) //11Ϊ�ֶ���ʱ��
		{
			StopTimer();
		}
	}

	//�������������������
	
	String cmdStringFromTcp = GetCmdFromTcp();


	if (cmdStringFromTcp != "")
	{
#ifdef DEBUG_tcp
		Serial.print("loop if:");
		Serial.println(cmdStringFromTcp);
#endif // DEBUG_tcp

		
		char cmdchar[] = "";
		strcpy(cmdchar, cmdStringFromTcp.c_str());  //stringתchar[]
		char *cmd = strtok(cmdchar, ",");  //ȡ��һ������ǰ���ַ���		
		if (!strcmp(cmd, "timerstart"))  //�Ƚ��ַ�����strcmp���ַ������ʱ����0
		{
			StartTimer();
		}
		else if (!strcmp(cmd, "timerstop"))
		{
			StopTimer();
		}

	}

	//��ʱ֪ͨ��ĻĿǰ��wifi��serve״̬


	//��ʱ֪ͨ����������ʱ���ĵ�ǰ״̬��ʱ��


	/*



	while (client.available())//available()ͬARDUINO����������
	{
		
		char val = client.read();//read()ͬarduino
		if (val == 'a') {//pc�˷���0��1������
			
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
	Serial.write(0x22);  //˫����
	Serial.print(val);
	Serial.write(0x22); //˫����
	Serial.write(hexdata, 3);
}
void ChangeValofScreenObject(unsigned char val[], String obj)
{
	unsigned char hexdata[3] = { 0xff,0xff,0xff };
	int size = strlen((const char*)val);
	Serial.print(obj + "=");
	Serial.write(0x22);  //˫����
	Serial.write(val, size);
	Serial.write(0x22); //˫����
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

void ConnectServer(String ssid, String pwd, String serverip, String port)   //����wifi��tcp server������wifi�ɹ���������tcp server
{
#ifdef DEBUG
	Serial.print("WiFi.status():");
	Serial.println(WiFi.status());
	Serial.print("wifistatus.val");
	Serial.println(wifistatus);
#endif // DEBUG


	long beginConnectTime = millis();

	// ����WIFI
	if (WiFi.status() != WL_CONNECTED)
	{
		WiFi.begin(ssid.c_str(), pwd.c_str());
#ifdef DEBUG
		Serial.print("connect to:");
		Serial.println(ssid);
#endif // DEBUG

		
		
		SendCmdToScreen("wifistatus.val=1");  //֪ͨ��Ļ�Ѿ���ʼ����wifi
		wifistatus = 1;
		while ((millis() - beginConnectTime < 10000) && WiFi.status() != WL_CONNECTED)  //��10���ڲ���û�����ϣ���������
		{
			delay(500);			
		}
		if (WiFi.status() == WL_CONNECTED)  //���ӳɹ�
		{
			wifistatus = 2;
			SendCmdToScreen("wifistatus.val=2");
#ifdef DEBUG
			Serial.print("connect success!");
#endif // DEBUG			
			
		}
		else //wifiû�����ϣ�˵����ʱ�˳�ѭ��
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

	
	//����TCP SERVER
	if (WiFi.status() == WL_CONNECTED)
	{ 
		if (!client.connected())
		{
			beginConnectTime = millis();  //��ʼ��ʱ
			SendCmdToScreen("serverstatus.val=1");  //֪ͨ��Ļ������
			while ((millis() - beginConnectTime < 5000) && !client.connected())  //��5���ڲ���û�����ϣ���������
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


int ButtonPressed(int buttonPin)  //��ȡ��ť�Ĵ���״̬��δ���·���0�����·���1
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
