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

void print_mac(const unsigned char *mac);

void app_main(void)
{

    esp_chip_info_t chip_info;
    bool wifiFlag = false;
    esp_err_t errReturn = 0;

    printf("**Imprimiendo informacion sobre las capacidades WI-FI del SoC**\n\n");

    esp_chip_info(&chip_info);
    wifiFlag = (chip_info.features & CHIP_FEATURE_WIFI_BGN);
    printf("El SoC tiene capacidades Wi-fi?..: %s\n", wifiFlag ? "YES" : "NO");

    if (wifiFlag)
    {
        // Get base MAC address which is factory-programmed by Espressif in EFUSE.
        unsigned char mac_base[6] = {0};
        errReturn = esp_efuse_mac_get_default(mac_base);
        checkError(errReturn);

        // Read base MAC address, then calculates the MAC address of the specific interface requested,
        // refer to ESP-IDF Programming Guide for the algorithm.
        errReturn = esp_read_mac(mac_base, ESP_MAC_WIFI_STA);
        checkError(errReturn);

        printf("MAC Address: ");
        print_mac(mac_base);
    }

    printf("\nRestarting in %d seconds...: ", TIME_WAIT);
    for (int i = TIME_WAIT; i >= 0; i--)
    {
        printf("%d ", i);
        fflush(stdout);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}

void checkError(esp_err_t error)
{
    if (error != 0)
    {
        printf("%s\n", esp_err_to_name(error));
    }
}

void print_mac(const unsigned char *mac)
{
    printf("%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}