// (launch with OSC_send.ck)

// the patch
SndBuf buf => dac;
// load the file
"snare.wav" => buf.read;
// don't play yet
0 => buf.play; 

// create our OSC receiver
OscRecv recv;
// use port 6449
6449 => recv.port;
// start listening (launch thread)
recv.listen();

// create an address in the receiver, store in new variable
recv.event( "/point, i, f, f" ) @=> OscEvent oe;


class Drone
{
    // patch
    VoicForm voc => dac;
    
    // settings
    // voc.freq;
    0.0 => voc.gain;
    //.8 => r.gain;
    //.2 => r.mix;
    0.5 => voc.loudness;
    0.01 => voc.vibratoGain;
    
    2000.0 => float freqXrange;
    300.0 => float freqYrange;
    
    40 => int bphonMax;
    1 => int bphon;
    1 => int up;
    
    public void setPitch( int noteNum )
    { Std.mtof( noteNum ) => voc.freq; }
}    

Drone x, y;
float droneXloudness;
float droneXbphon;
float droneXfreq;
x.setPitch(30);

float droneYloudness;
float droneYbphon;
float droneYfreq;
y.setPitch(70);

spork ~ playDrone1();

spork ~ playDrone2();

// infinite event loop
while ( true )
{
    // wait for event to arrive
    oe => now;

    // grab the next message from the queue. 
    while ( oe.nextMsg() != 0 )
    { 
        int color;
        float xVal;
        float yVal;
        
        oe.getInt() => color;
        
        <<< "Color is : " + color >>>;
        
        oe.getFloat() => xVal;
        oe.getFloat() => yVal;
        
        if (color == 1) { // red
            xVal => droneXbphon;
            yVal => droneXfreq;
            0.5 => droneXloudness;
            
            //<<< droneBphon >>>;
        }
        
        if (color == 0) { // green
            xVal => droneYbphon;
            yVal => droneYfreq;
            0.5 => droneYloudness;
            
            //<<< droneBphon >>>;
        }
        
        //.5 + .5 * color => buf.play;
        
        // print
        //<<< "got (via OSC):", buf.play() >>>;
        //<<< "got (via OSC): color " + color + " at (" + xVal + ", " + yVal + ")">>>;
        
        // set play pointer to beginning
        //0 => buf.pos;
    }
}



// instrument functions below
//////////////////////////////



fun void playDrone1() {
    while( true )
    {
        droneXloudness => x.voc.gain;
        
        if (droneXloudness > 0.0) {
            droneXloudness - .01 => droneXloudness;
        }
        
        x.bphon => x.voc.phonemeNum;
        
        //<<< (droneXbphon * x.bphonMax) $ int >>>;
        
        5 +(droneXbphon * x.bphonMax) $ int => x.bphon;
        
        200.0 + ((1.0 - droneXfreq) * x.freqXrange) => x.voc.freq;
                
        10::ms => now;
    }
}

fun void playDrone2() {
    while( true )
    {
        droneYloudness => y.voc.gain;
        
        if (droneYloudness > 0.0) {
            droneYloudness - .01 => droneYloudness;
        }
        
        y.bphon => y.voc.phonemeNum;
        
        //<<< (droneYbphon * x.bphonMax) $ int >>>;
        
        10 +(droneYbphon * y.bphonMax) $ int => y.bphon;
        
        1.0 + ((1.0 - droneYfreq) * y.freqYrange) => y.voc.freq;
        
        10::ms => now;
    }
}

