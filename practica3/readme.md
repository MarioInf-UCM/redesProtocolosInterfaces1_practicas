<style>
  .columnImages {
    float: left;
    padding: 5px;
    height:100%
  }

  .row::after {
    content: "";
    clear: both;
    display: table;
    width: 100%;
    height: 30%
  }
</style>


# Aprovisionamiento de credenciales WIFI

En este apartado llevaremos a cabo el aprovisionamiento de nuestra placa STM32 mediante el uso de diferentes aplicaciones móviles. Para poder llevar a cabo la realización de aprovisionamiento vamos a ejecutar en nuestro SoC el ejemplo **examples/provisioning/wifi-prov-mgr**, el cuál será encargado de gestionar la recepción de las creedenciales desde nuestra aplicación movil.

>Tarea
>
>Utilizando las aplicaciones correspondientes a tu dispositivo móvil, tanto para el uso de BLE como de SoftAP, provisiona tu ESP32 utilizando las credenciales que correspondan a tu red WiFi. Recuerda, antes de cada repetición del experimento, utilizar la orden idf.py erase_flash para eliminar información de provisionamiento de sesiones anteriores. Comprueba el funcionamiento de los distintos niveles de seguridad.
>
>Añade a tu informe las capturas de pantalla correspondientes a la salida del ESP32 que evidencien que el proceso de provisionamiento se ha realizado correctamente.
## Aprovisionamiento mediante BLE con seguridad

Para realizar el aprovisionamiento mediante BLE utilizaremos la siguiente aplicación movil:

<img src="images/AplicacionBLE.png" alt="drawing" style="width:60%; 
    display: block;
    margin-left: auto;
    margin-right: auto;
    margin-top: 1%;
    margin-botton: 1%;
"/>

Antes de poner a correr cualquiera de los dos extremos (el SoC utilizado por un lado y nuestro dispositivo móvil por el otro), necesitaremos configurar adecuadamente tanto la aplicación de nuestro dispositivo movil como el ejemplo que se ejecutará en nuesta placa STM32. En las siguientes imágenes se puede observar tanto la configuración especificada en el ejemplo **wifi-prov-mgr** ejecutado en el SoC mediante el menú de configuración (izquierda), como la establecida en la aplicación movil utilizada para llevar a cabo el provisionamiento mediante BLE (derecha).

<div class="row">
  <div class="columnImages" style="width:50%; margin-left:10%">
    <img src="images/configuracionBLE_SOC_conSeguridad.png" alt="Imagen configuracion SoC" style="width:100%">
  </div>

  <div class="columnImages" style="width:19%; float: right; margin-right:10%">
    <img src="images/configuracionBLE_apMovil_conSeguridad.jpeg" alt="Imagen configruacion aplicación movil" style="width:100%">
  </div>
</div>

Conforme podemos ver en la configuración de ambos equipo, debemos tener en cuenta:
 - El tipo de provisionamiento soportado en ambos dispositivos es BLE.
 - El nivel de seguridad establecido es nivel 2, es decir, se llevará a cabo un cifrado entre ambos extremos.
 - El intento máximo de reintentos efectuados antes del provisionamiento serán 5.

Una vez llevada a cabo la configuración previa, realzaremos una limpieza de la posible información de provisionamiento quedada anteriormente en cuestro SoC mediante la herramienta **idf.py erase_flash** y ejecutaremos el programa de ejemplo **wifi-prov-mgr**. Una vez hecho esto podemos ver como obtenemos la siguiente salida desde el monitor serie:

