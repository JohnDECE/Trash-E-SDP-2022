
#include <stdio.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "pin_control.h"
#include "constants.h"
#include "analogWrite.h"
#include "driver/adc.h"
#include "time_control.h"
#include "Photo_Cell_Calibrator.h"
#include "motor_control.h"
//#include "delay.h"

/*#include <string.h>
#include <stdlib.h>
#include "driver/adc.h"
#include "Photo_Cell_Calibrator.h"
//#include "delay.h"
//#include "telegram_bot.h"
#include "esp_http_client.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include <sys/time.h>
#include "esp_log.h"
#include "esp_sleep.h"
//#include "/Users/stephentownsend/esp/SDP-ESP32/main/components/time_sync/include/time_sync.h"
*/
bool objectDetected = false;
bool loadDetected = false;
bool direction = true;

bool messageSent = false;
//bool atHome = true;
//bool atPickup = false;
bool findAttempt = false;
bool wasRev = false;
bool wasForward = true;

// uint8_t RIGHT_PHOTO = 34;
// uint8_t LEFT_PHOTO = 39;
uint8_t LEFT_LED = 12;
uint8_t RIGHT_LED = 4;
uint8_t ULTRA_DIRECTION = 26;
uint8_t ULTRA_DETECTION = 36;
uint8_t LOAD_DETECTION = 25;
int motorSpeed = 255;
uint16_t obstacle_removal_wait_time = 20000; // In ms
uint16_t load_removal_wait_time = 20000; // In ms
//uint8_t tempPin = 12;


//#define DEFAULT_VREF 1100

//#define BLINK_GPIO 14
//#define LED (GPIO_NUM_13)

void wait(int ms)
{
    vTaskDelay(ms/portTICK_PERIOD_MS);
}

void setup(void)
{
    pinMode(ULTRA_DETECTION, INPUT);
    pinMode(LOAD_DETECTION, INPUT);
    //pinMode(ULTRA_DIRECTION, OUTPUT);

    setupMotors();

    //analogADC1Setup(ADC1_CHANNEL_3, ADC_ATTEN_DB_11, 3);
    //analogADC1Setup(ADC1_CHANNEL_6, ADC_ATTEN_DB_11, 3);
    setupPhotoresistors(LEFT_LED, RIGHT_LED, ADC1_CHANNEL_3, ADC1_CHANNEL_6, 3, ADC_ATTEN_DB_11, ULTRA_DIRECTION);

    //digitalWrite(ULTRA_DIRECTION, 0);
}

photoVals photoValStruct = {
		.Left_Photo = -1,
		.Right_Photo = -1,
};

void run_trashe() {
	/*
	 *
	 */
	//loadDetected = digitalRead(LOAD_DETECTION);
	objectDetected = digitalRead(ULTRA_DETECTION);
	if(!objectDetected)
	{
		switch(trackLine(ADC1_CHANNEL_3, ADC1_CHANNEL_6, true, &photoValStruct, direction))
		{
			case 1: // Left side triggered
				digitalWrite(LEFT_LED, 1);
				stop();
				vTaskDelay(200/portTICK_RATE_MS);
				if (direction) {
					rotate_right(motorSpeed);
				}
				else {
					rotate_left(motorSpeed);
				}
				vTaskDelay(500 / portTICK_RATE_MS);
				break;

			case 2: // Right side triggered
				digitalWrite(RIGHT_LED, 1);
				stop();
				vTaskDelay(200 / portTICK_RATE_MS);
				if (direction) {
					rotate_left(motorSpeed);
				}
				else {
					rotate_right(motorSpeed);
				}
				vTaskDelay(500 / portTICK_RATE_MS);
				break;
			case 3:
				stop();
				vTaskDelay(1000 / portTICK_RATE_MS);
				if (trackLine(ADC1_CHANNEL_3, ADC1_CHANNEL_6, true, &photoValStruct, direction) == 3) {
					digitalWrite(RIGHT_LED, 1);
					digitalWrite(LEFT_LED, 1);
					vTaskDelay(500 / portTICK_RATE_MS);
					digitalWrite(RIGHT_LED, 0);
					digitalWrite(LEFT_LED, 0);
					// Begin waiting to see if the load gets removed
					int startTime = millis();
					while (millis() - startTime < load_removal_wait_time) {
						loadDetected = digitalRead(LOAD_DETECTION);
						if (!loadDetected) {
							break;
						}
					}
					// Begin Sleepy time?
					vTaskDelay(3000 / portTICK_RATE_MS);
					direction = !direction;
				}
				break;
			case -1: // Both off the line
				stop();
				vTaskDelay(500 / portTICK_RATE_MS);
				break;

			default: // Normal Case, Trashe should keep moving towards its destination
				if (direction) {
					forward(motorSpeed);
				}
				else {
					reverse(motorSpeed);
				}
				digitalWrite(LEFT_LED, 0);
				digitalWrite(RIGHT_LED, 0);
				vTaskDelay(500 / portTICK_RATE_MS);
				break;
		}
	}

	else
	{
		// Begin sleepy time?
		stop();
		// Begin waiting to see if obstacle remains in the way for a significant amount of time
		int obstacleStartTimer = millis();
		while (millis() - obstacleStartTimer < 20000) {
			// If obstacle gets removed then proceed onwards
			objectDetected = digitalRead(ULTRA_DETECTION);
			if (!objectDetected) {
				break;
			}
		}
		// We only change directions if the timer finished and we are going in the forward direction
		// In case that somehow the obstacle gets removed immediately after the the timer runs out
		if (millis() - obstacleStartTimer >= obstacle_removal_wait_time) {
			objectDetected = digitalRead(ULTRA_DETECTION);
			if (objectDetected && direction) {
				direction = !direction;
			}
		}
	}
}

