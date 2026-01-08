//
// Created by kai on 1/2/26.
//
#include "indication.h"
#include "../ulog/ulog.h"

void my_rpdo_indication(co_rpdo_t *pdo, co_unsigned32_t ac,
                               const void *ptr, size_t n, void *data) {
    if (ac != 0) {
        /* Fehler beim Empfang */
        ulog_info("RPDO error, abort code: 0x%08X\n", ac);
        return;
    }

    /* Erfolgreicher Empfang */
    ulog_info("RPDO received: %zu bytes\n", n);

    const uint8_t *bytes = (const uint8_t *)ptr;
    for (size_t i = 0; i < n; ++i) {
        ulog_info("Byte %zu: 0x%02X\n", i, bytes[i]);
    }

    /* Optional: benutzerspezifische Daten */
    if (data) {
        ulog_info("User data pointer: %p\n", data);
    }
}

void tpdo_indication(co_tpdo_t *pdo, co_unsigned32_t ac, const void *ptr,
                            size_t n, void *data) {
    if (ac != 0) {
        /* Fehler beim Empfang */
        ulog_info("[TPDO] error, abort code: 0x%08X\n", ac);
        return;
    }

    ulog_info("[TPDO] tx send %i bytes", n);

    const uint8_t *bytes = (const uint8_t *)ptr;
    for (size_t i = 0; i < n; ++i) {
        ulog_info("[TPDO] Byte %zu: 0x%02X\n", i, bytes[i]);
    }
}

void sync_indication(co_nmt_t *nmt, co_unsigned8_t cnt, void *data) {
//    ulog_info("[NMT] : received sync indication for count %i", cnt);
}