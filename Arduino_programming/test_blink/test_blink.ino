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
#define PIN_MODE_TRAIN 12
#define PIN_MODE_RECOGNIZE 14
#define PIN_LISTEN 25 // pin used to tell esp to start listening
#define PIN_MIC 34 // pin connected to mic, used to read analog values
#define PIN_TRAIN_WORD1 19
#define PIN_TRAIN_WORD2 18
#define PIN_TRAIN_WORD3 5
typedef double Sample;
typedef double* RI_Vector;// Real or Imaginary vector
typedef RI_Vector* Signal;// A signal consists of 2 RI_Signal (one real and the other imaginary)

// Array to store all signals
Signal all_signals[NUMBER_OF_SIGNALS];

// Create fft object from fft library
arduinoFFT FFT = arduinoFFT();

int x = 2;
int y = 23;
int curr_filtered = 0;
int prev_filtered = 0;
int curr_input    = 0;
int prev_input    = 0;
void setup()
{

  pinMode(PIN_MODE_TRAIN, INPUT);
  pinMode(PIN_MODE_RECOGNIZE, INPUT);
  pinMode(PIN_LISTEN, INPUT); 
  pinMode(PIN_MIC, INPUT);  
  pinMode(PIN_TRAIN_WORD1, INPUT);
  pinMode(PIN_TRAIN_WORD2, INPUT);
  pinMode(PIN_TRAIN_WORD3, INPUT);
  
  // Allocate memory space for all signals
  // for(int i=0; i < NUMBER_OF_SIGNALS; i++){
  //   signal_setup(all_signals[i]);
  // }
  // Serial monitor setup
  Serial.begin(115200);
  
}


 
void loop()
{
  Signal currSignal;
  while(digitalRead(PIN_LISTEN)){Serial.println("Waiting for input...."); delay(500);}
  if(!digitalRead(PIN_LISTEN)){
  listen(currSignal);
  }

  Serial.println("Done listening");  

  for(int i = 0; i < SIGNAL_LENGTH; i++){
  Serial.println(currSignal[REAL][i]);    
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