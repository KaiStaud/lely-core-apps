# Inputs für Übergänge

## Endsschalter
Wird ein Endschalter ausgelöst wird "Fault Reaction active" gesetzt.
Der State bleibt solange erhalten bis "Fault Reset" gesendet wird.

- Endschalter werden von Statemachine und homing.c gelesen
- Statemachine.c Setzt das lower byte vom Statusword.
- Folgende Operation Modes setzen das high byte:
    - Homing `homing.c`

| Bit | | note |
|-----|-|------|
| 0 | ready to switch on | |
| 1 | switched on | |
| 2 | operation enabled | |
| 3 | fault | |
| 4 | voltage enabled | |
| 5 | quick stop active | active low. | 
| 6 | switch on disabled | active high |
| 7 | warning | |

**Common expected statuswords**
- `xxxx xxxx x0xx 0000b` : Not ready to switch on

- `xxxx xxxx x1xx 0000b` : Switch on disabled

- `xxxx xxxx x01x 0001b` : Ready to switch on

- `xxxx xxxx x01x 0011b`: Switched on

- `xxxx xxxx x01x 0111b`: Operation enabled

- `xxxx xxxx x00x 0111b`: Quick stop active

- `xxxx xxxx x0xx 1111b` : Fault reaction active

- `xxxx xxxx x0xx 1000b` : Fault

**Highbyte at Operation Mode Homing**

| Bit | | note |
|-----|-|------|
| 8 | not available | |
| 9 | oms | |
| 10 | target reached | |
| 11| internal limit active | |
| 12 | homing attained | |
| 13| homing error | |
| 14 | manufacturer specific | |
| 15| manufacturer specific | |