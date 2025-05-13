#include "demo_main.h"
td_void micro_demo_task_body(td_u32 param)
{
	(td_void)param;
	
    for (;;)
    {
        demo_uart_recv_task();
        uapi_sleep(MICRO_PERI_MODULE_SLEEP_DURTION_MS/10);
        /*delay one second, You must add a delay to the task, otherwise the task will keep grabbing the CPU and cause other tasks to fail to run normally*/
    }
}

td_u32 micro_demo_task_init(td_void)
{
	static td_u32 task_id = 2;
	td_u32 ret;

	ret = uapi_task_create(&task_id, "MICRO_DEMO_TASK", micro_demo_task_body, NULL, MICRO_PERI_SRV_TASK_STACK_SIZE,MICRO_PERI_SRV_TASK_PRIORITY);
    if (ret != EXT_ERR_SUCCESS)
    {
        printf("[-IFO-] hi_task_create() MICRO_DEMO_TASK failed.\n");
        return EXT_ERR_FAILURE;
    }
	return EXT_ERR_SUCCESS;
}


__hot td_u32 demo_rx_process(app_data_buf *buf_head)
{
    td_pbyte data = buf_head->buffer;
    td_u16 data_length = buf_head->rx_length;

    app_print_buf("rx_data", data, data_length);

    if(data_length >= 512)
    {
        buf_head->rx_length = 0;
        return TD_FALSE;
    }
    // 处理数据后要清空缓存区
    memset_s(buf_head->buffer, 512, 0, 512);
    buf_head->rx_length = 0;

    return 0;
}
void demo_app_main(void)
{
    printf_info();
    //creat a task to running our application 
    // printf("Starting demo application driver init and creat demo_app task! \n");

    // demo_gpio_init();
    // pwm init
    // demo_pwm_init();
    // uart init
    // demo_uart_init();
    // creat task to running our application
    // micro_demo_task_init();
    // response ready
    // demo_uart_tx_data("+READY\r\n", strlen("+READY\r\n"));
    
    // 挂载 uart0 回调函数
    uart_set_callbk_func(demo_rx_process);
    return ;
}