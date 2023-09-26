# MODO PUNTO DE ACCESO

En el presente entregable se trabajará con el uso de la biblioteca WIFI proporcionada para la placa STM32 en su modo para trabajar como Punto de Acceso.

## Ejemplo de conexión

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

```
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

```
I (20319) wifi:new:<1,1>, old:<1,1>, ap:<1,1>, sta:<255,255>, prof:1
I (20319) wifi:station: f8:94:c2:b9:6b:08 join, AID=1, bgn, 40U
I (20359) wifi softAP: station f8:94:c2:b9:6b:08 join, AID=1
I (20459) esp_netif_lwip: DHCP server assigned IP to a client, IP is: 192.168.4.2
I (20679) wifi:<ba-add>idx:2 (ifx:1, f8:94:c2:b9:6b:08), tid:0, ssn:15, winSize:64
```

Para finalizar vamos a llevar a cabo la desconexión de la estación para obtener la siguiente salida, donde vemos que esta ha sido detectada por el Punto de Acceso. Tener en cuenta que esta salida se realiza dentro de la manejadora que trata el evento **WIFI_EVENT_AP_STADISCONNECTED**.

```
I (54499) wifi:station: f8:94:c2:b9:6b:08 leave, AID = 1, bss_flags is 8391747, bss:0x3ffb91f0
I (54499) wifi:new:<1,0>, old:<1,1>, ap:<1,1>, sta:<255,255>, prof:1
I (54509) wifi:<ba-del>idx:2, tid:0
I (54509) wifi softAP: station f8:94:c2:b9:6b:08 leave, AID=1
```


## Configuración de eventos

En el ejemplo del cual parte el entregable se realiza el manejo únicamente de dos eventos:
 - WIFI_EVENT_AP_STACONNECTED: Lanzado cuando una estación lleva a cabo una conexión con el Punto de Acceso.
 
 - WIFI_EVENT_AP_STADISCONNECTED: Lanzado cuando una estación se desconecta del Punto de acceso. 

 Ya hemos visto la salida que producen ambos eventos en el apartado anterior, donde hemos llevado a cabo la conexión y desconexión de una estación, por lo que no volveremos a visualizarla ahora.
 
 Sin embargo, examinando todos los posibles eventos lanzados por el sistema WIFI configurado como Punto de Acceso y filtrando aquellos que sean relevantes, observamos que WIFI_EVENT_AP_START y WIFI_EVENT_AP_STOP no se encuentran tratados correctamente. Debido a esto, pasaremos a implementar manejadores espedificas para cada uno de ellos.

 ### Tratando el evento WIFI_EVENT_AP_START

 ### Tratando el evento WIFI_EVENT_AP_STOP