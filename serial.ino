#include <SoftwareSerial.h>

#define SSID_WIFI "Ngejajann"
#define SSID_KEY "nyemilyuk"
#define WEB_SERVER "192.168.43.181" //Host

bool koneksiWifi;   //Simpan status koneksi
String QUERY="/b.php?a=";   //Kueri
SoftwareSerial myESP(2,3); // RX, TX

bool enable = true;
unsigned int button = 8;
unsigned int time;

bool konekWifi();
bool kirimLog();    
bool initWifi();
char keyRead();

void setup() {
    Serial.begin(9600);
    myESP.begin(9600);
    initWifi();
    koneksiWifi = konekWifi();
    pinMode(button, INPUT_PULLUP);
}



void loop()
{
    
    if(digitalRead(button)==LOW && enable){
        if(koneksiWifi){
            time = millis();
            Segitrial.print("Mengirim: ");
            Serial.println(String(time));
            kirimLog(String(time));
        }
    }
    
   //ui();
}


bool konekWifi(){
    Serial.begin(9600);
    //setmode
    myESP.println("AT+CWMODE=1");
    if(myESP.find("OK")){
        Serial.println("CWMODE OK");
        String w = "AT+CWJAP=\"";
        w = w + SSID_WIFI;
        w = w + "\",\"";
        w = w + SSID_KEY;
        w = w + "\"";
        myESP.println(w);
        myESP.setTimeout(15000);
        if(myESP.find("GOT IP")){
            Serial.println("CWJAP OK");
            myESP.setTimeout(200);
            myESP.println("AT+CIPMUX=0");
            if(myESP.find("OK")){
                Serial.println("CIPMUX OK\n");
                return 1;
            }else{
                Serial.println("CIPMUX NOT OK");
                return 0;
            }
        }else{
            Serial.println("CWJAP NOT OK");
            return 0;
        } 
    }else{
        Serial.println("CWMODE NOT OK");
        return 0;
    }
}

bool kirimLog(String pesanLog){
    Serial.begin(9600);
    myESP.setTimeout(200);
    String con;
    con = "AT+CIPSTART=\"TCP\",\"";
    con = con + WEB_SERVER;
    con = con +"\",80";
    myESP.println(con);
    myESP.setTimeout(1000);
    if(myESP.find("CONNECT")){
        Serial.println("READY TO GET");
    }else{
        Serial.println("CANNOT OPEN COMMUNICATION");
        return 0;
    }
    String request;
    request = "GET "+ QUERY + pesanLog;
    request = request+ " HTTP/1.1\r\n";
    request = request + "Host: " + WEB_SERVER + "\r\n\r\n";
    request = request + "Host: " + WEB_SERVER + "\r\n\r\n";
    Serial.println(request);
    myESP.print("AT+CIPSEND=");
    myESP.println(request.length());

    if(myESP.find(">")){
        myESP.println(request);
        if(myESP.find("SEND OK")){
            Serial.print("HTTP REQ SENT");
            myESP.println("AT+CIPCLOSE");
            if(myESP.find("CLOSED")){
                return 1;
            }
        }
    }else{
        Serial.println("CANNOT SEND REQUEST");
        return 0;
    }
}

bool initWifi(){
    myESP.setTimeout(5000);
    myESP.println("AT+RST");
    delay(1000);
}

void serb(){
    if(Serial.available()){
        myESP.write(Serial.read());
    }
    if(myESP.available()){
        Serial.write(myESP.read());
    }
}
