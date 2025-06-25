#include "stm32f10x.h"
#include "board.h"
#include "rtthread.h"






//静态:
#if 0 
static struct rt_thread led1_thread;  //线程控制块 ->身份证

ALIGN(RT_ALIGN_SIZE)

static rt_uint8_t rt_led1_thread_stack[1024];

static void led1_thread_entry(void* parameter);


static void led1_thread_entry(void* parameter)
{
	while (1)
	{

		PBout(5) = 0;
		rt_thread_delay(500); /* 延时 500 个 tick */

		PBout(5)= 1;
		rt_thread_delay(500); /* 延时 500 个 tick */		
		

	}
}


	rt_thread_init(&led1_thread, /* 线程控制块 */
			"led1", /* 线程名字 */
			led1_thread_entry, /* 线程入口函数 */
			RT_NULL, /* 线程入口函数参数 */
			&rt_led1_thread_stack[0], /* 线程栈起始地址 */
			sizeof(rt_led1_thread_stack), /* 线程栈大小 */
			3, /* 线程的优先级 */
			20); /* 线程时间片 */
			
			
	rt_thread_startup(&led1_thread); /* 启动线程，开启调度 */
		
#endif




static rt_mq_t test_mq = RT_NULL;


static rt_thread_t send_thread = RT_NULL;
static rt_thread_t receive_thread = RT_NULL;


static void send_thread_entry(void *para);

static void receive_thread_entry(void *para);

int main(void)
{	

	test_mq = rt_mq_create("testmq",10,5,RT_IPC_FLAG_FIFO);
	
	if(test_mq != RT_NULL)
	{
		rt_kprintf("消息队列create success !\r\n");
		
	}
	
	send_thread = rt_thread_create("send",
									send_thread_entry,
									RT_NULL,
									512,
									3,
									100);
	
	if(send_thread != RT_NULL)
		rt_thread_startup(send_thread);
	
	receive_thread = rt_thread_create("receive",
									receive_thread_entry,
									RT_NULL,
									512,
									2,
									100);	
									
	if(receive_thread != RT_NULL)
		rt_thread_startup(receive_thread);

}


static void send_thread_entry(void *para)
{
	uint32_t data = 1;
	uint32_t data2 = 2;
	rt_err_t ret = RT_EOK;
	while(1)
	{
		if(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) ==1)
		{
			ret = rt_mq_send(test_mq, &data, sizeof(data));
			
			if(ret !=RT_EOK)
			{
				rt_kprintf("send data 1 failed !\r\n");
			}
	
			
		}

		if(Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) ==1)
		{
			rt_kprintf("111111!\r\n");
			ret = rt_mq_send(test_mq, &data2, sizeof(data2));
			
			if(ret !=RT_EOK)
			{
				rt_kprintf("send data 2 failed !\r\n");
			}
	
			
		}

		rt_thread_delay(20);
	}
	
}

static void receive_thread_entry(void *para)
{
	uint32_t buff;
	rt_err_t ret = RT_EOK;
	while(1)
	{
		ret = rt_mq_recv(test_mq, &buff, sizeof(buff), 15000);
		
		if( ret!= RT_EOK)
		{
			rt_kprintf("receive failed ! \r\n");
			
		}else if(ret == RT_EOK)
		{
			rt_kprintf("receive data is %d\r\n",buff);
		}
		rt_thread_delay(20);
	}
	
}




