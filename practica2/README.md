# Entregable 1 - wifi/getting_started/station

En la tarea se compila el modo station en los siguientes escenarios:

## Conectar ESP32 con un punto de acceso existente
Para ello se actualizan los parámetros existente "WiFi SSID" y "WiFi Password" en el apartado de SDK Configuration Editor.

Tras compilar y flashear se observa la siguiente salida:
```BASH
I (600) wifi station: ESP_WIFI_MODE_STA
I (610) wifi:wifi driver task: 3ffbf94c, prio:23, stack:6656, core=0
I (630) wifi:wifi firmware version: ce9244d
I (640) wifi:wifi certification version: v7.0
I (640) wifi:config NVS flash: enabled
I (640) wifi:config nano formating: disabled
I (640) wifi:Init data frame dynamic rx buffer num: 32
I (640) wifi:Init management frame dynamic rx buffer num: 32
I (650) wifi:Init management short buffer num: 32
I (650) wifi:Init dynamic tx buffer num: 32
I (660) wifi:Init static rx buffer size: 1600
I (660) wifi:Init static rx buffer num: 10
I (670) wifi:Init dynamic rx buffer num: 32
I (670) wifi_init: rx ba win: 6
I (670) wifi_init: tcpip mbox: 32
I (680) wifi_init: udp mbox: 6
I (680) wifi_init: tcp mbox: 6
I (680) wifi_init: tcp tx win: 5744
I (690) wifi_init: tcp rx win: 5744
I (690) wifi_init: tcp mss: 1440
I (700) wifi_init: WiFi IRAM OP enabled
I (700) wifi_init: WiFi RX IRAM OP enabled
I (710) phy_init: phy_version 4670,719f9f6,Feb 18 2021,17:07:07
I (820) wifi:mode : sta (8c:aa:b5:b8:bf:f4)
I (820) wifi:enable tsf
I (820) wifi station: wifi_init_sta finished.
I (950) wifi:new:<1,0>, old:<1,0>, ap:<255,255>, sta:<1,0>, prof:1
I (2110) wifi:state: init -> auth (b0)
I (2110) wifi:state: auth -> assoc (0)
I (2120) wifi:state: assoc -> run (10)
I (2150) wifi:connected with WIFIAP, aid = 1, channel 1, BW20, bssid = b2:08:43:4c:40:bf
I (2150) wifi:security: WPA2-PSK, phy: bgn, rssi: -28
I (2170) wifi:pm start, type: 1

I (2170) wifi:<ba-add>idx:0 (ifx:0, b2:08:43:4c:40:bf), tid:0, ssn:0, winSize:64
I (2240) wifi:AP's beacon interval = 102400 us, DTIM period = 2
I (3180) esp_netif_handlers: sta ip: 192.168.223.86, mask: 255.255.255.0, gw: 192.168.223.152
I (3180) wifi station: got ip:192.168.223.86
I (3180) wifi station: connected to ap SSID:WIFIAP password:Wifiap123?
I (3190) main_task: Returned from app_main()
```

## Conectar ESP32 con un punto de acceso inexistente
En este caso se modifica el parámetro "WiFi SSID" con un valor aleatorio observando la siguiente salida:
```BASH
I (599) wifi station: ESP_WIFI_MODE_STA
I (609) wifi:wifi driver task: 3ffbf94c, prio:23, stack:6656, core=0
I (629) wifi:wifi firmware version: ce9244d
I (639) wifi:wifi certification version: v7.0
I (639) wifi:config NVS flash: enabled
I (639) wifi:config nano formating: disabled
I (639) wifi:Init data frame dynamic rx buffer num: 32
I (639) wifi:Init management frame dynamic rx buffer num: 32
I (649) wifi:Init management short buffer num: 32
I (649) wifi:Init dynamic tx buffer num: 32
I (659) wifi:Init static rx buffer size: 1600
I (659) wifi:Init static rx buffer num: 10
I (669) wifi:Init dynamic rx buffer num: 32
I (669) wifi_init: rx ba win: 6
I (669) wifi_init: tcpip mbox: 32
I (679) wifi_init: udp mbox: 6
I (679) wifi_init: tcp mbox: 6
I (679) wifi_init: tcp tx win: 5744
I (689) wifi_init: tcp rx win: 5744
I (689) wifi_init: tcp mss: 1440
I (699) wifi_init: WiFi IRAM OP enabled
I (699) wifi_init: WiFi RX IRAM OP enabled
I (709) phy_init: phy_version 4670,719f9f6,Feb 18 2021,17:07:07
I (819) wifi:mode : sta (8c:aa:b5:b8:bf:f4)
I (819) wifi:enable tsf
I (819) wifi station: wifi_init_sta finished.
I (3229) wifi station: retry to connect to the AP
I (3239) wifi station: connect to the AP fail
I (5649) wifi station: retry to connect to the AP
I (5649) wifi station: connect to the AP fail
I (8059) wifi station: retry to connect to the AP
I (8059) wifi station: connect to the AP fail
I (10469) wifi station: retry to connect to the AP
I (10469) wifi station: connect to the AP fail
I (12879) wifi station: retry to connect to the AP
I (12879) wifi station: connect to the AP fail
I (15289) wifi station: connect to the AP fail
I (15289) wifi station: Failed to connect to SSID:WIFIAPINEXISTENTE, password:Wifiap123?
I (15299) main_task: Returned from app_main()
```

