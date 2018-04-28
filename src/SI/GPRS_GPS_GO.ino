#include <GPRS_Shield_Arduino.h>
#include <TroykaGPS.h>
#include <SoftwareSerial.h>
#define IMEI "861508038977104"
#define INTERVAL 30000
#define LEN 370
#define MAX_SIZE_MAS 16
char tcpBuffer[LEN];
SoftwareSerial GPRSSerial(10, 11);
GPS gps(Serial1);
GPRS gprs(GPRSSerial);

void setup()
{
	Serial.begin(9600); 
	while(!Serial){}
	Serial.println("SERIAL INIT OK");
	delay(100);
	Serial1.begin(115200);
	Serial.println("GPS SERIAL INIT OK");
	GPRSSerial.begin(9600);
	Serial.println("GPRS SERIAL INIT OK");
	delay(1000);
	Serial.println("WAITING FOR NETWORK");
	while(1){
		delay(1000);
		if(gps.available()){
				gps.readParsing();
				if(gps.getState() == GPS_OK) break;
			
		}
		else{
			Serial.println("GPS NOT AVAILABLE");
		}
	}
	Serial.println("NETWORK FOUND");
	gprs.powerOn();
	while(!gprs.init()){
		Serial.println("GPRS INIT ERROR");
		delay(10);
	}
	Serial.println("GPRS INIT SUCCESS");
	delay(3000);
	while(!gprs.join("internet.beeline.ru", "beeline", "beeline")){
		Serial.println("GPRS JOINING NETWORK ERROR");
		delay(1000);
	}
	Serial.println("GPRS OK");
	Serial.print("IP ADDRESS IS ");
	Serial.println(gprs.getIPAddress());
}

void loop()
{
	while (!gprs.connect(TCP, "188.44.53.81", 80)) {
		Serial.println("CONNECT ERROR");
		delay(1000);
	}
	Serial.println("CONNECT SUCCESS");
	tcpSend();
	gprs.close();
	gprs.disconnect();
	delay(100);
	Serial.println("SENT");
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
		gps.readParsing();
		switch(gps.getState()) {
			case GPS_OK:
				char lon[16], lat[16], date[16], time[16];
				gps.getTime(time, MAX_SIZE_MAS);
				gps.getDate(date, MAX_SIZE_MAS);
				dtostrf(gps.getLatitudeBase10(), 9, 6, lat);
				dtostrf(gps.getLongitudeBase10(), 9, 6, lon);
				sprintf(tcpBuffer, "%s_%s_%s_%s_%s", IMEI, date, time, lat, lon);
				Serial.print("FORMED: ");
				Serial.println(tcpBuffer);
				Serial.print("PENDING TO SEND... ");
				break;
			// ошибка данных
			case GPS_ERROR_DATA:
				gprs.getDateTime(buf);
				sprintf(tcpBuffer, "ERROR_DATA_%s_%s", buf, IMEI);
				Serial.println("SENDING GPS_ERROR_DATA");
				Serial.print("FORMED: ");
				Serial.println(tcpBuffer);
				Serial.print("PENDING TO SEND... ");
				break;
			// нет соединения со спутниками
			case GPS_ERROR_SAT:
				gprs.getDateTime(buf);
				sprintf(tcpBuffer, "ERROR_SAT_%s_%s", buf, IMEI);
				Serial.println("SENDING GPS_ERROR_SAT");
				Serial.print("FORMED: ");
				Serial.println(tcpBuffer);
				Serial.print("PENDING TO SEND... ");
				break;
		}
	}
}

void clearTcpBuffer()
{
	for (int t = 0; t < LEN; t++) {
		tcpBuffer[t] = 0;
	}
}
