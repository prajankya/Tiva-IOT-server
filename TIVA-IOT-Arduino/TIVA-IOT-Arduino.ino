
#include "Ethernet.h"

void printEthernetData();


EthernetServer self(80);
IPAddress server(10,4,2,62);
EthernetClient myNode;

EthernetClient client;

int statusConfig = 0;

void setup() {
  Serial.begin(115200);
  pinMode(D1_LED, OUTPUT);
  pinMode(D2_LED, OUTPUT);
  pinMode(PUSH1, INPUT_PULLUP);
  pinMode(PUSH2, INPUT_PULLUP);

  Ethernet.begin(0);

  self.begin();

  Serial.println("Connecting to server...");

  if (myNode.connect(server, 3000)) {
    Serial.println("connected");
    IPAddress ip = Ethernet.localIP();

    String ip_address="";
    for(byte thisByte=0;thisByte < 4;thisByte++){
      ip_address += String(ip[thisByte], DEC);
      if(thisByte < 3){
        ip_address += ".";
      }
    }

    String s=String("GET /add/") + ip_address + String(" HTTP/1.0");
    myNode.println(s);
    myNode.println();
  }

  printEthernetData();
}

boolean f_1 = false, f_2 = false;
void loop() {
  client = self.available();

  if(digitalRead(PUSH1) && f_1 == false){
    Serial.println("Push1_release");
    f_1 = true;
    if (myNode.connect(server, 3000)) {
      Serial.println("Server connected");
      myNode.println("GET /button/1/off HTTP/1.0");
      myNode.println();
    }
  }
  else{
    if(digitalRead(PUSH1)==0 && f_1==true){
      Serial.println("Push1_press");
      if (myNode.connect(server, 3000)) {
        Serial.println("Server connected");
        myNode.println("GET /button/1/on HTTP/1.0");
        myNode.println();
      }
      f_1 = false;
    }
  }
  if(digitalRead(PUSH2) && f_2 == false){
    Serial.println("Push2_release");
    f_2 = true;
    if (myNode.connect(server, 3000)) {
      Serial.println("Server connected");
      myNode.println("GET /button/2/off HTTP/1.0");
      myNode.println();
    }
  }
  else{
    if(digitalRead(PUSH2)==0 && f_2==true){
      Serial.println("Push2_press");
      if (myNode.connect(server, 3000)) {
        Serial.println("Server connected");
        myNode.println("GET /button/2/on HTTP/1.0");
        myNode.println();
      }
      f_2 = false;
    }
  }

  if (client) { 
    Serial.print("new client on port "); 
    Serial.println(client.port());
    String currentLine = "";    
    boolean newConnection = true; 
    unsigned long connectionActiveTimer;

    while (client.connected()) {
      if (newConnection){
        connectionActiveTimer = millis();
        newConnection = false;
      }
      if (!newConnection && connectionActiveTimer + 1000 < millis()){
        break;
      }

      if (client.available()) {
        char c = client.read();
        Serial.print(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            break;
          }          
          else { 
            currentLine = "";
          }
        }
        else if (c != '\r') {
          currentLine += c;
        }

        if (currentLine.endsWith("GET /LED_1_on")) {
          digitalWrite(D1_LED, HIGH);
        }
        if (currentLine.endsWith("GET /LED_1_off")) {
          digitalWrite(D1_LED, LOW);
        }
        if (currentLine.endsWith("GET /LED_2_on")) {
          digitalWrite(D2_LED, HIGH);
        }
        if (currentLine.endsWith("GET /LED_2_off")) {
          digitalWrite(D2_LED, LOW);
        }
      }
    }
    client.stop();
    Serial.println("client disonnected");
  }
}



void printEthernetData() {
  Serial.println();
  Serial.println("IP Address Information:");
  IPAddress ip = Ethernet.localIP();
  Serial.print("IP Address:\t");
  Serial.println(ip);

  IPAddress subnet = Ethernet.subnetMask();
  Serial.print("NetMask:\t");
  Serial.println(subnet);

  IPAddress gateway = Ethernet.gatewayIP();
  Serial.print("Gateway:\t");
  Serial.println(gateway);

  IPAddress dns = Ethernet.dnsServerIP();
  Serial.print("DNS:\t\t");
  Serial.println(dns);

}


