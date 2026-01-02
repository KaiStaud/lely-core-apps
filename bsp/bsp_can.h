#ifndef CAN_COMM_H
#define CAN_COMM_H

#include <stdint.h>
#include <linux/can.h>
#include <lely/co/time.h>
#include <lely/can/msg.h>

/**
 * Öffnet ein CAN-Interface (z.B. "can0" oder "vcan0")
 * @param ifname Name des CAN-Interfaces
 * @return Socket-Descriptor oder -1 bei Fehler
 */
int can_open(const char *ifname);

/**
 * Sendet eine CAN-Nachricht
 * @param sock CAN-Socket
 * @param can_id CAN-ID (Standard oder Extended)
 * @param dlc Datenlänge (0..8)
 * @param data Datenpuffer
 * @return 0 bei Erfolg, -1 bei Fehler
 */
int can_send(int sock, uint32_t can_id, uint8_t dlc, const uint8_t *data);

/**
 * Empfängt eine CAN-Nachricht (blockierend)
 * @param sock CAN-Socket
 * @param frame Pointer auf can_frame Struktur
 * @return 0 bei Erfolg, -1 bei Fehler
 */
int can_receive(int sock, struct can_frame *frame);

/**
 * Schließt den CAN-Socket
 * @param sock CAN-Socket
 */
void can_close(int sock);

int on_can_send(const struct can_msg *msg, void *data);
int can_recv(struct can_msg *ptr, size_t n);
#endif /* CAN_COMM_H */
