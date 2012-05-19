/**
 * Slugscapes
 * Visuals
 *
 * Slork 2012
 * Hunter McCurry, Jiffer Harriman, Michael Wilson
 */

import oscP5.*;
import netP5.*;


// Network Variables
OscP5 oscP5;
NetAddress myRemoteLocation;
int listen_port = 6472;

// View Layout Things
float bottom_frame_height = 0.618033988; // Golden Ratio, just cuz


PGraphics pg;
PGraphics common_layer;
PGraphics upper_left_layer;
PGraphics upper_middle_layer;
PGraphics upper_right_layer;

int lower_width;
int lower_height;  
int upper_width;
int upper_height;


// Global things
int my_machine = 0;
int mode;
boolean initialized = false;

void setup() {
  size(1280,800);
  background(0);
  frameRate(25);
  smooth();
  
  lower_width = width;
  lower_height = (int) ( bottom_frame_height * height);  
  upper_width = width / 3;
  upper_height = (int) ((1.0 - bottom_frame_height) * height);

    
  println("lower dimensions: ( " + lower_width + ", " + lower_height + " )");
  println("upper dimensions: ( " + upper_width + ", " + upper_height + " )");
  
  pg = createGraphics(320, 240, P3D);
  common_layer = createGraphics(lower_width, lower_height, P3D);
  upper_left_layer = createGraphics(upper_width, upper_height, P3D);
  upper_middle_layer = createGraphics(upper_width, upper_height, P3D);
  upper_right_layer = createGraphics(upper_width, upper_height, P3D);
  
  initLayerColors();
  
  /* start oscP5, listening for incoming messages at listn_port */
  oscP5 = new OscP5(this,listen_port);
  
  /* myRemoteLocation is a NetAddress. a NetAddress takes 2 parameters,
   * an ip address and a port number. myRemoteLocation is used as parameter in
   * oscP5.send() when sending osc packets to another computer, device, 
   * application. usage see below. for testing purposes the listening port
   * and the port of the remote location address are the same, hence you will
   * send messages back to this sketch.
   */
  myRemoteLocation = new NetAddress("127.0.0.1",listen_port);
}

void initLayerColors() {

  common_layer.noStroke();
  common_layer.fill(150, 150, 150, 255);
  common_layer.beginDraw();
  common_layer.rect(0,0,lower_width,lower_height);
  common_layer.endDraw();
  
  upper_left_layer.noStroke();
  upper_left_layer.fill(70, 20, 20, 255);
  upper_left_layer.beginDraw();
  upper_left_layer.rect(0, 0, upper_width, upper_height);
  upper_left_layer.endDraw();
  
  upper_middle_layer.noStroke();
  upper_middle_layer.fill(20, 70, 20, 255);
  upper_middle_layer.beginDraw();
  upper_middle_layer.rect(0, 0, upper_width, upper_height);
  upper_middle_layer.endDraw();
  
  upper_right_layer.noStroke();
  upper_right_layer.fill(20, 20, 70, 255);
  upper_right_layer.beginDraw();
  upper_right_layer.rect(0, 0, upper_width, upper_height);
  upper_right_layer.endDraw();
}

void drawRecord(float pitch, float amplitude, int machine_num, int frame_num) {
  
  int radius = (int) map(amplitude, 0.0, 1.0, 0.0, 20.0);
  int x = (int) map(frame_num, 0, 255, 0, upper_width);
  int y = (int) map(pitch, 40, 1200, upper_height, 0); // I don't know what input pitch range is! guess: 40 to 90 in midi
  
  if (machine_num == 0) {
    upper_left_layer.beginDraw();
    upper_left_layer.smooth();
    upper_left_layer.stroke(255, 20, 20, 255);
    upper_left_layer.strokeWeight(1);
    upper_left_layer.fill(150, 20, 20, 255);
    upper_left_layer.ellipse(x, y, radius, radius);
    upper_left_layer.endDraw();
  }
  else if (machine_num == 1) {
    upper_middle_layer.beginDraw();
    upper_middle_layer.smooth();
    upper_middle_layer.stroke(20, 255, 20, 255);
    upper_middle_layer.strokeWeight(1);
    upper_middle_layer.fill(20, 150, 20, 255);
    upper_middle_layer.ellipse(x, y, radius, radius);
    upper_middle_layer.endDraw();
  }
  else if (machine_num == 2) {
    upper_right_layer.beginDraw();
    upper_right_layer.smooth();
    upper_right_layer.stroke(20, 20, 255, 255);
    upper_right_layer.strokeWeight(1);
    upper_right_layer.fill(20, 20, 150, 255);
    upper_right_layer.ellipse(x, y, radius, radius);
    upper_right_layer.endDraw();
  }  
}

