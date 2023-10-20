# PRACTICA 5 - Implementación de un servidor GATT basado en tablas

En la presente práctica vamos a llevar a cabo la implementación de un servidor GATT basado en tablas, para lo cual necesitaremos especificar dos partes:
- **El servidor BLE:** Será implementado en nuestra SoC STM32 mediante el ejemplo **examples/bluetooth/bluedroid/ble/gatt_server_service_table**.
- **El cliente BLE:** Será desarrollado mediante un equipo de sobremesa Linux a través de la herramienta **gatttool** en línea de comandos.

<br />

## Escaneado de dispositivos Bluetooth disponibles

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

Una vez hecho esto, ejecutaremos un escaneo de dispositivos Bluetooth con la herramienta **hcitool** con el objetivo de verificar que nuestro dispositivo se encuentra corriendo adecuadamente. Para lo cual ejecutaremos la la orden **sudo hcitool lescan | grep "RPI1"**, filtrando el resultado para obtener así un resultado más limpio. 

Recordar que para poder llevar a cabo el escaneo correctamente, nuestro equipo tienen que tener activada la tecnología Bluetooth. En la siguiente imagen podemos ver como se detecta correctamente los mensajes de anuncio de nuestro SoC y la dirección MAC con el que este se está anunciando.

```BASH
debian12:~$ sudo hcitool lescan | grep "RPI1"
24:0A:C4:EA:36:B6 ESP_GATTS_PEPE
```

<br />

## Interaccionando con nuestro servidor GATT

```BASH
mario@debian12:~$ gatttool -b 24:0A:C4:EA:36:B6 -I
[24:0A:C4:EA:36:B6][LE]> connect 
Attempting to connect to 24:0A:C4:EA:36:B6
Connection successful
[24:0A:C4:EA:36:B6][LE]> 
```


TAREA 2


```C
/* Service */
static const uint16_t GATTS_SERVICE_UUID_TEST      = 0x00FF;
static const uint16_t GATTS_CHAR_UUID_TEST_A       = 0xFF01;
static const uint16_t GATTS_CHAR_UUID_TEST_B       = 0xFF02;
static const uint16_t GATTS_CHAR_UUID_TEST_C       = 0xFF03;
```

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




```BASH
handle: 0x0029, char properties: 0x1a, char value handle: 0x002a, uuid: 0000ff01-0000-1000-8000-00805f9b34fb
```
Su manejador es 0x002A


TAREA 3

```BASH
[24:0A:C4:EA:36:B6][LE]> char-read-hnd 0x002A
Characteristic value/descriptor: 11 22 33 44 
```

```BASH
mario@debian12:~$ gatttool -b 24:0A:C4:EA:36:B6 -I
[24:0A:C4:EA:36:B6][LE]> connect 
Attempting to connect to 24:0A:C4:EA:36:B6
Connection successful
[24:0A:C4:EA:36:B6][LE]> char-read-hnd 0x002A
Characteristic value/descriptor: 55 66 77 88 
```

```BASH
[24:0A:C4:EA:36:B6][LE]> char-write-cmd 0x002A 12345678
[24:0A:C4:EA:36:B6][LE]> char-read-hnd 0x002A
Characteristic value/descriptor: 12 34 56 78 
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
