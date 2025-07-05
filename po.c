#include <stdio.h>
#include <stdlib.h>


#include <stddef.h>
// container_of宏（用户态实现）
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))

typedef struct Led_strip_t Led_strip_t;


typedef struct Led_strip_t *Led_strip_handle_t;



struct Led_strip_t
{
    int (*led_on)(Led_strip_t *strip, int led_index);
    int (*led_off)(Led_strip_t *strip, int led_index);
};


typedef struct Led_rmt
{
    int chanel;
    Led_strip_t base;

    void *origin_ptr; // 存储malloc返回的地址，以便在程序结束时释放内存
    
}Led_rmt_obj;

static int led_rmton(Led_strip_t *strip, int index)
{
    printf("Led[%d] rmton,channel=%d\r\n",index, ((Led_rmt_obj *)container_of(strip, Led_rmt_obj, base))->chanel);

    return 0;
}

static int led_rmtoff(Led_strip_t *strip, int index)
{
    printf("LED[%d] OFF\n", index);
    return 0;
}


Led_strip_handle_t config_led(void)
{
    

    printf("Configuring LED strip...\n");
    Led_rmt_obj *led_rmt = (Led_rmt_obj *)malloc(sizeof(Led_rmt_obj));

    printf("malloc返回地址led_rmt address: %p\n", (void *)led_rmt);

    led_rmt->origin_ptr = led_rmt;

    led_rmt->base.led_on = led_rmton;
    led_rmt->base.led_off = led_rmtoff;

    led_rmt->chanel =1;

    Led_strip_handle_t led_strip ;
 
    led_strip =&(led_rmt->base) ;//这个地址实际上是整个结构体起始地址+偏移量（即chanel的大小）。
    //因此，&led_rmt->base 不等于 led_rmt（因为base不是第一个成员）。
    //free函数需要接收一个由malloc返回的指针,(即分配的内存块的起始地址)


    printf("[2] 对象地址：%p | base成员地址：%p | 存储的原始指针：%p\n",
        (void *)led_rmt, 
        (void *)&led_rmt->base,
        led_rmt->origin_ptr);
   printf("返回base地址 led_strip address: %p\n", (void *)led_strip);
    return led_strip;
}

// 安全释放函数
void led_strip_delete(Led_strip_handle_t handle) {
    if (!handle) return;
    
    // 通过container_of获取完整对象
    Led_rmt_obj *temp = container_of(handle, Led_rmt_obj, base);
    
    printf("[3] 释放原始地址：%p\n", temp->origin_ptr);
    free(temp->origin_ptr); // 使用存储的原始指针释放
}


// void led_strip_delete(Led_strip_handle_t led_strip)
// {
//     // if (led_strip) {
//     //     Led_rmt_obj *led_rmt = (Led_rmt_obj *)((char *)led_strip - offsetof(Led_rmt_obj, base));
//     //     free(led_rmt->origin_ptr);
//     // }
//     Led_rmt_obj* temp = container_of(led_strip, Led_rmt_obj, base);
//     free(temp->origin_ptr);
// }


int main() 
{
    Led_strip_handle_t led_strip = config_led();

    if(led_strip)
    {
        led_strip->led_on(led_strip,1);
        led_strip->led_off(led_strip,2);
    }

    printf("释放的 地址 led_strip address: %p\n", (void *)led_strip);
    led_strip_delete(led_strip);
 //   free(led_strip);
//  led_strip_delete(led_strip);
  //  free((Led_rmt_obj*)led_strip); // 释放原始指针，确保释放的是malloc返回的地址

    return 0;
}