En este caso, no se puede conectar al SSID WIFIAPINEXISTENTE intentando la reconexión tal cual se establece en el manejador event_handler ante el evento WIFI_EVENT_STA_DISCONNECTED.

## Apagar el punto de acceso mientras la ip está concendida

```BASH
I (600) wifi station: ESP_WIFI_MODE_STA
I (610) wifi:wifi driver task: 3ffbf94c, prio:23, stack:6656, core=0
I (630) wifi:wifi firmware version: ce9244d
I (640) wifi:wifi certification version: v7.0
I (640) wifi:config NVS flash: enabled
I (640) wifi:config nano formating: disabled
I (640) wifi:Init data frame dynamic rx buffer num: 32
I (640) wifi:Init management frame dynamic rx buffer num: 32
I (650) wifi:Init management short buffer num: 32
I (650) wifi:Init dynamic tx buffer num: 32
I (660) wifi:Init static rx buffer size: 1600
I (660) wifi:Init static rx buffer num: 10
I (670) wifi:Init dynamic rx buffer num: 32
I (670) wifi_init: rx ba win: 6
I (670) wifi_init: tcpip mbox: 32
I (680) wifi_init: udp mbox: 6
I (680) wifi_init: tcp mbox: 6
I (680) wifi_init: tcp tx win: 5744
I (690) wifi_init: tcp rx win: 5744
I (690) wifi_init: tcp mss: 1440
I (700) wifi_init: WiFi IRAM OP enabled
I (700) wifi_init: WiFi RX IRAM OP enabled
I (710) phy_init: phy_version 4670,719f9f6,Feb 18 2021,17:07:07
I (820) wifi:mode : sta (8c:aa:b5:b8:bf:f4)
I (820) wifi:enable tsf
I (820) wifi station: wifi_init_sta finished.
I (830) wifi:new:<6,0>, old:<1,0>, ap:<255,255>, sta:<6,0>, prof:1
I (830) wifi:state: init -> auth (b0)
I (840) wifi:state: auth -> assoc (0)
I (850) wifi:state: assoc -> run (10)
I (870) wifi:connected with WIFIAP, aid = 1, channel 6, BW20, bssid = b2:08:43:4c:40:bf
I (870) wifi:security: WPA2-PSK, phy: bgn, rssi: -32
I (870) wifi:pm start, type: 1

I (880) wifi:<ba-add>idx:0 (ifx:0, b2:08:43:4c:40:bf), tid:0, ssn:0, winSize:64
I (940) wifi:AP's beacon interval = 102400 us, DTIM period = 2
I (1880) esp_netif_handlers: sta ip: 192.168.223.86, mask: 255.255.255.0, gw: 192.168.223.152
I (1880) wifi station: got ip:192.168.223.86
I (1880) wifi station: connected to ap SSID:WIFIAP password:Wifiap123?
I (1890) main_task: Returned from app_main()
I (35040) wifi:state: run -> init (1c0)
I (35040) wifi:pm stop, total sleep time: 32217443 us / 34166625 us

I (35040) wifi:<ba-del>idx:0, tid:0
I (35040) wifi:new:<6,0>, old:<6,0>, ap:<255,255>, sta:<6,0>, prof:1
I (35050) wifi station: retry to connect to the AP
I (35050) wifi station: connect to the AP fail
I (35060) wifi:new:<6,0>, old:<6,0>, ap:<255,255>, sta:<6,0>, prof:1
I (35070) wifi:state: init -> auth (b0)
I (35070) wifi:state: auth -> init (3c0)
I (35070) wifi:new:<6,0>, old:<6,0>, ap:<255,255>, sta:<6,0>, prof:1
I (35080) wifi station: retry to connect to the AP
I (35080) wifi station: connect to the AP fail
I (37490) wifi station: retry to connect to the AP
I (37490) wifi station: connect to the AP fail
I (39900) wifi station: retry to connect to the AP
I (39900) wifi station: connect to the AP fail
I (42320) wifi station: retry to connect to the AP
I (42320) wifi station: connect to the AP fail
I (44720) wifi station: connect to the AP fail
```

