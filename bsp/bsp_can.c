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

int can_open(const char *ifname)
{
    int sock;
    struct ifreq ifr;
    struct sockaddr_can addr;

    sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock < 0) {
        perror("socket");
        return -1;
    }

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ - 1);

    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
        perror("ioctl");
        close(sock);
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.can_family  = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sock);
        return -1;
    }

    return sock;
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
