/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: APP UART handle.
 */

#include "app_uart.h"
#include "app_proto.h"
#include "app_common.h"
#include "soc_mdm_uart.h"
#include "stdio.h"
#include "soc_mdm_task.h"
#include "soc_mdm_io.h"
#ifdef __cplusplus
extern "C" {
#endif

// uart0 接收数据回调
uart_data_callbk g_uart_data_cb = NULL;

td_void uart_set_callbk_func(uart_data_callbk callbk)
{
    g_uart_data_cb = callbk;
}

EXT_PRV td_void app_uart_rx(app_data_buf *buf_head, td_u8 ch);

EXT_PRV ext_uart_port g_app_uart_port = EXT_UART_0;
EXT_PRV ext_uart_init_type g_app_uart_cfg = { 115200, 8, 1, 0 };    // 为方便上位机调试，在此禁用奇偶校验

td_u32 app_uart_init(td_void)
{
    td_u32 ret;

    // 需要在此复用GPIO9和GPIO10为 UART0_RXD和UART0_TXD
    uapi_io_set_func(EXT_GPIO_IDX_9,  1);     /* GPIO_9_SEL:  UART0_RXD */	
    uapi_io_set_func(EXT_GPIO_IDX_10, 1);     /* GPIO_10_SEL: UART0_TXD */

    uapi_uart_close(g_app_uart_port);
    ret = uapi_uart_open(g_app_uart_port);
    ret |= uapi_uart_ioctl(g_app_uart_port, UART_CFG_SET_ATTR, (uintptr_t)&g_app_uart_cfg);
    if (ret != EXT_ERR_SUCCESS) {
        app_printf("open uart %d failed.\n", g_app_uart_port);
        return EXT_ERR_FAILURE;
    }
    else{
        app_printf("open uart %d [%d]success.\n", g_app_uart_port, g_app_uart_cfg.baud_rate);
    }

    return EXT_ERR_SUCCESS;
}

td_void app_uart_task_body(td_u32 param)
{
    app_data_buf *buf_head = (app_data_buf *)(uintptr_t)param;
    td_s32 len;
    td_u8 ch = 0;

    for (;;) {
        len = uapi_uart_read(g_app_uart_port, &ch, 1);
        if (len == 1) {
            app_uart_rx(buf_head, ch);
            continue;
        }

        uapi_sleep(APP_UART_SLEEP_DURTION_MS);
    }
}

EXT_PRV td_void app_uart_rx(app_data_buf *buf_head, td_u8 ch)
{
    // 如果 sta 注册了此回调函数，则在各个子设备中处理
    if (g_uart_data_cb != NULL)
    {
        if (buf_head->rx_length < APP_DATA_BUF_SIZE)
        {
            buf_head->buffer[buf_head->rx_length] = ch;//将变量 ch 的值存储到 buf_head->buffer 数组的下一个位置
            buf_head->rx_length++;

            g_uart_data_cb(buf_head);
        }

        return;
    }

    if ((buf_head->rx_length == 0) && (ch != APP_FRAME_HEAD_BYTE)) {
        return;
    }

    if (buf_head->rx_length < APP_DATA_BUF_SIZE) {
        buf_head->buffer[buf_head->rx_length] = ch;
        buf_head->rx_length++;
        app_proto_parse(buf_head);
    }
}

td_u32 app_uart_tx(td_pbyte data, td_u16 data_length)
{
    if (data == TD_NULL) {
        return EXT_ERR_INVALID_PARAMETER;
    }

    if (uapi_uart_write(g_app_uart_port, data, data_length) != (td_s32)data_length) {
        app_printf("app_uart_tx(): send failed.\n");
        return EXT_ERR_FAILURE;
    }
    // printf("app_uart_tx():%s \n", data);
    return EXT_ERR_SUCCESS;
}

#ifdef __cplusplus
}
#endif

