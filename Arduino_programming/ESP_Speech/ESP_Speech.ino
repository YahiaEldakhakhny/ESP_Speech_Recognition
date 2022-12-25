#include <arduinoFFT.h>
#include "Complex.h"

// General purpose defines
#define HIGH 1
#define LOW 0
// Defines related to the signals
#define SIGNAL_LENGTH 8192 // Must be a power of 2 so I chose 2^13
// #define NUMBER_OF_SIGNALS 6
// #define NUMBER_OF_TRAINING_SIGNALS 5
// Indicies to get real and imaginary components of a signal
// #define REAL 0
// #define IMAG 1
// Defines of Pins
// TODO: change pins to be suitable for hardware connections
#define PIN_MODE_TRAIN 12
#define PIN_MODE_RECOGNIZE 14
#define PIN_LISTEN 25 // pin used to tell esp to start listening
#define PIN_MIC 34 // pin connected to mic, used to read analog values
// #define PIN_TRAIN_WORD1 19
// #define PIN_TRAIN_WORD2 18
// #define PIN_TRAIN_WORD3 5
#define PIN_TRUTH 5
// Training constants
#define LEARNING_RATE 0.9



// Array to store optimal signals and their corresponding tolerances
double reference_signal[SIGNAL_LENGTH] = {1/SIGNAL_LENGTH};

// training constant
// const double learning_rate = 0.9;

// Create fft object from fft library
arduinoFFT FFT = arduinoFFT();


double predict = 0;



 
void setup(){
  // Setup for digital pins
  pinMode(PIN_MODE_TRAIN, INPUT);
  pinMode(PIN_MODE_RECOGNIZE, INPUT);
  pinMode(PIN_LISTEN, INPUT);
  pinMode(PIN_TRUTH, INPUT);
  pinMode(26, OUTPUT);


 
  // Setup for analog pin
  pinMode(PIN_MIC, INPUT);
    
  // Serial monitor setup
  Serial.begin(115200);
  
}


// Note that the logic of most pins is acive low 
void loop(){
  if(!digitalRead(PIN_MODE_RECOGNIZE)){  
  while(!digitalRead(PIN_MODE_RECOGNIZE)){
  int i = 0;

  Serial.println("recognision mode....");
  // wait for the user to speak
  // while(digitalRead(PIN_LISTEN) ){Serial.println("waitng for input...."); delay(1000);}
  if(!digitalRead(PIN_LISTEN)){
    double curr_signal[SIGNAL_LENGTH] = {0};

    Serial.println("Listening....");
    delay(100);
    // when the user speaks
    listen(curr_signal);
    Serial.println("*****End of listen****");

    delay(1000);
    compute_fft(curr_signal);
    bool res = recognise(curr_signal);
    if(res){
      Serial.println("Match");
    } else{
      Serial.println("No Match");
    }
    free(curr_signal);    
    delay(2000);
  }  
  }
  }

if(!digitalRead(PIN_MODE_TRAIN)){
  while(!digitalRead(PIN_MODE_TRAIN)){
    // training_mode();
    // delay(100);
    digitalWrite(26, 1);    
    delay(5000);    
    digitalWrite(26, 0);    
    delay(5000);   
  }
   } 
  
           
}

void recognision_mode(){
  int i = 0;

  Serial.println("recognision mode....");
  // wait for the user to speak
  //while(digitalRead(PIN_LISTEN) ){Serial.println("waitng for input...."); delay(1000);}
  if(!digitalRead(PIN_LISTEN)){
    double curr_signal[SIGNAL_LENGTH] = {0};

    Serial.println("Listening....");
    delay(100);
    // when the user speaks
    listen(curr_signal);
    Serial.println("*****End of listen****");

    delay(1000);
    compute_fft(curr_signal);
    bool res = recognise(curr_signal);
    if(res){
      Serial.println("Match");
    } else{
      Serial.println("No Match");
    }
    delay(2000);
  }
  else{ return;}
  
}

void training_mode(){
  int i = 0;

  Serial.println("training mode....");
  
  if(!digitalRead(PIN_LISTEN)){
    double curr_signal[SIGNAL_LENGTH] = {0};

    Serial.println("Listening....");
    delay(100);
    // when the user speaks
    while(!digitalRead(PIN_LISTEN) && i < SIGNAL_LENGTH){
      // analogRead returns a uint16_t so we need to cast it to sample
      double sample = (double) analogRead(PIN_MIC);
      curr_signal[i] =  sample;
      Serial.println("Listening....");
      i++;
      delayMicroseconds(100);
    }
    Serial.println("*****End of lisen****");
    compute_fft(curr_signal);
    train_step(curr_signal);
    Serial.print("Cost = ");
    Serial.println(compute_cost());
    delay(2000);
  }else{ return;}

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
// double get_vector_magnitude(double v[]){
//   double sqr_sum = 0;
//   for(int i = 0; i < SIGNAL_LENGTH; i++){
//     sqr_sum += v[i] * v[i];
//   }
//   return sqrt(sqr_sum);
// }

// normalises a vector (divides it by its magnitude)
// void normalise(double v[]){
//   double mag = get_vector_magnitude(v);
//   for(int i = 0; i < SIGNAL_LENGTH; i++){
//     v[i] /= mag;
//   }
// }

double dot_product(double v1[], double v2[]){
  double result = 0;
  for(int i = 0; i < SIGNAL_LENGTH; i++){
    result += v1[i] * v2[i];
  }
}


// int readTrainingPins(){
//   int b1 = !digitalRead(PIN_TRAIN_WORD1);
//   int b2 = !digitalRead(PIN_TRAIN_WORD2);
//   int b3 = !digitalRead(PIN_TRAIN_WORD3);
//   return (b1 + (b2 << 1) + (b3 << 2));
// }

double sigmoid(double x){
  double res;
  res = 1/(1+exp(-1*x));
  return res;
}

bool check_label(){
  int truth = !digitalRead(PIN_TRUTH);
  return truth;
}

double compute_cost(){
  double p1 = log(predict + pow(10, -6))/ log(2);
  double p2 = log(1-predict + pow(10, -6))/ log(2);
  int st_lbl =  check_label();
  return st_lbl * p1 - (1-st_lbl)* p2;
}



// FUNCTION TO BE RUN WHEN ESP ENTERS  RECOGNISION MODE
bool recognise(double s[]){
  predict = dot_product(s, reference_signal);
  predict = sigmoid(predict);
  if(predict > 0.5){
    return true;
  }
  return false;
}

void train_step(double s[]){
  double d = dot_product(s, reference_signal);
  d = sigmoid(d);
  double loss = d - check_label();
  for(int k = 0; k < SIGNAL_LENGTH; k++){
    s[k] *= loss;
    reference_signal[k] = reference_signal[k] - LEARNING_RATE* s[k];
  }

}








