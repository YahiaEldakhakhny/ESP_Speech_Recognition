#include <stdio.h>
#include <unistd.h>
// includes for delay
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// include to control gpio pins
#include "driver/gpio.h"

// include for ADC
#include "driver/adc.h"

#define True 1
#define HIGH 1
#define FALSE 0
#define LOW 0

#define MODE_RECOGNISE_PIN 18
#define MODE_TRAIN_PIN 19
#define LISTEN_PIN 13
#define ANALOG_PIN ADC1_CHANNEL_6 // analog pin is pin 34 which is channel 6 on ADC1
#define SIGNAL_SIZE 12000 // Assume that a single word is at most 1.5 sec
#define NUMBER_OF_SIGNALS 11



typedef short signed int Sample;
typedef Sample* Signal;

// Global Array to store all signals
Signal all_signals[NUMBER_OF_SIGNALS];



/********************* MAIN FUNCTION IS THE LAST FUNCTION *********************/

// FUNCTION TO CREATE DELAY IN MILLISECONDS
void delay(int ms){
	vTaskDelay(ms / portTICK_RATE_MS);
}

// This function reads the analog pin and stores the samples in a signal
void listen(Signal destination){
	for(short unsigned int n = 0; n < SIGNAL_SIZE; n++){
		if(gpio_get_level(LISTEN_PIN)){
			destination[n] = adc1_get_raw(ADC1_CHANNEL_6);
		}
		else{
			return;
		}
	}
}



void app_main(void)
{
	// Setup for digital IO pins
	gpio_set_direction(MODE_RECOGNISE_PIN, GPIO_MODE_INPUT);
	gpio_set_direction(MODE_TRAIN_PIN, GPIO_MODE_INPUT);

	// setup for analog pin
	adc1_config_width(ADC_WIDTH_BIT_12);
	// ADC1_CHANNEL_6 is pin 34, ADC_ATTEN_DB_11 allows voltages up to 3.9
	adc1_config_channel_atten(ANALOG_PIN, ADC_ATTEN_DB_11);



    while (1) {
        printf("%d", sizeof(int));
        sleep(1);
    }
}
