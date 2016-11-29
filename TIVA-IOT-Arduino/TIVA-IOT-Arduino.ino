
#include "Ethernet.h"

void printEthernetData();


EthernetServer self(80);
IPAddress server(10, 4, 2, 62);
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

    String ip_address = "";
    for (byte thisByte = 0; thisByte < 4; thisByte++) {
      ip_address += String(ip[thisByte], DEC);
      if (thisByte < 3) {
        ip_address += ".";
      }
    }

    String s = String("GET /add/") + ip_address + String(" HTTP/1.0");
    myNode.println(s);
    myNode.println();
  }

  printEthernetData();
}

boolean f_1 = false, f_2 = false;
void loop() {
  client = self.available();

  if (client) {
    Serial.print("new client on port ");
    Serial.println(client.port());

    boolean newConnection = true;
    unsigned long connectionActiveTimer;

    while (client.connected()) {
      if (newConnection) {
        connectionActiveTimer = millis();
        newConnection = false;
      }

      if (!newConnection && connectionActiveTimer + 10000 < millis()) {
        break;
      }

      if (client.available()) {
        String line = client.readString();

        //Serial.println(line);
        String s = line.substring(line.indexOf('GET') + 3, line.indexOf('HTTP') - 3);
        Serial.println(s);
        String fun = s.substring(0, s.indexOf('/'));
        String pin = s.substring(s.indexOf('/') + 1);
        String val = "0";
        if (pin.indexOf('/') >= 0) {
          val = pin.substring(pin.indexOf('/') + 1);
          pin = pin.substring(0, pin.indexOf('/'));
        }

        int pin_ = pin.toInt();
        int val_ = val.toInt();

        Serial.println("Fun:" + fun + "\tPin:" + pin + "\tVal:" + val_);
        String out = runFun(fun, pin_, val_);
        Serial.println("Out : " + out);
        client.println(out);
        client.stop();
      }
    }
    client.stop();
    Serial.println("client disonnected");
  }
}

String runFun(String f, int pin, int val) {
  if (f == "analogRead") {
    return String(analogRead(pin));
  } else if (f == "analogWrite") {
    analogWrite(pin, val);
    return "done";
  } else if (f == "digitalRead") {
    return String(digitalRead(pin));
  } else if (f == "digitalWrite") {
    digitalWrite(pin, val);
    return "done";
  }else{
    return "Illegal Function";
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


