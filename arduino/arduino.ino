
#include "Ethernet.h"

void printEthernetData();


EthernetServer self(80);
IPAddress server(10,4,0,69);
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
    // Make a HTTP request:
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
      // Make a HTTP request:
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
      // Make a HTTP request:
      if (myNode.connect(server, 3000)) {
        Serial.println("Server connected");
        myNode.println("GET /button/2/on HTTP/1.0");
        myNode.println();
      }
      f_2 = false;
    }
  }

  if (client) {                             // if you get a client,
    Serial.print("new client on port ");           // print a message out the serial port
    Serial.println(client.port());
    String currentLine = "";                // make a String to hold incoming data from the client
    boolean newConnection = true;     // flag for new connections
    unsigned long connectionActiveTimer;  // will hold the connection start time

    while (client.connected()) {       // loop while the client's connected
      if (newConnection){                 // it's a new connection, so
        connectionActiveTimer = millis(); // log when the connection started
        newConnection = false;          // not a new connection anymore
      }
      if (!newConnection && connectionActiveTimer + 1000 < millis()){
        // if this while loop is still active 1000ms after a web client connected, something is wrong
        break;  // leave the while loop, something bad happened
      }


      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        // This lockup is because the recv function is blocking.
        Serial.print(c);
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
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
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}



void printEthernetData() {
  // print your IP address:
  Serial.println();
  Serial.println("IP Address Information:");
  IPAddress ip = Ethernet.localIP();
  Serial.print("IP Address:\t");
  Serial.println(ip);

  // print your MAC address:

  IPAddress subnet = Ethernet.subnetMask();
  Serial.print("NetMask:\t");
  Serial.println(subnet);

  // print your gateway address:
  IPAddress gateway = Ethernet.gatewayIP();
  Serial.print("Gateway:\t");
  Serial.println(gateway);

  // print your gateway address:
  IPAddress dns = Ethernet.dnsServerIP();
  Serial.print("DNS:\t\t");
  Serial.println(dns);

}
