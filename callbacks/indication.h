//
// Created by kai on 1/2/26.
//

#ifndef CPP_TUTORIAL_INDICATIONS_H
#define CPP_TUTORIAL_INDICATIONS_H

#include <lely/co/nmt.h>
#include <lely/co/rpdo.h>
#include <lely/co/tpdo.h>
void my_rpdo_indication(co_rpdo_t *pdo, co_unsigned32_t ac,
                               const void *ptr, size_t n, void *data);
void tpdo_indication(co_tpdo_t *pdo, co_unsigned32_t ac, const void *ptr,
                            size_t n, void *data);
void sync_indication(co_nmt_t *nmt, co_unsigned8_t cnt, void *data);

#endif //CPP_TUTORIAL_INDICATIONS_H