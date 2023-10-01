# ESCANEADO DE REDES WIFI

## Analizando el ejemplo scan
>Tarea
>
>Compila, flashea y ejecuta el ejemplo de escaneado. Observa si los resultados son los esperados en el laboratorio o en un entorno doméstico. Modifica el código para conseguir distintos tipos de escaneado, asegurándote, por ejemplo, de que si fijas un canal específico en el que tu punto de acceso está trabajando, éste es detectado corretamente. Estudia y modifica los tiempos de espera en el escaneado y observa su efecto en el tiempo total de escaneado.

Para comenzar vamos a utilizar el código de ejemplo **scan** proporcioando por la plataforma, y tras un tiempo analizando, colocando y reorganizando las salidas para que este sea más visible, estamos listos para ejecutarlo y asi ver la salida proporcionada por el mismo.

En lo que corresponde a la configuración de los elementos previos, podemos ver como hemos incluido la especificación del país en el cual nos encontramos, con el objetivo de realizar un mejor escaneo de redes. A continuación podemos ver el fragmento de codigo mencionado y la salida obtenida de su ejecución:

```C
//PASO 1..: REALIZACIÓN DEL ESCANEO
ESP_LOGI(TAG, "PASO 1..: CONFIGURACIÓN DEL ESCANEO");
esp_wifi_set_country_code("ES", true);
ESP_ERROR_CHECK(esp_wifi_start());
```

```BASH
I (719) scan: PASO 1..: CONFIGURACIÓN DEL ESCANEO
I (719) wifi:set country: cc=ES schan=1 nchan=13 policy=0
I (729) phy_init: phy_version 4771,450c73b,Aug 16 2023,11:03:10
I (809) wifi:mode : sta (24:0a:c4:ea:36:b4)
I (809) wifi:enable tsf
```

A continuación, podemos ver como la función **esp_wifi_scan_start()**, encargada de realizar el escaneo de redes WIFI, no recibe ningúna estructura de configuración en su primer argumento, lo que le lleva a emplear la configuración por defecto, la cual especifica las siguientes opciones de escaneo:
- **show_hidden:false..:** Se ignoran los SSID ocultos.
- **scan_type:active..:** En escaneo se realiza de forma activa, es decir, re llevan a cabo el envío de paquetes **probe**.
- **scan_time.active.min:0..:** Tiempo mínimo durante el cual se llevará a cabo el escaneado activo.
- **scan_time.active.max:120 miliseconds..:** Tiempo máximo durante el cual se llevará a cabo el escaneado activo.
- **scan_time.passive:360 miliseconds..:** Tiempo durante el cual se llevará a cabo el escaneado pasivo.

A continuación podemos ver el fragmento de codigo mencionado y la salida obtenida de su ejecución, la cual esta completamente vacía, ya que no se ha producido ningún error y la función de escaneo no imprime nada por defecto:

```C
//PASO 2..: REALIZACIÓN DEL ESCANEO
ESP_LOGI(TAG, "PASO 2..: REALIZACIÓN DEL ESCANEO");
esp_wifi_scan_start(NULL, true);
```

```BASH
I (809) scan: PASO 2..: REALIZACIÓN DEL ESCANEO
```

Una vez especificado todo esto, vamos a llevar a cabo el escaneo de redes y observar los resultados obtenidos. Tener en cuenta que hemos activado la red WIFI **RPI1__test**, la cual estamos utilizando como prueba para analizar mejor los resultados. En el siguiente cuadro podemos ver el fragmento de codigo correspondiente a la obtención e impresión de las redes detectadas:

```C
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
```

