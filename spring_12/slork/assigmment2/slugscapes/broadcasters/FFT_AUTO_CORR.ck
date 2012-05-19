// Auto-Correlation of the adc input
public class FFT_AUTO_CORR
{
    // FFT of the input, or some audio stream
    adc => LPF f_that => FFT fft => blackhole;
    // IFFT object to go back to time domain
    IFFT ifft => blackhole;
    2000 => f_that.freq;
    
    44100 => int srate;
    1024 => int fft_size;
    fft_size => fft.size;
    fft.size() => ifft.size;
    
    (fft.size()/2)$ int => int size_over_two;
    // vars to hold results
    complex g[size_over_two];
    complex f[size_over_two];
    complex y[size_over_two];
    float acor_out[size_over_two];
        
    fun void set_fft_size(int size){
        // TODO: handle size other than factor of 2
        size => fft.size;
        fft.size() => ifft.size;
        (fft.size()/2)$ int => size_over_two;
        // update buffers:
        complex g[size_over_two];
        complex f[size_over_two];
        complex y[size_over_two];
        float acor_out[size_over_two];

    }
    
    // correlation function
    fun void calc_acor(){
      
            // take fft
            fft.upchuck();
            // store complex spectrum (need it twice to create the complex conjugate with one)
            fft.spectrum(g);
            fft.spectrum(f);
            
            // freq domain correlation
            for (0=>int i; i < size_over_two; i++){
                // conjugate f
                -f[i].im => f[i].im;
                
                // complex multiply
                // mult in the freq domain is convolution in time!
                f[i] * g[i] => y[i];
            }
            
            // take ifft
            ifft.transform(y);
            // copy results into float array
            ifft.samples(acor_out);
        
    }
    
}
        