Revisando el tratamiento de eventos del código del ejemplo observamos que los siguientes eventos se tratan dentro de la función event_handler:
- WIFI_EVENT_STA_START //Se llama a la función esp_wifi_connect();
- WIFI_EVENT_STA_DISCONNECTED //Se reintenta la conexión llamando la función esp_wifi_connect();
- IP_EVENT_STA_GOT_IP //Se muestra la ip obtenida por DHCP

Dentro del apartado "Modo Station" se especifican eventos adicionales dentro de las distintas fases de conexión wifi:
- WIFI_EVENT_STA_CONNECTED // Se muestra el nombre del AP al que se ha conectado correctamente

```BASH
I (600) wifi station: ESP_WIFI_MODE_STA
I (610) wifi:wifi driver task: 3ffbf94c, prio:23, stack:6656, core=0
I (630) wifi:wifi firmware version: ce9244d
I (640) wifi:wifi certification version: v7.0
I (640) wifi:config NVS flash: enabled
I (640) wifi:config nano formating: disabled
I (640) wifi:Init data frame dynamic rx buffer num: 32
I (640) wifi:Init management frame dynamic rx buffer num: 32
I (650) wifi:Init management short buffer num: 32
I (650) wifi:Init dynamic tx buffer num: 32
I (660) wifi:Init static rx buffer size: 1600
I (660) wifi:Init static rx buffer num: 10
I (670) wifi:Init dynamic rx buffer num: 32
I (670) wifi_init: rx ba win: 6
I (670) wifi_init: tcpip mbox: 32
I (680) wifi_init: udp mbox: 6
I (680) wifi_init: tcp mbox: 6
I (690) wifi_init: tcp tx win: 5744
I (690) wifi_init: tcp rx win: 5744
I (690) wifi_init: tcp mss: 1440
I (700) wifi_init: WiFi IRAM OP enabled
I (700) wifi_init: WiFi RX IRAM OP enabled
I (710) phy_init: phy_version 4670,719f9f6,Feb 18 2021,17:07:07
I (810) wifi:mode : sta (8c:aa:b5:b8:bf:f4)
I (820) wifi:enable tsf
I (820) wifi station: wifi_init_sta finished.
I (830) wifi:new:<1,0>, old:<1,0>, ap:<255,255>, sta:<1,0>, prof:1
I (830) wifi:state: init -> auth (b0)
I (830) wifi:state: auth -> assoc (0)
I (850) wifi:state: assoc -> run (10)
I (870) wifi:state: run -> init (2c0)
I (870) wifi:new:<1,0>, old:<1,0>, ap:<255,255>, sta:<1,0>, prof:1
I (880) wifi station: retry to connect to the AP
I (880) wifi station: connect to the AP fail
I (3290) wifi station: retry to connect to the AP
I (3290) wifi station: connect to the AP fail
I (3300) wifi:new:<1,0>, old:<1,0>, ap:<255,255>, sta:<1,0>, prof:1
I (3300) wifi:state: init -> auth (b0)
I (3300) wifi:state: auth -> assoc (0)
I (3310) wifi:state: assoc -> run (10)
I (3450) wifi:connected with WIFIAP, aid = 1, channel 1, BW20, bssid = b2:08:43:4c:40:bf
I (3450) wifi:security: WPA2-PSK, phy: bgn, rssi: -34
I (3460) wifi:pm start, type: 1

I (3460) wifi:<ba-add>idx:0 (ifx:0, b2:08:43:4c:40:bf), tid:0, ssn:0, winSize:64
I (3470) wifi station: Connected successfully to AP: WIFIAP
I (3520) wifi:AP's beacon interval = 102400 us, DTIM period = 2
I (4470) esp_netif_handlers: sta ip: 192.168.223.86, mask: 255.255.255.0, gw: 192.168.223.152
I (4470) wifi station: got ip:192.168.223.86
I (4470) wifi station: connected to ap SSID:WIFIAP password:Wifiap123?
I (4480) main_task: Returned from app_main()

```

