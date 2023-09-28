# MODO COMBINADO ESTACION / PUNTO DE ACCESO

>Entregable 3
>
>Modifica el ejemplo station para que el ESP32 se comporte a la vez como estación y como punto de acceso. Añade las opciones de configuración necesarias para que todos los parámetros se puedan modificar vía menuconfig . Comprueba que el ESP32 efectivamente se conecta al punto de acceso y que a la vez es posible conectar otro dispositivo al mismo (por ejemplo, tu teléfono móvil). Entrega el código.

En el presente apartado vamos a configurar la placa STM32 para que funcione como Punto de Acceso WIFI y al mismo tiempo pueda conectarse a otra red indicada como Estación. Para llevar esto a cabo vamos a partir del ejemplo **Station** y fusionaremos el código proporcionado con el desarrollado en el entregable 2.

El primer paso será definir nos menús de configuración, uno con los parámetros utilziados en modo Estacion y otro para los empleados cuando operamos como Punto de Acceso. Modificaremos levemente el nombre de las variables de configuración empleadas definidas en la configuración de AP con el objetivo de que no entren conflicto unas con otras.

En el siguiente cuadro podemos ver la definición de variables en el fichero **Kconfig.projbuild** utilizadas para la conexión con una red WIFI en el modo Estación:

```C
menu "Example Configuration - Station Mode"

    config ESP_WIFI_SSID
        string "WiFi SSID"
        default "RPI1_test"
        help
            SSID (network name) for the example to connect to.

    config ESP_WIFI_PASSWORD
        string "WiFi Password"
        default "test1234"
        help
            WiFi password (WPA or WPA2) for the example to use.
```

En el siguiente cuadro podemos ver la definición de variables en el fichero **Kconfig.projbuild** utilizadas para la establecer la red WIFI en el modo Punto de Acceso:

```C
menu "Example Configuration - Access Point Mode"

    config ESP_WIFI_AP_SSID
        string "WiFi SSID"
        default "RPI1_wifi"
        help
            SSID (network name) for the example to connect to.

    config ESP_WIFI_AP_PASSWORD
        string "WiFi Password"
        default "wifi1234"
        help
            WiFi password (WPA or WPA2) for the example to use.
```

Una vez establecidas las variables de configuración necesarias, deberemos enlazar la pila TCP/IP tanto para el uso como Punto de Acceso como Estación mediante el uso de las funciones **esp_netif_create_default_wifi_ap()** y **esp_netif_create_default_wifi_sta()** respectivamente. En el siguiente cuadro podremos ver la configuración de elementos previos, donde se puede apreciar la configuración de ambos elementos con las pilas:

```C
//PASO 1..: Creamos e inicializamos los elementos necesarios para operar el WIFI.
SP_ERROR_CHECK(esp_netif_init());
ESP_ERROR_CHECK(esp_event_loop_create_default());
esp_netif_create_default_wifi_sta();        //Creamos la interfaz con la pila TCP/IP para el modo Estacion
esp_netif_create_default_wifi_ap();         //Creamos la interfaz con la pila TCP/IP para el modo Punto de Acceso
```

Una vez establecida la configruación de los elementos previos, deberemos llevar a cabo la especificación de los parámetros a utilizar para cada conexión, siendo en nuestro caso dos, una para el modo Punto de Acceso y otra para el modo Estacion. Esta también nos llevará a indicar que el modo de operación para el driver WIFI será el modo convinado. 

En el siguiente cuadro podemos ver la configuración tanto para el modo de operación combinado como para la conexión a realizar para ambos modos de operación:

```C
    //PASO 2..: Configuramos la conexión a realizar y especificamos los parámetros 
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA) );       //Indicamos el modo de conexión combinado
    
    wifi_config_t wifiConfig_STAmode = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS,
            .threshold.authmode = ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD,
            .sae_pwe_h2e = ESP_WIFI_SAE_MODE,
            .sae_h2e_identifier = EXAMPLE_H2E_IDENTIFIER,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifiConfig_STAmode) );     //Configuramos el modo Punto de Acceso


    wifi_config_t wifiConfig_APMode = {
        .ap = {
            .ssid = EXAMPLE_ESP_WIFI_AP_SSID,
            .ssid_len = strlen(EXAMPLE_ESP_WIFI_AP_SSID),
            .channel = EXAMPLE_ESP_WIFI_AP_CHANNEL,
            .password = EXAMPLE_ESP_WIFI_AP_PASS,
            .max_connection = EXAMPLE_ESP_WIFI_AP_MAX_CONN,
            #ifdef CONFIG_ESP_WIFI_SOFTAP_SAE_SUPPORT
                .authmode = WIFI_AUTH_WPA3_PSK,
                .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
            #else /* CONFIG_ESP_WIFI_SOFTAP_SAE_SUPPORT */
                .authmode = WIFI_AUTH_WPA2_PSK,
            #endif
            .pmf_cfg = {
                    .required = true,
            },
        },
    };
    if (strlen(EXAMPLE_ESP_WIFI_AP_PASS) == 0) {
        wifiConfig_APMode.ap.authmode = WIFI_AUTH_OPEN;
    }
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifiConfig_APMode) );    //Configuramos el modo Estación
```

Una vez realizados los pasos anteriores ya habremos configurado correctamenteel driver WIFI nuestra placa STM32 para funcionar en modo combinado, por lo que únicamente falta probarlo. Para esto primero conectaremos automáticamente nuestra placa a la red IP **RIP1_test**, y una ver realizado estableceremos la conexión de un segundo dispositivo a la red WIFI a la cual da soporte nyestra placa y a la que hemos llamado **RPI1_wifi**

En el siguiente cuadro tenemos la salida obtenida al llevar a cabo la conexión de nuestra placa a la red WIFI indicada en los parámetros de configuración:
```BASH
I (6160) esp_netif_handlers: sta ip: 192.168.43.198, mask: 255.255.255.0, gw: 192.168.43.1
I (6160) wifi station: got ip:192.168.43.198
I (6160) wifi station: connected to ap SSID:RPI1_test password:test1234
I (6170) main_task: Returned from app_main()
I (83240) wifi:new:<10,2>, old:<10,0>, ap:<10,2>, sta:<10,0>, prof:1
I (83240) wifi:station: f8:94:c2:b9:6b:08 join, AID=1, bgn, 40D
```

En el siguiente cuadro tenemos la salida obtenida al llevar a cabo la conexión de un tercer dispositivo a la red WIFI a la cual da soporte nuestra placa según indican los parámetros de configuración:

```BASH
I (83260) wifi station: station f8:94:c2:b9:6b:08 join, AID=1
I (83360) esp_netif_lwip: DHCP server assigned IP to a client, IP is: 192.168.4.2
I (83450) wifi:<ba-add>idx:2 (ifx:1, f8:94:c2:b9:6b:08), tid:0, ssn:18, winSize:64
```

