Dump von Lely-Beispiel Code "PDO Ping Pong":
```
 (000.558368)  vcan0  080   [0]           <-- Sync        
 (000.000241)  vcan0  182   [4]  00 00 00 00      <-- TPDO 1 von Node-ID 2 an Master
 (000.000183)  vcan0  202   [4]  00 00 00 00      <-- RPDO 1 von Master an Node-ID 2
 (000.441198)  vcan0  702   [1]  05      <-- heartbeat + NMT State OP von Node-ID 2
```
Dump von eigener Application:
```
 (000.026656)  can0  702   [1]  00 <--- Slave 2 Boot-Up
 (000.001688)  can0  602   [8]  40 00 10 00 00 00 00 00
 (000.030128)  can0  582   [8]  43 00 10 00 00 00 00 00
 (000.001263)  can0  602   [8]  40 18 10 01 00 00 00 00
 (000.008565)  can0  582   [8]  43 18 10 01 60 03 00 00
 (000.001602)  can0  602   [8]  23 16 10 01 D0 07 01 00
 (000.009487)  can0  582   [8]  60 16 10 01 00 00 00 00
 (000.001491)  can0  602   [8]  2B 17 10 00 E8 03 00 00
 (000.008843)  can0  582   [8]  60 17 10 00 00 00 00 00
 (000.001609)  can0  602   [8]  23 40 60 00 06 00 00 00
 (000.008747)  can0  582   [8]  60 40 60 00 00 00 00 00
 (000.265478)  can0  702   [1]  7F <-- Heartbeat + Slave 2 Pre-Op
 (000.235479)  can0  080   [0]  <-- Sync
 (000.000814)  can0  202   [4]  00 00 00 00 <-- RPDO 1 von Master an Node-ID 2
 (000.001072)  can0  602   [8]  23 40 60 00 07 00 00 00 <-- Upload 0x7 in 0x6040:00 
 (000.008850)  can0  582   [8]  60 40 60 00 00 00 00 00 <-- Download OK
 (000.501584)  can0  602   [8]  23 40 60 00 0F 00 00 00 <-- Upload 0xF in 0x6040:00
 (000.008850)  can0  582   [8]  60 40 60 00 00 00 00 00 <--  Download OK
 (000.268098)  can0  702   [1]  7F <-- Heartbeat + Slave 2 Pre-Op
 (000.233372)  can0  000   [2]  01 02 <-- Start Remote Node 0x2
 (000.000545)  can0  080   [0] 
 (000.000796)  can0  202   [4]  00 00 00 00 <-- RPDO 1 von Master an Node-ID 2
 (000.682877)  can0  080   [0] 
 (000.000827)  can0  202   [4]  00 00 00 00 <-- RPDO 1 von Master an Node-ID 2
 (000.106212)  can0  702   [1]  05 <-- heartbeat + NMT State OP von Node-ID 2
```

Ausgaben von apps/master.cpp zusammen mit STM32
Slave:
NMT: heartbeat time out occurred for node 1   
EMCY: 8130 11
NMT: communication error indicated

Slave Configuration:
- Hearbeat Consumer: Timeout 2000ms, Monitor Node 1 `Wait(AsyncWrite<uint32_t>(0x1016, 1, (1 << 16) | 2000));`
- Eigener Heatbeat: Producer mit 1000ms  `Wait(AsyncWrite<uint16_t>(0x1017, 0, 1000));`

Master Konfiguration:
-  Consumer: `ConfigHeartbeat(2000ms);`
