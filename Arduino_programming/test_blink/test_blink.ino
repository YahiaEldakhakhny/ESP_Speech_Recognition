#include <arduinoFFT.h>


// Defines related to the signals
#define SIGNAL_LENGTH 8192 // Must be a power of 2 so I chose 2^13

// Defines of Pins
// TODO: change pins to be suitable for hardware connections
#define PIN_MODE_TRAIN 12
#define PIN_MODE_RECOGNIZE 14
#define PIN_LISTEN 25 // pin used to tell esp to start listening
#define PIN_MIC 34 // pin connected to mic, used to read analog values


// Array to store all signals
double ref_signal[SIGNAL_LENGTH];

// Create fft object from fft library
arduinoFFT FFT = arduinoFFT();

void setup()
{

  pinMode(PIN_MODE_TRAIN, INPUT);
  pinMode(PIN_MODE_RECOGNIZE, INPUT);
  pinMode(PIN_LISTEN, INPUT); 
  pinMode(PIN_MIC, INPUT);
  
  // Serial monitor setup
  Serial.begin(115200);
  
}


 
void loop()
{
  while(!digitalRead(PIN_LISTEN)){
    Serial.println(analogRead(PIN_MIC));
  }
  
}

int readTrainingPins(){
  int b1 = !digitalRead(PIN_TRAIN_WORD1);
  int b2 = !digitalRead(PIN_TRAIN_WORD2);
  int b3 = !digitalRead(PIN_TRAIN_WORD3);
  return (b1 + (b2 << 1) + (b3 << 2));
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
}

// Function that reads analog values and stores them in a signal
void listen(Signal s){
  int i = 0;
  while(!digitalRead(PIN_LISTEN) && i < SIGNAL_LENGTH){
    // analogRead returns a uint16_t so we need to cast it to sample
    Sample currSample = (Sample) analogRead(PIN_MIC);
    s[REAL][i] = currSample;
    i++;
    delayMicroseconds(122);
  }
}