// Pitch Estimation class
// shred this and look at static variable:
//
// ---  PITCH_ESTIMATE.freqEst  ---
//
// Depends on FFT_AUTO_CORR class

public class PITCH_ESTIMATE
{
    FFT_AUTO_CORR acor;
    acor.set_fft_size(1024);
    // the estimate
    400.0 => static float freqEst; 
    // median filter variables
    5 => int medianFiltSize;
    float freqEstArray[medianFiltSize];
    float freqEstArrayTemp[medianFiltSize];
    0 => int freqEstArrayPtr;
    
    // the threshold
    0.004 => static float threshold;
    
    fun void setThresh(float newThresh){
        newThresh => threshold;
    }
    
    10::ms => dur updateRate;
    fun void setUpdateRate(dur rate){
        rate => updateRate;
    }
    
    static float max;
    int max_index;
    35 => int min_lag;
    acor.size_over_two => int max_lag;
    
    fun void find_max_lag(){
        // zero out variables
        0.0 => max;
        0 => max_index;
        
        
        // loop through lags
        for (min_lag => int l; l < max_lag; l++){
            if (acor.acor_out[l] > max){
                l=>max_index;
                acor.acor_out[l] => max;
            }
            else if(-acor.acor_out[l] > max){
                l=> max_index;
                -acor.acor_out[l] => max;
            }
        }
    }
    
    fun void estimatePitch(){
        while(1){
            // calculate the auto correlation
            acor.calc_acor();
            find_max_lag();
            
            if (max > threshold){
                // acor.srate/max_index => freqEst; // without Median Filter
                acor.srate/max_index => freqEstArray[freqEstArrayPtr];
                // copy to temporary array
                for (0 => int n; n < medianFiltSize; n++){
                    freqEstArray[n] => freqEstArrayTemp[n];
                }
                // Median Filter!
                for (0 => int i; i < medianFiltSize; i++) {
                    for( 0 => int j; j < medianFiltSize; j++){
                        if (freqEstArrayTemp[i] < freqEstArrayTemp[j])
                        {
                            // swap places
                            freqEstArrayTemp[i] => float temp;
                            freqEstArrayTemp[j] => freqEstArrayTemp[i];
                            temp => freqEstArrayTemp[j];
                        }
                    }
                }
                
                medianFiltSize/2 + 1 $int => int median;
                
                freqEstArrayTemp[median] => freqEst;
                
                // increment pointer
                freqEstArrayPtr++;
                freqEstArrayPtr % medianFiltSize => freqEstArrayPtr;
                
               // <<< "close enough, freq estimate: ", freqEst >>>;
            }
            
            updateRate => now;
        }
    }
}
0.0 => PITCH_ESTIMATE.max;
0 => PITCH_ESTIMATE.threshold;
400.0 => PITCH_ESTIMATE.freqEst;
PITCH_ESTIMATE estimator;
spork ~ estimator.estimatePitch();
while(1){
    1::day=> now;
}