```BASH
I (910) BTDM_INIT: BT controller compile version [f6018c5]
I (920) BTDM_INIT: Bluetooth MAC: 24:0a:c4:ea:36:b6
I (1160) protocomm_nimble: BLE Host Task Started
I (1170) wifi_prov_mgr: Provisioning started with service name : PROV_EA36B4 
I (1170) app: Provisioning started
I (1170) app: Scan this QR code from the provisioning application for Provisioning.
I (1180) QRCODE: Encoding below text with ECC LVL 0 & QR Code Version 10
I (1180) QRCODE: {"ver":"v1","name":"PROV_EA36B4","username":"wifiprov","pop":"abcd1234","transport":"ble"}
I (1200) NimBLE: GAP procedure initiated: advertise; 
I (1200) NimBLE: disc_mode=2
I (1200) NimBLE:  adv_channel_map=0 own_addr_type=0 adv_filter_policy=0 adv_itvl_min=256 adv_itvl_max=256
I (1210) NimBLE: 

                                          
  █▀▀▀▀▀█ ▄▀ ▄█▀██▄  ▀▀ ▄█ ▀███ █▀▀▀▀▀█   
  █ ███ █ ▀███▀ ▄█▀▀ █▄ ███  █▀ █ ███ █   
  █ ▀▀▀ █ █▄ ▄ █▀█▀ ▄▄▀▀▄▀▄▀██  █ ▀▀▀ █   
  ▀▀▀▀▀▀▀ █▄▀ █ █ █▄▀ ▀▄▀▄▀ ▀▄▀ ▀▀▀▀▀▀▀   
  █▀ █▄ ▀▀ ▄█▄ █  ▀▄▀ ▀█▄ ███   ██▀▄▀█▄   
  ▄█▀▄▄█▀ ▀ ▀  ▄█  ▀▀█▀█▀▄██    ▄█▄▄▀ █   
  █▀▄▀▄▀▀█▄▄▄▄██▄▀▄▀▀▄███ █▀▄▄▀▄█  █  █   
  █▀▄▄ ▄▀▄▀ ▄ █▄█ ▀█  ▀█▀█ █▄ ███▀ ▄  ▀   
   ▀▀▀▄▀▀██ ▄▀ ▀█  ▀▀▀▀▀ ██▄▄ ▀█▀▀▄▄▀▀█   
  ██▀▄▄▀▀▀ ▀▀▄ █ ▄▀█▀▄▀▄▄ ▄ ▄ ▄▄▀██ ▄██   
   ▄▀▄▄▀▀▄█████▄▀ ▄▄▄ ▄█▀▀▄██   ▀ ▀▀  █   
  ▀▀ █ █▀▄▀█▀███▀    █▄█ ▄▀▄█▄▀ █▄█▄▀▀█   
  ▀   ▄ ▀█▄ ▄█ ▄█  █▄▄██  ▀█ ▄▀█▀▄ █▄▄█   
  ▀▄▄▀▀█▀▀▄█ █ ▀▀ ▄▄▀  ▄▀▀▀█▀▄▄ ▄ ██▀▀█   
  ▀▀    ▀ █▀█ ██▄▀▄▀ ▀█▀▀█ ▄▄▄█▀▀▀█▄▀▄▄   
  █▀▀▀▀▀█ ▀ ▀▀█ █▄█▄█▄▀█  ▀ █ █ ▀ █▄▄██   
  █ ███ █ ▄▄▀▄  ▄ ▄  ▀▀█▄▀▀▀▀ █▀▀██ ▄▀    
  █ ▀▀▀ █ ▄▀    ▄ ▀▀ ▀█▄  ▄█ ▀▀█▀▄▀▄ ▀▀   
  ▀▀▀▀▀▀▀ ▀ ▀ ▀    ▀  ▀▀▀ ▀▀ ▀▀   ▀  ▀▀   
                                          

I (1480) app: If QR code is not visible, copy paste the below URL in a browser.
https://espressif.github.io/esp-jumpstart/qrcode.html?data={"ver":"v1","name":"PROV_EA36B4","username":"wifiprov","pop":"abcd1234","transport":"ble"}
```

Observamos como se ha generado el código QR necesario apra realziar el aprovisionamiento, además de como se especifica información adyacente a cerca de la configuración establecida para el mismo, como el tipo de transporte o el nombre de indentificación del dispositivo. En el caso de duda, se puede replicar el código obtenido mediante el enlace que se encuentra justo después del mismo.

Una vez generado nuestro medio de enlace entre el dispositivo y el provisionador, escaneamos el código QR con la aplicación de utilizada con el objetivo de establecer el enlace entre ambas partes. En este momento nuestro teléfono movil y el SoC se encuentran conectados y la espera de que el primero le trasmita las claves de aprovisionamiento al segundo. 

En el siguiente cuadro podemos ver la salida que nos devuelve el SoC al realizar dicha conexión con el provisionador, en la cual nos especifica que se está utilizando el protocolo BLE y como se esta realizando un cifrado mediante claves, a la ver que se ha verificado la clave pública utilizada.

```BASH
I (20350) app: BLE transport: Connected!
I (21440) protocomm_nimble: mtu update event; conn_handle=0 cid=4 mtu=256
I (22550) security2: Using salt and verifier to generate public key...
I (23180) app: Secured session established!
```

