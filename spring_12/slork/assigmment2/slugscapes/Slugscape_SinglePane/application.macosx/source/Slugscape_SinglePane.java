import processing.core.*; 
import processing.xml.*; 

import processing.opengl.*; 
import oscP5.*; 
import netP5.*; 

import java.applet.*; 
import java.awt.Dimension; 
import java.awt.Frame; 
import java.awt.event.MouseEvent; 
import java.awt.event.KeyEvent; 
import java.awt.event.FocusEvent; 
import java.awt.Image; 
import java.io.*; 
import java.net.*; 
import java.text.*; 
import java.util.*; 
import java.util.zip.*; 
import java.util.regex.*; 

public class Slugscape_SinglePane extends PApplet {

/**
 * Slugscapes
 * Visuals
 *
 * Slork 2012
 * Hunter McCurry, Jiffer Harriman, Michael Wilson
 */





// Network Variables
OscP5 oscP5;
NetAddress myRemoteLocation;
int listen_port = 6472;

// Slug things
Slug banjo = new Slug(0);
Slug flute = new Slug(1);;
Slug violin = new Slug(2);;

// Draw things
float max_slug_height = 20; // pixels
int opacity_count = 0;

int last_red_low;
int last_red_high;
int last_green_low;
int last_green_high;
int last_blue_low;
int last_blue_high;

// Global things
int my_machine = 0;
//int mode = 0; // to start

public void setup() {
  size(1280,768, P3D);
  background(0);
  //frameRate(50);
  smooth();
    
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

public void randomShape() {
  
  beginShape(QUADS);
  noStroke();
  fill(random(255), random(255), random(255), 150);
  vertex(random(width),random(height));
  fill(random(255), random(255), random(255), 150);
  vertex(random(width),random(height));
  fill(random(255), random(255), random(255), 150);
  vertex(random(width),random(height));
  fill(random(255), random(255), random(255), 150);
  vertex(random(width),random(height));
  endShape();

}


public void draw() {

  
//  if ( true ) {
//    randomShape();
//  }

  banjo.drawSegment();
  flute.drawSegment();
  violin.drawSegment();
  
  
    strokeWeight(5);
    noFill();
    beginShape(QUADS);
    stroke(255, 255, 255);
    vertex(0,0);
    vertex(width-1,0);
    vertex(width-1,height-1);
    vertex(0,height-1);
    endShape();
    
  if (opacity_count >= 5) {
    // frame and fade
    noStroke();
    fill(0, 0, 0, 1);
    beginShape(QUADS);
    stroke(255, 255, 255);
    vertex(0,0);
    vertex(width-1,0);
    vertex(width-1,height-1);
    vertex(0,height-1);
    endShape();
    opacity_count = 0;
  }
  opacity_count++;
}



public void mousePressed() { // for testing

  /* create a new osc message object */
  OscMessage myMessage = new OscMessage("/e2p");
  
  myMessage.add(40 + random(50)); // example pitch
  myMessage.add(random(1)); // example amplitude
  myMessage.add((int) random(256));
  myMessage.add(random(1)); // example m_1
  myMessage.add(random(1)); // example m_2
  myMessage.add(random(1)); // example m_3
  
  /* send the message */
  oscP5.send(myMessage, myRemoteLocation);
}


public void oscEvent(OscMessage theOscMessage) {
  /* check if theOscMessage has the address pattern we are looking for. */
  
  if(theOscMessage.checkAddrPattern("/pulse")==true) {
    
    // Current Mode, Machine Index
    if(theOscMessage.checkTypetag("ii")) {
      /* parse theOscMessage and extract the values from the osc message arguments. */
      int temp_mode = theOscMessage.get(0).intValue();
      int machine_index = theOscMessage.get(1).intValue();
      
      if (machine_index == 0) {
        banjo.updateMode(temp_mode);
      }
      else if (machine_index == 1) {
        flute.updateMode(temp_mode);
      }
      else if (machine_index == 2) {
        violin.updateMode(temp_mode);
      }
      else {
        println("INVALID MODE MACHINE INDEX");
      }
      
      //print("### received an osc message /slork/slugscapes/pulse with typetag i.");
      //println(" values: " + temp_mode + " " + machine_index);
      return;
    }
  }
    
  else if(theOscMessage.checkAddrPattern("/e2p")==true) {
    
    // Pitch, Amplitude, Machin Index, Frame Number, % machine 0, % machine 1, % machine 2
    if(theOscMessage.checkTypetag("ffiifff")) {
      /* parse theOscMessage and extract the values from the osc message arguments. */
      
      float pitch = theOscMessage.get(0).floatValue();
      float amplitude = theOscMessage.get(1).floatValue();
      int machine_index = theOscMessage.get(2).intValue();
      int frame_num = theOscMessage.get(3).intValue(); // 0 to 255 for now
      float m_1 = theOscMessage.get(4).floatValue();
      float m_2 = theOscMessage.get(5).floatValue();
      float m_3 = theOscMessage.get(6).floatValue();
      
      if (machine_index == 0) {
        banjo.updateInfo(pitch, amplitude, frame_num, m_1, m_2, m_3);
      }
      else if (machine_index == 1) {
        flute.updateInfo(pitch, amplitude, frame_num, m_1, m_2, m_3);
      }
      else if (machine_index == 2) {
        violin.updateInfo(pitch, amplitude, frame_num, m_1, m_2, m_3);
      }
      else {
        println("INVALID ENVELOPE MACHINE INDEX");
      }

      
      //print("### received an osc message /slork/slugscapes/envelope with typetag ffii.");
      //println(" values: " + pitch + " " + amplitude + " " + machine_index + " " + frame_num + " " + m_1 + " " + m_2 + " " + m_3);
      return;
    }
  } 

  println("### received a strange osc message. with address pattern " + theOscMessage.addrPattern());
}


class Slug 
{
  
  int max_height; // tallest --> loudest
  
  int my_inst_num;
  int mode;
  
  int last_low;
  int last_high;
  int last_x;
  int last_r, last_g, last_b = 0;
  
  int cur_height;
  int cur_radius;
  int cur_low;
  int cur_high;
  int cur_x;
  int cur_r, cur_g, cur_b = 0;
  
  int banjo_color = color(255, 0, 0, 150);
  int flute_color = color(0, 255, 0, 150);
  int violin_color = color(0, 0, 255, 150);
 
  Slug(int inst_num) {
    my_inst_num = inst_num;
    
    max_height = 8; // pixels
  
    last_x = height/2;
    last_low = height/2;
    last_high = height/2;
    cur_x = height/2;
    cur_low = height/2;
    cur_high = height/2;
  }
 
  public void updateMode(int mode_num) {
    mode = mode_num;
  }

  //updateInfo(pitch, amplitude, frame_num, m_1, m_2, m_3);
  public void updateInfo(float pitch, float amplitude, int frame_num, float m_1, float m_2, float m_3) {
    
    //println("update info is: " + pitch + " " + amplitude  + " " + frame_num  + " " + m_1  + " " + m_2  + " " + m_3);
    last_x = cur_x;
    last_low = cur_low;
    last_high = cur_high;
    
    last_r = cur_r;
    last_g = cur_g;
    last_b = cur_b;
    
    cur_r = (int) (m_1 * 255);
    cur_g = (int) (m_2 * 255);
    cur_b = (int) (m_3 * 255);
    
    cur_radius = (int) map(amplitude, 0.0f, 1.0f, 0.0f, max_height);
    cur_height = (int) map(pitch, 40, 1200, height, 0);
    cur_x = (int) map(frame_num, 0, 255, 0, width);
    cur_low = cur_height - cur_radius; // minus actually goes up probably
    cur_high = cur_height + cur_radius;
    
    //prev_frame_num = current_frame_num;
    //current_frame_num = frame_num;
  }
  
  public void drawSegment() {
    smooth();
    strokeWeight(2);
    stroke(255, 255, 255);
        
    //ellipse(cur_x, cur_height, cur_radius, cur_radius
    
//    println("last color is: ( " + cur_r + " " + cur_g + " " + cur_b + " )");
//    println("cur color is: ( " + last_r + " " + last_g + " " + last_b + " )");
    
    if ((cur_x > last_x) && ((cur_x - last_x) < 10)) {
//      println(my_inst_num);
//      if (my_inst_num == 0) {
//        stroke(banjo_color);
//        fill(banjo_c  olor);
//      }
//      else if (my_inst_num == 1) {
//        stroke(flute_color);
//        fill(flute_color);
//      }
//      else if (my_inst_num == 2) {
//        stroke(violin_color);
//        fill(violin_color);
//      }
      
      if (mode == 0) {
        beginShape(LINES);
        strokeWeight(2);
        stroke(last_r, last_g, last_b, 150);
        //fill(last_r, last_g, last_b, 150);
        vertex(last_x, last_low);
        
        stroke(cur_r, cur_g, cur_b, 150);
        //fill(cur_r, cur_g, cur_b, 150);
        vertex(cur_x, cur_low);
        vertex(cur_x, cur_high);
        
        stroke(last_r, last_g, last_b, 150);
        //fill(last_r, last_g, last_b, 150);
        vertex(last_x, last_high);
        endShape();
      }
      else if (mode == 4) {
        beginShape(QUADS);
        
        noStroke();
        //stroke(last_r, last_g, last_b, 150);
        fill(last_r, last_g, last_b, 255);
        vertex(last_x, last_low);
        
        //stroke(cur_r, cur_g, cur_b, 150);
        fill(cur_r, cur_g, cur_b, 255);
        vertex(cur_x, cur_low);
        vertex(cur_x, cur_high);
        
        //stroke(last_r, last_g, last_b, 150);
        fill(last_r, last_g, last_b, 255);
        vertex(last_x, last_high);
        endShape();
      }
    }
  }
}

  static public void main(String args[]) {
    PApplet.main(new String[] { "--present", "--bgcolor=#666666", "--hide-stop", "Slugscape_SinglePane" });
  }
}
