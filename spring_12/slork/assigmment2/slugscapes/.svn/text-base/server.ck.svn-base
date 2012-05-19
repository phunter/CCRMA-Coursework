////////////////////////////////////////////////////////////
// Slugscapes
// Server module
////////////////////////////////////////////////////////////
// Slork 2012
// Hunter McCurry, Jiffer Harriman, Michael Wilson
////////////////////////////////////////////////////////////

// Current mode for each client
int current_mode[Config.NUMCLIENTS];
// Envelope and amplitude data that we collect:
float sample_pitches[Config.NUMCLIENTS][Config.NUMFRAMES];
float sample_amplitudes[Config.NUMCLIENTS][Config.NUMFRAMES];
// Indexes for collecting data:
int sample_index[Config.NUMCLIENTS];
int record_stop_index[Config.NUMCLIENTS];
int start_index; // Should be greater than or equal to 0
int stop_index; // Should be up to and including Config.NUMFRAMES-1
// Space to store calculated average values:
float average_pitches[Config.NUMFRAMES];
float average_amplitudes[Config.NUMFRAMES];
float average_contribs[Config.NUMCLIENTS][Config.NUMFRAMES];
// OSC transmission ports:
OscSend xmit_to_client[Config.NUMCLIENTS];
OscSend xmit_to_processing[Config.NUMCLIENTS];

// Initialize:
for (0 => int index; index < Config.NUMCLIENTS; index++)
{
    xmit_to_client[index].setHost ( Config.host_name(index), Config.server_to_client_port );
    xmit_to_processing[index].setHost ( Config.host_name(index), Config.server_to_processing_port );
    Config.MODE_IDLE => current_mode[index];
    (Config.NUMFRAMES-1) => record_stop_index[index];
}

// OSC receive ports:
OscRecv recv;
Config.client_to_server_port => recv.port;
recv.listen();

////////////////////////////////////////////////////////////////////////////////

// Send mode change messages immediately to all clients and their processing instances:
fun void broadcast_mode_message()
{
    for( 0 => int index; index < Config.NUMCLIENTS; index++ )
    {
        xmit_to_client[index].startMsg( Config.osc_message_pulse(), Config.osc_message_pulse_data() );
        current_mode[index] => xmit_to_client[index].addInt;
        index => xmit_to_client[index].addInt;
	    (Config.osc_burst_delay)::ms => now;
        for( 0 => int index2; index2 < Config.NUMCLIENTS; index2++ )
        {
            xmit_to_processing[index2].startMsg( Config.osc_message_pulse(), Config.osc_message_pulse_data() );
            current_mode[index] => xmit_to_processing[index2].addInt;
            index => xmit_to_processing[index2].addInt;
	        (Config.osc_burst_delay)::ms => now;
        }
    }
}

// Send play information immediately to a client
fun void transmit_envelope_to_client(int client_index, float pitch, float amplitude)
{
    xmit_to_client[client_index].startMsg( Config.osc_message_envelope_to_client(), Config.osc_message_envelope_to_client_data() );
    pitch => xmit_to_client[client_index].addFloat;
    amplitude => xmit_to_client[client_index].addFloat;
}

// Send play information immediately to all processing instances
fun void transmit_envelope_to_processing(float pitch, float amplitude, int machine_index, int frame_number, float contrib0, float contrib1, float contrib2)
{
    for (0 => int client_index; client_index < Config.NUMCLIENTS; client_index++)
    {
        xmit_to_processing[client_index].startMsg( Config.osc_message_envelope_to_processing(), Config.osc_message_envelope_to_processing_data() );
        pitch => xmit_to_processing[client_index].addFloat;
        amplitude => xmit_to_processing[client_index].addFloat;
        machine_index => xmit_to_processing[client_index].addInt;
        frame_number => xmit_to_processing[client_index].addInt;
        contrib0 => xmit_to_processing[client_index].addFloat;
        contrib1 => xmit_to_processing[client_index].addFloat;
        contrib2 => xmit_to_processing[client_index].addFloat;
	    (Config.osc_burst_delay)::ms => now;
    }
}

