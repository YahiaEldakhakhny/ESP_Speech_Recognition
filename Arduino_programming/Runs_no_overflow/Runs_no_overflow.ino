#include <arduinoFFT.h>


#define SIGNAL_LENGTH 4096 // Must be a power of 2 so I chose 2^13
// Defines of Pins
#define PIN_MODE_TRAIN 12
#define PIN_MODE_RECOGNIZE 14
#define PIN_LISTEN 25 // pin used to tell esp to start listening
#define PIN_MIC 34 // pin connected to mic, used to read analog values
#define PIN_TRUTH 5

// used in training model
#define LEARNING_RATE 0.9


double reference_signal[SIGNAL_LENGTH] = {1/SIGNAL_LENGTH};
double curr_signal[SIGNAL_LENGTH] = {0};

// Create fft object from fft library
arduinoFFT FFT = arduinoFFT();
// used in training model
double predict = 0;
// imaginary part of the signal (all zeros but needed for fft)
double imag[SIGNAL_LENGTH] = {0};



void setup() {

  pinMode(PIN_MODE_TRAIN, INPUT);
  pinMode(PIN_MODE_RECOGNIZE, INPUT);
  pinMode(PIN_LISTEN, INPUT);
  pinMode(PIN_TRUTH, INPUT);

  // Setup for analog pin
  pinMode(PIN_MIC, INPUT);

  // put your setup code here, to run once:
  Serial.begin(115200);

}

void loop() {
  // Recognition mode
  if(!digitalRead(PIN_MODE_RECOGNIZE)){
    Serial.println("recognition mode....");
    if(!digitalRead(PIN_LISTEN)){
      // make all elements of curr_signal = 0
      for(int k = 0; k < SIGNAL_LENGTH; k++){
        curr_signal[k] = 0;
      }
      int i = 0;

      while(!digitalRead(PIN_LISTEN) && i < SIGNAL_LENGTH){
        Serial.println("Listening....");
        curr_signal[i] = (double) analogRead(PIN_MIC);
        i++;
        delayMicroseconds(100);
      }
      Serial.println("*****End of listen****");
      delay(5000);
      compute_fft(curr_signal);
       
      if(recognise(curr_signal)){
        Serial.println("Match");
      } else{
        Serial.println("No Match");
      }
    }
    // delay before polling PIN_MODE_RECOGNIZE again
    delay(100);
  } // END OF Recognition mode
  
/////////////////////// MODE BORDER ////////////////////

  // Training mode
  if(!digitalRead(PIN_MODE_TRAIN)){
    Serial.println("training mode....");
    if(!digitalRead(PIN_LISTEN)){
      // make all elements of curr_signal = 0
      for(int k = 0; k < SIGNAL_LENGTH; k++){
        curr_signal[k] = 0;
      }
      int i = 0;

      while(!digitalRead(PIN_LISTEN) && i < SIGNAL_LENGTH){
        Serial.println("Listening....");
        curr_signal[i] = (double) analogRead(PIN_MIC);
        i++;
        delayMicroseconds(100);
      }
      Serial.println("*****End of listen****");
      delay(2500);
      compute_fft(curr_signal);
      // update current reference signal with the new signal
      train(curr_signal);
      Serial.print("Cost = ");
      Serial.println(compute_cost());
      delay(2500);
    }
    // delay before polling PIN_MODE_TRAIN again
    delay(100);
  } // END OF Recognition mode

} // END OF LOOP 



void compute_fft(double s[]){
  // make all elements of imag = 0
  for(int k = 0; k < SIGNAL_LENGTH; k++){
    imag[k] = 0;
  }

  FFT.DCRemoval();
  FFT.Windowing(s, SIGNAL_LENGTH, FFT_WIN_TYP_HAMMING, FFT_FORWARD);	/* Weigh data */
  FFT.Compute(s, imag, SIGNAL_LENGTH, FFT_FORWARD); /* Compute FFT */
  FFT.ComplexToMagnitude(s, imag, SIGNAL_LENGTH); /* Compute magnitudes */
}

double dot_product(double v1[], double v2[]){
  double result = 0;
  for(int i = 0; i < SIGNAL_LENGTH; i++){
    result += v1[i] * v2[i];
  }
}


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


bool recognise(double s[]){
  predict = dot_product(s, reference_signal);
  predict = sigmoid(predict);
  return(predict > 0.5);
  
}

void train(double s[]){
  double d = dot_product(s, reference_signal);
  d = sigmoid(d);
  double loss = d - check_label();
  for(int k = 0; k < SIGNAL_LENGTH; k++){
    s[k] *= loss;
    reference_signal[k] = reference_signal[k] - LEARNING_RATE* s[k];
  }

}