# Modo Punto de Acceso

En el presente entregable se trabajará con el uso de la biblioteca WIFI proporcionada para la placa STM32 en su modo para trabajar como Punto de Acceso.

## Tarea - Ejemplo de conexión

>**Tarea**
>
>Analiza el ejemplo softAP, compílalo y flashealo. Estudia el tratamiento de eventos que realiza, y cómo estos son emitidos para casos reales. Para ello, conecta distintos clientes (stations), bien sean ESP32 o cualquier otro dispositivo, y analiza los eventos generados y su respuesta.

Para comprobar que la placa funciona correctamente como Punto de Acceso, antes de realizar la conexión con otro dispositivo, ejecutaremos el código entregado y examinaremos su salida.

A continuación podemos ver la salida obtenida y como hemos dividido el proceso de configuración wifi en tres partes:

- **Paso 1 - Creación de los elementos para operar con el WIFI:** En este paso realizamos las siguientes actividades:
    - Creamos la pila TCP/IP
    - Creamos el bucle para la gestión de los eventos
    - Creamos la interfáz de red para asociar el dispositivo a la pila TCP/IP
    - Inicializamos la tarea que gestionará la conexión WIFI


- **Paso 2: Configuramos los parámetros de la conexión:** Especificamos elementos como el SSID de la red, la contraseña utilizada y el modo de operación de la placa (en nuestro caso como AP). Podmeos ver que no obtenemos ninguna salida.

- **Paso 3: Ejecución del driver WIFI:** Podemos ver como este inicia y nos devuelve información acerca del mismo.

Para finalizar podemos ver como se muestra información util de cara al usuario sobre el punto de acceso obtenido, tal como la IP del dispositivo, el SSID de la red o la contraseña de la misma.

```BASH
**CONFIGURACIÓN WIFI - INICIO
*

FASE 1..: CREAMOS E INICIALIZAMOS LOS ELEMENTOS NECESARIOS PARA PODER OPERAR CON EL WIFI
I (629) wifi:wifi driver task: 3ffbf918, prio:23, stack:6656, core=0
I (659) wifi:wifi firmware version: e03c1ca
I (659) wifi:wifi certification version: v7.0
I (659) wifi:config NVS flash: enabled
I (659) wifi:config nano formating: disabled
I (659) wifi:Init data frame dynamic rx buffer num: 32
I (669) wifi:Init management frame dynamic rx buffer num: 32
I (669) wifi:Init management short buffer num: 32
I (679) wifi:Init dynamic tx buffer num: 32
I (679) wifi:Init static rx buffer size: 1600
I (679) wifi:Init static rx buffer num: 10
I (689) wifi:Init dynamic rx buffer num: 32
I (689) wifi_init: rx ba win: 6
I (689) wifi_init: tcpip mbox: 32
I (699) wifi_init: udp mbox: 6
I (699) wifi_init: tcp mbox: 6
I (709) wifi_init: tcp tx win: 5744
I (709) wifi_init: tcp rx win: 5744
I (709) wifi_init: tcp mss: 1440
I (719) wifi_init: WiFi IRAM OP enabled
I (719) wifi_init: WiFi RX IRAM OP enabled

FASE 2..: CONFIGURAMOS EL TIPO Y LOS PARAMETROS DE LA CONEXIÓN WIFI A REALIZAR

FASE 3..: INICIAMOS LA EJECUCIÓN DEL FRIVER WIFI
I (749) phy_init: phy_version 4771,450c73b,Aug 16 2023,11:03:10
I (829) wifi:mode : softAP (24:0a:c4:ea:36:b5)                
I (949) wifi:Total power save buffer number: 16
I (949) wifi:Init max length of beacon: 752/752
I (949) wifi:Init max length of beacon: 752/752

*
**CONFIGURACIÓN WIFI - FIN

I (949) esp_netif_lwip: DHCP server started on interface WIFI_AP_DEF with IP: 192.168.4.1
I (969) wifi softAP: wifi_init_softap finished. SSID:RPI1_test password:test1234 channel:1
I (969) main_task: Returned from app_main()
```

