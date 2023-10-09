PRIMERA CONEXIÓN
```BASH
I (882) wifi:mode : sta (24:0a:c4:ea:36:b4) + softAP (24:0a:c4:ea:36:b5)
I (882) wifi:enable tsf
I (892) wifi:Total power save buffer number: 16
I (892) wifi:Init max length of beacon: 752/752
I (892) wifi:Init max length of beacon: 752/752
I (892) mesh: <nvs>read layer:0, err:0x1102
I (902) mesh: <nvs>read assoc:0, err:0x1102
I (902) wifi:Set ps type: 1, coexist: 0

I (922) wifi:Total power save buffer number: 16
I (2082) wifi:mode : sta (24:0a:c4:ea:36:b4)
I (2102) mesh: <MESH_NWK_LOOK_FOR_NETWORK>need_scan:0x3, need_scan_router:0x0, look_for_nwk_count:1
```

```BASH
I (169682) mesh_main: layer:3, rtableSize:2, NODE
W (174542) wifi:(->sleep)busy, waked:1, dream:0, sleep:0
W (176092) mesh_main: [#RX:3/449][L:3] parent:24:0a:c4:ea:36:61, receive from c4:dd:57:5b:fc:f8, size:1460, heap:150884, flag:0[err:0x0, proto:0, tos:0]
W (178992) wifi:(->sleep)busy, waked:1, dream:0, sleep:0
```

RECONEXIÓN
```BASH
I (1029682) mesh_main: layer:2, rtableSize:1, NODE
W (1030752) wifi:(->sleep)busy, waked:1, dream:0, sleep:0
W (1031852) wifi:(->sleep)busy, waked:1, dream:0, sleep:0
W (1031912) mesh_main: [#RX:49/44][L:2] parent:24:0a:c4:ea:3c:a9, receive from 24:0a:c4:ea:3c:a8, size:1460, heap:149688, flag:0[err:0x0, proto:0, tos:0]
```

GRUPOS
```BASH
I (49092) mesh_main: layer:2, rtableSize:1, NODE
W (50612) wifi:(->sleep)busy, waked:1, dream:0, sleep:0
W (50642) mesh_main: [#RX:23/459][L:2] parent:24:0a:c4:ea:36:c9, receive from 24:0a:c4:ea:36:c8, size:1460, heap:156736, flag:0[err:0x0, proto:0, tos:0]
```