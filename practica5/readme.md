# PRACTICA 5 - Implementación de un servidor GATT basado en tablas

En la presente práctica vamos a llevar a cabo la implementación de un servidor GATT basado en tablas, para lo cual necesitaremos especificar dos partes:
- **El servidor BLE:** Será implementado en nuestra SoC STM32 mediante el ejemplo **examples/bluetooth/bluedroid/ble/gatt_server_service_table**.
- **El cliente BLE:** Será desarrollado mediante un equipo de sobremesa Linux a través de la herramienta **gatttool** en línea de comandos.




<br />

## Escaneado de dispositivos Bluetooth disponibles

>Tarea Básica
>
>Edita el fichero main/gatts_table_creat_demo.c y modifica el nombre de tu dispositivo, que se enviará en cada anuncio emitido en la fase de advertising . Para ello, debes modificar el campo correspondiente de la estructura raw_adv_data . A continuación, compila y flashea el ejemplo, y comienza una sesión de escaneado de dispositivos BLE mediante la orden: sudo hcitool lescan Deberás observar tu dispositivo en una de las líneas. Anota o recuerda su dirección MAC.

A continuación vamos a llevar a cabo el escaneado de los posibles servidores GATT disponibles mediante la interfaz Bluetooth de nuestro equipo Linux, para lo cual necesitaremos utilizar la herramienta de línea de comandos **hcitool**, la cual nos permitirá gestionar dicha interfaz y realizar un análisis de los dispositivos existentes. En el caso de que no se disponga de dicha herramienta, necesitaremos instalarla en nuestro equipo. En el siguiente [enlace](https://command-not-found.com/hcitool) tendremos un breve resumen de las instrucciones para llevar a cabo la instalación en las principales distribuciones Linux.S

Antes de llevar a cabo el escaneo, necesitaremos ejecutar el ejemplo mencionado **gatt_server_service_table** en nuestro SoC, para lo cual tenemos que modificar el nombre con el que dicho servidor se publicitará, de modo que podamos identificarlo sin problemas. En los siguientes cuadros podemos ver las modificaciones necesarias para cambiar el nombre del servidor GATT, las cuales se encuentran en la parte inicial del fichero **gatts_table_creat_demo.c**.


```C
#define SAMPLE_DEVICE_NAME          "ESP32_RPI1"    //"ESP_GATTS_DEMO"
.
.
.
static uint8_t raw_adv_data[] = {
        /* flags */
        0x02, 0x01, 0x06,
        /* tx power*/
        0x02, 0x0a, 0xeb,
        /* service uuid */
        0x03, 0x03, 0xFF, 0x00,
        /* device name */
        0x0f, 0x09, 'E', 'S', 'P', '3', '2', '_', 'R', 'P', 'I', '1'
};

```

Tras la configuración pasamos a ejecutar el ejemplo podemos obtener la siguiente salida, donde vemos como se ha comenzado el envío de los mensajes de anuncio **advertising**:
```BASH
I (573) main_task: Calling app_main()
I (613) BTDM_INIT: BT controller compile version [946b762]
I (623) BTDM_INIT: Bluetooth MAC: 24:0a:c4:ea:36:b6
I (623) phy_init: phy_version 4670,719f9f6,Feb 18 2021,17:07:07
I (1043) main_task: Returned from app_main()
I (1043) GATTS_TABLE_DEMO: create attribute table successfully, the number handle = 8

I (1053) GATTS_TABLE_DEMO: SERVICE_START_EVT, status 0, service_handle 40
I (1063) GATTS_TABLE_DEMO: advertising start successfully
```

Una vez hecho esto, ejecutaremos un escaneo de dispositivos Bluetooth con la herramienta **hcitool** con el objetivo de verificar que nuestro dispositivo se encuentra corriendo adecuadamente. Para lo cual ejecutaremos la la orden `sudo hcitool lescan | grep "RPI1"`, filtrando el resultado para obtener así un resultado más limpio. 

Recordar que para poder llevar a cabo el escaneo correctamente, nuestro equipo tienen que tener activada la tecnología Bluetooth. En la siguiente imagen podemos ver como se detecta correctamente los mensajes de anuncio de nuestro SoC y la dirección MAC con el que este se está anunciando.

```BASH
debian12:~$ sudo hcitool lescan | grep "RPI1"
24:0A:C4:EA:36:B6 ESP_GATTS_PEPE
```



<br />

## Interaccionando con nuestro servidor GATT

En el presente apartado realizaremos diferentes interacciones con nuestro servidor GATT mediante el uso de la herramienta en línea de comandos **gatttol**. En el caso de no tenerla instalada previamente, en el siguiente (enlace)[https://command-not-found.com/gatttool] podemos ver un breve resumen de las instrucciones para llevar a cabo la instalación en las principales distribuciones Linux.

Para un mejor entendimiento de las acciones realizadas, dividiremos el presente apartado en pasos sucesivos.




<br />

### PASO 1: Establecimiento de conexión y consulta de características

>Tarea Básica
>
>Mediante el comando correspondiente ( characteristics ), consulta y anota las características disponibles en tu servidor GATT.

Una vez instalada la herramienta, realizaremos la conexión con nuestro servidor GATT mediante la orden `gatttool -b "direccionMAC" -I`, el cual iniciará el intérprete de instrucciones en línea de comandos y cuya salida podremos en el siguiente cuadro, donde se ha llevado a cabo la conexión:

```BASH
mario@debian12:~$ gatttool -b 24:0A:C4:EA:36:B6 -I
[24:0A:C4:EA:36:B6][LE]> connect 
Attempting to connect to 24:0A:C4:EA:36:B6
Connection successful
[24:0A:C4:EA:36:B6][LE]> 
```

De la misma manera, cuando llevamos acabo la conexión también obtenemos una notificación en nuestro servidor GATT, en el cual podemos ver la siguiente salida, la cual nos notifica que se hay un nuevo cliente conectado:

```BASH
**TODO**********
```

Una vez realizado esto, podemos determinar que la conexión ha sido establecida de exitosamente, por lo que ahora podremos examinar todas y cada una de las características disponibles en nuestro servidor GATT mediante la orden `characteristics`, enviada desde el interprete de comandos en nuestro sistema. En el siguiente cuadro podemos ver una salida de la ejecución de dicha orden:

```BASH
[24:0A:C4:EA:36:B6][LE]> characteristics 
handle: 0x0002, char properties: 0x20, char value handle: 0x0003, uuid: 00002a05-0000-1000-8000-00805f9b34fb
handle: 0x0005, char properties: 0x0a, char value handle: 0x0006, uuid: 00002b29-0000-1000-8000-00805f9b34fb
handle: 0x0007, char properties: 0x02, char value handle: 0x0008, uuid: 00002b2a-0000-1000-8000-00805f9b34fb
handle: 0x0015, char properties: 0x02, char value handle: 0x0016, uuid: 00002a00-0000-1000-8000-00805f9b34fb
handle: 0x0017, char properties: 0x02, char value handle: 0x0018, uuid: 00002a01-0000-1000-8000-00805f9b34fb
handle: 0x0019, char properties: 0x02, char value handle: 0x001a, uuid: 00002aa6-0000-1000-8000-00805f9b34fb
handle: 0x0029, char properties: 0x1a, char value handle: 0x002a, uuid: 0000ff01-0000-1000-8000-00805f9b34fb
handle: 0x002c, char properties: 0x02, char value handle: 0x002d, uuid: 0000ff02-0000-1000-8000-00805f9b34fb
handle: 0x002e, char properties: 0x08, char value handle: 0x002f, uuid: 0000ff03-0000-1000-8000-00805f9b34fb
[24:0A:C4:EA:36:B6][LE]> 
```

>Tarea Adicional
>
>El manejador que permite leer desde la característica *Heart Rate Value" tiene un manejador de tipo char asociado. Anota su valor.

Para poder determinar cual es el valor del UUID que corresponde a la característica **Heart Rate Value** necesitaremos examinar las variables variables definidas dentro del fichero **gatts_table_creat_demo.c**, teniendo en cuenta que se corresponde con la definida como **GATTS_CHAR_UUID_TEST_A**. En el siguiente cuadro podemos ver la definición de la variable en cuestión y de aquellas adyacentes.

```C
/* Service */
static const uint16_t GATTS_SERVICE_UUID_TEST      = 0x00FF;
static const uint16_t GATTS_CHAR_UUID_TEST_A       = 0xFF01;
static const uint16_t GATTS_CHAR_UUID_TEST_B       = 0xFF02;
```

Teniendo esto en cuenta y en base a la tabla de características devueltas anteriormente por el servidor GATT, podemos ver como el UUID asociado a dicha variable es **0000ff01-0000-1000-8000-00805f9b34fb** y su handler, el cual utilizaremos más adelante y es necesario para interaccionar con la misma, es **0x002a**



<br />

### PASO 2: Lectura y escritura de una característica

>Tarea Básica
>
>Obtén los datos de lectura de la característica de medición del valor de monitorización de ritmo cardíaco. ¿Cuáles son? Deberías observar un valor de retorno de cuatro bytes con valor 0x00. Estos valores corresponden a los de la variable char_value de tu código. Modifícalos, recompila y vuelve a flashear el código. ¿Han variado?

A continuación vamos a leer el valor asociado a la característica **Heart Rate Value** mediante el código de su manejador, para lo cual utilizaremos la instrucción `char-read-hnd "codigoManejador"`. Teniendo en cuenta que el valor de esta característica esta asociada a la variable **char_value** de nuestro código, deberemos obtener el mismo resultado a la hora de llevar a cabo la consulta. En los siguientes cuadros podemos ver el valor de dicha variable en el código y el resultado obtenido al ejecutar la instrucción desde nuestro sistema, respectivamente:


```C
static const uint8_t char_value[4]                 = {0x11, 0x22, 0x33, 0x44};
```
```BASH
[24:0A:C4:EA:36:B6][LE]> char-read-hnd 0x002A
Characteristic value/descriptor: 11 22 33 44 
```

Al llevar a cabo esto, nuestro SoC detecta la petición de lectura realizada y nos informa de ello mediante la salida del puerto serie. En el siguiente cuadro podemos ver dicha salida:

```BASH
**TODO**********
```

>Tarea Básica
>
>Intenta ahora escribir en la anterior característica. Para ello, utiliza el comando char-write-cmd handler valor , siendo valor, por ejemplo, 11223344 .

Antes de pasar al escribir en la característica, vamos a modificar el valor asociada a la misma directamente desde el fichero **gatts_table_creat_demo.c** y comprobar que ese se ha actualizado correctamente al llevar a cabo una nueva solicitud de lectura. Pera esto será necesario volver a establecer la conexión con nuestro servidor.

En los siguientes cuadros podemos el nuevo valor asociado a la variable y como este coincide con la respuesta obtenida al volver a llevar a cabo la petición de lectura desde nuestro sistema cliente:


```C
static const uint8_t char_value[4]                 = {0x55, 0x66, 0x77, 0x88};
```

```BASH
mario@debian12:~$ gatttool -b 24:0A:C4:EA:36:B6 -I
[24:0A:C4:EA:36:B6][LE]> connect 
Attempting to connect to 24:0A:C4:EA:36:B6
Connection successful
[24:0A:C4:EA:36:B6][LE]> char-read-hnd 0x002A
Characteristic value/descriptor: 55 66 77 88 
```

A continuación vamos a realizar la escritura del valor asociado a dicha característica mediante la instrucción `char-write-cmd "manejador" "nuevoValor"`, siendo el valor utilizado como ejemplo **12345678**. En los siguientes cuadros podemos ver tanto la respuesta generado por nuestro sistema cliente como la notificación que nos muestra el servidor GATT al recibir la petición de escritura, respectivamente:

```BASH
[24:0A:C4:EA:36:B6][LE]> char-write-cmd 0x002A 12345678
[24:0A:C4:EA:36:B6][LE]> char-read-hnd 0x002A
Characteristic value/descriptor: 12 34 56 78 
```

```BASH
**TODO**********
```


TAREA 4

```BASH
[24:0A:C4:EA:36:B6][LE]> char-desc
handle: 0x0001, uuid: 00002800-0000-1000-8000-00805f9b34fb
handle: 0x0002, uuid: 00002803-0000-1000-8000-00805f9b34fb
handle: 0x0003, uuid: 00002a05-0000-1000-8000-00805f9b34fb
handle: 0x0004, uuid: 00002902-0000-1000-8000-00805f9b34fb
handle: 0x0005, uuid: 00002803-0000-1000-8000-00805f9b34fb
handle: 0x0006, uuid: 00002b29-0000-1000-8000-00805f9b34fb
handle: 0x0007, uuid: 00002803-0000-1000-8000-00805f9b34fb
handle: 0x0008, uuid: 00002b2a-0000-1000-8000-00805f9b34fb
handle: 0x0014, uuid: 00002800-0000-1000-8000-00805f9b34fb
handle: 0x0015, uuid: 00002803-0000-1000-8000-00805f9b34fb
handle: 0x0016, uuid: 00002a00-0000-1000-8000-00805f9b34fb
handle: 0x0017, uuid: 00002803-0000-1000-8000-00805f9b34fb
handle: 0x0018, uuid: 00002a01-0000-1000-8000-00805f9b34fb
handle: 0x0019, uuid: 00002803-0000-1000-8000-00805f9b34fb
handle: 0x001a, uuid: 00002aa6-0000-1000-8000-00805f9b34fb
handle: 0x0028, uuid: 00002800-0000-1000-8000-00805f9b34fb
handle: 0x0029, uuid: 00002803-0000-1000-8000-00805f9b34fb
handle: 0x002a, uuid: 0000ff01-0000-1000-8000-00805f9b34fb
handle: 0x002b, uuid: 00002902-0000-1000-8000-00805f9b34fb
handle: 0x002c, uuid: 00002803-0000-1000-8000-00805f9b34fb
handle: 0x002d, uuid: 0000ff02-0000-1000-8000-00805f9b34fb
handle: 0x002e, uuid: 00002803-0000-1000-8000-00805f9b34fb
handle: 0x002f, uuid: 0000ff03-0000-1000-8000-00805f9b34fb
```

```BASH
[24:0A:C4:EA:36:B6][LE]> char-read-hnd 0x002B
Characteristic value/descriptor: 00 00 
[24:0A:C4:EA:36:B6][LE]> char-write-cmd 0x002B 1234
[24:0A:C4:EA:36:B6][LE]> char-read-hnd 0x002B
Characteristic value/descriptor: 12 34 
```
