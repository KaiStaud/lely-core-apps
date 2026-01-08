# Slave program flow
## CO-Library initialisieren
- TPDOs und RPDOs erstellen.
- Callbacks initialisieren
- Indikatinen einhängen

## Applikationsthreads erstellen
Einzelne Threas je nach Betriebsmodus starten.
Der Slave implementiert einzelne Betriebsmodi welche durch 0x6060 "Modes of Operation" ausgewählt werden.
Alle Profile und Cyclic-Modi teilen sich einen Thread. Der Homing-Mode besitzt einen eigenen Thread. 
Der aktive Operationsmodus wird in 06061 "Modes of Operation Display" angezeigt.

| Value for 0x6060 / 0x6061 | Control Mode       |
|---------------------------|--------------------|
| -3                        |Open loop field mode|
| -2                        |Diagnostics mode|
| -1                        |Cogging compensation recording mode|
| 0                         |No moode seleted|
| +1                        |Profile position mode|
| +3                        |Profile velocity mode|
| +4                        |Torque profile mode|
| +6                        |Homing mode|
| +8                        |Cyclic Syncronous Position Mode|
| +9                        |Cyclic Syncronous Velocity Mode|
| +10                       |Cyclic Syncronous Torque Mode|

### Homing
Nachdem der Operationsmodus auf Homing einsgestellt wurde, wird das Profil in 0x6098 eingesetzt.
Verfügbar sind "Homing on Positive Endstop" (-2) und "Homing on Negative Endstop" (-1).
Die Verfahrrichtung ist entweder positiv (links?/Dir 1) oder negativ (rechts?/Dir 0).
Zusätzlich werden Beschleunigung und Zielgeschwindigkeit eingestellt.
Nachdem Konfiguration erledigt ist, wird Bit 4 im Controlword "Homing start" gesetzt.

Wird ein Endstop ausgelöst wechselt der Antrieb in "Fault". Im Statusword sollte nun "Homing performed + Homing aquired" stehen.
Zusätzlich werden die GPIOs ausgelesen. Es sollte der entsprechende Endstop ausgelöst sein.
kann ein
Nach Homing wird der Fault durchs übertragen des Controlwords zurücksgesetzt.

| # | Schritt          | Objekt    | Value     | Spiegelobjekt | Access |
|---|------------------|-----------|-----------|---------------|--------|
| 1 | Homing aktivieren | 0x6060:00 | +6        | 0x6061        | SDO    |
| 2 | Profil einstellen | 0x6098:00 | -1        | -             | SDO    |
| 3 | Beschleunigung   | 0x609A    | ...       | 0x6081        | SDO    |
| 4 | Geschwindigkeit  | 0x6099    | ...       | 0x6083        | SDO    |
| 5 | Homing start     | 0x6060:00 | 0xF=1<<4  | -             | TPDO   |
| 6 | Statusword lesen | 0x6041:00 | ...       | -             | RPDO   |
| 7 | GPIOs auslese    | 0x60FD    | ...       | -             | RPDO   |
| 8 | Fault Reset | 0x6040:00 | 0xF0=1<<7 | 0x6041        | TPDO   |

**SDO**
- Geschwindigkeit
- Beschleunigung,
- Homing Mode
- Mode of Operation

**PDO Mappings**

Ein PDO kann maximal 64 Bit beinhalten:
- Slave-TPDO:Statusword,GPIOs
- Slave-TPDO_2:Geschwindigkeit,Beschleuningung
- Slave-RPDO:Controlword
- Slave-RPDO:Beschleunigung,Geschwindigkeit
```mermaid

```