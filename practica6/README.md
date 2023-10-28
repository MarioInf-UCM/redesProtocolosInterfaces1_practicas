# PRÁCTICA 6 - Implementación de un cliente GATT

## Tarea previa
```C
static const char remote_device_name[] = "ESP_GATTS_CLIENT_NOCONNECT";
```

## Tarea 1
> Configura los parámetros de escaneo para que éste se produzca con menos frecuencia (e.g. 1 segundo o un valor superior). 
Para ello, adapta el valor del campo `scan_interval` con el valor apropiado.

Para configurar el varlo de `scan_interval` a 4 segundos consultamos la documentación oficial en [este enlace](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/bluetooth/esp_gap_ble.html#_CPPv4N21esp_ble_scan_params_t13scan_intervalE).

En este caso 4 segundos = 4000 ms = 6400 * 0.625.
Convirtiendo este valor a hexadecimal obtenemos 0x1900, estableciendolo como valor por defecto en el parámetro `BLE_SCAN_INTERVAL` en sdkconfig.

```C
static esp_ble_scan_params_t ble_scan_params = {
    .scan_type              = BLE_SCAN_TYPE_ACTIVE,
    .own_addr_type          = BLE_ADDR_TYPE_PUBLIC,
    .scan_filter_policy     = BLE_SCAN_FILTER_ALLOW_ALL,
    .scan_interval          = BLE_SCAN_LE_INTERVAL, 
    .scan_window            = BLE_SCAN_LE_WINDOW,
    .scan_duplicate         = BLE_SCAN_DUPLICATE_DISABLE
};
```

## Tarea 2
> El valor de duración del proceso de escaneado es configurable. En esta primera parte de la práctica, auméntalo para que el proceso sea más largo y así tengas más tiempo para analizar la salida producida. Comprueba que efectivamente el tiempo en el que el dispositivo se encuentra en fase de escaneo es el seleccionado.

Aumentamos el valor por defecto a 60 segundos para poder analizar la salida:

```c
case ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT:
    {
        // the unit of the duration is second
        uint32_t duration = 60;
        esp_ble_gap_start_scanning(duration);
        break;
    }
```

## Tarea 3
> Analiza el código y modifícalo para que el proceso de escaneo se produzca cíclicamente, con un parámetro de separación entre fases de escaneado definido a través de los menús de configuración. De la misma manera, añade a dichos menús un parámetro adicional que indique el inervalo entre eventos de escaneo.

Para conseguir que el proceso sea cíclico se han definido los siguientes parámetros en el fichero `Kconfig.projbuild`:

```
    config BLE_SCAN_LE_INTERVAL
        hex "Ble scan interval (Time = N * 0.625 msec Time Range: 2.5 msec to 10240 msec)"
        range 0x0004 0x4000
        default 0x1900
    
    config BLE_SCAN_LE_WINDOW
        hex "The duration of the LE scan. LE_Scan_Window shall be less than or equal to LE_Scan_Interval (Time = N * 0.625 msec Time Range: 2.5 msec to 10240 msec)"
        range 0x0004 0x4000
        default 0x0010
    
    config BLE_SCAN_DURATION
        int "Duration of a scan (s)"
        range 60 300
        default 60
    
    config BLE_SCAN_DELAY
        int "Delay before starting new scan (s)"
        range 10 600
        default 30
```

Los parámetros `BLE_SCAN_LE_INTERVAL` y `BLE_SCAN_LE_WINDOW` se han definido como 4s y 1s respectivamente en la estructura `esp_ble_scan_params_t`.

```c
static esp_ble_scan_params_t ble_scan_params = {
    .scan_type = BLE_SCAN_TYPE_ACTIVE,
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    .scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ALL,
    .scan_interval = BLE_SCAN_LE_INTERVAL,
    .scan_window = BLE_SCAN_LE_WINDOW,
    .scan_duplicate = BLE_SCAN_DUPLICATE_DISABLE};

```

Por otro lado utilizamos `BLE_SCAN_DURATION` a la hora de iniciar un nuevo escaneo:

```c
    uint32_t duration = BLE_SCAN_DURATION; // the unit of the duration is second
    esp_ble_gap_start_scanning(duration);
```

Una vez finalizado el escaneo, dentro del tratamiento del evento `ESP_GAP_SEARCH_INQ_CMPL_EVT` esperamos 30s segundos establecidos por defecto en el parámetro `BLE_SCAN_DELAY`:

```c
    case ESP_GAP_SEARCH_INQ_CMPL_EVT:
        // Wait BLE_SCAN_DELAY seconds
        ESP_LOGI(GATTC_TAG, "Waiting %d seconds before starting a new scan", BLE_SCAN_DELAY);
        vTaskDelay(BLE_SCAN_DELAY * 1000 / portTICK_PERIOD_MS);
        // Initiate new scan
        ESP_LOGI(GATTC_TAG, "Starting a new %d seconds duration scan", BLE_SCAN_DURATION);
        uint32_t duration = BLE_SCAN_DURATION; // the unit of the duration is second
        esp_ble_gap_start_scanning(duration);
        break;

```

Resumiendo, los valores establecidos para los parámetros son los siguientes:
- BLE_SCAN_LE_INTERVAL: 4s
- BLE_SCAN_LE_WINDOW: 10ms
- BLE_SCAN_DURATION: 60s
- BLE_SCAN_DELAY: 30s

## Tarea 4
> Muestra por pantalla el valor de RSSI de cada dispositivo BLE encontrado. Para ello, busca en laestructura de parámetros recibidos el campo correspondiente.

Examinamos los parámetros de la estructura `esp_ble_gap_cb_param_t` recibida dentro del tratamiento del evento `ESP_GAP_BLE_SCAN_RESULT_EVT` en la [documentación oficial](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/bluetooth/esp_gap_ble.html#_CPPv4N22esp_ble_gap_cb_param_t25ble_scan_result_evt_param4rssiE) mostrando su valor:

```c
        case ESP_GAP_SEARCH_INQ_RES_EVT:
            ESP_LOGI(GATTC_TAG, "Device found");
            esp_log_buffer_hex(GATTC_TAG, scan_result->scan_rst.bda, 6);
            ESP_LOGI(GATTC_TAG, "searched Adv Data Len %d, Scan Response Len %d", scan_result->scan_rst.adv_data_len, scan_result->scan_rst.scan_rsp_len);
            adv_name = esp_ble_resolve_adv_data(scan_result->scan_rst.ble_adv,
                                                ESP_BLE_AD_TYPE_NAME_CMPL, &adv_name_len);
            ESP_LOGI(GATTC_TAG, "searched Device Name Len %d", adv_name_len);
            ESP_LOGI(GATTC_TAG, "searched Device RSSI %d", scan_result->scan_rst.rssi);
            esp_log_buffer_char(GATTC_TAG, adv_name, adv_name_len);
```

Verificamos como el valor es distinto para cada dispositivo detectado en el escaneo:

```BASH
I (237932) GATTC_CLIENT: Device found
I (237932) GATTC_CLIENT: bc 14 85 ea 9c 18
I (237932) GATTC_CLIENT: searched Adv Data Len 28, Scan Response Len 0
I (237932) GATTC_CLIENT: searched Device Name Len 0
I (237942) GATTC_CLIENT: searched Device RSSI -74
I (237942) GATTC_CLIENT:

I (237952) GATTC_CLIENT: Device found
I (237952) GATTC_CLIENT: 88 0f 10 8d df 4e
I (237962) GATTC_CLIENT: searched Adv Data Len 31, Scan Response Len 0
I (237962) GATTC_CLIENT: searched Device Name Len 0
I (237972) GATTC_CLIENT: searched Device RSSI -77
I (237982) GATTC_CLIENT:

I (238052) GATTC_CLIENT: Device found
I (238052) GATTC_CLIENT: bc 14 85 ea 9c 18
I (238052) GATTC_CLIENT: searched Adv Data Len 28, Scan Response Len 0
I (238062) GATTC_CLIENT: searched Device Name Len 0
I (238062) GATTC_CLIENT: searched Device RSSI -75
I (238072) GATTC_CLIENT:

I (241052) GATTC_CLIENT: Waiting 30 seconds before starting a new scan
```

## Tarea 5
> Añade a los menús de configuración un campo que indique la dirección MAC BLE de un dispositivo(conocido) que esté en fase de anuncio. Puedes utilizar los códigos de servidor de la práctica anterior.

Para obtener la MAC podemos examinar la salida del servidor:
```BASH
I (572) main_task: Calling app_main()
I (622) BTDM_INIT: BT controller compile version [946b762]
I (622) BTDM_INIT: Bluetooth MAC: 24:0a:c4:ea:28:c2
I (632) phy_init: phy_version 4670,719f9f6,Feb 18 2021,17:07:07
I (1042) main_task: Returned from app_main()
I (1042) GATTS_TABLE_DEMO: create attribute table successfully, the number handle = 8

I (1052) GATTS_TABLE_DEMO: SERVICE_START_EVT, status 0, service_handle 40
I (1062) GATTS_TABLE_DEMO: advertising start successfully
```

Configuramos por tanto un nuevo parámetro en el fichero `Kconfig.projbuild` del cliente:

```
    config MAC_KNOWN_DEVICE
        string "MAC for a known device on advertising state"
        default "24:0a:c4:ea:28:c2"
        help
            Specify a MAC address for the ethernet interface in the form of
            six hex 8-bit chars separated by colons (e.g.:
            aa:33:cc:22:e2:c0).  The default is an empty string, which
            means the code will make 00:00:5E:00:53:XX, where XX will be
            random.
```

Añadiendo su definición en el fichero `gattc_demo.c` para poder ser utilizada más adelante:
```c
#define MAC_KNOWN_DEVICE CONFIG_MAC_KNOWN_DEVICE
```

## Tarea 6
> Modifica la fase de escaneado para que únicamente se muestre información (al menos nombre y RSSI) sobre el dispositivo cuya dirección MAC se haya configurado.

Para ello añadimos un nuevo filtro dentro del tratamiento del evento `ESP_GAP_SEARCH_INQ_RES_EVT`:

```c
case ESP_GAP_SEARCH_INQ_RES_EVT:
    // MAC_KNOWN_DEVICE
    char macdetected[18];
    memset(&macdetected[0], 0, sizeof(macdetected)); // Clear array
    esp_bd_addr_to_string(scan_result->scan_rst.bda, macdetected);
    if (strcmp(macdetected, MAC_KNOWN_DEVICE) == 0)
    {
        adv_name = esp_ble_resolve_adv_data(scan_result->scan_rst.ble_adv,
                                            ESP_BLE_AD_TYPE_NAME_CMPL, &adv_name_len);
        if (adv_name_len > 0)
        {
            ESP_LOGI(GATTC_TAG, "MAC: %s | NAME: %s | RSSI: %d", macdetected, adv_name, scan_result->scan_rst.rssi);
        }
    }
```

Podemos ver en la salida la información filtrada por la MAC introducida como parámetro:

```BASH
I (6885072) GATTC_CLIENT: MAC: 24:0a:c4:ea:28:c2 | NAME: ESP_GATTS_SERVER | RSSI: -50
I (6889072) GATTC_CLIENT: MAC: 24:0a:c4:ea:28:c2 | NAME: ESP_GATTS_SERVER | RSSI: -48
I (6893092) GATTC_CLIENT: MAC: 24:0a:c4:ea:28:c2 | NAME: ESP_GATTS_SERVER | RSSI: -45
I (6897082) GATTC_CLIENT: MAC: 24:0a:c4:ea:28:c2 | NAME: ESP_GATTS_SERVER | RSSI: -50
I (6901062) GATTC_CLIENT: Waiting 30 seconds before starting a new scan
I (6931062) GATTC_CLIENT: Starting a new 60 seconds duration scan
I (6931062) GATTC_CLIENT: scan start success
I (6931092) GATTC_CLIENT: MAC: 24:0a:c4:ea:28:c2 | NAME: ESP_GATTS_SERVER | RSSI: -49
I (6935082) GATTC_CLIENT: MAC: 24:0a:c4:ea:28:c2 | NAME: ESP_GATTS_SERVER | RSSI: -45
I (6939082) GATTC_CLIENT: MAC: 24:0a:c4:ea:28:c2 | NAME: ESP_GATTS_SERVER | RSSI: -50
I (6943072) GATTC_CLIENT: MAC: 24:0a:c4:ea:28:c2 | NAME: ESP_GATTS_SERVER | RSSI: -48
I (6947072) GATTC_CLIENT: MAC: 24:0a:c4:ea:28:c2 | NAME: ESP_GATTS_SERVER | RSSI: -46
I (6951072) GATTC_CLIENT: MAC: 24:0a:c4:ea:28:c2 | NAME: ESP_GATTS_SERVER | RSSI: -49
I (6955072) GATTC_CLIENT: MAC: 24:0a:c4:ea:28:c2 | NAME: ESP_GATTS_SERVER | RSSI: -46
I (6959092) GATTC_CLIENT: MAC: 24:0a:c4:ea:28:c2 | NAME: ESP_GATTS_SERVER | RSSI: -46
I (6963092) GATTC_CLIENT: MAC: 24:0a:c4:ea:28:c2 | NAME: ESP_GATTS_SERVER | RSSI: -49
I (6967072) GATTC_CLIENT: MAC: 24:0a:c4:ea:28:c2 | NAME: ESP_GATTS_SERVER | RSSI: -47
I (6971072) GATTC_CLIENT: MAC: 24:0a:c4:ea:28:c2 | NAME: ESP_GATTS_SERVER | RSSI: -45
I (6975072) GATTC_CLIENT: MAC: 24:0a:c4:ea:28:c2 | NAME: ESP_GATTS_SERVER | RSSI: -49
I (6983082) GATTC_CLIENT: MAC: 24:0a:c4:ea:28:c2 | NAME: ESP_GATTS_SERVER | RSSI: -46
I (6987072) GATTC_CLIENT: MAC: 24:0a:c4:ea:28:c2 | NAME: ESP_GATTS_SERVER | RSSI: -49
I (6991062) GATTC_CLIENT: Waiting 30 seconds before starting a new scan
```

## Tarea 7
> Modifica el firmware básico para que, en función del valor de RSSI obtenido para el dispositivo deinterés, el cliente reporte con más o menos frecuencia por pantalla un valor proporcional a la distancia supuesta para dicho dispositivo. Si dispones de un LED conectado a la placa, puedes hacer que la frecuencia de parpadeo del mismo sea proporcional a dicha distancia.

En este caso hemos bajado el valor del parámetro `BLE_SCAN_LE_INTERVAL` a 1s es decir 0x640 en hexadecimal.

Al final de la función app_main hemos inicializado el led:

```c
    // Configure led for blinking
    ESP_LOGI(GATTC_TAG, "Configure blinking led!");
    gpio_reset_pin(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
```

Se ha establecido un parámetro estático `static int delay_blinking = 0;` y una función que realiza la operación de parpadeo y espera el tiempo establecido:

```c
static void taskFunction_blinking(void *parameters)
{

    while (1)
    {
        if (delay_blinking > 0)
        {
            /* Set the GPIO level according to the state (LOW or HIGH)*/
            gpio_set_level(BLINK_GPIO, s_led_state);
            /* Toggle the LED state */
            s_led_state = !s_led_state;
            vTaskDelay(delay_blinking / portTICK_PERIOD_MS);
        }
    }
    vTaskDelete(NULL);
}
```

Este parámetro se calcula automáticamente en funcion del RSSI multiplicándolo por 4 para ver las diferencias en milisegundos:

```c
delay_blinking = abs(scan_result->scan_rst.rssi) * 4;
ESP_LOGI(GATTC_TAG, "MAC: %s | NAME: %s | RSSI: %d | Blinking delay %d ms", macdetected, adv_name, scan_result->scan_rst.rssi, delay_blinking);
```

Por último la salida sería como la siguiente:

```BASH
I (106095) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -50 | Blinking delay 200 ms
I (107085) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -52 | Blinking delay 208 ms
I (108085) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -57 | Blinking delay 228 ms
I (109075) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -50 | Blinking delay 200 ms
I (110065) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -51 | Blinking delay 204 ms
I (111075) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -53 | Blinking delay 212 ms
I (112065) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -50 | Blinking delay 200 ms
I (115075) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -50 | Blinking delay 200 ms
I (118075) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -50 | Blinking delay 200 ms
I (119095) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -52 | Blinking delay 208 ms
I (122065) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -41 | Blinking delay 164 ms
I (123095) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -24 | Blinking delay 96 ms
I (124085) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -24 | Blinking delay 96 ms
I (126075) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -22 | Blinking delay 88 ms
I (130095) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -23 | Blinking delay 92 ms
I (132095) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -79 | Blinking delay 316 ms
I (133085) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -73 | Blinking delay 292 ms
I (134085) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -80 | Blinking delay 320 ms
I (138075) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -79 | Blinking delay 316 ms
I (141095) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -59 | Blinking delay 236 ms
I (143095) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -52 | Blinking delay 208 ms
I (145085) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -53 | Blinking delay 212 ms
I (146075) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -52 | Blinking delay 208 ms
I (148095) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -54 | Blinking delay 216 ms
I (149075) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -52 | Blinking delay 208 ms
I (150085) GATTC_CLIENT: MAC: 8c:aa:b5:b8:bf:f6 | NAME: ESP_GATTS_SERVER | RSSI: -57 | Blinking delay 228 ms
I (151065) GATTC_CLIENT: Waiting 30 seconds before starting a new scan
```

## Tarea 8
> Modifica el cliente para que las notificaciones enviadas por tu servidor GATT modificado en lapráctica anterior sean recibidas y monitorizadas por parte del cliente GATT.