////////////////////////////////////////////////////////////////////////////////

// Calculate contributions:
fun float contrib0_from_index(int which_client)
{
    if (which_client == 0)
    {
        return 1.0;
    }
    else
    {
        return 0.0;
    }
}
fun float contrib1_from_index(int which_client)
{
    if (which_client == 1)
    {
        return 1.0;
    }
    else
    {
        return 0.0;
    }
}
fun float contrib2_from_index(int which_client)
{
    if (which_client == 2)
    {
        return 1.0;
    }
    else
    {
        return 0.0;
    }
}

// Calculate average values:
fun void do_averaging()
{
    // TODO maybe make this able to just average two lines as well?
    float max_amplitude;
    for( 0 => int counter; counter < Config.NUMFRAMES; counter++ )
    {
        0 => average_pitches[counter];
        0 => average_amplitudes[counter];
        0 => max_amplitude;
        for( 0 => int index; index < Config.NUMCLIENTS; index++ )
        {
            // TODO weight pitch based on amplitude
            sample_pitches[index][counter] + average_pitches[counter] => average_pitches[counter];
            sample_amplitudes[index][counter] + average_amplitudes[counter] => average_amplitudes[counter];
            if (sample_amplitudes[index][counter] > max_amplitude)
            {
                sample_amplitudes[index][counter] => max_amplitude;
            }
        }
        for( 0 => int index; index < Config.NUMCLIENTS; index++ )
        {
            sample_amplitudes[index][counter] / max_amplitude => average_contribs[index][counter];
        }
        average_pitches[counter] / Config.NUMCLIENTS => average_pitches[counter];
        average_amplitudes[counter] / Config.NUMCLIENTS => average_amplitudes[counter];
    }
}

////////////////////////////////////////////////////////////////////////////////

// Check if any mode is the specified mode:
fun int any_mode_is(int mode)
{
    0 => int result;
    for (0 => int index; index < Config.NUMCLIENTS; index++)
    {
        if (current_mode[index] == mode)
        {
            1 => result;
        }
    }
    return result;
}

// Switch these things on:
fun void change_modes(int who[], int mode)
{
    for( 0 => int index; index < Config.NUMCLIENTS; index++ )
    {
        if (who[index])
        {
            mode => current_mode[index];
        }
    }
    broadcast_mode_message();
}

// Set up our recording:
// Start should be >= 0
// Stop should be <= Config.NUMFRAMES-1
fun void set_recording(int who[], int start, int stop)
{
    for( 0 => int index; index < Config.NUMCLIENTS; index++ )
    {
        if (who[index])
        {
            start => sample_index[index];
            stop => record_stop_index[index];
        }
    }
    change_modes(who, Config.MODE_COLLECTING);
}

// Wait for recordings to finish
fun void wait_on_recordings()
{
    while (any_mode_is(Config.MODE_COLLECTING))
    {
        50::ms => now;
    }
    broadcast_mode_message();
}

////////////////////////////////////////////////////////////////////////////////

