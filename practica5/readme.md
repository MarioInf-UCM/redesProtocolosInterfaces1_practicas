# PRACTICA 5 - Implementación de un servidor GATT basado en tablas

En la presente práctica vamos a llevar a cabo la implementación de un servidor GATT basado en tablas, para lo cual necesitaremos especificar dos partes:
- **El servidor BLE:** Será implementado en nuestra SoC STM32 mediante el ejemplo

## Escaneado de dispositivos disponibles

A continuación vamos a llevar a cabo el escaneado de los posibles servidores GATT disponibles, para la cual vamos a utilizar el ejemplo

```BASH
debian12:~$ sudo hcitool lescan | grep "PEPE"
24:0A:C4:EA:36:B6 ESP_GATTS_PEPE
```

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
