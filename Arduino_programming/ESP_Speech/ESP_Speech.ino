#include <arduinoFFT.h>

// General purpose defines
#define HIGH 1
#define LOW 0
// Defines related to the signals
#define SIGNAL_LENGTH 8192 // Must be a power of 2 so I chose 2^13
#define NUMBER_OF_SIGNALS 6
// Indicies to get real and imaginary components of a signal
#define REAL 0
#define IMAG 1
// Defines of Pins
// TODO: change pins to be suitable for hardware connections
#define PIN_MODE_TRAIN 10
#define PIN_MODE_RECOGNIZE 11
#define PIN_LISTEN 12 // pin used to tell esp to start listening
#define PIN_MIC 34 // pin connected to mic, used to read analog values

typedef double Sample;
typedef double* RI_Vector;// Real or Imaginary vector
typedef RI_Vector* Signal;// A signal consists of 2 RI_Signal (one real and the other imaginary)

// Array to store all signals
Signal all_signals[NUMBER_OF_SIGNALS];

// Create fft object from fft library
arduinoFFT FFT = arduinoFFT();


 
void setup()
{
  // Setup for digital pins
  pinMode(PIN_MODE_TRAIN, INPUT);
  pinMode(PIN_MODE_RECOGNIZE, INPUT);
  pinMode(PIN_LISTEN, INPUT);
  // Setup for analog pin
  pinMode(PIN_MIC, INPUT);
    
  // Allocate memory space for all signals
  for(int i=0; i < NUMBER_OF_SIGNALS; i++){
    signal_setup(all_signals[i]);
  }
  // Serial monitor setup
  Serial.begin(115200);
  
}


 
void loop()
{
  // TODO: Write the main logic of loop
  // if the user wants to be in the Recognition mode
  while(digitalRead(PIN_MODE_RECOGNIZE)){
    if(digitalRead(PIN_LISTEN)){
      
    }
  }
      
}

// Function used for testing it serves no actual purpose
double get_max_value(double vector[]){
  double max = vector[0];
  for(int i=1; i< SIGNAL_LENGTH; i++){
    if(vector[i] > max){
      max = vector[i];
    }
  } 
  return max;
}

void compute_fft(Signal s){
  FFT.DCRemoval();
  FFT.Windowing(s[REAL], SIGNAL_LENGTH, FFT_WIN_TYP_HAMMING, FFT_FORWARD);	/* Weigh data */
  FFT.Compute(s[REAL], s[IMAG], SIGNAL_LENGTH, FFT_FORWARD); /* Compute FFT */
  FFT.ComplexToMagnitude(s[REAL], s[IMAG], SIGNAL_LENGTH); /* Compute magnitudes */
}

void signal_setup(Signal s){
  s = (Signal) malloc(2 * sizeof(RI_Vector));
  s[REAL] =(RI_Vector) malloc(SIGNAL_LENGTH * sizeof(Sample));
  s[IMAG] =(RI_Vector) malloc(SIGNAL_LENGTH * sizeof(Sample));

  // Initialize the real and imaginary arrays with all zeros
  for(int i = 0; i < SIGNAL_LENGTH; i++){
    s[REAL][i] = 0;
    s[IMAG][i] = 0;
  }
}

// Function that reads analog values and stores them in a signal
void listen(Signal s){
  int i = 0;
  while(digitalRead(PIN_LISTEN) && i < SIGNAL_LENGTH){
    // analogRead returns a uint16_t so we need to cast it to sample
    Sample currSample = (Sample) analogRead(PIN_MIC);
    s[REAL][i] = currSample;
    i++;
    delay(1);
  }
}