// Play interpolated lines:
// Amount: 0.0 is all original, 1.0 is all average
fun void interpolate_lines(int who[], float amount, dur timing)
{
    change_modes(who, Config.MODE_PLAYBACK);
    for( start_index => int counter; counter <= stop_index; counter++ )
    {
        now + timing => time goal;
        for( 0 => int index; index < Config.NUMCLIENTS; index++ )
        {
            if (who[index])
            {
                ((1.0 - amount) * sample_pitches[index][counter]) + (amount * average_pitches[counter]) => float interpolated_pitch;
                ((1.0 - amount) * sample_amplitudes[index][counter])  + (amount * average_amplitudes[counter]) => float interpolated_amplitude;
                ((1.0 - amount) * contrib0_from_index(index)) + (amount * average_contribs[0][counter]) => float interpolated_contrib0;
                ((1.0 - amount) * contrib1_from_index(index)) + (amount * average_contribs[1][counter]) => float interpolated_contrib1;
                ((1.0 - amount) * contrib2_from_index(index)) + (amount * average_contribs[2][counter]) => float interpolated_contrib2;
                transmit_envelope_to_client(index, interpolated_pitch, interpolated_amplitude);
	            (Config.osc_burst_delay)::ms => now;
                transmit_envelope_to_processing(interpolated_pitch, interpolated_amplitude, index, counter, interpolated_contrib0, interpolated_contrib1, interpolated_contrib2);
	            (Config.osc_burst_delay)::ms => now;
            }
        }
        if (now < goal)
        {
            goal => now;
        }
    }
    change_modes(who, Config.MODE_IDLE);
}

// Play some other lines:
fun void play_these_lines(int who[], int what[], dur timing)
{
    change_modes(who, Config.MODE_PLAYBACK);
    for( start_index => int counter; counter <= stop_index; counter++ )
    {
        now + timing => time goal;
        for( 0 => int index; index < Config.NUMCLIENTS; index++ )
        {
            if (who[index])
            {
                transmit_envelope_to_client(index, sample_pitches[what[index]][counter], sample_amplitudes[what[index]][counter]);
	            (Config.osc_burst_delay)::ms => now;
                transmit_envelope_to_processing(sample_pitches[what[index]][counter], sample_amplitudes[what[index]][counter], index, counter, contrib0_from_index(index), contrib1_from_index(index), contrib2_from_index(index));
	            (Config.osc_burst_delay)::ms => now;
            }
        }
        if (now < goal)
        {
            goal => now;
        }
    }
    change_modes(who, Config.MODE_IDLE);
}

// Send data to play to clients:
// who[] is an array of 0 or 1, 1 means that instrument plays, 0 means not to play
fun void play_your_line(int who[], dur timing)
{
    int what[Config.NUMCLIENTS];
    for( 0 => int index; index < Config.NUMCLIENTS; index++ )
    {
        index => what[index];
    }
    play_these_lines(who, what, timing);
}

////////////////////////////////////////////////////////////////////////////////

// Shred to listen for responses from clients:
fun void listen_envelope()
{
    recv.event( Config.osc_message_envelope_to_server(), Config.osc_message_envelope_to_server_data() ) @=> OscEvent oe_envelope;
    while ( true )
    {
        oe_envelope => now;
        while( oe_envelope.nextMsg() != 0 )
        {
            oe_envelope.getFloat() => float pitch;
            oe_envelope.getFloat() => float amplitude;
            oe_envelope.getInt() => int machine_number;
            if (current_mode[machine_number] == Config.MODE_COLLECTING)
            {
                sample_index[machine_number] => int machine_sample_index;
                // If we're not already full here:
                if (machine_sample_index < Config.NUMFRAMES)
                {
                    // Store the data:
                    pitch => sample_pitches[machine_number][machine_sample_index];
                    amplitude => sample_amplitudes[machine_number][machine_sample_index];

                    // <<< "Server storing machine", machine_number, "sample", machine_sample_index, "pitch", pitch, "amplitude", amplitude >>>;

                    // Echo the data back to the processing instances of each client:
                    transmit_envelope_to_processing(pitch, amplitude, machine_number, machine_sample_index, contrib0_from_index(machine_number), contrib1_from_index(machine_number), contrib2_from_index(machine_number));

                    // Increment the sample index:
                    sample_index[machine_number] + 1 => sample_index[machine_number];

                    // Check if we're full, and if we are then transition out of this mode:
                    if (sample_index[machine_number] > record_stop_index[machine_number])
                    {
                        Config.MODE_IDLE => current_mode[machine_number];
                    }
                }
            }
        }
    }
}

