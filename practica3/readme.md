# Comprobaciones de seguridad mediante Wireshark

Para llevar a cabo las comprobaciones de seguridad a la hora de realizar el provisionamiento se utilizarán:
 - El ejemplo **provisioning/wifi-prov-mgr** en el SoC utilizado.
 - La herramienta **esp_prov.py** proporcionada por **Espressif** y que se encuentra en el directorio **esp-idf/tools/esp_prov**.
 - La herramienta de análisis de paquetes **Wireshark** para analizar las credenciales enviadas en el provisionamiento.

A continuación realizaremos dos ejecuciones de nuestra herramienta de provisionamiento, una utilizando un cifrado sobre las claves proporcionadas y otra sin implementar ninguna medida de seguridad.


```BASH
python esp_prov.py --transport softap --service_name "192.168.4.1:80" --sec_ver 0 --ssid RPI1_test --passphrase test1234
```




CON CIFRADO

```BASH
 python esp_prov.py --transport softap --service_name "192.168.4.1:80" --sec_ver 1 --ssid RPI1_test --passphrase test1234
++++ Connecting to 192.168.4.1:80++++
Proof of Possession required: 

==== Starting Session ====
==== Session Established ====

==== Sending Wi-Fi Credentials to Target ====
==== Wi-Fi Credentials sent successfully ====

==== Applying Wi-Fi Config to Target ====
==== Apply config sent successfully ====

==== Wi-Fi connection state  ====
==== WiFi state: Connected ====
==== Provisioning was successful ====
```







SIN CIFRADO


```BASH
 python esp_prov.py --transport softap --service_name "192.168.4.1:80" --sec_ver 1 --ssid RPI1_test --passphrase test1234
++++ Connecting to 192.168.4.1:80++++
Proof of Possession required: 

==== Starting Session ====
==== Session Established ====

==== Sending Wi-Fi Credentials to Target ====
==== Wi-Fi Credentials sent successfully ====

==== Applying Wi-Fi Config to Target ====
==== Apply config sent successfully ====

==== Wi-Fi connection state  ====
==== WiFi state: Connected ====
==== Provisioning was successful ====
```