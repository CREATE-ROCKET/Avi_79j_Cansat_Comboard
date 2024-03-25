#include <Arduino.h>
#include <SoftwareSerial.h>
char gpsreceiveC[80]={}; // 缶サット用
char gpsreceiveH[80]={}; // 本体用
int count=0;  // 本体カウント
int countH=0; // 缶サットカウント
SoftwareSerial mySerial(3,1); // RX, TX
#define UART_RX1 35
#define UART_RX2 25
#define UART_RXD 19
#define UART_TX1 26
#define UART_TX2 33
#define UART_TXD 18
#define LED 13

void setup(){
    Serial.begin(9600, SERIAL_8N1, UART_RXD, UART_TXD);     // gps
    Serial1.begin(115200, SERIAL_8N1, UART_RX2, UART_TX2);  // 本部
    Serial2.begin(115200, SERIAL_8N1, UART_RX1, UART_TX1);  // 本体　    
    mySerial.begin(115200);
    delay(1000);
    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);
}


// 本部とログ基盤に缶サットのGPSを送る
void loop(){
    if (Serial.available()){ 
        char j = Serial.read();                                                                                                                                               Serial1.write(j);
        gpsreceiveC[count]=j;
        count++;
        if(count>79 || j==0x0A){
            Serial1.print("CansatGPS :");
            Serial1.write(gpsreceiveC,count);
            mySerial.write(gpsreceiveC,count);
            count=0;
            for(int i=0;i<80;i++){
            gpsreceiveC[i]=0;
            }
        }
    }


// 中継
    if (Serial2.available()){
        u_int8_t cmd = Serial2.read();
        char rcmd = char(cmd);
        gpsreceiveH[countH]=rcmd;
        countH++;
        if(countH>79 || rcmd==0x0A){
            Serial1.print("GPS from Hontai :");
            Serial1.write(gpsreceiveH,countH);
            countH=0;
            for(int i=0;i<80;i++){
            gpsreceiveH[i]=0;
            }
        }
    }

//　本部からコマンドを受け取る
    if (Serial1.available()){
        char cmd = Serial1.read();
        Serial1.println(cmd);
        if(cmd == 'h'){
            Serial.write("$PMTK161,0*28\r\n");
        }else if(cmd =='g'){
            Serial.write("$PMTK161,1*29\r\n");
        }else if(cmd =='r'){
            digitalWrite(LED, HIGH);
        }else if (cmd =='t'){
            digitalWrite(LED, LOW);
        }else{
            mySerial.write(cmd);
        }
    }
}

