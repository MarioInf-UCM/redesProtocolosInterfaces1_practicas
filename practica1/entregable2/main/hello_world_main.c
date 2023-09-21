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
#include "esp_mac.h"
#define TIME_WAIT 100

void checkError(esp_err_t error);

void app_main(void){

    esp_chip_info_t chip_info;
    bool wifiFlag = false;
    esp_err_t errReturn = 0;
    uint8_t mac;

    printf("**Imprimiendo informacion sobre las capacidades WI-FI del SoC**\n\n");

    esp_chip_info(&chip_info);
    wifiFlag = (chip_info.features & CHIP_FEATURE_WIFI_BGN);
    printf("El SoC tiene capacidades Wi-fi?..: %s\n", wifiFlag ? "YES" : "NO");

    if(wifiFlag){
        errReturn = esp_base_mac_addr_get(&mac);
        printf("esp_base_mac_addr_get: %d\n", mac);
        checkError(errReturn);

        errReturn = esp_efuse_mac_get_default(&mac);
        printf("esp_efuse_mac_get_default: %d\n", mac);
        checkError(errReturn);





    }


    //chip_info.features;

/*     uint8_t macWIFI;
    read_mac(&macWIFI, ESP_MAC_WIFI_STA);
    printf("\nPEPEPE %s", macWIFI);
 */


/*     unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);
    if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        printf("Get flash size failed");
        return;
    }

    printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());
 */
    printf("\nRestarting in %d seconds...: ", TIME_WAIT);
    for (int i = TIME_WAIT; i >= 0; i--) {
        printf("%d ", i);
        fflush(stdout);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}


void checkError(esp_err_t error){
    if(error!=0){
        printf("%s\n", esp_err_to_name(error));
    }
}