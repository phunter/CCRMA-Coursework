// Energy tracker over time
// this will sum the RMS calculation for an fft of size specified
// for a rough energy level look at static var
//
//    ---  RMS_TRACKER.rmsSum  ---
//    ---  RMS_TRACKER.rmsSumShort ---
//    ---  RMS_TRACKER.rmsSumShorter --- // this one returns the rms value per 2048 frame
//    ---  RMS_TRACKER.rmsSumLong ---

public class RMS_TRACKER{
    adc => FFT fft =^ RMS rms => blackhole;
    // set parameters
    2048 => fft.size;
    // set hann window
    Windowing.hann(fft.size()) => fft.window;
    
    200 => int numWindows;
    (numWindows/2)$int => int numWindowsShort;
    numWindows * 2 => int numWindowsLong;
    
    float rms_array[numWindows];
    
    float rms_arrayShort[numWindowsShort];
    float rms_arrayLong[numWindowsLong];
    
    static float rmsSum;
    static float rmsSumShorter;
    static float rmsSumShort;
    static float rmsSumLong;
    0 => int ptr;
    0 => int ptrShort;
    0 => int ptrShorter;
    0 => int ptrLong;
    
    // control loop
    fun void track(){
    while( true )
    {
        
        // upchuck: take fft then rms
        rms.upchuck() @=> UAnaBlob blob;
        
        10 * blob.fval(0) => rms_array[ptr];
        20 * blob.fval(0) => rms_arrayShort[ptrShort];
        5 * blob.fval(0) => rms_arrayLong[ptrLong];
        
        0=>rmsSum;
        // sum
        for (0 => int i; i < numWindows; i++){
            rms_array[i] +=> rmsSum;
        }
        ptr++;
        numWindows %=> ptr;
        
        // Short
        0=>rmsSumShort;
        // sum
        for (0 => int i; i < numWindowsShort; i++){
            rms_arrayShort[i] +=> rmsSumShort;
        }
        ptrShort++;
        numWindowsShort %=> ptrShort;
        
        // Shorter
        1000 * blob.fval(0) => rmsSumShorter;
        
        
        // Long
        0=>rmsSumLong;
        // sum
        for (0 => int i; i < numWindowsLong; i++){
            rms_arrayLong[i] +=> rmsSumLong;
        }
        ptrLong++;
        numWindowsLong %=> ptrLong;
       

        // advance time
        fft.size()::samp => now;
    }
}
}
0.0 => RMS_TRACKER.rmsSum;
0.0 => RMS_TRACKER.rmsSumShort;
0.0 => RMS_TRACKER.rmsSumLong;
RMS_TRACKER summer;
spork ~ summer.track();
while(1)
{
    1::day=> now;
}