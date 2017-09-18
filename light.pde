import hypermedia.net.*;

UDP udp;  // define the UDP object
String dane="";
int time = millis();
boolean switch1=false;
boolean switch2=false;
PImage on, off;

void setup() {
  size(200, 200);
  surface.setResizable(false);
  surface.setAlwaysOnTop(true);
  surface.setLocation(displayWidth-204, displayHeight-267);
  on = loadImage("data/on.png");
  off = loadImage("data/off.png");
  udp = new UDP( this, 11111 );  // create a new datagram connection on port 6000
  udp.listen( true );           // and wait for incoming message
  send("echo");
}

void draw() {
  background(255);
  if (switch1) {
    image(on, 20, 20, 160, 58);
  } else image(off, 20, 20, 160, 58);
  if (switch2) {
    image(on, 20, 120, 160, 58);
  } else image(off, 20, 120, 160, 58);

  if (millis() > time + 800)
  {
    send("echo");
    time = millis();
  }
}

void send(String x) {
  String ip       = "192.168.0.2"; // the remote IP address
  int port        = 11111;        // the destination port
  udp.send(x, ip, port );   // the message to send
}

void receive( byte[] data ) {
  dane="";
  for (int i=0; i < data.length; i++) {
    dane+=char(data[i]);
  }
  if (dane.contains("00")) {
    switch1=false;
    switch2=false;
  }
  if (dane.contains("10")) {
    switch1=true;
    switch2=false;
  }
  if (dane.contains("01")) {
    switch1=false;
    switch2=true;
  }
  if (dane.contains("11")) {
    switch1=true;
    switch2=true;
  }
}

void mousePressed() {
  if (mouseY<100) {
    if (switch1) {
      send("02");
    } else send("12");
  } else {
    if (switch2) {
      send("03");
    } else send("13");
  }
}