Una vez que se ha llevado a cabo la inicialización del sistema como Punto de acceso y hemos comprobado la salida, pasaremos a conectar un dispositivo al mismo y observar lo que este nos devuelve. Tener en cuenta que esta salida se realiza dentro de la manejadora que trata el evento **WIFI_EVENT_AP_STACONNECTED**.

En este caso podemos observar como se ha solicitado la conexión desde una nueva estación a nuestro Punto de Acceso con dirección MAC DE:23:9C:51:10:93. Por otra parte vemos como el sistema ha asignado a esta nueva estación la dirección IP 192.168.4.2

```BASH
I (20319) wifi:new:<1,1>, old:<1,1>, ap:<1,1>, sta:<255,255>, prof:1
I (20319) wifi:station: f8:94:c2:b9:6b:08 join, AID=1, bgn, 40U
I (20359) wifi softAP: station f8:94:c2:b9:6b:08 join, AID=1
I (20459) esp_netif_lwip: DHCP server assigned IP to a client, IP is: 192.168.4.2
I (20679) wifi:<ba-add>idx:2 (ifx:1, f8:94:c2:b9:6b:08), tid:0, ssn:15, winSize:64
```

Para finalizar vamos a llevar a cabo la desconexión de la estación para obtener la siguiente salida, donde vemos que esta ha sido detectada por el Punto de Acceso. Tener en cuenta que esta salida se realiza dentro de la manejadora que trata el evento **WIFI_EVENT_AP_STADISCONNECTED**.

```BASH
I (54499) wifi:station: f8:94:c2:b9:6b:08 leave, AID = 1, bss_flags is 8391747, bss:0x3ffb91f0
I (54499) wifi:new:<1,0>, old:<1,1>, ap:<1,1>, sta:<255,255>, prof:1
I (54509) wifi:<ba-del>idx:2, tid:0
I (54509) wifi softAP: station f8:94:c2:b9:6b:08 leave, AID=1
```

## Entregable 2 - Configuración de eventos

>Entregable 2
>
>Revisa el tratamiento de eventos del código anterior, añade el tratamiento de los eventos que falten por tratar. Añade en tu código un comentario explicando el código añadido.

En el ejemplo del cual parte el entregable se realiza el manejo únicamente de dos eventos:
 - WIFI_EVENT_AP_STACONNECTED: Lanzado cuando una estación lleva a cabo una conexión con el Punto de Acceso.
 
 - WIFI_EVENT_AP_STADISCONNECTED: Lanzado cuando una estación se desconecta del Punto de acceso. 

Ya hemos visto la salida que producen ambos eventos en el apartado anterior, donde hemos llevado a cabo la conexión y desconexión de una estación, por lo que no volveremos a visualizarla ahora.

Sin embargo, examinando todos los posibles eventos lanzados por el sistema WIFI configurado como Punto de Acceso y filtrando aquellos que sean relevantes, observamos que WIFI_EVENT_AP_START y WIFI_EVENT_AP_STOP no se encuentran tratados correctamente. Debido a esto, pasaremos a implementar manejadores espedificas para cada uno de ellos.

### Tratando el evento WIFI_EVENT_AP_START

Este evento es enviado atomáticamente al ejecutar la funcion esp_wifi_start(), y para controlarlo vamos a enlazar una función manejadora que imprimirá en pantalla que se ha llevado correctamente el arranque del driver Wifi.


En el siguiente cuadro podemos ver la función manejadora en cuestión:

```C
static void wifi_event_handler_stop(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
    ESP_ERROR_CHECK(esp_wifi_deinit());
    ESP_LOGI(TAG, "Detectada parada del driver WIFI. Cierre ordenado realizado con exito. | "
                MACSTR" join, AID=%d",MAC2STR(event->mac), event->aid);
}
```

