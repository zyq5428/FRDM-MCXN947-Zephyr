/* * 包含 Zephyr 内核头文件 
 */
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>

/* * 注册当前文件的日志模块，名字叫 "main"
 */
LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    /* * 延时一小段时间，确保串口助手已经打开并准备好接收 
     */
    k_msleep(100);

    /* 使用 printk 打印简单字符串 */
    printk("\n------------------------------------------\n");
    printk("Hello World! FRDM-MCXN947 Zephyr Project\n");
    printk("Build Time: %s %s\n", __DATE__, __TIME__);
    printk("------------------------------------------\n");

    /* 使用 LOG 模块打印带格式的信息 */
    LOG_INF("系统初始化成功，准备进入主循环...");

    int counter = 0;

    while (1) {
        /* 每隔 1000 毫秒（1秒）打印一次计数器 */
        LOG_INF("运行中... 计数次数: %d", counter++);
        
        /* 线程睡眠函数，释放 CPU 给其他任务 */
        k_msleep(1000);
    }

    return 0;
}