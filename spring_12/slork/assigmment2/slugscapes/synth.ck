////////////////////////////////////////////////////////////
// Slugscapes
// Synth module
////////////////////////////////////////////////////////////
// Slork 2012
// Hunter McCurry, Jiffer Harriman, Michael Wilson
////////////////////////////////////////////////////////////

0 => int my_machine_number;

if (me.args() > 0)
{
    Std.atoi(me.arg(0)) => my_machine_number;
}

///
// synth patch
SinOsc m => SinOsc c => LPF f => ADSR e => Envelope g => JCRev r => Dyno limiter;
SinOsc vib => c;
Noise n => LPF n_f => ADSR n_e => Gain n_g => r;
SawOsc bass => LPF bass_f => ADSR bass_e => Gain bass_g => r;
0.0 => bass_g.gain;
650.0 => bass_f.freq;
// set dyno to limiter
limiter.limit();

for (0 => int index; index < dac.channels(); index++)
{
    limiter => dac.chan(index);
}

0.02 => r.mix;
10::ms => g.duration;
// FM
2 => c.sync;
float fm_ratio;
// vibrato
6 => vib.freq;
10 => vib.gain;
Envelope vib_rate;
Envelope vib_depth;
vib_depth => blackhole;
vib_rate => blackhole;
300::ms => dur vib_delay;
10.0 => float vib_depth_amount;
// octave
1.0 => float octave;

// Filter Sweep
Envelope f_sweep;
f_sweep => blackhole;
// noise burst
n_e.keyOff();
n_e.set(15::ms, 5::ms, .7, 10::ms);
0.0 => n_g.gain;
5 => n_f.Q;
2000.0 => n_f.freq;
10::ms => dur n_dur;


// not quite an ADSR
// attack is instant
// specify start freq, decay time to sustain freq1, release time to sustained freq2
fun void filtSweep(float start, dur decay, float sustain, dur release, float end){
    // initial filter freq value:
    start => f_sweep.value;
    // set the initial target freq
    sustain => f_sweep.target;
    // set init target time
    decay => f_sweep.duration;
    // wait for the initial decay time
    decay => now;
    // set the 2nd target freq
    end => f_sweep.target;
    // set the 2nd decay time
    release => f_sweep.duration;  
}
dur sweepDecayTime;
float sweepStartRatio;
float sweepSusRatio;
dur sweepRelTime;
float sweepRelVal;

// specify when to start sweeping to the vibrato in
// and how long to get to full sustained vibrato
// depth is in Hz
fun void vibSweepDepth(dur delay, dur ramp_duration, float depth){
    // initial vibrato value:
    0 => vib_depth.value;
    0 => vib_depth.target;
    // wait for it..
    delay => now;
    // set the target depth and ramp rate
    depth => vib_depth.target;
    ramp_duration => vib_depth.duration;    
}

fun void noiseHit(dur duration){
    n_e.keyOn();
    
    duration => now;
    n_e.keyOff();
}

// trigger the ADSR and the filter sweep function
fun void trigger(){
    e.keyOn();
    bass_e.keyOn();
    spork ~ vibSweepDepth(vib_delay, 100::ms, vib_depth_amount);
    spork ~ noiseHit(n_dur);
    filtSweep(sweepStartRatio * c.freq(), sweepDecayTime, sweepSusRatio * c.freq(), sweepRelTime, sweepRelVal);
}

// pitch glide
Envelope portamento => blackhole;

if (my_machine_number == 0) // banjo
{
    <<<"loading banjo">>>;
    // set envelope
    e.set(10::ms, 200::ms, 0.3, 200::ms);
    15::ms => portamento.duration;
    // FM 
    600 => m.gain;
    1/1.5 => fm_ratio;
    // filter 
    3 => f.Q;
    // filter sweep
    60::ms => sweepDecayTime;
    5.0 => sweepStartRatio;
    1.1 => sweepSusRatio;
    1::second => sweepRelTime;
    40 => sweepRelVal;
    // octave
    0.5 => octave;
    // noise burst
    0.25 => n_g.gain;
    1200 => n_f.freq;
    8 => n_f.Q;
    10::ms => n_dur;
    n_e.set(5::ms, 5::ms, .7, 10::ms);
    // Vibrato
    100::ms => dur vib_delay;
    0.0 => float vib_depth_amount;
    // bass
    .5 => bass_g.gain;
    
}

