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

