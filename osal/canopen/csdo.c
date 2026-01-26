#include "csdo.h"
#include <lely/co/csdo.h>
#include <lely/util/errnum.h>

int csdo_dn_value(co_csdo_t *csdo,uint16_t idx, uint8_t subidx, uint32_t value)
{
    int rc =co_csdo_dn_val_req(csdo,idx,subidx,CO_DEFTYPE_UNSIGNED32,&value,NULL,NULL);
    if (rc !=0){
        return get_errc();
    }
    else{
        return 0;
    }
}
/*
int csdo_up_value(co_unsigned16_t type,uint16_t idx, uint8_t subidx, void *data)
{
    int rc=co_csdo_up_req(csdo,idx,subidx,NULL,data);
    co_csdo_destroy(csdo);
    if (rc !=0){
        return get_errc();
    }
    else{
        return 0;
    }
}
*/