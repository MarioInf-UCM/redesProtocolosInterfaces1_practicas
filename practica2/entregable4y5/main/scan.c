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
#define KNOWN_NETWORKS_LIST CONFIG_KNOWN_NETWORKS_LIST
#define KNOWN_NETWORKS_SSID_SIZE 32
#define KNOWN_NETWORKS_PASS_SIZE 64

#define EXAMPLE_ESP_MAXIMUM_RETRY  CONFIG_ESP_MAXIMUM_RETRY
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

#if CONFIG_ESP_WPA3_SAE_PWE_HUNT_AND_PECK
#define ESP_WIFI_SAE_MODE WPA3_SAE_PWE_HUNT_AND_PECK
#define EXAMPLE_H2E_IDENTIFIER ""
#elif CONFIG_ESP_WPA3_SAE_PWE_HASH_TO_ELEMENT
#define ESP_WIFI_SAE_MODE WPA3_SAE_PWE_HASH_TO_ELEMENT
#define EXAMPLE_H2E_IDENTIFIER CONFIG_ESP_WIFI_PW_ID
#elif CONFIG_ESP_WPA3_SAE_PWE_BOTH
#define ESP_WIFI_SAE_MODE WPA3_SAE_PWE_BOTH
#define EXAMPLE_H2E_IDENTIFIER CONFIG_ESP_WIFI_PW_ID
#endif

#if CONFIG_ESP_WIFI_AUTH_OPEN
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_OPEN
#elif CONFIG_ESP_WIFI_AUTH_WEP
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WEP
#elif CONFIG_ESP_WIFI_AUTH_WPA_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA2_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA2_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA_WPA2_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA_WPA2_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA3_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA3_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA2_WPA3_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA2_WPA3_PSK
#elif CONFIG_ESP_WIFI_AUTH_WAPI_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WAPI_PSK
#endif



//*************************************************
// DEFINICIÓN DE TIPO DE DATOS - INICIO
struct knownNetwork {
    uint8_t *sentence;
    uint8_t *SSID;
    uint8_t *pass;
}knownNetwork;

void knownNetwork_initValue(struct knownNetwork *data){
    data->sentence=NULL;
    data->SSID=NULL;
    data->pass=NULL;
    return;
}
// DEFINICIÓN DE TIPO DE DATOS - FIN
//*************************************************

static char* print_auth_mode(int authmode);
static char* print_cipher_type(int pairwise_cipher, int group_cipher);
static void wifi_scan(void);
static void splitKnownNetworks(struct knownNetwork *list, char *string);
static void initStruct_knownNetwork(struct knownNetwork *data);
static void printKnownNetworksList(struct knownNetwork* list);
static EventGroupHandle_t s_wifi_event_group;

