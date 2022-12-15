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



// Array to store optimal signals and their corresponding tolerances
double reference_signal[SIGNAL_LENGTH];

// Create fft object from fft library
arduinoFFT FFT = arduinoFFT();

  double s[SIGNAL_LENGTH] = {0};



 
void setup(){
  // Setup for digital pins
  pinMode(PIN_MODE_TRAIN, INPUT);
  pinMode(PIN_MODE_RECOGNIZE, INPUT);
  pinMode(PIN_LISTEN, INPUT);
  pinMode(PIN_TRAIN_WORD1, INPUT);
  pinMode(PIN_TRAIN_WORD2, INPUT);
  pinMode(PIN_TRAIN_WORD3, INPUT);

  // Set interrupts for changing the mode 
  // attachInterrupt(PIN_MODE_TRAIN, training_mode, FALLING);
  // attachInterrupt(PIN_MODE_RECOGNIZE, recognision_mode, FALLING);
  
  // Setup for analog pin
  pinMode(PIN_MIC, INPUT);
    
  // Serial monitor setup
  Serial.begin(115200);
  
}


// Note that the logic of most pins is acive low 
void loop(){
  while(!digitalRead(PIN_MODE_RECOGNIZE)){
    recognision_mode();
  }


  while(!digitalRead(PIN_MODE_TRAIN)){
    training_mode();
  }
    
  
           
}

void recognision_mode(){
  int i = 0;

  Serial.println("recognision mode....");
  // wait for the user to speak
  //while(digitalRead(PIN_LISTEN) ){Serial.println("waitng for input...."); delay(1000);}
  if(!digitalRead(PIN_LISTEN)){
    Serial.println("Listening....");
    delay(100);
    // when the user speaks
    while(!digitalRead(PIN_LISTEN) && i < SIGNAL_LENGTH){
      // analogRead returns a uint16_t so we need to cast it to sample
      double sample = (double) analogRead(PIN_MIC);
      s[i] =  sample;
      Serial.println("Listening....");
      i++;
      delayMicroseconds(100);
    }
    Serial.println("*****End of lisen****");

    delay(3000);

  }
  else{ return;}
  prinSig(s);
  
}

void training_mode(){
  Serial.println("training mode....");

  // while(digitalRead(PIN_LISTEN) ){Serial.println("waitng for input...."); delay(1000);}


}

void prinSig(double s[]){
  for(int j = 0; j < SIGNAL_LENGTH; j++){
    Serial.println(s[j]);

    delay(200);
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

void compute_fft(double s[]){
  double imag[SIGNAL_LENGTH] = {0};
  FFT.DCRemoval();
  FFT.Windowing(s, SIGNAL_LENGTH, FFT_WIN_TYP_HAMMING, FFT_FORWARD);	/* Weigh data */
  FFT.Compute(s, imag, SIGNAL_LENGTH, FFT_FORWARD); /* Compute FFT */
  FFT.ComplexToMagnitude(s, imag, SIGNAL_LENGTH); /* Compute magnitudes */
}



// Function that reads analog values and stores them in a signal
void listen(double s[]){
  int i = 0;
  // wait for the user to speak
  while(digitalRead(PIN_LISTEN) ){Serial.println("waitng for input....");}
  Serial.println("Listening....");
  delay(100);
  // when the user speaks
  while(!digitalRead(PIN_LISTEN) && i < SIGNAL_LENGTH){
    // analogRead returns a uint16_t so we need to cast it to sample
    double sample = (double) analogRead(PIN_MIC);
    s[i] =  sample;
    Serial.println(sample);
    i++;
    delayMicroseconds(120);
  }
}


/* if a vector v = <v0, v1, v2, ...>
its magnitude = sqrt(v0^2 + v1^2 + v2^2 + ....)
*/
double get_vector_magnitude(double v[]){
  double sqr_sum = 0;
  for(int i = 0; i < SIGNAL_LENGTH; i++){
    sqr_sum += v[i] * v[i];
  }
  return sqrt(sqr_sum);
}

// normalises a vector (divides it by its magnitude)
void normalise(double v[]){
  double mag = get_vector_magnitude(v);
  for(int i = 0; i < SIGNAL_LENGTH; i++){
    v[i] /= mag;
  }
}

double dot_product(double v1[], double v2[]){
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
 
}

void train(){
 
}