Llegados a este punto, necesitaremos seleccionar en la aplicación de provisionamiento aquella red WIFI a la cual queremos conectar el dispositivo provisionado, en nuestro caso la STM32. De este modo, la aplicación le transmitirá las credenciales tanto de identificación como de acceso a la red WIFI y este podrá conectarse a la misma. En la siguiente imagen podemos ver la seleccción de redes WIFI disponibles, entre las cuales utilizaremos nuestra red de pruebas **RPI1_test**.



Una vez seleccionada la red WIFI con la que queremos realizar la conexión (y aportadas las credenciales de acceso en el caso de ser necesario), la aplicación de provisionamiento transmitirá dicha información al SoC, el cual realizará automáticamente la conexión con la red que anteriormente hemos indicado. En el siguiente cuadro podemos ver la salida obtenida por el puerto serie cuando el SoC recibe las credenciales de acceso para poder conectarse a la red WIFI.

Podemos ver como tanto el SSID como la contraseña coinciden con las credenciales especificadas en nuestra red WIFI de prueba. Finalmente, una vez se ha realizando la conexión con dicha web, la aplicación nos muestra tanto la dirección IP como la máscara obtenida desde el Punto de Acceso que proporciona servicio a dicha red.


```BASH
I (176540) wifi:<ba-add>idx:0 (ifx:0, ca:df:8d:9a:30:e2), tid:0, ssn:0, winSize:64
I (176580) wifi:AP's beacon interval = 102400 us, DTIM period = 2
I (177520) app: Connected with IP Address:192.168.43.198
I (177520) esp_netif_handlers: sta ip: 192.168.43.198, mask: 255.255.255.0, gw: 192.168.43.1
I (177520) wifi_prov_mgr: STA Got IP
I (177530) app: Provisioning successful
I (177530) app: Hello World!
I (178530) app: Hello World!
I (23180) app: Secured session established!
I (170850) app: Received Wi-Fi credentials
        SSID     : RPI1_test
        Password : test1234
I (175240) wifi:new:<13,0>, old:<1,0>, ap:<255,255>, sta:<13,0>, prof:1
I (176430) wifi:state: init -> auth (b0)
I (176440) wifi:state: auth -> assoc (0)
I (176450) wifi:state: assoc -> run (10)
I (176500) wifi:connected with RPI1_test, aid = 1, channel 13, BW20, bssid = ca:df:8d:9a:30:e2
I (176500) wifi:security: WPA2-PSK, phy: bgn, rssi: -39
I (176510) wifi:pm start, type: 1

I (176540) wifi:<ba-add>idx:0 (ifx:0, ca:df:8d:9a:30:e2), tid:0, ssn:0, winSize:64
I (176580) wifi:AP's beacon interval = 102400 us, DTIM period = 2
I (177520) app: Connected with IP Address:192.168.43.198
I (177520) esp_netif_handlers: sta ip: 192.168.43.198, mask: 255.255.255.0, gw: 192.168.43.1
I (177520) wifi_prov_mgr: STA Got IP
I (177530) app: Provisioning successful
I (177530) app: Hello World!
I (178530) app: Hello World!
```

Una vez que el SoC ha obtenido la dirección IP de la red a la cual se ha conectado, este cerrará la conexión con la aplciación de provisionamiento. En el siguiente cuadro podemos ver la salida generada por dicha desconexión, la cual se produce de manera automática después de la salida anterior y pone punto y final al proceso de provisionamiento.

```BASH
I (179300) NimBLE: GAP procedure initiated: stop advertising.

I (179300) NimBLE: GAP procedure initiated: stop advertising.

I (179300) NimBLE: GAP procedure initiated: terminate connection; conn_handle=0 hci_reason=19

E (179370) protocomm_nimble: Error setting advertisement data; rc = 30
I (179380) wifi_prov_mgr: Provisioning stopped
I (179380) app: BLE transport: Disconnected!
I (179380) app: BLE transport: Disconnected!
I (179390) wifi_prov_scheme_ble: BTDM memory released
I (179530) app: Hello World!
I (180530) app: Hello World!
I (181530) app: Hello World!
I (182530) app: Hello World!
```

## Aprovisionamiento mediante BLE sin seguridad

## Aprovisionamiento mediante SoftAp

Para realizar el aprovisionamiento mediante SoftAP utilizaremos la siguiente aplicación movil:

<img src="images/AplicacionSoftAP.png" alt="drawing" style="width:60%; 
    display: block;
    margin-left: auto;
    margin-right: auto;
    margin-top: 1%;
    margin-botton: 1%;
"/>