// Periodically transmit the current mode
fun void send_pulses()
{
    (1000 - (Config.NUMCLIENTS*(1+Config.NUMCLIENTS)*Config.osc_burst_delay))::ms => dur pulse_delay;
    while (true)
    {
	    pulse_delay => now;
        broadcast_mode_message();
    }
}

////////////////////////////////////////////////////////////////////////////////

// Spork shreds:
spork ~listen_envelope();
spork ~send_pulses();

// Pre-delay for clients to come up:
2::second => now;

////////////////////////////////////////////////////////////////////////////////

// Local test:
if (Config.NUMCLIENTS != 3)
{
    while (true)
    {
        // Collect data:
        0 => sample_index[0];
        0 => start_index;
        (Config.NUMFRAMES-1) => stop_index;
        Config.MODE_COLLECTING => current_mode[0];
        broadcast_mode_message();

        // Wait for solo to finish:
        while (current_mode[0] == Config.MODE_COLLECTING)
        {
            50::ms => now;
        }
        broadcast_mode_message();

        // Play back solo:
        Config.MODE_PLAYBACK => current_mode[0];
        broadcast_mode_message();
        play_your_line([1], 50::ms);

        Config.MODE_IDLE => current_mode[0];
        broadcast_mode_message();
        1::second => now;
    }
}

////////////////////////////////////////////////////////////////////////////////

// // Delay a bit for clients to react
// 2::second => now;

// // Maybe we want something like "record and play"

// /////////////////////////
// <<< "Solos" >>>;
// /////////////////////////

// 0 => start_index;
// (Config.NUMFRAMES-1) => stop_index;

// set_recording([1,0,0], 0, Config.NUMFRAMES-1);
// wait_on_recordings();

// set_recording([0,1,0], 0, Config.NUMFRAMES-1);
// wait_on_recordings();

// set_recording([0,0,1], 0, Config.NUMFRAMES-1);
// wait_on_recordings();

// play_your_line([1,0,0], 50::ms);
// play_your_line([0,1,0], 50::ms);
// play_your_line([0,0,1], 50::ms);


// /////////////////////////
// <<< "Interrupted solos" >>>;
// /////////////////////////

// // Solo with interruptions:
// 0 => start_index;
// (Config.NUMFRAMES / 4) => stop_index;
// set_recording([0,0,1], start_index, stop_index);
// wait_on_recordings();
// play_your_line([0,0,1], 50::ms);

// stop_index+1 => start_index;
// (2*Config.NUMFRAMES / 4) => stop_index;
// set_recording([0,1,0], start_index, stop_index);
// wait_on_recordings();
// play_your_line([0,1,0], 50::ms);

// stop_index+1 => start_index;
// (3*Config.NUMFRAMES / 4) => stop_index;
// set_recording([1,0,0], start_index, stop_index);
// wait_on_recordings();
// play_your_line([1,0,0], 50::ms);

// stop_index+1 => start_index;
// (Config.NUMFRAMES-1) => stop_index;
// set_recording([1,1,1], start_index, stop_index);
// wait_on_recordings();
// play_your_line([1,1,1], 50::ms);

// 0 => start_index;
// (Config.NUMFRAMES-1) => stop_index;


// /////////////////////////
// <<< "Averaging" >>>;
// /////////////////////////
// do_averaging();
// interpolate_lines([1,1,1], 0.0, 10::ms);
// interpolate_lines([1,1,1], 0.2, 10::ms);
// interpolate_lines([1,1,1], 0.4, 10::ms);
// interpolate_lines([1,1,1], 0.6, 10::ms);
// interpolate_lines([1,1,1], 0.8, 10::ms);
// interpolate_lines([1,1,1], 1.0, 10::ms);


// /////////////////////////
// <<< "Computer Duets" >>>;
// /////////////////////////

// 0 => start_index;
// (Config.NUMFRAMES-1) => stop_index;

// set_recording([1,0,0], 0, Config.NUMFRAMES-1);
// play_your_line([0,0,1], 50::ms);
// wait_on_recordings();

