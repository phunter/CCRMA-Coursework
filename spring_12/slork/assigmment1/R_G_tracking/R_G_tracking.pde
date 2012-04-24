/**
 * Red and Green Tracking by Hunter McCurry
 * 
 * Modified from Brightness Tracking by Golan Levin. 
 *
 */

import oscP5.*;
import netP5.*;


OscP5 osc;
NetAddress local;

import processing.video.*;

Capture video;

int threshold = 150;


void setup() {
  size(640, 480); // Change size to 320 x 240 if too slow at 640 x 480
  
  frameRate(60);
  background(0);
  
  osc = new OscP5(this, 12000);
  local = new NetAddress("localhost", 6449);
  
  // Uses the default video input, see the reference if this causes an error
  video = new Capture(this, width, height, 30);
  
  //colorMode(HSB, 1.0);
  noStroke();
  smooth();
}

void sendMessage(int c, float x, float y) {
  OscMessage msg = new OscMessage("/point");
  msg.add(c);
  msg.add(x);
  msg.add(y);
  osc.send(msg, local);
}

void draw() {
  if (video.available()) {
    video.read();
    
    int redestX = 0;
    int redestY = 0;
    int greenestX = 0;
    int greenestY = 0;
    
    float redestValue = 0;
    float greenestValue = 0;
    
    // Search for the brightest pixel: For each row of pixels in the video image and
    // for each pixel in the yth row, compute each pixel's index in the video
    
    video.loadPixels();
    int index = 0;
    for (int y = 0; y < video.height; y++) {
      for (int x = 0; x < video.width; x++) {
        // Get the color stored in the pixel
        int pixelValue = video.pixels[index];
        float pixelRedness = red(pixelValue);       
        float pixelGreenness = green(pixelValue);
        float pixelBlueness = blue(pixelValue);
        
        // If that value is reder than any previous, then store the
        // redness of that pixel, as well as its (x,y) location
        if ((pixelRedness > redestValue) && (pixelGreenness < 120) && (pixelBlueness < 120)) {
          redestValue = pixelRedness;
          redestY = y;
          redestX = x;
        }
        // If that value is greener than any previous, then store the
        // greenness of that pixel, as well as its (x,y) location
        if ((pixelGreenness > greenestValue) && (pixelRedness < 120) && (pixelBlueness < 120)) {
          greenestValue = pixelGreenness;
          greenestY = y;
          greenestX = x;
        }
        
        index++;
      }
    }
    // Draw a Big semi-transparent black rectangle to clear screen
    noStroke();
    fill(0, 10);
    rect(0, 0, width, height);
    
    
    if (redestValue > threshold) {
      println("red: ( " + redestX + " , " + redestY + " )");
      
      float redX = redestX;
      float redY = redestY;
      sendMessage(1, redX / width , redY / height);
      //sendMessage(0, redestX, redestY);
      
      // Draw a red circle
      fill(255, 0, 0, 200);
      ellipse(width - redestX, redestY, 20, 20);
    }
    
    if (greenestValue > threshold) {
      println("green: ( " + greenestX + " , " + greenestY + " )");
      
      float greenX = greenestX;
      float greenY = greenestY;
      sendMessage(0, greenX / width , greenY / height);
      
      // Draw a green circle
      fill(0, 255, 0, 200);
      ellipse(width - greenestX, greenestY, 20, 20);
    }
  }
}
