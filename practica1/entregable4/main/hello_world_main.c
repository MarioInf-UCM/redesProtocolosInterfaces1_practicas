/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "sdkconfig.h"

#define TIME_WAIT_TASK1 CONFIG_TIME_WAIT_TASK1 
#define TIME_WAIT_TASK2 CONFIG_TIME_WAIT_TASK2

void task1(void *chip_info_parameter);
void task2(void *chip_info_parameter);


void app_main(void)
{

    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    xTaskCreate(&task1, "task1", 2048, &chip_info, 5, NULL);
    xTaskCreate(&task2, "task2", 2048, &chip_info, 5, NULL);

}



void task1(void *chip_info_parameter){
    
    int count = 0;
    esp_chip_info_t *chip_info = (esp_chip_info_t *) chip_info_parameter;

    while(1){
        count++;
        printf("\n**Información imprimida por la tarea 1 cada %d segundos (mensaje numero: %d)**\n", TIME_WAIT_TASK1, count);    
        printf("\tThis is %s chip with %d CPU core(s), %s%s%s%s\n\n, ",
            CONFIG_IDF_TARGET,
            chip_info->cores,
            (chip_info->features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
            (chip_info->features & CHIP_FEATURE_BT) ? "BT" : "",
            (chip_info->features & CHIP_FEATURE_BLE) ? "BLE" : "",
            (chip_info->features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

        vTaskDelay(TIME_WAIT_TASK1*1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);

} 


void task2(void *chip_info_parameter){

    esp_chip_info_t *chip_info = (esp_chip_info_t *) chip_info_parameter;
    uint32_t flash_size;
    int count=0;

    while(1){
        count++;
        printf("\n**Información imprimida por la tarea 2 cada %d segundos (mensaje numero: %d)**\n", TIME_WAIT_TASK2, count);    
        unsigned major_rev = chip_info->revision / 100;
        unsigned minor_rev = chip_info->revision % 100;
        printf("\tsilicon revision v%d.%d, ", major_rev, minor_rev);
        if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
            printf("Get flash size failed");
            return;
        }
        printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
            (chip_info->features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
        printf("\tMinimum free heap size: %" PRIu32 " bytes\n\n", esp_get_minimum_free_heap_size());

        vTaskDelay(TIME_WAIT_TASK2*1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
    
} 