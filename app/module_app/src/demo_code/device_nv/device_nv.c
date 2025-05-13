#include "device_nv.h"

static ext_nv_ftm_product_id g_ftm_product_id;
static ext_nv_ftm_device_id g_ftm_device_id;
static modem_version_nv g_modem_version_nv;
static keep_status_nv g_keep_status_nv;
td_void device_nv_init(td_void)
{
    app_printf(" **** device_nv_init **** \n");

    nv_read_ftm_product_id(NULL);
    nv_read_ftm_device_id(NULL);
    nv_read_modem_version_id(NULL);
    nv_read_keep_status_id(NULL);
}

td_u32 nv_read_ftm_product_id(ext_nv_ftm_product_id *product_nv)
{
    td_u32 ret = EXT_ERR_SUCCESS;
    static td_bool init = TD_TRUE;
    if (init)
    {
        init = TD_FALSE;
        memset_s(&g_ftm_product_id, sizeof(ext_nv_ftm_product_id), 0x00,
                 sizeof(ext_nv_ftm_product_id));
        ret = uapi_factory_nv_read(EXT_NV_FTM_PRODUCT_ID, &g_ftm_product_id,
                                   sizeof(ext_nv_ftm_product_id));
    }

    if (ret == EXT_ERR_SUCCESS)
    {
        if ((ret == EXT_ERR_SUCCESS) && product_nv)
        {
            memcpy_s(product_nv, sizeof(ext_nv_ftm_product_id),
                    &g_ftm_product_id, sizeof(ext_nv_ftm_product_id));
        }
    }
    return ret;
}

td_u32 nv_read_ftm_device_id(ext_nv_ftm_device_id *device_nv)
{
    td_u32 ret = EXT_ERR_SUCCESS;
    static td_bool init = TD_TRUE;
    if (init)
    {
        init = TD_FALSE;
        memset_s(&g_ftm_device_id, sizeof(ext_nv_ftm_device_id), 0x00,
                 sizeof(ext_nv_ftm_device_id));
        ret = uapi_factory_nv_read(EXT_NV_FTM_DEVICE_INFO, &g_ftm_device_id,
                                   sizeof(ext_nv_ftm_device_id));
    }

    if (ret == EXT_ERR_SUCCESS)
    {
        if ((ret == EXT_ERR_SUCCESS) && device_nv)
        {
            memcpy_s(device_nv, sizeof(ext_nv_ftm_device_id),
                    &g_ftm_device_id, sizeof(ext_nv_ftm_device_id));
        }
    }
    return ret;
}

td_u32 nv_read_modem_version_id(modem_version_nv *version_nv)
{
    td_u32 ret = EXT_ERR_SUCCESS;
    static td_bool init = TD_TRUE;
    if (init)
    {
        init = TD_FALSE;
        memset_s(&g_modem_version_nv, sizeof(modem_version_nv), 0x00,
                 sizeof(modem_version_nv));
        ret = uapi_nv_read(MODEM_VERSION_NV, &g_modem_version_nv,
                                   sizeof(modem_version_nv));
    }
    if (ret == EXT_ERR_SUCCESS)
    {
        if ((ret == EXT_ERR_SUCCESS) && version_nv)
        {
            memcpy_s(version_nv, sizeof(modem_version_nv),
                    &g_modem_version_nv, sizeof(modem_version_nv));
        }
    }
    return ret;
}

td_u32 nv_read_keep_status_id(keep_status_nv *status_nv)
{
    td_u32 ret = EXT_ERR_SUCCESS;
    static td_bool init = TD_TRUE;
    if (init)
    {
        init = TD_FALSE;
        memset_s(&g_keep_status_nv, sizeof(keep_status_nv), 0x00,
                 sizeof(keep_status_nv));
        ret = uapi_nv_read(KEEP_STATUS_NV, &g_keep_status_nv,
                                   sizeof(keep_status_nv));
    }
    if (ret == EXT_ERR_SUCCESS)
    {
        if ((ret == EXT_ERR_SUCCESS) && status_nv)
        {
            memcpy_s(status_nv, sizeof(keep_status_nv),
                    &g_keep_status_nv, sizeof(keep_status_nv));
        }
    }
    return ret;
}

ext_nv_ftm_product_id *get_ftm_product_id(td_void)
{
    return &g_ftm_product_id;
}

ext_nv_ftm_device_id *get_ftm_device_id(td_void)
{
    return &g_ftm_device_id;
}

modem_version_nv *get_modem_version_id(td_void)
{
    return &g_modem_version_nv;
}
keep_status_nv *get_keep_status_id(td_void)
{
    return &g_keep_status_nv;
}