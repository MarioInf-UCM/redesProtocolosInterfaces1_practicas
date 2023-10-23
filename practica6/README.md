# PRÁCTICA 6 - Implementación de un cliente GATT



Tarea previa
```C
static const char remote_device_name[] = "ESP_GATTS_CLIENT_NOCONNECT";
```

Tarea  1
```C
static esp_ble_scan_params_t ble_scan_params = {
    .scan_type              = BLE_SCAN_TYPE_ACTIVE,
    .own_addr_type          = BLE_ADDR_TYPE_PUBLIC,
    .scan_filter_policy     = BLE_SCAN_FILTER_ALLOW_ALL,
    .scan_interval          = 0x500, //0x50
    .scan_window            = 0x30,
    .scan_duplicate         = BLE_SCAN_DUPLICATE_DISABLE
};
```

Tarea 2 y 3

```C
    config SCANNING_TIME_LENGTH
        int "Duration of a scan (s)"
        range 100 300
        default 300

    config PERIOD_BEWTEEN_TWO_SCAN
        int "Period between two scan (s)"
        range 100 30000
        default 3000

    config PERIOD_BEWTEEN_TWO_SCAN_EVENTS
        int "Period between two scan (1,25ms * X)"
        range 100 3000
        default 500
```

```C
static esp_ble_scan_params_t ble_scan_params = {
    .scan_type              = BLE_SCAN_TYPE_ACTIVE,
    .own_addr_type          = BLE_ADDR_TYPE_PUBLIC,
    .scan_filter_policy     = BLE_SCAN_FILTER_ALLOW_ALL,
    .scan_interval          = (u_int16_t) PERIOD_BEWTEEN_TWO_SCAN_EVENTS,
    .scan_window            = 0x30,
    .scan_duplicate         = BLE_SCAN_DUPLICATE_DISABLE
};
```



```C
static void taskFunction_scanning(void *parameters){

    int duration = (int) parameters;
    while(1){
        ESP_LOGI(GATTC_TAG, "** START SCANNING PROCESS **");
        esp_ble_gap_start_scanning(duration); 
        vTaskDelay(period*1000 / portTICK_PERIOD_MS);
        ESP_LOGI(GATTC_TAG, "** FINISH SCANNING PROCESS **");
    }
    vTaskDelete(NULL);
}
```






