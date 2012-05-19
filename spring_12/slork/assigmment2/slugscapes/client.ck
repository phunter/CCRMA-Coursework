////////////////////////////////////////////////////////////
// Slugscapes
// Client module
////////////////////////////////////////////////////////////
// Slork 2012
// Hunter McCurry, Jiffer Harriman, Michael Wilson
////////////////////////////////////////////////////////////
// Dependencies:
// PITCH_ESTIMATE.ck, FFT_AUTO_CORR.ck, RMS_TRACKER.ck
////////////////////////////////////////////////////////////

// Default machine number; override if we pass in a command-line
// argument:
0 => int my_machine_number;
if (me.args() > 0)
{
    Std.atoi(me.arg(0)) => my_machine_number;
}

1.0 => float inputGain;
300.0 => float freq_lower_bound;
1200.0 => float freq_upper_bound;
if (my_machine_number == 0) // banjo
{
    2.5 => inputGain;
    300.0 => freq_lower_bound;
    1200.0 => freq_upper_bound;
}
else if (my_machine_number == 1) // flute
{
    300.0 => freq_lower_bound;
    2400.0 => freq_upper_bound;
}
else if (my_machine_number == 2) // fiddle
{
    2.0 => inputGain;
    300.0 => float freq_lower_bound;
    2400.0 => float freq_upper_bound;
}

OscRecv recv;
Config.server_to_client_port => recv.port;
recv.listen();

OscSend xmit_to_server;
xmit_to_server.setHost( Config.server_name(), Config.client_to_server_port );

OscSend xmit_to_synth;
xmit_to_synth.setHost( "localhost", Config.synth_port );

// Mode we are currently in:
Config.MODE_IDLE => int current_mode;

// Listen for mode change messages:
fun void listen_pulse()
{
    recv.event( Config.osc_message_pulse(), Config.osc_message_pulse_data() ) @=> OscEvent oe_pulse;
    while ( true )
    {
        oe_pulse => now;
        while( oe_pulse.nextMsg() != 0 )
        {
            oe_pulse.getInt() => int mode;
            oe_pulse.getInt() => int index;
            if (index == my_machine_number)
            {
                mode => current_mode;
            }
            <<< "Client", my_machine_number, "changed to mode", current_mode >>>;
        }
    }    
}

// Listen for envelope change messages, and play them through the synth:
fun void listen_envelope()
{
    recv.event( Config.osc_message_envelope_to_client(), Config.osc_message_envelope_to_client_data() ) @=> OscEvent oe_envelope;
    while ( true )
    {
        oe_envelope => now;
        while( oe_envelope.nextMsg() != 0 )
        {
            oe_envelope.getFloat() => float pitch;
            oe_envelope.getFloat() => float amplitude;
            // Forward message to synth:
            xmit_to_synth.startMsg( Config.osc_message_envelope_to_client(), Config.osc_message_envelope_to_client_data() );
            pitch => xmit_to_synth.addFloat;
            amplitude => xmit_to_synth.addFloat;
            //<<< "Got envelope: ", pitch, amplitude >>>;
        }
    }
}

// Send envelope when we're recording:
fun void send_envelope()
{
    while( true )
    {
        if (current_mode == Config.MODE_COLLECTING)
        {
            xmit_to_server.startMsg( Config.osc_message_envelope_to_server(), Config.osc_message_envelope_to_server_data() );
            PITCH_ESTIMATE.freqEst => float freq;
            if (freq > 0)
            {        
                while(freq < freq_lower_bound)
                {
                    freq * 2 => freq;
                }
                while(freq > freq_upper_bound)
                {
                    freq / 2 => freq;
                }
            }
            freq => xmit_to_server.addFloat; // Pitch
            inputGain * RMS_TRACKER.rmsSumShorter => xmit_to_server.addFloat; // Amplitude
            my_machine_number => xmit_to_server.addInt; // Who are you?
        }
        // The RMS tracker updates about every 50 ms (2048 frame FFTs)
        50::ms => now;
    }
}

spork ~listen_pulse();
spork ~listen_envelope();
spork ~send_envelope();

while (true)
{
    1::day => now;   
}