else if (my_machine_number == 1) // flute
{
    <<<"loading flute">>>;
    e.set(10::ms, 20::ms, .6, 100::ms);
    25::ms => portamento.duration;
    // FM index
    40 => m.gain;
    2.05 => fm_ratio;
    // filter
    1 => f.Q;
    // filter sweep
    500::ms => sweepDecayTime;
    4.0 => sweepStartRatio;
    3.0 => sweepSusRatio;
    1::second => sweepRelTime;
    1000 => sweepRelVal;
    // octave
    2.0 => octave;
    // noise burst
    .15 => n_g.gain;
    900 => n_f.freq;
    4 => n_f.Q;
    20::ms => n_dur;
    n_e.set(10::ms, 5::ms, .5, 5::ms);
    // Vibrato
    300::ms => dur vib_delay;
    10.0 => float vib_depth_amount;
}
else if (my_machine_number == 2) // fiddle
{
    <<<"loading fiddle">>>;
    e.set(100::ms, 50::ms, .7, 100::ms);
    40::ms => portamento.duration;
    // FM index
    1500 => m.gain;
    3.03 => fm_ratio;
    // filter
    3.0 => f.Q;
    // filter sweep
    180::ms => sweepDecayTime;
    4.0 => sweepStartRatio;
    2.0 => sweepSusRatio;
    2::second => sweepRelTime;
    500 => sweepRelVal;
    //octave
    1.0  => octave;
    // noise burst
    0.1 => n_g.gain;
    1600 => n_f.freq;
    3 => n_f.Q;
    5::ms => n_dur;
    n_e.set(15::ms, 5::ms, .4, 10::ms);
    // Vibrato
    200::ms => dur vib_delay;
    10.0 => float vib_depth_amount;
}


// Receive data via OSC 
OscRecv recv;
Config.synth_port => recv.port;
recv.listen();

float pitch;
float amplitude;

fun void listen_envelope()
{
    recv.event( Config.osc_message_envelope_to_client(), Config.osc_message_envelope_to_client_data() ) @=> OscEvent oe_envelope;
    while ( true )
    {
        oe_envelope => now;
        while( oe_envelope.nextMsg() != 0 )
        {
            oe_envelope.getFloat() => pitch; //pitch;
            
            
            pitch => portamento.target;
            
            oe_envelope.getFloat() => amplitude;
            
            //<<< "Synth Got envelope: ", pitch, amplitude >>>;
        }
    }
}

spork ~listen_envelope();


0 => int noteOn;
float amp_history[10];
int amp_ptr;
1.0 => float note_threshold;

fun void note_on_off(){
    while(1){
        // check if the amplitude is changing and above the threshold
        0 => int varying_amplitude;
        0 => int above_thresh;
        
        // compare to the last 5 values to make sure it's varying
        for (0 => int n; n < 10; n++){
            // check if above threshold
            if (amplitude > note_threshold)
                1 => above_thresh;
            // check if it is changing
            if (amplitude != amp_history[n])
                1 => varying_amplitude;
        }
        
        if (above_thresh && varying_amplitude)
        {
            1 => noteOn;
        }
        else {
            0 => noteOn;
        }
        
        amplitude => amp_history[amp_ptr];
        amp_ptr++;
        if (amp_ptr == 10)
            0 => amp_ptr;
        
        // this is how often the server is sending notes
        25::ms => now;
    }
}

spork ~ note_on_off();

fun void play(){
    while(1){
        if (noteOn){
            // if state =  done (4), or release(3)?
            if (e.state() == 4 || e.state() == 3)
            {
                //<<< "note on" >>>;
                spork ~ trigger();
            }
            portamento.value() => c.freq;
            octave * c.freq() * fm_ratio => m.freq;
            if (my_machine_number == 0 ){ //banjo
                .5 * c.freq() => bass.freq;
            }
            
            amplitude / 10 => float target;
            if (target > .95)
                .95 => target;
            
            target => g.target;
            
        }
        else{
            //<<< "note off" >>>;
            e.keyOff();
            bass_e.keyOff();
        }
        
        // update the filter frequency envelope
        f_sweep.value() => f.freq;
        vib_depth.value() => vib.gain;
        //<<< f.freq()>>>;
        10::ms => now;
    }
}

spork ~play();

while(1)
{
    1::day => now;
}