// set_recording([0,1,0], 0, Config.NUMFRAMES-1);
// play_your_line([1,0,0], 50::ms);
// wait_on_recordings();

// set_recording([0,0,1], 0, Config.NUMFRAMES-1);
// play_your_line([0,1,0], 50::ms);
// wait_on_recordings();


// /////////////////////////
// <<< "Averaging" >>>;
// /////////////////////////
// do_averaging();
// interpolate_lines([1,1,1], 0.25, 50::ms);
// interpolate_lines([1,1,1], 0.5, 50::ms);
// interpolate_lines([1,1,1], 0.75, 50::ms);
// interpolate_lines([1,1,1], 1.0, 50::ms);


// /////////////////////////
// <<< "Duets" >>>;
// /////////////////////////
// set_recording([1,1,0], 0, Config.NUMFRAMES-1);
// wait_on_recordings();
// play_your_line([1,1,0], 25::ms);

// set_recording([0,1,1], 0, Config.NUMFRAMES-1);
// wait_on_recordings();
// play_your_line([0,1,1], 25::ms);

// set_recording([1,0,1], 0, Config.NUMFRAMES-1);
// wait_on_recordings();
// play_your_line([1,0,1], 25::ms);


// /////////////////////////
// <<< "Unison" >>>;
// /////////////////////////
// set_recording([1,1,1], 0, Config.NUMFRAMES-1);
// wait_on_recordings();
// play_your_line([1,1,1], 50::ms);


// <<< "The end!" >>>;

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Actual main performance (assume we have exactly 3 clients):
////////////////////////////////////////////////////////////////////////////////

// Delay a bit for clients to react
2::second => now;

/////////////////////////
<<< "Human / Computer Solos" >>>;
/////////////////////////

0 => start_index;
(Config.NUMFRAMES-1) => stop_index;

set_recording([0,0,1], 0, Config.NUMFRAMES-1);
wait_on_recordings();
play_your_line([0,0,1], 50::ms);

set_recording([1,0,0], 0, Config.NUMFRAMES-1);
wait_on_recordings();
play_your_line([1,0,0], 50::ms);

set_recording([0,1,0], 0, Config.NUMFRAMES-1);
wait_on_recordings();
play_your_line([0,1,0], 50::ms);


/////////////////////////
<<< "Interrupted solos" >>>;
/////////////////////////

0 => start_index;
(Config.NUMFRAMES / 4) => stop_index;
set_recording([1,0,0], start_index, stop_index);
wait_on_recordings();
play_your_line([1,0,0], 50::ms);

stop_index+1 => start_index;
(2*Config.NUMFRAMES / 4) => stop_index;
set_recording([1,0,0], start_index, stop_index);
wait_on_recordings();
play_your_line([1,0,0], 50::ms);

0 => start_index;
(Config.NUMFRAMES / 4) => stop_index;
set_recording([0,1,0], start_index, stop_index);
wait_on_recordings();
play_your_line([0,1,0], 50::ms);

stop_index+1 => start_index;
(2*Config.NUMFRAMES / 4) => stop_index;
set_recording([0,1,0], start_index, stop_index);
wait_on_recordings();
play_your_line([0,1,0], 50::ms);

0 => start_index;
(Config.NUMFRAMES / 4) => stop_index;
set_recording([0,0,1], start_index, stop_index);
wait_on_recordings();
play_your_line([0,0,1], 50::ms);

stop_index+1 => start_index;
(2*Config.NUMFRAMES / 4) => stop_index;
set_recording([0,0,1], start_index, stop_index);
wait_on_recordings();
play_your_line([0,0,1], 50::ms);


/////////////////////////
<<< "Averaging" >>>;
/////////////////////////

0 => start_index;
(Config.NUMFRAMES-1) => stop_index;

do_averaging();
interpolate_lines([1,1,1], 0.0, 40::ms);
interpolate_lines([1,1,1], 0.33, 40::ms);
interpolate_lines([1,1,1], 0.66, 40::ms);
interpolate_lines([1,1,1], 1.0, 40::ms);