```BASH
I (3319) scan: PASO 3..: OBTENCIÓN DE RESULTADOS
I (3319) scan: Total APs scanned = 12
I (3319) scan: SSID: RPI1_test    RSSI: -46    Authmode: WIFI_AUTH_WPA2_PSK    Pairwise Cipher: WIFI_CIPHER_TYPE_CCMP    CHANNEL: 10
I (3329) scan: SSID: UCM-CONGRESO    RSSI: -64    Authmode: WIFI_AUTH_WPA2_PSK    Pairwise Cipher: WIFI_CIPHER_TYPE_CCMP    CHANNEL: 13
I (3339) scan: SSID: UCMOT    RSSI: -64    Authmode: WIFI_AUTH_WPA2_PSK    Pairwise Cipher: WIFI_CIPHER_TYPE_CCMP    CHANNEL: 13
I (3349) scan: SSID: eduroam    RSSI: -64    Authmode: WIFI_AUTH_WPA2_ENTERPRISE    Pairwise Cipher: WIFI_CIPHER_TYPE_CCMP    CHANNEL: 13
I (3359) scan: SSID: UCM    RSSI: -64    Authmode: WIFI_AUTH_OPEN    Pairwise Cipher: WIFI_CIPHER_TYPE_NONE    CHANNEL: 13
I (3379) scan: SSID: AndroidAP477B    RSSI: -71    Authmode: WIFI_AUTH_WPA2_PSK    Pairwise Cipher: WIFI_CIPHER_TYPE_CCMP    CHANNEL: 6
I (3389) scan: SSID: UCM-CONGRESO    RSSI: -76    Authmode: WIFI_AUTH_WPA2_PSK    Pairwise Cipher: WIFI_CIPHER_TYPE_CCMP    CHANNEL: 9
I (3399) scan: SSID: UCMOT    RSSI: -77    Authmode: WIFI_AUTH_WPA2_PSK    Pairwise Cipher: WIFI_CIPHER_TYPE_CCMP    CHANNEL: 9
I (3409) scan: SSID: UCM    RSSI: -77    Authmode: WIFI_AUTH_OPEN    Pairwise Cipher: WIFI_CIPHER_TYPE_NONE    CHANNEL: 9
I (3429) scan: SSID: eduroam    RSSI: -78    Authmode: WIFI_AUTH_WPA2_ENTERPRISE    Pairwise Cipher: WIFI_CIPHER_TYPE_CCMP    CHANNEL: 9
I (3439) main_task: Returned from app_main()
```

Podemos ver como hemos detectado una gran cantidad de redes WIFI, entre las que se encuentran tres redes relevantes que nos indican que el proceso se ha llevado a cabo correctamente:
 - La red de pruebas: 
    - SSID: RPI1_test    RSSI: -46    Authmode: WIFI_AUTH_WPA2_PSK    Pairwise Cipher: WIFI_CIPHER_TYPE_CCMP    CHANNEL: 10.
 - La red de la universidad: 
    - SSID: UCM    RSSI: -64    Authmode: WIFI_AUTH_OPEN    Pairwise Cipher: WIFI_CIPHER_TYPE_NONE    CHANNEL: 13.
 - La red de la red Eduroan: 
    - SSID: eduroam    RSSI: -64    Authmode: WIFI_AUTH_WPA2_ENTERPRISE    Pairwise Cipher: WIFI_CIPHER_TYPE_CCMP    CHANNEL: 13.

A continuación vamos a observar los resultados obtenidos cuando llevamos a cabo algunas modificaciones en la configuración especificada, por ejemplo, escanear unicamente el canal donde esta trabajando al red **RPI1_test** (siendo en nuetsro caso el canal 10).

Para esto necesitaremos crear una estructura del tipo **wifi_scan_config_t** y pasarsela a la función **esp_wifi_scan_start()**. En los siguientes cuadros tenemos la configuración realizada y el resultado obtenido por pantalla:

```C
ESP_LOGI(TAG, "PASO 2..: REALIZACIÓN DEL ESCANEO");
wifi_scan_config_t scan_config = {
    .show_hidden = false,
    .channel = 10,
    .scan_type = WIFI_SCAN_TYPE_ACTIVE,
    .scan_time.active.min = 1200,
    .scan_time.active.max = 2400,
    .scan_time.passive = 360
};
ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));
```

```BASH
I (2419) scan: PASO 3..: OBTENCIÓN DE RESULTADOS
I (2419) scan: Total APs scanned = 1
I (2419) scan: SSID: RPI1_test    RSSI: -35    Authmode: WIFI_AUTH_WPA2_PSK    Pairwise Cipher: WIFI_CIPHER_TYPE_CCMP    CHANNEL: 10
I (2429) main_task: Returned from app_main()
```

Como podemos ver, nuestra placa ha sido capaz de escanear la red WIFI de prueba que estamos utilizando, por lo que podemos deducir que el escaneo funciona correctamente. Además, también hemos aumentado drasticamente el tiempo de escaneo en modo activo, lo que repercute en que la placa tarde más en finalziar el escaneo de cada uno de los canales.

## Implementar la conexión a redes conocidas


>Entregable 4
>
>Diseña un firmware de nodo que realice un escaneado de las redes disponibles. Si el nodo detecta la presencia de una o más de las redes conocidas, se conectará en modo STA a la red de mayor prioridad entre las conocidas. Probadlo usando como redes conocidas la del laboratorio, vuestro móvil y vuestro domicilio.

>Entregable 5
>
>Codificar el código de la tarea anterior para que la lista de redes conocidas y la prioridad relativa se puedan configurar con menuconfig.

En este apartado se van a realiiar conjuntamente los entregable 4 y 5, puesto que se piensa desarrollar el uso del menu de configuración a medida que se implementa el desarrollo solicitado.







