#include <GPRS_Shield_Arduino.h> //подключение библиотеки для работы с GPRS шилдом
#include <TroykaGPS.h>
#include <SoftwareSerial.h> // подключение библиотеки для работы с эмулированными Serial-портами
#define IMEI "861508038977104" //макрос IMEI-номера
#define INTERVAL 30000
#define LEN 370
#define MAX_SIZE_MAS 16
char tcpBuffer[LEN];//буфер отправки
SoftwareSerial GPRSSerial(10, 11);//Объект эиулированного Serial соединения для gprs
GPS gps(Serial1);//объект GPS шилда
GPRS gprs(GPRSSerial);//Объект GPRS шилда, принимет на вход объект Serial содинения

void setup()
{
	Serial.begin(9600); //Serial соединение Arduino-консоль
	while(!Serial){}
	Serial.println("Serial init OK");
	delay(100);
	Serial1.begin(115200);//открываем соединение Arduino-GPS шилд
	Serial.println("GPS serial init OK");
	GPRSSerial.begin(9600);//Serial соединение Arduino-GPRS шилд
	Serial.println("GPRS serial init OK");
	while(1){
		if(gps.available()){
				gps.readParsing();
				if(gps.getState() == GPS_OK) break;
				else Serial.println("Waiting for network");
				delay(1000);
			
		}
		else{
			Serial.println("GPS not available");
		}
	}
}

void loop()
{
	gprs.powerOn();//включение GPRS шилда
	//Serial.println("1");
	while (!gprs.init()) { // проверка на подключеие к сети
		//Serial.println("2");
		//delay(1000);
		//Serial.println("3");
		Serial.println("GPRS Init error");
	}
	//Serial.println("4");
	Serial.println("GPRS init success");
	delay(3000);
	while (!gprs.join("internet.beeline.ru", "beeline", "beeline")) { //проверка на подключение к интернету
		Serial.println("GPRS JOINING NETWORK ERROR");
		delay(1000);
	}
	Serial.println("GPRS OK");
	Serial.print("IP Address is "); //вывод IP аддреса в консоль
	Serial.println(gprs.getIPAddress());
	while (!gprs.connect(TCP, "188.44.53.81", 80)) { //проверка на подкжючение к сервису
		Serial.println("Connect error");
		delay(1000);
	}
	Serial.println("Connect success");
	tcpSend();
	gprs.close();
	gprs.disconnect();
	delay(100);
	Serial.println("Sent");
	gprs.powerOff();
	delay(15000);
}

void tcpSend()
{
	tcpBufferForm();
	gprs.send(tcpBuffer);
	clearTcpBuffer();
}

void tcpBufferForm(){
	//strcat(tcpBuffer, "GET /recvdata.php?filename=ard.txt&data=123 HTTP/1.0\r\n\r\n");
	char buf[100];
	if (gps.available()) {
		// считываем данные и парсим
		gps.readParsing();
		// проверяем состояние GPS-модуля
		switch(gps.getState()) {
			// всё OK
			case GPS_OK:
				char lon[16], lat[16], date[16], time[16];
				gps.getTime(time, MAX_SIZE_MAS);
				gps.getDate(date, MAX_SIZE_MAS);
				dtostrf(gps.getLatitudeBase10(), 9, 6, lat);
				dtostrf(gps.getLongitudeBase10(), 9, 6, lon);
				sprintf(tcpBuffer, "GET /recvdata.php?filename=%s_%s.txt&data=%s_%s_%s_%s_%s HTTP/1.0\r\n\r\n", date, time, IMEI, date, time, lat, lon);
				Serial.print("formed: ");
				Serial.println(tcpBuffer);
				Serial.print("pending to send...");
				break;
			// ошибка данных
			case GPS_ERROR_DATA:
				gprs.getDateTime(buf);
				sprintf(tcpBuffer, "GET /recvdata.php?filename=%s_err.txt&data=ERROR_DATA_%s_%s HTTP/1.0\r\n\r\n", buf, buf, IMEI);
				Serial.println("Sending GPS_ERROR_DATA");
				Serial.print("formed: ");
				Serial.println(tcpBuffer);
				Serial.print("pending to send...");
				break;
			// нет соединения со спутниками
			case GPS_ERROR_SAT:
				gprs.getDateTime(buf);
				sprintf(tcpBuffer, "GET /recvdata.php?filename=%s_err.txt&data=ERROR_SAT_%s_%s HTTP/1.0\r\n\r\n", buf, buf, IMEI);
				Serial.println("Sending GPS_ERROR_SAT");
				Serial.print("formed: ");
				Serial.println(tcpBuffer);
				Serial.print("pending to send...");
				break;
		}
	}
}

void clearTcpBuffer()//функция очистки буфера
{
	for (int t = 0; t < LEN; t++) {
		tcpBuffer[t] = 0;
	}
}
