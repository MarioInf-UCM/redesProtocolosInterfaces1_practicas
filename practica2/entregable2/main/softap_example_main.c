/*  WiFi softAP Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

/* The examples use WiFi configuration that you can set via project configuration menu.

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#define EXAMPLE_ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define EXAMPLE_ESP_WIFI_CHANNEL   CONFIG_ESP_WIFI_CHANNEL
#define EXAMPLE_MAX_STA_CONN       CONFIG_ESP_MAX_STA_CONN

static const char *TAG = "wifi softAP";

/***************************************************
* FUNCIONES PARA EL TRATAMIENDO DE EVENTOS - INICIO
****************************************************/

static void wifi_event_handler_start(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    ESP_LOGI(TAG, "Driver WIFI iniciado correctamente.");
}


static void wifi_event_handler_stop(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    ESP_LOGI(TAG, "Detectada parada del driver WIFI. Cerrando el driver WIFI.");
    ESP_ERROR_CHECK(esp_wifi_deinit());
    ESP_LOGI(TAG, "Driver WIFI cerrado con exito.");
}

static void wifi_event_handler_staconnected(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
    ESP_LOGI(TAG, "station "MACSTR" join, AID=%d",MAC2STR(event->mac), event->aid);
}


static void wifi_event_handler_stadisconnected(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
    ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d",MAC2STR(event->mac), event->aid);
}


/***************************************************
* FUNCIONES PARA EL TRATAMIENDO DE EVENTOS - FIN
****************************************************/


void wifi_init_softap(void)
{
    ESP_LOGI(TAG, "\n\n*\n**CONFIGURACIÓN WIFI - INICIO\n");

    //FASE 1..: CREAMOS E INICIALIZAMOS LOS ELEMENTOS NECESARIOS PARA PODER OPERAR CON EL WIFI
    ESP_LOGI(TAG, "\n\nFASE 1..: CREAMOS E INICIALIZAMOS LOS ELEMENTOS NECESARIOS PARA PODER OPERAR CON EL WIFI");
    ESP_ERROR_CHECK(esp_netif_init());                      //Creamos la pila TCP/IP
    ESP_ERROR_CHECK(esp_event_loop_create_default());       //Creamos el bucle para la gestión de los eventos
    esp_netif_create_default_wifi_ap();                     //Creamos la interfáz de red para asociar el dispositivo a la pila TCP/IP
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));                   //Inicializamos la tarea que gestionará la conexión WIFI

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        WIFI_EVENT_AP_START,
                                                        &wifi_event_handler_start,
                                                        NULL,
                                                        NULL));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        WIFI_EVENT_AP_STOP,
                                                        &wifi_event_handler_stop,
                                                        NULL,
                                                        NULL));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        WIFI_EVENT_AP_STACONNECTED,
                                                        &wifi_event_handler_staconnected,
                                                        NULL,
                                                        NULL));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        WIFI_EVENT_AP_STADISCONNECTED,
                                                        &wifi_event_handler_stadisconnected,
                                                        NULL,
                                                        NULL));

    //FASE 2..:  CONFIGURAMOS EL TIPO Y LOS PARAMETROS DE LA CONEXIÓN WIFI A REALIZAR
    ESP_LOGI(TAG, "\n\nFASE 2..: CONFIGURAMOS EL TIPO Y LOS PARAMETROS DE LA CONEXIÓN WIFI A REALIZAR");
    wifi_config_t wifi_config = {                       //Creamos la estructura de configuración de conexión
        .ap = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,                  //SSID de la red wifi
            .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),      //Lonfitud de la SSID
            .channel = EXAMPLE_ESP_WIFI_CHANNEL,            //Canal a utilizar en la comunicación
            .password = EXAMPLE_ESP_WIFI_PASS,              //Contraseña se la red
            .max_connection = EXAMPLE_MAX_STA_CONN,         //Número maximo de conexiones permitidas

            #ifdef CONFIG_ESP_WIFI_SOFTAP_SAE_SUPPORT
                .authmode = WIFI_AUTH_WPA3_PSK,
                .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
            #else /* CONFIG_ESP_WIFI_SOFTAP_SAE_SUPPORT */
                .authmode = WIFI_AUTH_WPA2_PSK,             //Tipo de autentificación utilizada
            #endif

            .pmf_cfg = {
                    .required = true,
            },
        },
    };
    if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));               //Configuramos el modo como Punto de Acceso
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config)); //Establecemos la configuración de la conexión en base a los parámetros espeficiados anteriormente en la estructura.
   
   //FASE 3..: INICIAMOS LA EJECUCIÓN DEL FRIVER WIFI
    ESP_LOGI(TAG, "\n\nFASE 3..: INICIAMOS LA EJECUCIÓN DEL FRIVER WIFI");
    ESP_ERROR_CHECK(esp_wifi_start());                              //Iniciamos el deriver WIFI
   
    ESP_LOGI(TAG, "\n\n*\n**CONFIGURACIÓN WIFI - FIN\n");

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
             EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS, EXAMPLE_ESP_WIFI_CHANNEL);
}

void app_main(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
    wifi_init_softap();


    ESP_LOGI(TAG, "**CIERRE DEL PRINTO DE ACCESO EN %d SEGUNDOS", CONFIG_ESP_TIME_CLOSE_WIFI) ;
    for (int i = CONFIG_ESP_TIME_CLOSE_WIFI; i >= 0; i--)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    ESP_LOGI(TAG, "**TIEMPO FINALIZARO, CERRANDO DRIVER WIFI") ;
    esp_wifi_disconnect();
    esp_wifi_stop();

}