```BASH
I (711) wifi_init: WiFi IRAM OP enabled
I (711) wifi_init: WiFi RX IRAM OP enabled
I (721) scan: PASO 1..: CONFIGURACIÓN DEL ESCANEO
I (721) wifi:set country: cc=ES schan=1 nchan=13 policy=0

I (731) phy_init: phy_version 4771,450c73b,Aug 16 2023,11:03:10
I (811) wifi:mode : sta (24:0a:c4:ea:36:b4)
I (811) wifi:enable tsf
I (811) scan: EXTRAYENDO REDES CONOCIDAS: RPI1_test:test1234 RPI1_2test:2test1234;
I (821) scan: Redes conocidas..:
I (821) scan:   Prioridad 1)    (RPI1_test:test1234) SSID: RPI1_test    Password: test1234
I (831) scan:   Prioridad 2)    (RPI1_2test:2test1234) SSID: RPI1_2test    Password: 2test1234
I (841) scan: PASO 2..: REALIZACIÓN DEL ESCANEO
I (3741) scan: PASO 3..: OBTENCIÓN DE RESULTADOS
I (3741) scan: Total APs scanned = 16
I (3741) scan: SSID: RPI1_test    RSSI: -38    Authmode: WIFI_AUTH_WPA2_PSK    Pairwise Cipher: WIFI_CIPHER_TYPE_CCMP    CHANNEL: 12
I (3751) scan: SSID: Oppo    RSSI: -49    Authmode: WIFI_AUTH_WPA2_PSK    Pairwise Cipher: WIFI_CIPHER_TYPE_CCMP    CHANNEL: 10
I (3761) scan: SSID: UCM-CONGRESO    RSSI: -55    Authmode: WIFI_AUTH_WPA2_PSK    Pairwise Cipher: WIFI_CIPHER_TYPE_CCMP    CHANNEL: 1
I (3771) scan: SSID: eduroam    RSSI: -55    Authmode: WIFI_AUTH_WPA2_ENTERPRISE    Pairwise Cipher: WIFI_CIPHER_TYPE_CCMP    CHANNEL: 1
I (3781) scan: SSID: UCM    RSSI: -55    Authmode: WIFI_AUTH_OPEN    Pairwise Cipher: WIFI_CIPHER_TYPE_NONE    CHANNEL: 1
I (3801) scan: SSID: UCM-CONGRESO    RSSI: -73    Authmode: WIFI_AUTH_WPA2_PSK    Pairwise Cipher: WIFI_CIPHER_TYPE_CCMP    CHANNEL: 13
I (3811) scan: SSID: UCM-CONGRESO    RSSI: -74    Authmode: WIFI_AUTH_WPA2_PSK    Pairwise Cipher: WIFI_CIPHER_TYPE_CCMP    CHANNEL: 13
I (3821) scan: SSID: eduroam    RSSI: -74    Authmode: WIFI_AUTH_WPA2_ENTERPRISE    Pairwise Cipher: WIFI_CIPHER_TYPE_CCMP    CHANNEL: 13
I (3831) scan: SSID: UCM    RSSI: -74    Authmode: WIFI_AUTH_OPEN    Pairwise Cipher: WIFI_CIPHER_TYPE_NONE    CHANNEL: 13
I (3851) scan: SSID: UCMOT    RSSI: -74    Authmode: WIFI_AUTH_WPA2_PSK    Pairwise Cipher: WIFI_CIPHER_TYPE_CCMP    CHANNEL: 13
I (3861) scan: PASO 4..: CONEXIÓN A RED CONOCIDA
I (3861) scan: INTENTANDO CONECTAR A: RPI1_test (test1234)
I (4731) wifi:new:<12,0>, old:<1,0>, ap:<255,255>, sta:<12,0>, prof:1
I (4731) wifi:state: init -> auth (b0)
I (4781) wifi:state: auth -> assoc (0)
I (4791) wifi:state: assoc -> run (10)
I (4851) wifi:connected with RPI1_test, aid = 1, channel 12, BW20, bssid = ca:df:8d:9a:30:e2
I (4851) wifi:security: WPA2-PSK, phy: bgn, rssi: -40
I (4861) wifi:pm start, type: 1

I (4881) wifi:<ba-add>idx:0 (ifx:0, ca:df:8d:9a:30:e2), tid:0, ssn:0, winSize:64
I (4931) wifi:AP's beacon interval = 102400 us, DTIM period = 2
I (5871) esp_netif_handlers: sta ip: 192.168.43.198, mask: 255.255.255.0, gw: 192.168.43.1

```