void app_main(void)
{
	setup();
    while(1)
    {
    	/*
    	 * Main code flow:
    	 * Check for scheduled time
    	 * 		If scheduled time
    	 * 			Begin running main loop
    	 * 			In main loop
    	 * 				Move towards destination (Direction = TRUE) indicates the forward direction towards the pickup location,
    	 * 				(Direction = FALSE) indicates the backwards direction towards start location
    	 * 				Follow line using trackLine
    	 * 					if off line completely send message
    	 * 				Stop and send message when an obstacle is detected, ignore when at a destination
    	 * 					Start timer
    	 * 					if timer (or put into light sleep with gpio wakeup and timer wakeup) runs out before obstacle is no longer detected
    	 * 						return back to the start
    	 * 					else
    	 * 				Check for load is gone when at pickup location, ignore otherwise
    	 * 		Else
    	 * 			Go back to sleep and wait more
    	 *
    	 */

    	run_trashe();
    	//printf("Left Photo: %d, Right Photo: %d\n", readRawPhotoVal(ADC1_CHANNEL_3), readRawPhotoVal(ADC1_CHANNEL_6));
    	//vTaskDelay(1000 / portTICK_RATE_MS);
    	/*switch(trackLine(ADC1_CHANNEL_3, ADC1_CHANNEL_6, true, &photoValStruct, direction))
    	{
    		case 1: // Left side triggered
    			digitalWrite(LEFT_LED, 1);
    			stop();
    			vTaskDelay(500 / portTICK_RATE_MS);
    			if (direction) {
    				rotate_right(motorSpeed);
    			}
				else {
					rotate_left(motorSpeed);
				}
    			vTaskDelay(500 / portTICK_RATE_MS);
    			break;
    		case 2: // Right Side Triggered
    			digitalWrite(RIGHT_LED, 1);
    			stop();
    			vTaskDelay(500 / portTICK_RATE_MS);
    			if (direction) {
    				rotate_left(motorSpeed);
    			}
    			else {
    				rotate_right(motorSpeed);
    			}
    			vTaskDelay(500 / portTICK_RATE_MS);
    			break;
    		case 3:
    			stop();
    			vTaskDelay(1000 / portTICK_RATE_MS);
				if (trackLine(ADC1_CHANNEL_3, ADC1_CHANNEL_6, true, &photoValStruct, direction) == 3) {
					digitalWrite(RIGHT_LED, 1);
					digitalWrite(LEFT_LED, 1);
					vTaskDelay(500 / portTICK_RATE_MS);
					digitalWrite(RIGHT_LED, 0);
					digitalWrite(LEFT_LED, 0);

					vTaskDelay(5000 / portTICK_RATE_MS);
					direction = !direction;
				}
				break;
    		case -1:
    			stop();
    			vTaskDelay(500 / portTICK_RATE_MS);
    			break;
    		default:
    			digitalWrite(LEFT_LED, 0);
    			digitalWrite(RIGHT_LED, 0);
    			if (direction) {
    				forward(motorSpeed);
    			}
    			else {
    				reverse(motorSpeed);
    			}
    			vTaskDelay(500 / portTICK_RATE_MS);
    	}*/
    }
} 
