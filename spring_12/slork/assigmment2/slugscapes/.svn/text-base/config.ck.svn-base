////////////////////////////////////////////////////////////
// Slugscapes
// Configuration module
////////////////////////////////////////////////////////////
// Slork 2012
// Hunter McCurry, Jiffer Harriman, Michael Wilson
////////////////////////////////////////////////////////////

// Holds common information
public class Config
{
    // Config mode
    0 => static int mode;

    // Name of clients, update before each run
    fun static string host_name(int index)
    {
        if (mode == 1)
        {
            return "localhost";
        }
	    else if (mode == 2)
	    {
            if (index == 0)
            {
                return "jambalaya.local";
            }
            else if (index == 1)
            {
                return "icetea.local";
            }
            else if (index == 2)
            {
                return "hamburger.local";
            }
            else
            {
                <<< "ERROR: Requested client index too high!" >>>;
                return "";
            }
	    }
        else
        {
            if (index == 0)
            {
                return "foiegras.local";
            }
            else if (index == 1)
            {
                return "chowder.local";
            }
            else if (index == 2)
            {
                return "albacore.local";
            }
            else
            {
                <<< "ERROR: Requested client index too high!" >>>;
                return "";
            }
        }
    }

    // Name of server, update before each run
    fun static string server_name()
    {
        if (mode == 1)
        {
            return "localhost";
        }
	    else if (mode == 2)
	    {
            return "kimchi.local";
	    }
        else
        {
            return "donut.local";
        }
    }
    
    // Number of clients:
    3 => static int NUMCLIENTS;

    // The number of data points to collect in a set:
    256 => static int NUMFRAMES;

    // Different modes we can be in:
    0 => static int MODE_COLLECTING;
    4 => static int MODE_PLAYBACK;
    5 => static int MODE_IDLE;
    
    // OSC ports:
    6470 => static int server_to_client_port;
    6471 => static int client_to_server_port;
    6472 => static int server_to_processing_port;
    6473 => static int synth_port;

    // Delay between OSC server burst messages, in ms
    1 => static int osc_burst_delay;
    
    // OSC message definitions:
    fun static string osc_message_pulse()
    {
        return "/pulse";
    }
    fun static string osc_message_pulse_data()
    {
        // Current Mode, Machine Index
        return "i i";
    }

    fun static string osc_message_envelope_to_client()
    {
        return "/e2c";
    }
    fun static string osc_message_envelope_to_client_data()
    {
        // Pitch, Amplitude
        return "f f";
    }

    fun static string osc_message_envelope_to_server()
    {
        return "/e2s";
    }
    fun static string osc_message_envelope_to_server_data()
    {
        // Pitch, Amplitude, Machine Index
        return "f f i";
    }

    fun static string osc_message_envelope_to_processing()
    {
        return "/e2p";
    }
    fun static string osc_message_envelope_to_processing_data()
    {
        // Pitch, Amplitude, Machine Index, Frame Number, % machine 0, % machine 1, % machine 2
        return "f f i i f f f";
    }
}
// Instantiate to initialize static variables:
Config config;
if (me.args() > 0)
{
    if (me.arg(0) == "local")
    {
	    <<< "Localtest" >>>;
        1 => Config.mode;
    	1 => Config.NUMCLIENTS;
    }
    else if (me.arg(0) == "listeningroom")
    {
        2 => Config.mode;
    }
}
