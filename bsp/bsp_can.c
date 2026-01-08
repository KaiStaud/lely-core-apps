#include "bsp_can.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <net/if.h>
#include <linux/can/raw.h>
#include "../ulog/ulog.h"
int can_socket;
int can_open(const char *ifname)
{
    struct ifreq ifr;
    struct sockaddr_can addr;

    can_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (can_socket < 0) {
        perror("socket");
        return -1;
    }

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ - 1);

    if (ioctl(can_socket, SIOCGIFINDEX, &ifr) < 0) {
        perror("ioctl");
        close(can_socket);
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.can_family  = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(can_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(can_socket);
        return -1;
    }

    return can_socket;
}

int can_send(int sock, uint32_t can_id, uint8_t dlc, const uint8_t *data)
{
    struct can_frame frame;

    if (dlc > 8) {
        fprintf(stderr, "Ungültige DLC: %u\n", dlc);
        return -1;
    }

    frame.can_id  = can_id;
    frame.can_dlc = dlc;
    memcpy(frame.data, data, dlc);

    if (write(sock, &frame, sizeof(frame)) != sizeof(frame)) {
        perror("write");
        return -1;
    }

    return 0;
}

int can_receive(int sock, struct can_frame *frame)
{
    int nbytes;

    nbytes = read(sock, frame, sizeof(struct can_frame));
    if (nbytes < 0) {
        perror("read");
        return -1;
    }

    if (nbytes < sizeof(struct can_frame)) {
        fprintf(stderr, "Unvollständiger CAN-Frame\n");
        return -1;
    }

    return 0;
}

void can_close(int sock)
{
    close(sock);
}

int on_can_send(const struct can_msg *msg, void *data) {
    uint32_t can_id;
    if (!msg || !data) {
    }
    can_id = msg->id;
    if (can_id == 0x27f){
        //    can_id=0x182;
    }
    if (msg->flags & CAN_FLAG_RTR) {
        can_id |= CAN_RTR_FLAG;
        ulog_info("[CAN] tx: Id with added RTR-Flag");
    }

    /* Datenlänge prüfen */
    if (msg->len > CAN_MAX_LEN) {
        ulog_info("CAN_MAX_LEN < msg size");
    }
    int err = can_send(can_socket, can_id, msg->len, msg->data);
//    ulog_info("[CAN] tx: %s %4x [%i]", "vcan0", can_id, msg->len);
}

int can_recv(struct can_msg *ptr, size_t n) {
    struct can_frame frame;
    struct can_msg *msg;
    ssize_t nbytes;

    if (!ptr || n == 0 || can_socket < 0) {
        errno = EINVAL;
        return -1;
    }

    /* Genau ein Frame lesen (blockierend) */
    nbytes = read(can_socket, &frame, sizeof(frame));
    if (nbytes < 0) {
        /* errno bleibt gesetzt */
        return -1;
    }

    if ((size_t)nbytes < sizeof(struct can_frame)) {
        errno = EIO;
        return -1;
    }

    msg = &ptr[0];
    memset(msg, 0, sizeof(*msg));

    /* Identifier */
    if (frame.can_id & CAN_EFF_FLAG) {
        msg->flags |= CAN_FLAG_IDE;
        msg->id = frame.can_id & CAN_MASK_EID;
    } else {
        msg->id = frame.can_id & CAN_MASK_BID;
    }

    /* RTR */
    if (frame.can_id & CAN_RTR_FLAG) {
        msg->flags |= CAN_FLAG_RTR;
        msg->len = frame.can_dlc;
    } else {
        msg->len = frame.can_dlc;
        memcpy(msg->data, frame.data, frame.can_dlc);
    }
    return 1;
}
