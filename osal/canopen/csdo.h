// Client library for handling download (client -> server) and upload (server -> client) requests.
#pragma once
#include <lely/co/csdo.h>
#include <lely/can/net.h>
enum CSDO_ERROR
{
    csdo_no_error = 0
};
int csdo_dn_value(co_csdo_t *csdo,uint16_t idx, uint8_t subidx, uint32_t value);
//int csdo_up_value(co_unsigned16_t type,uint16_t idx, uint8_t subidx, void *data);
