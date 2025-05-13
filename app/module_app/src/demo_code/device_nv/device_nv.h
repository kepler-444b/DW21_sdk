#ifndef __DEVICE_NV_H__
#define __DEVICE_NV_H__

#include <stdio.h>
#include <app_nv.h>
#include <soc_ft_nv.h>
#include <soc_types.h>
#include <soc_mdm_nv.h>
#include <string.h>
#include "app_common.h"
#include "app_main.h"
#include "securec.h"


td_void device_nv_init(td_void);

td_u32 nv_read_ftm_product_id(ext_nv_ftm_product_id *nv);
td_u32 nv_read_ftm_device_id(ext_nv_ftm_device_id *nv);
td_u32 nv_read_modem_version_id(modem_version_nv *nv);
td_u32 nv_read_keep_status_id(keep_status_nv *status_nv);

/// @brief 从全局获取ext_nv_ftm_product_id(0x03)数据
ext_nv_ftm_product_id *get_ftm_product_id(td_void);

/// @brief 从全局获取ext_nv_ftm_device_id(0x08)数据
ext_nv_ftm_device_id *get_ftm_device_id(td_void);

/// @brief 从全局获取modem_version_id(0x76)数据
modem_version_nv *get_modem_version_id(td_void);

/// @brief 从全局获取keep_status_nv(0x92)数据
keep_status_nv *get_keep_status_id(td_void);
#endif