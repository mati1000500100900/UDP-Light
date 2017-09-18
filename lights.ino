#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008

bool state8 = true;
bool state9 = true;

byte mac[] = {
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
IPAddress ip(192, 168, 0, 2);

unsigned int localPort = 11111;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  //buffer to hold incoming packet,
char  ReplyBuffer[] = "a";       // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(8,INPUT_PULLUP);
  pinMode(9,INPUT_PULLUP);
  // start the Ethernet and UDP:
  Ethernet.begin(mac, ip);
  Udp.begin(localPort);
  state8=digitalRead(8);
  state9=digitalRead(9);
  Serial.begin(9600);
}

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    Serial.print(Udp.remoteIP());
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    Serial.print("Contents: ");
    Serial.println(packetBuffer);
    String data=packetBuffer;
    String get1=data.substring(1,2);
    String get2=data.substring(0,1);
    int pin=get1.toInt();
    int state=get2.toInt();
    digitalWrite(pin,state);

    // send a reply to the IP address and port that sent us the packet we received
    delay(10);
    Udp.beginPacket(Udp.remoteIP(), localPort);
    Udp.write(ReplyBuffer);
    Udp.print(digitalRead(2));
    Udp.print(digitalRead(3));
    Udp.endPacket();
  }
  checkPin(8,&state8);
  checkPin(9,&state9);
  delay(10);
}

void checkPin(int i,bool *x){
  if(digitalRead(i)!=*x){
    delay(20);
    if(digitalRead(i)!=*x){
      digitalWrite(i-6,!digitalRead(i-6));
      *x=!*x;
    }
  }
}