/////////////////////////
<<< "Solos in thirds" >>>;
/////////////////////////

0 => start_index;
(Config.NUMFRAMES / 3) => stop_index;
set_recording([1,0,0], start_index, stop_index);
wait_on_recordings();

stop_index+1 => start_index;
(2*Config.NUMFRAMES / 3) => stop_index;
set_recording([0,1,0], start_index, stop_index);
wait_on_recordings();

stop_index+1 => start_index;
Config.NUMFRAMES-1 => stop_index;
set_recording([0,0,1], start_index, stop_index);
wait_on_recordings();



/////////////////////////
<<< "Middle section overlaps:" >>>;
/////////////////////////

0 => start_index;
(Config.NUMFRAMES-1) => stop_index;

set_recording([0,1,1], 0, Config.NUMFRAMES-1);
wait_on_recordings();

set_recording([1,0,0], 0, Config.NUMFRAMES-1);
play_your_line([0,1,1], 50::ms);
wait_on_recordings();


/////////////////////////
<<< "Middle section snippets:" >>>;
/////////////////////////

0 => start_index;
(Config.NUMFRAMES / 3) => stop_index;
play_your_line([1,1,1], 50::ms);

stop_index+1 => start_index;
(3 *Config.NUMFRAMES / 6) => stop_index;
play_your_line([1,1,1], 50::ms);

stop_index+1 => start_index;
(4 * Config.NUMFRAMES / 6) => stop_index;
play_your_line([1,1,1], 50::ms);

stop_index+1 => start_index;
(Config.NUMFRAMES-1) => stop_index;
play_your_line([1,1,1], 50::ms);


/////////////////////////
<<< "Averaging" >>>;
/////////////////////////

0 => start_index;
Config.NUMFRAMES-1 => stop_index;

do_averaging();
interpolate_lines([1,1,1], 1.0, 50::ms);


/////////////////////////
<<< "Retraining" >>>;
/////////////////////////

0 => start_index;
(Config.NUMFRAMES / 3) => stop_index;
for (0 => int i ; i < 3; i++)
{
    set_recording([1,0,0], start_index, stop_index);
    wait_on_recordings();
    play_your_line([1,0,0], 50::ms);
}

stop_index+1 => start_index;
(2*Config.NUMFRAMES / 3) => stop_index;
for (0 => int i ; i < 2; i++)
{
    set_recording([0,1,0], start_index, stop_index);
    wait_on_recordings();
    play_your_line([0,1,0], 50::ms);
}

stop_index+1 => start_index;
Config.NUMFRAMES - 1 => stop_index;
for (0 => int i ; i < 1; i++)
{
    set_recording([0,0,1], start_index, stop_index);
    wait_on_recordings();
    play_your_line([0,0,1], 50::ms);
}

// Playback with no averaging:
0 => start_index;
(Config.NUMFRAMES / 3) => stop_index;
play_your_line([1,0,0], 50::ms);

stop_index+1 => start_index;
(2*Config.NUMFRAMES / 3) => stop_index;
play_these_lines([1,1,0], [1,1,0], 50::ms);

stop_index+1 => start_index;
Config.NUMFRAMES - 1 => stop_index;
play_these_lines([1,1,1], [2,2,2], 50::ms);


/////////////////////////
<<< "Trio" >>>;
/////////////////////////

0 => start_index;
(Config.NUMFRAMES-1) => stop_index;
set_recording([1,1,1], 0, Config.NUMFRAMES-1);
wait_on_recordings();
play_your_line([1,1,1], 50::ms);

do_averaging();
interpolate_lines([1,1,1], 0.33, 50::ms);
interpolate_lines([1,1,1], 0.66, 50::ms);
interpolate_lines([1,1,1], 1.0, 50::ms);


/////////////////////////
<<< "The end!" >>>;
/////////////////////////