static const char *TAG = "scan";
static int numKnownNetworks=0;


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
    s_wifi_event_group = xEventGroupCreate();
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
    
    char *stringKnownNetworksList = KNOWN_NETWORKS_LIST;
    struct knownNetwork knownNetworksList[KNOWN_NETWORKS_LIST_SIZE];
    for (int i=0 ; i<KNOWN_NETWORKS_LIST_SIZE ; i++){
        knownNetwork_initValue(&knownNetworksList[i]);
    }
    splitKnownNetworks(knownNetworksList, stringKnownNetworksList);
    printKnownNetworksList(knownNetworksList);

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

    //PASO 4..: CONEXIÓN A RED CONOCIDAS
    ESP_LOGI(TAG, "PASO 4..: CONEXIÓN A RED CONOCIDA");
    int posToConnect = -1;
    for(int i=0 ; i<numKnownNetworks ; i++){
        for(int j=0 ; (j < DEFAULT_SCAN_LIST_SIZE) && (j < ap_count) ; j++){
            if(strcmp((const char *)knownNetworksList[i].SSID, (const char *)ap_info[j].ssid)==0){
                posToConnect=i;
            }
        }
    }


    wifi_config_t wifiConfig = {
        .sta = {
            .ssid = " ",
            .password = " ",
            .threshold.authmode = ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD,
            .sae_pwe_h2e = ESP_WIFI_SAE_MODE,
            .sae_h2e_identifier = EXAMPLE_H2E_IDENTIFIER
        },
    };
    if(posToConnect==-1){
        ESP_LOGI(TAG, "NO SE HA DETECTADO NINGUNA RED CONOCIDA");
    }else{
        for(int i=0 ; i<KNOWN_NETWORKS_SSID_SIZE ; i++){
            wifiConfig.sta.ssid[i] = knownNetworksList[posToConnect].SSID[i];
        }
        for(int i=0 ; i<KNOWN_NETWORKS_PASS_SIZE ; i++){
            wifiConfig.sta.password[i] = knownNetworksList[posToConnect].pass[i];
        }
    
        ESP_LOGI(TAG, "INTENTANDO CONECTAR A: %s (%s)", wifiConfig.sta.ssid, wifiConfig.sta.password);
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifiConfig));
        esp_wifi_connect();
    }

    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                            pdFALSE,
                                            pdFALSE,
                                            portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                 wifiConfig.sta.ssid, wifiConfig.sta.password);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
                 wifiConfig.sta.ssid, wifiConfig.sta.password);
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }


    return;
}


static void splitKnownNetworks(struct knownNetwork *list, char *string){
   
    int i=0, j=0, k=0, k2=0;
    bool flagBreak = true;
    ESP_LOGI(TAG, "EXTRAYENDO REDES CONOCIDAS: %s", string);

    if(strlen(string)>0){
        initStruct_knownNetwork(&list[0]);
    }

    while(string[i]!=';' || flagBreak==false){
        if(string[i]!=' ' && string[i]!=';'){
            list[j].sentence[k] = string[i];
            flagBreak=false;
            k++;
        }else{
            //ESP_LOGI(TAG, "VALORRRRRRRRR: %s", list[j].sentence);

            numKnownNetworks++;
            k=0;
            while(list[j].sentence[k]!=':'){
            //ESP_LOGI(TAG, "VALOR: %c", list[j].sentence[k]);
                list[j].SSID[k] = list[j].sentence[k];
                k++;
            }
            
            k++;
            k2=0;
            while(list[j].sentence[k]!='\0'){
                list[j].pass[k2] = list[j].sentence[k];
                k++;
                k2++;
            }

            flagBreak=true;
            j++;
            k=0;
            if(string[i]!=';'){
                initStruct_knownNetwork(&list[j]);
            }else{
                break;
            }
        } 
        i++;
    } 
    return;
}

static void initStruct_knownNetwork(struct knownNetwork *data){
    
    data->sentence = (uint8_t *) malloc((KNOWN_NETWORKS_SSID_SIZE+KNOWN_NETWORKS_PASS_SIZE+1)*sizeof(uint8_t));
    memset ( data->sentence, '\0', KNOWN_NETWORKS_SSID_SIZE+KNOWN_NETWORKS_PASS_SIZE+1);
    
    data->SSID = (uint8_t *) malloc((KNOWN_NETWORKS_SSID_SIZE)*sizeof(uint8_t));
    memset ( data->SSID, '\0', KNOWN_NETWORKS_SSID_SIZE);
    
    data->pass = (uint8_t *) malloc((KNOWN_NETWORKS_PASS_SIZE)*sizeof(uint8_t));
    memset ( data->pass, '\0', KNOWN_NETWORKS_PASS_SIZE);

    return;
}


static void printKnownNetworksList(struct knownNetwork* list){
    ESP_LOGI(TAG, "Redes conocidas..:");
    for(int i=0 ; i<numKnownNetworks ; i++){
        ESP_LOGI(TAG, "\tPrioridad %d)\t(%s) SSID: %s    Password: %s", 
                    i+1, 
                    list[i].sentence, 
                    list[i].SSID, 
                    list[i].pass);
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
