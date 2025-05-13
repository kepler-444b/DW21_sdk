#include "device_info.h"

void printf_info(void)
{
    #if defined PRODUCT_CFG_PRODUCT_TYPE_CCO
    app_printf(" **** this is cco **** \n");
    #endif 

    #if defined PRODUCT_CFG_PRODUCT_TYPE_STA
    app_printf(" **** this is sta **** \n");
    #endif
    device_nv_init();
    ext_nv_ftm_product_id * product_nv = get_ftm_product_id();
    ext_nv_ftm_device_id * device_nv = get_ftm_device_id();
    modem_version_nv *version_nv = get_modem_version_id();
    keep_status_nv *status_nv = get_keep_status_id();

    app_printf("MAC: ");
    for(td_u8 i = 0; i < EXT_PLC_MAC_ADDR_LEN; i++)
    {
        app_printf("%02x:", product_nv->plc_mac[i]);
    }
    app_printf("\n");

    app_printf("dev_category: %d\n", device_nv->dev_type);
    app_printf("version_nv: %d\n", version_nv->firmware_ver);
    app_printf("status_nv: %d\n", status_nv->reserve3);

}