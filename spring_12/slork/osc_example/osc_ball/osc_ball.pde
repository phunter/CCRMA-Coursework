
import oscP5.*;
import netP5.*;


OscP5 osc;
NetAddress remote;
NetAddress local;


float x;
float y;

float vx;
float vy;

void setup()
{
  size(720, 480);
  frameRate(60);
  
  colorMode(HSB, 1.0);
  
  x = width/2;
  y = height/2;
  
  vx = 6;
  vy = 6;
  
  osc = new OscP5(this, 12000);
  remote = new NetAddress("laserbeak.local", 6449);  
  local = new NetAddress("localhost", 6449);
}

void mousePressed()
{
  x = mouseX;
  y = mouseY;
}


void draw()
{
  /** update **/
  
  x += vx;
  y += vy;
  
  boolean sendRemote = false;
  boolean sendLocal = false;
  
  if(x > width || x < 0)
  {
    sendRemote = true;
    vx = -vx;
  }
  if(y > height || y < 0)
  {
    sendLocal = true;
    vy = -vy;
  }
  
  if(sendRemote)
  {
    OscMessage msg = new OscMessage("/sndbuf/buf/rate");
    float p = random(0.2,1.5);
    msg.add(p);
    osc.send(msg, remote);
  }
  
  if(sendLocal)
  {
    OscMessage msg = new OscMessage("/sndbuf/buf/rate");
    float p = random(0.2,1.5);
    msg.add(p);
    osc.send(msg, local);
  }
  
  /** draw **/
  
  background(0, 0, 0);
  
  translate(x, y);
  
  fill(0.5, 0.5, 0.5);
  ellipse(0, 0, 100, 100);
}
