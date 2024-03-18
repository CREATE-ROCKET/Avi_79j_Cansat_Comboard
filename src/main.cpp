#include <Arduino.h>
#include <SoftwareSerial.h>
char gpsreceive[80]={};
int count=0;
bool gpsflag = false;
SoftwareSerial mySerial;
#define UART_RX1 35
#define UART_RX2 25
#define UART_RXD 18
#define UART_TX1 26
#define UART_TX2 33
#define UART_TXD 19

void setup(){
    Serial.begin(9600, SERIAL_8N1, UART_RXD, UART_TXD);   // gps
    Serial1.begin(115200, SERIAL_8N1, UART_RX1, UART_TX1);  // teilite1
    Serial2.begin(115200, SERIAL_8N1, UART_RX2, UART_TX2);  // teilite2
    mySerial.begin(115200) // to Log
}

void loop(){
    if (Serial.available()){ 
        char j = Serial.read();
        // Serial1.write(j);
        gpsreceive[count]=j;
        count++;
        if(count>79||j==0x0A){
            if(gpsflag == true){
            Serial2.write(gpsreceive,count);
            mySerial.write(gpsreceive,count);
            }
            count=0;
            for(int i=0;i<80;i++){
            gpsreceive[i]=0;
            }
        }
    }

    if (Serial2.available()){
        char cmd = Serial2.read();
        Serial1.write(cmd);
    }

    if (Serial1.available()){
        char cmd = Serial1.read();
        mySerial.write(cmd);
        if(cmd == 'g'){
            !gpsflag;
        }
    }
}
