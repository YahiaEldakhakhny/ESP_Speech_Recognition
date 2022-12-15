#include <arduinoFFT.h>

// General purpose defines
#define HIGH 1
#define LOW 0
// Defines related to the signals
#define SIGNAL_LENGTH 8192 // Must be a power of 2 so I chose 2^13
#define NUMBER_OF_SIGNALS 6
#define NUMBER_OF_TRAINING_SIGNALS 5
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
typedef RI_Vector* Signal;// A signal consists of 2 RI_Vector (one real and the other imaginary)

// Array to store optimal signals and their corresponding tolerances
Signal optimal_signals[NUMBER_OF_SIGNALS];
double tolerances[NUMBER_OF_SIGNALS] = {0};

// Create fft object from fft library
arduinoFFT FFT = arduinoFFT();


 
void setup(){
  // Setup for digital pins
  pinMode(PIN_MODE_TRAIN, INPUT);
  pinMode(PIN_MODE_RECOGNIZE, INPUT);
  pinMode(PIN_LISTEN, INPUT);
  pinMode(PIN_TRAIN_WORD1, INPUT);
  pinMode(PIN_TRAIN_WORD2, INPUT);
  pinMode(PIN_TRAIN_WORD3, INPUT);

  // Set interrupts for changing the mode 
  attachInterrupt(PIN_MODE_TRAIN, training_mode, FALLING);
  attachInterrupt(PIN_MODE_TRAIN, recognision_mode, FALLING);
  
  // Setup for analog pin
  pinMode(PIN_MIC, INPUT);
    
  // Allocate memory space for all signals
  for(int i=0; i < NUMBER_OF_SIGNALS; i++){
    signal_setup(optimal_signals[i]);
  }
  // Serial monitor setup
  Serial.begin(115200);
  
}


// Note that the logic of most pins is acive low 
void loop(){
           
}

void recognision_mode(){
    while(!digitalRead(PIN_MODE_RECOGNIZE)){
    Serial.println("Recognition Mode");
    if(!digitalRead(PIN_LISTEN)){
      recognise();
    }
    else{
      continue;
    }
  }
}

void training_mode(){
  while(!digitalRead(PIN_MODE_TRAIN)){
    Serial.println("Training Mode");
    if(!digitalRead(PIN_LISTEN)){
      Serial.println("Training....");
      train();
    }
    else{
      continue;
    }
  }
}

// Function used to find the index of the largest element in an array of doubles
int get_max_index(double vector[], int length){
  double max = vector[0];
  int max_index = 0;
  for(int i=1; i< length; i++){
    if(vector[i] > max){
      max_index = i;
    }
  } 
  return max_index;
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
  // wait for the user to speak
  while(digitalRead(PIN_LISTEN) ){Serial.println("waitng for input....");}
  Serial.println("Listening....");
  delay(100);
  // when the user speaks
  while(!digitalRead(PIN_LISTEN) && i < SIGNAL_LENGTH){
    // analogRead returns a uint16_t so we need to cast it to sample
    Sample currSample = (Sample) analogRead(PIN_MIC);
    s[REAL][i] = currSample;
    i++;
    delayMicroseconds(120);
  }
}


/* if a vector v = <v0, v1, v2, ...>
its magnitude = sqrt(v0^2 + v1^2 + v2^2 + ....)
*/
double get_vector_magnitude(RI_Vector v){
  double sqr_sum = 0;
  for(int i = 0; i < SIGNAL_LENGTH; i++){
    sqr_sum += v[i] * v[i];
  }
  return sqrt(sqr_sum);
}

// normalises a vector (divides it by its magnitude)
void normalise(RI_Vector v){
  double mag = get_vector_magnitude(v);
  for(int i = 0; i < SIGNAL_LENGTH; i++){
    v[i] /= mag;
  }
}

double dot_product(RI_Vector v1, RI_Vector v2){
  double result = 0;
  for(int i = 0; i < SIGNAL_LENGTH; i++){
    result += v1[i] * v2[i];
  }
}


int readTrainingPins(){
  int b1 = !digitalRead(PIN_TRAIN_WORD1);
  int b2 = !digitalRead(PIN_TRAIN_WORD2);
  int b3 = !digitalRead(PIN_TRAIN_WORD3);
  return (b1 + (b2 << 1) + (b3 << 2));
}




// FUNCTION TO BE RUN WHEN ESP ENTERS  RECOGNISION MODE
void recognise(){
  Signal currSignal;
  double diviations[NUMBER_OF_SIGNALS] = {0};// DIVIATIONS BETWEEN CURRENT SIGNAL AND EACH OF THE OPTIMAL SIGNALS
  // allocate space for the current signal
  signal_setup(currSignal);
  //listen to samples and store them in the current signal
  listen(currSignal);
  Serial.println("Processing....");
  compute_fft(currSignal);
  // The magnitude spectrum of the fft is stored in the real part of the signal so we are only interested in the real vector
  normalise(currSignal[REAL]);

  for(int i = 0; i < NUMBER_OF_SIGNALS; i++){
  double dot_prod = dot_product(currSignal[REAL], optimal_signals[i][REAL]);
    if(dot_prod >= tolerances[i]){
      diviations[i] = dot_prod;
    }
  }

  // the final word is the index of the optimal signal that most resembles the current signal
  int final_word = get_max_index(diviations, NUMBER_OF_SIGNALS);
  if(diviations[final_word] == 0){
    Serial.println(-1);
  }
  else{
    Serial.println(final_word);
  }
  

  free(currSignal);
}

void train(){
  int currTrainingWord = readTrainingPins();
  
  int training_index = 0;
  Signal training_signals[NUMBER_OF_TRAINING_SIGNALS];
  Signal optimalSignal = optimal_signals[currTrainingWord];
  for(int i = 0; i < NUMBER_OF_TRAINING_SIGNALS; i++){
    signal_setup(training_signals[i]);
  }
  while(!digitalRead(PIN_MODE_TRAIN) && training_index < NUMBER_OF_TRAINING_SIGNALS){
    if(!digitalRead(PIN_LISTEN)){
      listen(training_signals[training_index]);
      training_index++;
    }
  }

  // calculate fft for all training signals and normalise them
  for(int i = 0; i < training_index; i++){
    compute_fft(training_signals[i]);
    normalise(training_signals[i][REAL]);
  }  
  // Calculate average of all training signals
  // TODO: correct this logic
  for(int i = 0; i < SIGNAL_LENGTH; i++){
    for(int j = 0; j < training_index; j++){
      optimalSignal[REAL][i] += (training_signals[j][REAL])[i]/training_index;
    }
  }

  normalise(optimalSignal[REAL]);
  //Compare the optimal signal with all training signals
  double min_dot_product, curr_dot_product;
  for(int i =0; i < training_index; i++){
    curr_dot_product = dot_product(optimalSignal[REAL], training_signals[i][REAL]);    
    if(i == 0 || curr_dot_product < min_dot_product){
      min_dot_product = curr_dot_product;
    }
  }
  tolerances[currTrainingWord] = min_dot_product;
}








