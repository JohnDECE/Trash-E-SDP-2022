
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
bool towardsPickup = true;
bool messageSent = false;
bool atHome = true;
bool atPickup = false;
bool findAttempt = false;
bool wasRev = false;
bool wasForward = true;

// uint8_t RIGHT_PHOTO = 34;
// uint8_t LEFT_PHOTO = 39;
uint8_t LEFT_LED = 16;
uint8_t RIGHT_LED = 17;
uint8_t ULTRA_DIRECTION = 26;
uint8_t ULTRA_DETECTION = 36;
int motorSpeed = 255;
uint8_t tempPin = 12;


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
    setupMotors();

    setupPhotoresistors(LEFT_LED, RIGHT_LED, ADC1_CHANNEL_3, ADC1_CHANNEL_6, 3, ADC_ATTEN_DB_11, ULTRA_DIRECTION);
}

photoVals photoValStruct = {
		.Left_Photo = -1,
		.Right_Photo = -1,
};



void app_main(void)
{
	bool direction = true;
	setup();
    while(1)
    {
    	//forward(motorSpeed);
        objectDetected = digitalRead(ULTRA_DETECTION);

        if(!objectDetected)
        {

            //forward(motorSpeed);
            switch(trackLine(ADC1_CHANNEL_3, ADC1_CHANNEL_6, true, &photoValStruct, direction))
            {
                case 1: // Left side triggered
                    digitalWrite(LEFT_LED, 1);
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
                case -1: // Both off the line
                    stop();
                    vTaskDelay(500 / portTICK_RATE_MS);
                    break;

                default: // Default;
                	if (direction) {
                		forward(motorSpeed);
                	}
                	else {
                		reverse(motorSpeed);
                	}
                    digitalWrite(LEFT_LED, 0);
                    digitalWrite(RIGHT_LED, 0);
                    //vTaskDelay(500 / portTICK_RATE_MS);
                    break;
            }
            //printf("Track line result: %d, %d\n", photoValStruct.Left_Photo, photoValStruct.Right_Photo);
            //vTaskDelay(500 / portTICK_RATE_MS);
        }

        else
        {
            stop();

            // int x = 0;
            // stop();
            // while(objectDetected)
            // {
            //     if(x >= 5000)
            //     {
            //         send_to_bot();
            //     }
            //     stop();
            //     objectDetected = digitalRead(ULTRA_DETECTION);
            //     x++;
            // }
        }
    }
} 
