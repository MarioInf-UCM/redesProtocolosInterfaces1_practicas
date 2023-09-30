/* Scan Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

/*
    This example shows how to scan for available set of APs.
*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "sdkconfig.h"

#define DEFAULT_SCAN_LIST_SIZE CONFIG_EXAMPLE_SCAN_LIST_SIZE

#define WIFICONF_CHANNEL CONFIG_WIFICONF_CHANNEL
#define WIFICONF_SCANTIME_ACTIVE_MIN CONFIG_WIFICONF_SCANTIME_ACTIVE_MIN
#define WIFICONF_SCANTIME_ACTIVE_MAX CONFIG_WIFICONF_SCANTIME_ACTIVE_MAX
#define WIFICONF_SCANTIME_PASSIVE CONFIG_WIFICONF_SCANTIME_PASSIVE
#if CONFIG_WIFICONF_SHOW_HIDDEN_ENABLE
#define WIFICONF_SHOW_HIDDEN CONFIG_WIFICONF_SHOW_HIDDEN_ENABLE
#elif CONFIG_WIFICONF_SHOW_HIDDEN_DISABLE
#define WIFICONF_SHOW_HIDDEN CONFIG_WIFICONF_SHOW_HIDDEN_DISABLE
#endif
#if CONFIG_WIFICONF_SCAN_MODE_ACTIVE
#define WIFICONF_SCAN_MODE CONFIG_WIFICONF_SCAN_MODE_ACTIVE
#elif CONFIG_WIFICONF_SCAN_MODE_PASIVE
#define WIFICONF_SCAN_MODE CONFIG_WIFICONF_SCAN_MODE_PASIVE
#endif

#define KNOWN_NETWORKS_LIST_SIZE CONFIG_KNOWN_NETWORKS_LIST_SIZE
#define KNOWN_NETWORKS_SSID_SIZE CONFIG_KNOWN_NETWORKS_SSID_SIZE
#define KNOWN_NETWORKS_PASS_SIZE CONFIG_KNOWN_NETWORKS_PASS_SIZE
#define KNOWN_NETWORKS_LIST CONFIG_KNOWN_NETWORKS_LIST


static char* print_auth_mode(int authmode);
static char* print_cipher_type(int pairwise_cipher, int group_cipher);
static void wifi_scan(void);
static void splitKnownNetworks(char* string);

static struct knownNetwork {
  char myNum[20];
  char myLetter[];
};

static const char *TAG = "scan";

static char* print_auth_mode(int authmode)
{
    switch (authmode) {
    case WIFI_AUTH_OPEN:
        return "Authmode: WIFI_AUTH_OPEN";
        break;
    case WIFI_AUTH_OWE:
        return "Authmode: WIFI_AUTH_OWE";
        break;
    case WIFI_AUTH_WEP:
        return "Authmode: WIFI_AUTH_WEP";
        break;
    case WIFI_AUTH_WPA_PSK:
        return "Authmode: WIFI_AUTH_WPA_PSK";
        break;
    case WIFI_AUTH_WPA2_PSK:
        return "Authmode: WIFI_AUTH_WPA2_PSK";
        break;
    case WIFI_AUTH_WPA_WPA2_PSK:
        return "Authmode: WIFI_AUTH_WPA_WPA2_PSK";
        break;
    case WIFI_AUTH_WPA2_ENTERPRISE:
        return "Authmode: WIFI_AUTH_WPA2_ENTERPRISE";
        break;
    case WIFI_AUTH_WPA3_PSK:
        return "Authmode: WIFI_AUTH_WPA3_PSK";
        break;
    case WIFI_AUTH_WPA2_WPA3_PSK:
        return "Authmode: WIFI_AUTH_WPA2_WPA3_PSK";
        break;
    default:
        return "Authmode: WIFI_AUTH_UNKNOWN";
        break;
    }
}

static char* print_cipher_type(int pairwise_cipher, int group_cipher)
{
    switch (pairwise_cipher) {
    case WIFI_CIPHER_TYPE_NONE:
        return "Pairwise Cipher: WIFI_CIPHER_TYPE_NONE";
        break;
    case WIFI_CIPHER_TYPE_WEP40:
        return "Pairwise Cipher: WIFI_CIPHER_TYPE_WEP40";
        break;
    case WIFI_CIPHER_TYPE_WEP104:
        return "Pairwise Cipher: WIFI_CIPHER_TYPE_WEP104";
        break;
    case WIFI_CIPHER_TYPE_TKIP:
        return "Pairwise Cipher: WIFI_CIPHER_TYPE_TKIP";
        break;
    case WIFI_CIPHER_TYPE_CCMP:
        return "Pairwise Cipher: WIFI_CIPHER_TYPE_CCMP";
        break;
    case WIFI_CIPHER_TYPE_TKIP_CCMP:
        return "Pairwise Cipher: WIFI_CIPHER_TYPE_TKIP_CCMP";
        break;
    case WIFI_CIPHER_TYPE_AES_CMAC128:
        return "Pairwise Cipher: WIFI_CIPHER_TYPE_AES_CMAC128";
        break;
    case WIFI_CIPHER_TYPE_SMS4:
        return "Pairwise Cipher: WIFI_CIPHER_TYPE_SMS4";
        break;
    case WIFI_CIPHER_TYPE_GCMP:
        return "Pairwise Cipher: WIFI_CIPHER_TYPE_GCMP";
        break;
    case WIFI_CIPHER_TYPE_GCMP256:
        return "Pairwise Cipher: WIFI_CIPHER_TYPE_GCMP256";
        break;
    default:
        return "Pairwise Cipher: WIFI_CIPHER_TYPE_UNKNOWN";
        break;
    }

    switch (group_cipher) {
    case WIFI_CIPHER_TYPE_NONE:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_NONE");
        break;
    case WIFI_CIPHER_TYPE_WEP40:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_WEP40");
        break;
    case WIFI_CIPHER_TYPE_WEP104:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_WEP104");
        break;
    case WIFI_CIPHER_TYPE_TKIP:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_TKIP");
        break;
    case WIFI_CIPHER_TYPE_CCMP:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_CCMP");
        break;
    case WIFI_CIPHER_TYPE_TKIP_CCMP:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_TKIP_CCMP");
        break;
    case WIFI_CIPHER_TYPE_SMS4:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_SMS4");
        break;
    case WIFI_CIPHER_TYPE_GCMP:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_GCMP");
        break;
    case WIFI_CIPHER_TYPE_GCMP256:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_GCMP256");
        break;
    default:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_UNKNOWN");
        break;
    }
}

/* Initialize Wi-Fi as sta and set scan method */
static void wifi_scan(void)
{
    //PASO 0..: CONFIGURACIÓN DE ELEMENTOS ANEXOS AL ESCANEO
    ESP_LOGI(TAG, "PASO 0..: CONFIGURACIÓN DE ELEMENTOS ANEXOS AL ESCANEO");
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    uint16_t number = DEFAULT_SCAN_LIST_SIZE;
    wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE];
    uint16_t ap_count = 0;
    memset(ap_info, 0, sizeof(ap_info));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));


    //PASO 1..: CONFIGURACIÓN DEL ESCANEO
    ESP_LOGI(TAG, "PASO 1..: CONFIGURACIÓN DEL ESCANEO");
    esp_wifi_set_country_code("ES", true);
    ESP_ERROR_CHECK(esp_wifi_start());

    //PASO 2..: REALIZACIÓN DEL ESCANEO
    ESP_LOGI(TAG, "PASO 2..: REALIZACIÓN DEL ESCANEO");
    wifi_scan_config_t scan_config = {
        #ifdef WIFICONF_SHOW_HIDDEN_ENABLE
		.show_hidden = true,
        #elif WIFICONF_SHOW_HIDDEN_DISABLE
		.show_hidden = false,
        #endif
		.channel = WIFICONF_CHANNEL,
		.scan_type = WIFICONF_SCAN_MODE,
		.scan_time.active.min = WIFICONF_SCANTIME_ACTIVE_MIN,
		.scan_time.active.max = WIFICONF_SCANTIME_ACTIVE_MAX,
        .scan_time.passive = WIFICONF_SCANTIME_PASSIVE
	};

    ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));

    //PASO 3..: OBTENCIÓN DE RESULTADOS
    ESP_LOGI(TAG, "PASO 3..: OBTENCIÓN DE RESULTADOS");
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));

    ESP_LOGI(TAG, "Total APs scanned = %u", ap_count);
    for (int i = 0; (i < DEFAULT_SCAN_LIST_SIZE) && (i < ap_count); i++) {
        if (ap_info[i].authmode != WIFI_AUTH_WEP) {
            ESP_LOGI(TAG, "SSID: %s    RSSI: %d    %s    %s    CHANNEL: %d", 
                    ap_info[i].ssid, 
                    ap_info[i].rssi,  
                    print_auth_mode(ap_info[i].authmode),
                    print_cipher_type(ap_info[i].pairwise_cipher, ap_info[i].group_cipher),
                    ap_info[i].primary);
        }else{
            ESP_LOGI(TAG, "SSID: %s    RSSI: %d    %s    CHANNEL: %d", 
                    ap_info[i].ssid, 
                    ap_info[i].rssi,  
                    print_auth_mode(ap_info[i].authmode),
                    ap_info[i].primary);
        }
    }

}

static void splitKnownNetworks(char* string){
   char * token = strtok(string, " ");
   while( token != NULL ) {
      printf( " %s\n", token );
      token = strtok(NULL, " ");
   }
   return;
}


void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    wifi_scan();
}