void drawAveraging(float pitch, float amplitude, int machine_num, int frame_num) {
  
  int radius = (int) map(amplitude, 0.0, 1.0, 0.0, 20.0);
  int x = (int) map(frame_num, 0, 255, 0, lower_width);
  int y = (int) map(pitch, 40, 1200, lower_height, 0); // I don't know what input pitch range is! guess: 40 to 90 in midi

  color c = color(0, 0, 0); // initialize to make processing happy
  if (machine_num == 0) {
    c = color(150, 20, 20);
  }
  else if (machine_num == 1) {
     c = color(20, 150, 20);
  }
  else if (machine_num == 2) {
     c = color(20, 20, 150);    
  }

  common_layer.noStroke();
  common_layer.fill(c);
  common_layer.beginDraw();
  common_layer.ellipse(x, y, radius, radius);
  common_layer.endDraw();
}

void draw() {
  if (!initialized) {
    initLayerColors();
    initialized = true;
  }
  
  image(common_layer, 0, upper_height);
  image(upper_left_layer, 0, 0);
  image(upper_middle_layer, upper_width, 0);  
  image(upper_right_layer, 2 * upper_width, 0);

  stroke(255);
  strokeWeight(5);
  beginShape();
  
  line(0, 0, width-1, 0);
  line(width-1, 0, width-1, height-1);
  line(width-1, height-1, 0, height-1);
  line(0, height-1, 0, 0);
  
  line(0, upper_height, width, upper_height);
  line(upper_width, 0, upper_width, upper_height);
  line(2*upper_width, 0, 2*upper_width, upper_height);
  
  endShape();
}




void mousePressed() { // for testing

  if (random(1) > .15) { // most of the time send envelope msg
    /* create a new osc message object */
    OscMessage myMessage = new OscMessage("/slork/slugscapes/envelope");
    
    myMessage.add(40 + random(50)); // example pitch
    myMessage.add(random(1)); // example amplitude
    myMessage.add((int) random(3)); // example machine_index
    myMessage.add((int) random(256));
    
    /* send the message */
    oscP5.send(myMessage, myRemoteLocation);
  }

  else { // send pulse msg
    /* create a new osc message object */
    OscMessage myMessage = new OscMessage("/slork/slugscapes/pulse");
    
    if (random(1) < .4) {
      myMessage.add(0); // collecting
    }
    else if (random(1) < .8){
      myMessage.add(4); // example consensus
    }
    else {
      myMessage.add(5); // clear screen
    }
    
    
    /* send the message */
    oscP5.send(myMessage, myRemoteLocation);
  }
}


void oscEvent(OscMessage theOscMessage) {
  /* check if theOscMessage has the address pattern we are looking for. */
  
  if(theOscMessage.checkAddrPattern("/slork/slugscapes/pulse")==true) {
    
    /* check if the typetag is the right one. */
    if(theOscMessage.checkTypetag("i")) {
      /* parse theOscMessage and extract the values from the osc message arguments. */
      mode = theOscMessage.get(0).intValue();
      
      print("### received an osc message /slork/slugscapes/pulse with typetag i.");
      println(" values: " + mode);
      return;
    }
  }
  
  
  else if(theOscMessage.checkAddrPattern("/slork/slugscapes/envelope")==true) {
    
    /* check if the typetag is the right one. */
    if(theOscMessage.checkTypetag("ffii")) {
      /* parse theOscMessage and extract the values from the osc message arguments. */
      
      float pitch = theOscMessage.get(0).floatValue();
      float amplitude = theOscMessage.get(1).floatValue();
      int machine_index = theOscMessage.get(2).intValue();
      int frame_num = theOscMessage.get(3).intValue(); // 0 to 255 for now
      
      if (mode == 0) { // we're in record mode
        drawRecord(pitch, amplitude, machine_index, frame_num); 
      }
      else if (mode == 4) { // we're doing the averaging phase (main window)
        drawAveraging(pitch, amplitude, machine_index, frame_num); 
      }
      else if (mode == 5) { // we're doing nothing
        initialized = false;  // clear the screen
      }
      
      print("### received an osc message /slork/slugscapes/envelope with typetag ffii.");
      println(" values: " + pitch + " " + amplitude + " " + machine_index + " " + frame_num);
      return;
    }
  } 
  println("### received a strange osc message. with address pattern " + theOscMessage.addrPattern());
}