En el siguiente cuadro podemos ver el enlace de la función manejadora:

```C
ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                    WIFI_EVENT_AP_STOP,
                                                    &wifi_event_handler_start,
                                                    NULL,
                                                    NULL));
```

En el siguiente cuadro tenemos la salida obtenida tras ejecutar el programa. Podmeos ver como después de terminar la configuración obtenemos el mensaje enviado desde la manejadora enlazada.

```BASH
*
**CONFIGURACIÓN WIFI - FIN

I (940) esp_netif_lwip: DHCP server started on interface WIFI_AP_DEF with IP: 192.168.4.1
I (950) wifi softAP: Driver WIFI iniciado correctamente.
I (960) wifi softAP: wifi_init_softap finished. SSID:RPI1_test password:test1234 channel:1
I (970) main_task: Returned from app_main()
```

### Tratando el evento WIFI_EVENT_AP_STOP

Este evento es enviado atomáticamente al ejecutar la funcion esp_wifi_stop() o en el caso de que se produzca un cierre repentino del driver WIFI. Igual que ne el caso anterior, vamos a enlazar una función manejadora que, en este caso, lleve a cabo un cierre ordenado de los recursos asignados al driver WIFI.

Antes de nada, para poder hacer esto utilizaremos un temporizador que tras pasados un número determinado de segundos cierre el driver WIFI. Esta será definida como una variable de confuguración accesible desde 'menuconfig', cuya definición dentro del fichero **Kconfig.projbuild** es:

```C
    config ESP_TIME_CLOSE_WIFI
        int "Time to close the WIFI Access Point"
        range 10 99
        default 30
```

Por otra parte, el contador de segundos y la llamada a la función esp_wofo_stop se llevan a cabo al final de la función app_main(). En nuestro caso, como la placa STM32 se encuentra tabajando en modo Punto de Acceso, no necesitará aplicar la función **esp_wifi_disconnect()**, además de realziar la limpieza de recursos asignados mediante al función **esp_wifi_deinit()** una vez ha finalziado la manejadora del evento **ESP_EVENT_AP_STOP**.

En el siguiente cuadro podemos ver el fracmento de código indicado:

```C
ESP_LOGI(TAG, "**CIERRE DEL PUNTO DE ACCESO EN %d SEGUNDOS", CONFIG_ESP_TIME_CLOSE_WIFI) ;
for (int i = CONFIG_ESP_TIME_CLOSE_WIFI; i >= 0; i--)
{
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}
ESP_LOGI(TAG, "**TIEMPO FINALIZADO, CERRANDO DRIVER WIFI") ;
ESP_ERROR_CHECK(esp_wifi_stop());
ESP_ERROR_CHECK(esp_wifi_deinit());
```

Por otra parte, en el siguiente cuadro podemos ver la función manejadora en cuestión:

```C
static void wifi_event_handler_stop(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    ESP_LOGI(TAG, "Detectada parada del driver WIFI. Driver WIFI cerrado con exito.");
}
```

En el siguiente cuadro podemos ver el enlace de la función manejadora:

```C
ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                    WIFI_EVENT_AP_STOP,
                                                    &wifi_event_handler_stop,
                                                    NULL,
                                                    NULL));
```

Una vez implementado todo lo anterior, procedemos a la ajecución del dentregable y esperamos el tiempo necesario para que se lleve a cabo el cierre del driver WIFI. Obtenemos la siguiente salida

```BASH
I (960) wifi softAP: wifi_init_softap finished. SSID:RPI1_test password:test1234 channel:1
I (970) wifi softAP: **CIERRE DEL PUNTO DE ACCESO EN 30 SEGUNDOS
I (31980) wifi softAP: **TIEMPO FINALIZADO, CERRANDO DRIVER WIFI
E (31980) wifi:NAN WiFi stop
I (31980) wifi:flush txq
I (31980) wifi:stop sw txq
I (31980) wifi:lmac stop hw txq
I (31980) wifi softAP: Detectada parada del driver WIFI. Driver WIFI cerrado con exito.
I (32000) wifi:Deinit lldesc rx mblock:10
I (32010) main_task: Returned from app_main()
```